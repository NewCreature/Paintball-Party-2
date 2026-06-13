#include "t3f/t3f.h"
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

		length = al_fread32le(f);          /* read chunk length */

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

			al_fread32le(f);                /* skip six bytes */
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
	int sample_bytes = 1;
	ALLEGRO_SAMPLE_INSTANCE * ip = al_create_sample_instance(SamplePointer);
	if(!ip)
	{
		return 0;
	}
	int m = 1;
	int freq = al_get_sample_instance_frequency(ip);
	ALLEGRO_AUDIO_DEPTH d = al_get_sample_instance_depth(ip);
	ALLEGRO_CHANNEL_CONF c = al_get_sample_instance_channels(ip);
	if(d == ALLEGRO_AUDIO_DEPTH_INT16)
	{
		sample_bytes = 2;
		m *= 2;
	}
	if(c == ALLEGRO_CHANNEL_CONF_2)
	{
		m *= 2;
	}

    /* create the header */
	char RiffHeader[4] = {'R', 'I', 'F', 'F'};
	unsigned long FileLength = 36 + al_get_sample_instance_length(ip) * m;
	char WaveHeader[4] = {'W', 'A', 'V', 'E'};
	char FormatHeader[4] = {'f', 'm', 't', ' '};
	unsigned long FormatLength = 0x10;
	unsigned int ExtraByte = 0x01;
	unsigned int Channels = 0x01;
	unsigned long SampleRate = al_get_sample_frequency(SamplePointer);
	unsigned long BytesPerSecond = al_get_sample_frequency(SamplePointer) * m;
	unsigned int BytesPerSample = sample_bytes;
	unsigned int BitsPerSample = sample_bytes * 8;
	char DataHeader[4] = {'d', 'a', 't', 'a'};
	unsigned long SampleLength = al_get_sample_length(SamplePointer);

    /* write header to file */
	al_fwrite(f, RiffHeader, 4);
	al_fwrite32le(f, FileLength);
	al_fwrite(f, WaveHeader, 4);
	al_fwrite(f, FormatHeader, 4);
	al_fwrite32le(f, FormatLength);
	al_fwrite16le(f, ExtraByte);
	al_fwrite16le(f, Channels);
	al_fwrite32le(f, SampleRate);
	al_fwrite32le(f, BytesPerSecond);
	al_fwrite16le(f, BytesPerSample);
	al_fwrite16le(f, BitsPerSample);
	al_fwrite(f, DataHeader, 4);
	al_fwrite32le(f, SampleLength);

  /* write sample data to file */
	al_fwrite(f, al_get_sample_data(SamplePointer), SampleLength * BytesPerSample);
	al_destroy_sample_instance(ip);

	return 1;
}
