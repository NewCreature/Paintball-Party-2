/*
 * Allegro5 WAV reader
 * author: Matthew Leverton 
 */

#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

typedef struct WAVFILE
{
   ALLEGRO_FILE *f; 
   size_t dpos;     /* the starting position of the data chunk */
   int freq;        /* e.g., 44100 */
   short bits;      /* 8 (unsigned char) or 16 (signed short) */
   short channels;  /* 1 (mono) or 2 (stereo) */
   int samples;     /* # of samples. size = samples * (bits/8) * channels */
   double loop_start;
   double loop_end;
} WAVFILE;

/* read16:
 *  Reads a signed, 16-bit little endian integer and places the value in 'v'.
 *
 *  Returns true on success, false on EOF
 */
static bool read16(ALLEGRO_FILE *f, signed short *v)
{
   const int a = al_fgetc(f), b = al_fgetc(f);
   
   if (b != EOF) {
      *v = (a | (b << 8));
      return true;
   }

   return false;
}

/* read32:
 *  Reads a signed, 32-bit little endian integer and places the value in 'v'.
 *
 *  Returns true on success, false on EOF
 */
static bool read32(ALLEGRO_FILE *f, int *v)
{
   const int a = al_fgetc(f), b = al_fgetc(f), c = al_fgetc(f), d = al_fgetc(f);

   if (d != EOF) {
      *v = (a | (b << 8) | (c << 16) | (d << 24));
      return true;
   }

   return false;
}

static WAVFILE * wav_open_f(ALLEGRO_FILE * f)
{
	WAVFILE * wavfile = NULL;
	char buffer[12];

	if(!f)
	{
		goto wav_open_error;
	}

	/* prepare default values */
	wavfile = malloc(sizeof(WAVFILE));
	if(!wavfile)
	{
		return NULL;
	}
	wavfile->f = f;
	wavfile->freq = 22050;
	wavfile->bits = 8;
	wavfile->channels = 1;

	/* check the header */
	if(al_fread(f, buffer, 12) != 12)
	{
		goto wav_open_error;
	}

	if(memcmp(buffer, "RIFF", 4) || memcmp(buffer+8, "WAVE", 4))
	{
		goto wav_open_error;
	}
   
	/* read as many leading fmt chunks as exist */
	while(true)
	{
		int length = 0;
		short pcm = 0;

		if(al_fread(f, buffer, 4) != 4)
		{
			goto wav_open_error;
		}

		/* check to see if it's a fmt chunk */
		if(memcmp(buffer, "fmt ", 4))
		{
			break;
		}

		read32(f, &length);
		if(length < 16)
		{
			goto wav_open_error;
		}

		/* should be 1 for PCM data */
		read16(f, &pcm);
		if(pcm != 1)
		{
			goto wav_open_error;
		}

		/* mono or stereo data */
		read16(f, &wavfile->channels);

		if((wavfile->channels != 1) && (wavfile->channels != 2))
		{
			goto wav_open_error;
		}

		/* sample frequency */
		read32(f, &wavfile->freq);

		/* skip six bytes */
		al_fseek(f, 6, ALLEGRO_SEEK_CUR);   

		/* 8 or 16 bit data? */
		read16(f, &wavfile->bits);
		if((wavfile->bits != 8) && (wavfile->bits != 16))
		{
			goto wav_open_error;
		}

		/* Skip remainder of chunk */
		length -= 16;
		if(length > 0)
		{
			al_fseek(f, length, ALLEGRO_SEEK_CUR);
		}
	}

	/* now there should be a data chunk */
	if(memcmp(buffer, "data", 4))
	{
		goto wav_open_error;
	}

	/* find out how many samples exist */
	read32(f, &wavfile->samples);
	
	if(wavfile->channels == 2)
	{
		wavfile->samples = (wavfile->samples + 1) / 2;
	}

	if(wavfile->bits == 16)
	{
		wavfile->samples /= 2;
	}

	wavfile->dpos = al_ftell(f);
	return wavfile;

	wav_open_error:

		if(wavfile)
		{
			free(wavfile);
		}

   return NULL;
}

/* wav_read:
 *  Reads up to 'samples' number of samples from the wav ALLEGRO_FILE into 'data'.
 *  Returns the actual number of samples written to 'data'.
 */
static size_t wav_read(WAVFILE *wavfile, void *data, size_t samples)
{
   size_t n;

   n = wavfile->channels * samples;

   if (wavfile->bits == 8) {
      return al_fread(wavfile->f, data, n) / wavfile->channels;
   }
   else {
      size_t i;
      signed short *d = (signed short *) data;

      for (i = 0; i < n; i++) {
         signed short s;
         if (!read16(wavfile->f, &s)) break;

         *d++ = s;
      }

      return i / wavfile->channels;
   }
}

/* wav_close:
 *  Closes the ALLEGRO_FILE and frees the WAVFILE struct.
 */
