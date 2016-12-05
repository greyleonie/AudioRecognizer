#include <windows.h>
#include <string.h>
#include <stdio.h>


#define	WAV_HEAD_LENGTH		58


unsigned char wav_head[WAV_HEAD_LENGTH] = {
	0x52, 0x49, 0x46, 0x46, 
	0x00, 0x00, 0x00, 0x00, 
	0x57, 0x41, 0x56, 0x45, 

	0x66, 0x6D, 0x74, 0x20, 
	0x12, 0x00, 0x00, 0x00, 
	0x06, 0x00, 
	0x01, 0x00, 
	0x40, 0x1F, 0x00, 0x00, 
	0x40, 0x1F, 0x00, 0x00, 
	0x01, 0x00, 
	0x08, 0x00, 
	
	0x00, 0x00, 
	
	0x66, 0x61, 0x63, 0x74, 
	0x04, 0x00, 0x00, 0x00, 0x00, 0xBC, 
	0x00, 0x00, 
	
	0x64, 0x61, 0x74, 0x61, 
	0x00, 0x00, 0x00, 0x00
};


#define	WAV_TAIL_LENGTH		102

unsigned char wav_tail[WAV_TAIL_LENGTH] = {
	0x63, 0x75, 0x65, 0x20, 0x1C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
	0x00, 0x5A, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x5A, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x3A, 0x00, 0x00, 0x00, 0x61, 0x64, 0x74, 0x6C, 
	0x6C, 0x74, 0x78, 0x74, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x00, 
	0x72, 0x67, 0x6E, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6C, 0x61, 0x62, 0x6C, 
	0x12, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x62, 0x75, 0x69, 0x6C, 0x64, 0x69, 0x6E, 0x67, 
	0x2E, 0x61, 0x6C, 0x61, 0x77, 0x00
};

typedef struct _RIFF_WAV_CHUNK
{
	char	id[4];			//00H
	int		fileLength;		//04H
	char	wave[4];		//08H
}RIFF_WAV_CHUNK;

typedef struct _FORMAT_CHUNK
{
	char			id[4];			//0CH
	int				chunkSize;		//10H
	short			formatTag;		//14H
	unsigned short channels;		//16H
	unsigned long  samplesPerSec;	//18H
	unsigned long  avgBytesPerSec;	//1CH
	unsigned short blockAlign;		//20H
	unsigned short bitsPerSample;	//22H

}FORMAT_CHUNK;


typedef struct _DATA_CHUNK
{
	char			id[4];			//24H
	int				chunkSize;		//28H
}DATA_CHUNK;


int gDataSize = 0;

int
append_wav_head(
	char	*wavFileName
	)
{
	FILE	*wav_file;
	int		i;

	wav_file = fopen(wavFileName, "w");
	if (wav_file == NULL)
		return -1;

	for (i = 0; i < WAV_HEAD_LENGTH; ++i)
		fputc(wav_head[i], wav_file);

	fclose(wav_file);

	gDataSize = 0;

	return 0;
}


int
append_wav_tail(
	char	*wavFileName
	)
{
	FILE	*wav_file;
	int		i;
	int		fileSize = gDataSize + WAV_HEAD_LENGTH + WAV_TAIL_LENGTH - 8;

	wav_file = fopen(wavFileName, "r+");
	if (wav_file == NULL)
		return -1;

	//update file size
	fseek(wav_file, 4, SEEK_SET);
	fwrite(&fileSize, sizeof(int), 1, wav_file);
	//update raw data size
	fseek(wav_file, 54, SEEK_SET);
	fwrite(&gDataSize, sizeof(int), 1, wav_file);

	fseek(wav_file, 0, SEEK_END);
	for (i = 0; i < WAV_TAIL_LENGTH; ++i)
		fputc(wav_tail[i], wav_file);

	fclose(wav_file);	

	return 0;
}


int
append_raw_data(
	char	*wavFileName,
	char	*rawFileName
	)
{
	FILE	*wav_file;
	FILE	*raw_file;
	char	ch;
	
	wav_file = fopen(wavFileName, "a");
	if (wav_file == NULL)
		return -1;

	raw_file = fopen(rawFileName, "r");
	if (raw_file == NULL)
	{
		fclose(wav_file);
		return -1;
	}

	ch = fgetc(raw_file);
	while(!feof(raw_file))
	{
		fputc(ch, wav_file);
		gDataSize++;
		ch = fgetc(raw_file);
	}

	fclose(raw_file);
	fclose(wav_file);

	return 0;
}

