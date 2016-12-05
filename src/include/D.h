#ifndef __D_H__
#define __D_H__

#include "NewSig.h"

#ifdef __cplusplus
extern "C" {
#endif

enum CHANNEL_STATE {

	ST_FREE,
	ST_RECEIVEID,
	ST_OFFHOOK,
	ST_WELCOME,

	ST_PLAY_NAME,
	ST_RECORD_NAME_START,
	ST_RECORD_NAME,
	ST_REC_NAME,

	ST_PLAY_PLACE,
	ST_RECORD_PLACE_START,
	ST_RECORD_PLACE,
	ST_REC_PLACE,

	ST_PLAY_TROUBLE,
	ST_RECORD_TROUBLE_START,
	ST_RECORD_TROUBLE,
	ST_REC_TROUBLE,

	ST_GOODBYE,
	ST_WAIT_ONHOOK,

};


struct LINESTRUCT
{
	int		nType;
	int		State;
	char	CallerID[32];
	char	Dtmf[32];
	int		nTimeElapse;
	char	StrTime[32];
	char	StrCaller[32];
	char	StrUser[32];
	char	Name[64];
	char	Place[256];
	char	Trouble[256];
	char	Sn[64];
	char	*pCaller;

	char	RecordNameFile[128];
	char	RecordPlaceFile[128];
	char	RecordTroubleFile[128];

	char	RecNameFile[128];
	char	RecPlaceFile[128];
	char	RecTroubleFile[128];

	char	XmlNameFile[128];
	char	XmlPlaceFile[128];
	char	XmlTroubleFile[128];
};

void WINAPI yzDoWork(void);
BOOL  WINAPI yzInitSystem(void);
void  WINAPI yzExitSystem(void);
void WINAPI yzDrawState(int trkno);
char yzConvertDtmf(int ch);
void GetVoicePath(void);



#ifdef __cplusplus
}
#endif

#endif //__D_H__