static void wav_close(WAVFILE *wavfile)
{
//   al_fclose(wavfile->f);
   free(wavfile);
}

ALLEGRO_SAMPLE * pp2_load_wav_f(ALLEGRO_FILE * fp)
{
	WAVFILE *wavfile = wav_open_f(fp);
	ALLEGRO_SAMPLE * spl = NULL;

	if(wavfile)
	{
		size_t n = (wavfile->bits / 8) * wavfile->channels * wavfile->samples;
		char *data = malloc(n);

		if(data)
		{
			spl = al_create_sample(data, wavfile->samples, wavfile->freq,
			(wavfile->bits / 8 == 1) ? ALLEGRO_AUDIO_DEPTH_UINT8 : ALLEGRO_AUDIO_DEPTH_UINT16,
			wavfile->channels == 1 ? ALLEGRO_CHANNEL_CONF_1 : ALLEGRO_CHANNEL_CONF_2, true);

			if(spl)
			{
				memset(data, 0, n);
				wav_read(wavfile, data, wavfile->samples);
			}
			else
			{
				free(data);
			}
		}
		wav_close(wavfile);
	}

	return spl;
}


bool pp2_save_wav_f(ALLEGRO_SAMPLE *spl, ALLEGRO_FILE *pf)
{
	size_t channels, bits, freq;
	size_t data_size;
	size_t samples;
	size_t i, n;
	int val;
	long lval;

	val = al_get_sample_channels(spl);
	channels = val == ALLEGRO_CHANNEL_CONF_1 ? 1 : 2;
	val = al_get_sample_depth(spl);
	bits = (val == ALLEGRO_AUDIO_DEPTH_UINT8 ? 8 : 16);
//	bits = 8;
	void * pval = NULL;

	if(channels < 1 || channels > 2)
	{
		return false;
	}

	lval = al_get_sample_length(spl);
	samples = lval;
	data_size = samples * channels * (bits / 8);
	n = samples * channels;
	
	freq = al_get_sample_frequency(spl);

	al_fputs(pf, "RIFF");
	al_fwrite32le(pf, 36 + data_size);
	al_fputs(pf, "WAVE");

	al_fputs(pf, "fmt ");
	al_fwrite32le(pf, 16);
	al_fwrite16le(pf, 1);
	al_fwrite16le(pf, channels);
	al_fwrite32le(pf, freq);
	lval = al_get_sample_length(spl);
	al_fwrite32le(pf, lval * channels * (bits / 8));
	al_fwrite16le(pf, channels * (bits / 8));
	al_fwrite16le(pf, bits);

	al_fputs(pf, "data");
	al_fwrite32le(pf, data_size);

	val = al_get_sample_depth(spl);
	if (val == ALLEGRO_AUDIO_DEPTH_UINT8)
	{
		pval = al_get_sample_data(spl);
		al_fwrite(pf, pval, samples * channels);
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_INT16)
	{
		pval = al_get_sample_data(spl);
		al_fwrite(pf, pval, samples * channels * 2);
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_INT8)
	{
		pval = al_get_sample_data(spl);
		int8_t *data = pval;
		for (i = 0; i < samples; ++i)
		{
			al_fputc(pf, *data++ + 0x80);
		}
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_UINT16)
	{
		pval = al_get_sample_data(spl);
		uint16_t *data = pval;
		for (i = 0; i < n; ++i)
		{
			al_fwrite16le(pf, *data++ - 0x8000);
		}
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_INT16)
	{
		pval = al_get_sample_data(spl);
		uint16_t *data = pval;
		for (i = 0; i < n; ++i)
		{
			al_fwrite16le(pf, *data++ - 0x8000);
		}
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_INT24)
	{
		pval = al_get_sample_data(spl);
		int32_t *data = pval;
		for (i = 0; i < n; ++i)
		{
			const int v = ((float)(*data++ + 0x800000) / 0x7FFFFF) * 0x7FFF - 0x8000;
			al_fwrite16le(pf, v);
		}
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_UINT24)
	{
		pval = al_get_sample_data(spl);
		uint32_t *data = pval;
		for (i = 0; i < n; ++i)
		{
			const int v = ((float)(*data++) / 0x7FFFFF) * 0x7FFF - 0x8000;
			al_fwrite16le(pf, v);
		}
	}
	else if (val == ALLEGRO_AUDIO_DEPTH_FLOAT32)
	{
		pval = al_get_sample_data(spl);
		float *data = pval;
		for (i = 0; i < n; ++i)
		{
			al_fwrite16le(pf, *data * 0x7FFF);
			data++;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool pp2_save_wav(ALLEGRO_SAMPLE *spl, char * fn)
{
	ALLEGRO_FILE * fp;
	
	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return false;
	}
	pp2_save_wav_f(spl, fp);
	al_fclose(fp);
	return true;
}
