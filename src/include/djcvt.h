#ifndef	_DJCVT_H_
#define	_DJCVT_H_

#ifdef __cplusplus
extern "C" {
#endif


int WINAPI PcmtoWave(char* PcmFileName,char* WaveFileName);
int WINAPI WavetoPcm(char* WaveFileName,char* PcmFileName);
int WINAPI AdtoPcm(char* AdpcmFileName,char* PcmFileName);
int WINAPI PcmtoAd(char* PcmFileName,char* AdpcmFileName);
int WINAPI Ad6ktoPcm(char* AdpcmFileName,char* PcmFileName);
int WINAPI PcmtoWaveNew(char* PcmFileName,char* WaveFileName,int WaveFormat);
int WINAPI WavetoPcm(char* WaveFileName,char* PcmFileName);
int WINAPI PcmMtoWaveM(char* PcmMemory,char* WaveMemory,int SizeofSrc);
// add next in 2006.05.09
int WINAPI PcmMtoAdM ( char* alaw_data, char* adpcm_data, int SizeofSrc, int InitFlag );
//add next by jzq in 2007.10.11
int WINAPI G729ToALaw(char* G729FileName, char* PcmFileName);
BYTE WINAPI linearToALaw(short inSpl);
int WINAPI G729ToWaveNew(char* G729FileName, char* WaveFileName, int WaveFormat);
//

#ifdef __cplusplus
}
#endif


#endif