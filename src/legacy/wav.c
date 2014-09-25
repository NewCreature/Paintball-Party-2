#include <allegro5/allegro5.h>
#include <allegro5/kcm_audio.h>
#include "wav.h"

typedef struct
{
	
	char          rID[4];
	unsigned long rLen;
	char          wID[4];
	char          fID[4];
	unsigned long fLen;
	unsigned int  wFormatTag;
	unsigned int  nChannels;
	unsigned long nSamplesPerSec;
	unsigned long nAvgBytesPerSec;
	unsigned int  nBlockAlign;
	unsigned int  nFormatSpecific;
	unsigned long sLength;
	
} PP2_LEGACY_WAVHEADER;

ALLEGRO_SAMPLE * pp2_legacy_load_wav_entry(ALLEGRO_FILE * f)
{
	char buffer[25];
	int i;
	int length, len;
	int freq = 22050;
	int bits = 8;
	int channels = 1;
	ALLEGRO_SAMPLE *spl = NULL;
	int loaded = 0;
	char * data = NULL;

	al_fread(f, buffer, 12);          /* check RIFF header */
	if(memcmp(buffer, "RIFF", 4) || memcmp(buffer+8, "WAVE", 4))
	{
		goto getout;
	}

	while(!al_feof(f) && !loaded)
	{
		if(al_fread(f, buffer, 4) != 4)
		{
			break;
		}

		length = al_fread32le(f, NULL);          /* read chunk length */

		if(memcmp(buffer, "fmt ", 4) == 0)
		{
			i = al_fread16le(f);            /* should be 1 for PCM data */
			length -= 2;
			if (i != 1) 
			{
				goto getout;
			}

			channels = al_fread16le(f);     /* mono or stereo data */
			length -= 2;
			if ((channels != 1) && (channels != 2))
			{
	    		goto getout;
    		}

			freq = al_fread16le(f);         /* sample frequency */
			length -= 4;

			al_fread32le(f, NULL);                /* skip six bytes */
			al_fread16le(f);
			length -= 6;

			bits = al_fread16le(f);         /* 8 or 16 bit data? */
			length -= 2;
			if ((bits != 8) && (bits != 16))
			{
	    		goto getout;
    		}
		}
		else if (memcmp(buffer, "data", 4) == 0)
		{
			len = length / channels;

			data = malloc(len * channels * (bits / 8));
			if(data)
			{
				al_fread(f, data, len * channels * (bits / 8));
			}
			ALLEGRO_AUDIO_DEPTH d = bits == 16 ? ALLEGRO_AUDIO_DEPTH_INT16 : ALLEGRO_AUDIO_DEPTH_INT8;
			ALLEGRO_CHANNEL_CONF c = channels == 2 ? ALLEGRO_CHANNEL_CONF_2 : ALLEGRO_CHANNEL_CONF_1;
			spl = al_create_sample(data, len, freq, d, c, true);
		}

		while (length > 0)              /* skip the remainder of the chunk */
		{
			if (al_fgetc(f) == EOF)
			{
				break;
			}
			length--;
		}
	}

	getout:
	return spl;
}

/* saves a wave file to file pointer */
int pp2_legacy_save_wav_entry(ALLEGRO_SAMPLE * SamplePointer, ALLEGRO_FILE * f)
{
	int i;
	ALLEGRO_SAMPLE_INSTANCE * ip = al_create_sample_instance(SamplePointer);
	if(!ip)
	{
		return 0;
	}
	int m = 1;
	int freq = 0;
	al_get_sample_instance_long(ip, ALLEGRO_AUDIOPROP_FREQUENCY, &freq);
	ALLEGRO_AUDIO_DEPTH d = al_get_sample_instance_enum(ALLEGRO_AUDIO_DEPTH);
	ALLEGRO_CHANNEL_CONF c = al_get_sample_instance_enum(ALLEGRO_CHANNEL_CONF);
	if(d == ALLEGRO_AUDIO_DEPTH_INT16)
	{
		m *= 2;
	}
	if(c == ALLEGRO_CHANNEL_CONF_2)
	{
		m *= 2;
	}

    /* create the header */
	char RiffHeader[4] = {'R', 'I', 'F', 'F'};
	unsigned long FileLength = 36 + al_get_sample_instance_long(ip, ALLEGRO_AUDIOPROP_LENGTH) * m;
	char WaveHeader[4] = {'W', 'A', 'V', 'E'};
	char FormatHeader[4] = {'f', 'm', 't', ' '};
	unsigned long FormatLength = 0x10;
	unsigned int ExtraByte = 0x01;
	unsigned int Channels = 0x01;
	unsigned long SampleRate = SamplePointer->freq;
	unsigned long BytesPerSecond = SamplePointer->freq;
	unsigned int BytesPerSample = SamplePointer->bits / 8;
	unsigned int BitsPerSample = SamplePointer->bits;
	char DataHeader[4] = {'d', 'a', 't', 'a'};
	unsigned long SampleLength = SamplePointer->len;

    /* write header to file */
	al_fwrite(f, RiffHeader, 4);
	al_fwrite32le(FileLength, f);
	al_fwrite(f, WaveHeader, 4);
	al_fwrite(f, FormatHeader, 4);
	al_fwrite32le(FormatLength, f);
	al_fwrite16le(ExtraByte, f);
	al_fwrite16le(Channels, f);
	al_fwrite32le(SampleRate, f);
	al_fwrite32le(BytesPerSecond, f);
	al_fwrite16le(BytesPerSample, f);
	al_fwrite16le(BitsPerSample, f);
	al_fwrite(f, DataHeader, 4);
	al_fwrite32le(SampleLength, f);

    /* write sample data to file */
	pack_fwrite(SamplePointer->data, SamplePointer->len * BytesPerSample, f);
	al_destroy_sample_instance(ip);

	return 1;
}
