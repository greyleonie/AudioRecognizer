//#include "include/stdafx.h"
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <windowsx.h>
#include <stdio.h>

/* Windows NT */
#include <process.h>


#include "include/D.h"
#include "include/tc08a32.h"
#include "resource/resource.h"
#include "include/NewSig.h"

// for nuance swi api
#include "include/SWIrecAPI.h"
#include "include/SWIepAPI.h"

// for system configuration file
#include "include/cfg.h"


WORD TotalLine;
extern HWND hGWnd;
LINESTRUCT *Lines;
char VoicePath[100];
char sTmp[128];
extern HINSTANCE hInst;
PARAMETER_CONFIG	gCfg;


#define	IS_DEBUG			0


// define playing file
#define	PLAY_WELCOME_FILE		"./voice/welcome"
#define	PLAY_NAME_FILE			"./voice/name"
#define	PLAY_BUILDING_FILE		"./voice/building"
#define	PLAY_ROOM_STAFF_FILE	"./voice/room_staff"
#define	PLAY_ROOM_STUDENT_FILE	"./voice/room_student"
#define	PLAY_PLACE_FILE			"./voice/place"
#define	PLAY_TROUBLE_FILE		"./voice/trouble"
#define	PLAY_GOODBYE_FILE		"./voice/goodbye"
#define	PLAY_DI_FILE			"./voice/di"
#define	PLAY_RECORD_END_FILE	"./voice/record_end"
#define	PLAY_RECORD_START_FILE	"./voice/record_start"

// define record file
#define	RECORD_NAME_FILE		"./output/name.alaw"
#define	RECORD_BUILDING_FILE	"./output/building.alaw"
#define	RECORD_ROOM_FILE		"./output/room.alaw"
#define	RECORD_PLACE_FILE		"./output/place.alaw"
#define	RECORD_TROUBLE_FILE		"./output/trouble.alaw"

#define	REC_NAME_FILE			"./output/name.ulaw"
#define	REC_BUILDING_FILE		"./output/building.ulaw"
#define	REC_ROOM_FILE			"./output/room.ulaw"
#define	REC_PLACE_FILE			"./output/place.ulaw"
#define	REC_TROUBLE_FILE		"./output/trouble.ulaw"

#define	XML_NAME_FILE			"./output/name.xml"
#define	XML_BUILDING_FILE		"./output/building.xml"
#define	XML_ROOM_FILE			"./output/room.xml"
#define	XML_PLACE_FILE			"./output/place.xml"
#define	XML_TROUBLE_FILE		"./output/trouble.xml"

#define	OUTPUT_PATH				"./output/"
#define	SAVE_PATH				"./data/"
#define	ZIP_EXE					"./bin/7z.exe"



#ifdef	__cplusplus
extern "C" {
#endif
int SWIOpen(void);
int SWIClose(void);
int SWIrecognizer(
	char const	*in_audio_file, 
	int			grammar, 
	char const	*out_audio_file,
	char const	*out_xml_file
	);

int
GetParaConfig(
	PARAMETER_CONFIG	*cfg
	);

#ifdef	__cplusplus
}
#endif

int
DataBaseInsert(
	char		*Name,				
	char		*Place,
	char		*Description,
	char		*Sn
	);



extern int
xml_parser(
	char const	*filename,
	char		*result
	);

int
make_zip(
	char const	*zip_exe,
	char const	*sn,
	char const	*src_path,
	char const	*dst_path,
	char const	*zip_file
	);

// make wav file
int
append_wav_head(
	char	*wavFileName
	);

int
append_wav_tail(
	char	*wavFileName
	);

int
append_raw_data(
	char	*wavFileName,
	char	*rawFileName
	);

void
GetReqSn(
	char		*sn,
	char const	*number
	)
{
	SYSTEMTIME	lt;
	char		temp[8];

	GetLocalTime(&lt);		// get current system time
	sn[0] = '\0';

	strcat(sn, "REQ_");

	strcat(sn, number);
	strcat(sn, "_");

	itoa(lt.wYear, temp, 10);
	strcat(sn, temp);

	if (lt.wMonth < 10)
		strcat(sn, "0");
	itoa(lt.wMonth, temp, 10);
	strcat(sn, temp);

	if (lt.wDay < 10)
		strcat(sn, "0");
	itoa(lt.wDay, temp, 10);
	strcat(sn, temp);

	if (lt.wHour < 10)
		strcat(sn, "0");
	itoa(lt.wHour, temp, 10);
	strcat(sn, temp);

	if (lt.wMinute < 10)
		strcat(sn, "0");
	itoa(lt.wMinute, temp, 10);
	strcat(sn, temp);

//	if (lt.wSecond < 10)
//		strcat(sn, "0");
//	itoa(lt.wSecond, temp, 10);
//	strcat(sn, temp);
}

void
CutStrBlanks(
	char		*dst,
	char const	*src
	)
{
	char	ch;

	while((ch = *src) != '\0')
	{
		if (ch != ' ')
			*dst++ = ch;
		src++;
	}
	*dst = '\0';
}


void
CutStrNumBlanks(
	char		*str
	)
{
	char	ch;
	char	isNum = 0;
	char	*pCurrent = str;
	char	*pCheck = str;
	char	*pMov1, *pMov2;

	while((ch = *pCheck) != '\0')
	{
		if (isNum)
		{
			if (ch == ' ')
				pCheck++;
			else if (ch >= '0' && ch <= '9')
			{
				//forward move data
				pMov1 = pCurrent;
				pMov2 = pCheck;
				do
				{
					*pMov1++ = *pMov2++;
				}
				while(*pMov2 != '\0');
				*pMov1 = *pMov2;

				pCurrent++;
				pCheck = pCurrent;
			}
			else
			{
				isNum = 0;
				pCheck++;
			}
		}
		else
		{
			if (ch >= '0' && ch <= '9')
				isNum = 1;		//number state
			
			pCheck++;
			pCurrent = pCheck;
		}
	}//end while
}

int
MakeWavFile(
	LINESTRUCT *line
	)
{
	char	save_path[128];

	save_path[0] = '\0';
	strcat(save_path, gCfg.save_dir);
	strcat(save_path, line->Sn);
	strcat(save_path, ".wav");

	append_wav_head(save_path);
	if (line->State == ST_RECORD_NAME)
	{
		append_raw_data(save_path, line->RecordNameFile);
	}
	//else if (line->State == ST_RECORD_PLACE)
	//{
		//append_raw_data(save_path, line->RecordNameFile);
		//	append_raw_data(save_path, line->RecordPlaceFile);
	//}
	else if (line->State == ST_RECORD_TROUBLE)
	{
		append_raw_data(save_path, line->RecordNameFile);
		//	append_raw_data(save_path, line->RecordPlaceFile);
		append_raw_data(save_path, line->RecordTroubleFile);
	}
	else
	{
		append_raw_data(save_path, line->RecordNameFile);
		//	append_raw_data(save_path, line->RecordPlaceFile);
		append_raw_data(save_path, line->RecordTroubleFile);
	}
	append_wav_tail(save_path);

	return 0;
}


//#pragma argsused
BOOL WINAPI yzInitSystem(void)
{
	int			i;
	SYSTEMTIME	lt;
	FILE		*fp_log;
	int			ret = 0;
	char		chNum[8];
	char		fileHead[128];
#if IS_DEBUG
	char		place[256];
#endif// IS_DEBUG
	
	if (GetParaConfig(&gCfg) != 0)
	{
		MessageBox(hGWnd, "Load Config File FAIL", "Message", MB_OK);
		return FALSE;
	}

	fp_log = fopen(gCfg.log_file, "a");
	if (fp_log == NULL)
	{
		MessageBox(hGWnd, "Open Log File FAIL.", "Message", MB_OK);
		return FALSE; 
	}
	fprintf(fp_log, "\n==================================================\n");

#if IS_DEBUG

	ret = xml_parser(XML_PLACE_FILE, place);
	CutStrNumBlanks(place);

	ret = DataBaseInsert("陆履平", "勤学楼 1308", "我 已经 通过 类似 机 密码 尽快 来源 我的 平台 几乎", "200909071601");

	SWIOpen();
	SWIrecognizer(RECORD_PLACE_FILE, 6, REC_PLACE_FILE, XML_PLACE_FILE);
	SWIClose();	

	MakeWavFile("1234");

#endif// IS_DEBUG

	GetLocalTime(&lt);
	fprintf(fp_log, "System Init Time:%d/%d/%d %d:%d:%d\n", lt.wMonth, lt.wDay, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);

	GetVoicePath();
	ret = LoadDRV();
	fprintf(fp_log, "LoadDRV() return:%d\n", ret);
	if (ret != 0) 
	{
		MessageBox(hGWnd, "Load driver FAIL", "Message", MB_OK);
		fprintf(fp_log, "==================================================\n");
		fclose(fp_log);
		return FALSE;
	}

	TotalLine = CheckValidCh();
//	TotalLine = 2;
	fprintf(fp_log, "CheckValidCh() return:%d\n", TotalLine);

	ret = EnableCard(TotalLine, 1024 * 8);
	fprintf(fp_log, "EnableCard(%d) return:%d\n", TotalLine, ret);
	if (ret != 0) 
	{
		FreeDRV();
		fprintf(fp_log, "FreeDRV()\n");
		MessageBox(hGWnd, "Enable Board FAIL", "Message", MB_OK);
		fprintf(fp_log, "==================================================\n");
		fclose(fp_log);
		return FALSE;
	}

	Lines=new LINESTRUCT[TotalLine];
	SetBusyPara(350);
	fprintf(fp_log, "SetBusyPara(350)\n");

	for(i = 0; i < TotalLine; i++)
	{
        Lines[i].nType = CheckChTypeNew(i);
		//Lines[i].nType = CHTYPE_USER;
		fprintf(fp_log, "CheckChTypeNew(%d) return:%d\n", i, Lines[i].nType);
		Lines[i].State = ST_FREE;
		Lines[i].CallerID[0] = '\0';
		Lines[i].Dtmf[0] = '\0';
		Lines[i].nTimeElapse = 0;
		Lines[i].StrTime[0] = '\0';
		Lines[i].StrCaller[0] = '\0';
		Lines[i].StrUser[0] = '\0';

		itoa(i, chNum, 10);
		fileHead[0] = '\0';
		strcat(fileHead, OUTPUT_PATH);
		strcat(fileHead, chNum);
		strcat(fileHead, "_");

		Lines[i].RecordNameFile[0] = '\0';
		strcat(Lines[i].RecordNameFile, fileHead);
		strcat(Lines[i].RecordNameFile, "name.alaw");

		Lines[i].RecordPlaceFile[0] = '\0';
		strcat(Lines[i].RecordPlaceFile, fileHead);
		strcat(Lines[i].RecordPlaceFile, "place.alaw");

		Lines[i].RecordTroubleFile[0] = '\0';
		strcat(Lines[i].RecordTroubleFile, fileHead);
		strcat(Lines[i].RecordTroubleFile, "trouble.alaw");

		Lines[i].RecNameFile[0] = '\0';
		strcat(Lines[i].RecNameFile, fileHead);
		strcat(Lines[i].RecNameFile, "name.ulaw");

		Lines[i].RecPlaceFile[0] = '\0';
		strcat(Lines[i].RecPlaceFile, fileHead);
		strcat(Lines[i].RecPlaceFile, "place.ulaw");

		Lines[i].RecTroubleFile[0] = '\0';
		strcat(Lines[i].RecTroubleFile, fileHead);
		strcat(Lines[i].RecTroubleFile, "trouble.ulaw");

		Lines[i].XmlNameFile[0] = '\0';
		strcat(Lines[i].XmlNameFile, fileHead);
		strcat(Lines[i].XmlNameFile, "name.xml");

		Lines[i].XmlPlaceFile[0] = '\0';
		strcat(Lines[i].XmlPlaceFile, fileHead);
		strcat(Lines[i].XmlPlaceFile, "place.xml");

		Lines[i].XmlTroubleFile[0] = '\0';
		strcat(Lines[i].XmlTroubleFile, fileHead);
		strcat(Lines[i].XmlTroubleFile, "trouble.xml");
	}//end for

	ret = Sig_Init(0);
	fprintf(fp_log, "Sig_Init(0) return:%d\n", ret);

	// init colume
	static int ColumnWidth[60] = {66, 78, 200, 72, 75, 100, 50, 150};
	LV_COLUMN lvc;

	lvc.mask = (LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM);

	lvc.iSubItem = 0;
	LoadString(hInst, IDS_CHANNEL_NO, sTmp, 128);
	lvc.pszText = sTmp;
	lvc.cx = ColumnWidth[0];
	ListView_InsertColumn(GetDlgItem(hGWnd,IDC_STATELIST), 0, &lvc);

	lvc.iSubItem = 1;
	LoadString(hInst, IDS_CHANNEL_TYPE, sTmp, 128);
	lvc.pszText = sTmp;
	lvc.cx = ColumnWidth[1];
	ListView_InsertColumn(GetDlgItem(hGWnd,IDC_STATELIST), 1, &lvc);
	
	lvc.iSubItem = 2;
	LoadString(hInst, IDS_FLOW, sTmp, 128);
	lvc.pszText = sTmp;
	lvc.cx = ColumnWidth[2];
	ListView_InsertColumn(GetDlgItem(hGWnd,IDC_STATELIST), 2, &lvc);

	lvc.iSubItem = 3;
	LoadString(hInst, IDS_CALLER_ID, sTmp, 128);
	lvc.pszText = sTmp;
	lvc.cx = ColumnWidth[3];
	ListView_InsertColumn(GetDlgItem(hGWnd,IDC_STATELIST), 3, &lvc);

	lvc.iSubItem = 4;
	LoadString(hInst, IDS_TIME, sTmp, 128);
	lvc.pszText = sTmp;
	lvc.cx = ColumnWidth[4];
	ListView_InsertColumn(GetDlgItem(hGWnd,IDC_STATELIST), 4, &lvc);

	// init line item
	LV_ITEM		lvi;
	char		dig[10];

	for(i = 0; i < TotalLine; i++)	
	{
		lvi.mask = LVIF_TEXT;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		itoa(i, dig, 10); 
		lvi.pszText = dig;
		ListView_InsertItem(GetDlgItem(hGWnd,IDC_STATELIST), &lvi);

		if(Lines[i].nType == CHTYPE_TRUNK)
		{
			LoadString(hInst, IDS_TRUNK, sTmp, 128);
			ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), i, 1, sTmp);
		}
		else if(Lines[i].nType == CHTYPE_USER)
		{
			LoadString(hInst, IDS_USER, sTmp, 128);
			ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), i, 1, sTmp);
		}
		else if(Lines[i].nType == CHTYPE_RECORD)
		{
			LoadString(hInst, IDS_RECORD, sTmp, 128);
			ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), i, 1, sTmp);
		}
		else if(Lines[i].nType == CHTYPE_EMPTY)
		{
			LoadString(hInst, IDS_EMPTY, sTmp, 128);
			ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), i, 1, sTmp);
		}
	}//end for

	ret = SWIOpen();
	fprintf(fp_log, "SWIOpen() return:%d\n", ret);
	if (ret != 0)		//open SWI recognizer
	{
		FreeDRV();
		fprintf(fp_log, "FreeDRV()\n");
		MessageBox(hGWnd, "SWI open failed.", "Message", MB_OK);
		fprintf(fp_log, "==================================================\n");
		fclose(fp_log);
		return FALSE;
	}
	fprintf(fp_log, "==================================================\n\n");

	fclose(fp_log);

	return TRUE;
}

void WINAPI yzExitSystem(void)
{
	FILE		*fp_log;
	SYSTEMTIME	lt;

	fp_log = fopen(gCfg.log_file, "a");
	fprintf(fp_log, "\n==================================================\n");

	SWIClose();		//close SWI recognizer
	fprintf(fp_log, "SWIClose()\n");

	DisableCard();
	fprintf(fp_log, "DisableCard()\n");

	FreeDRV();
	fprintf(fp_log, "FreeDRV()\n");

	
	GetLocalTime(&lt);
	fprintf(fp_log, "System close Time:%d/%d/%d %d:%d:%d\n", lt.wMonth, lt.wDay, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);

	fprintf(fp_log, "==================================================\n\n");
	fclose(fp_log);

	delete Lines;
}

void WINAPI yzDrawState(int trkno)
{	
	char	state[100];
	char	tmpstr[100];

	switch(Lines[trkno].State) 
	{
		case ST_FREE:
			LoadString(hInst, IDS_CH_FREE, state, 100);
			break;

		case ST_RECEIVEID:
			LoadString(hInst, IDS_CH_RECEIVEID, state, 100);
			break;	
			
		case ST_WELCOME:
			LoadString(hInst, IDS_CH_WELCOME, state, 100);
			break;

		case ST_OFFHOOK:
			LoadString(hInst, IDS_CH_OFFHOOK, state, 100);
			break;

		case ST_PLAY_NAME:
		case ST_PLAY_PLACE:
		case ST_PLAY_TROUBLE:
		case ST_RECORD_NAME_START:
		case ST_RECORD_PLACE_START:
		case ST_RECORD_TROUBLE_START:
			LoadString(hInst, IDS_PLAY_HELP, state, 100);
			break;

		case ST_RECORD_NAME:
		case ST_RECORD_PLACE:
		case ST_RECORD_TROUBLE:
			LoadString(hInst, IDS_RECORD, state, 100);
			break;

		case ST_REC_NAME:
		case ST_REC_PLACE:
		case ST_REC_TROUBLE:
			LoadString(hInst, IDS_RECOGNIZING, state, 100);
			break;

		case ST_GOODBYE:
			LoadString(hInst, IDS_PLAY_GOODBYE, state, 100);
			break;

		case ST_WAIT_ONHOOK:
			LoadString(hInst, IDS_WAIT_ONHOOK, state, 100);
			break;

		default:
			LoadString(hInst, IDS_UNKNOW, state, 100);
			break;

	}//end switch
	
	ListView_GetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 2, tmpstr, 99) ;
	if (strcmp(state, tmpstr) != 0) 
		ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 2, state);

	strcpy(state, Lines[trkno].StrCaller) ;

	ListView_GetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 3, tmpstr, 99) ;
	if (strcmp(state, tmpstr) != 0) 
		ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 3, state);

	strcpy(state,Lines[trkno].StrTime);

	ListView_GetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 4, tmpstr, 99) ;
	if (strcmp(state, tmpstr) != 0) 
		ListView_SetItemText(GetDlgItem(hGWnd,IDC_STATELIST), trkno, 4, state);
}

void yzResetChannel(int channelID)
{
	if(Lines[channelID].nType == CHTYPE_TRUNK)
	{
		HangUp(channelID);
		Sig_ResetCheck(channelID);
		StartSigCheck(channelID);
		//StopSigCheck(channelID);
	}
	Lines[channelID].State = ST_FREE;
	Lines[channelID].CallerID[0] = '\0';
	Lines[channelID].Dtmf[0] = '\0';
	Lines[channelID].nTimeElapse = 0;
	Lines[channelID].StrTime[0] = '\0';
	Lines[channelID].StrCaller[0] = '\0';
	Lines[channelID].StrUser[0] = '\0';

	Lines[channelID].Name[0] = '\0';
	Lines[channelID].Place[0] = '\0';
	Lines[channelID].Trouble[0] = '\0';
	Lines[channelID].Sn[0] = '\0';
	Lines[channelID].pCaller = NULL;
}

char yzConvertDtmf(int ch)
{
	char c;

	switch(ch)
    {
		case 10:
			c = '0';
			break;
		case 11:
			c = '*';
			break;
		case 12:
			c = '#';
			break;
        case 13:
        case 14:
        case 15:
            c = ch - 13 + 'a';
            break;
        case 0:
            c = 'd';
            break;
		default:
			c = ch + '0';//change DTMF from number to ASCII
	}

	return c;
}

void WINAPI yzDoWork(void)
{
	char		isRecordStart = 0;
	int			ret;
	static FILE		*fp_log = NULL;
	static int	chValidCnt = 0;

	PUSH_PLAY();
	FeedSigFunc();

    for(int i=0; i < TotalLine; i++)
    {
        yzDrawState(i);

        switch(Lines[i].State)
        {
  
			case ST_FREE:
				if(RingDetect(i))
				{
					chValidCnt++;
					if (fp_log == NULL)
					{
						fp_log = fopen(gCfg.log_file, "a");
						fprintf(fp_log, "\n==================================================\n");
					}

					fprintf(fp_log, "Channel:%d State:ST_FREE Ring Detected.\n", i);
					if(Lines[i].nType == CHTYPE_USER)
						Lines[i].State = ST_OFFHOOK;
					else 
					{
						Lines[i].State = ST_RECEIVEID;
						ResetCallerIDBuffer(i);
						Lines[i].pCaller = Lines[i].StrCaller;
						StartTimer(i, 4);
						fprintf(fp_log, "Channel:%d State:ST_RECEIVEID\n", i);
					}
				}
				break;

			case ST_RECEIVEID:
				if (ElapseTime(i, 4) > 1000)
				{
					OffHook(i);
					StartSigCheck(i);
					Lines[i].State = ST_OFFHOOK;
					fprintf(fp_log, "Channel:%d State:ST_OFFHOOK\n", i);
				}
				else
				{
					if (DtmfHit(i))
					{
						int		num = 0;

						num = GetDtmfCode(i);
						if (num != -1)
						{
							*Lines[i].pCaller++ = yzConvertDtmf(num);
						}
						else if (Lines[i].pCaller != Lines[i].StrCaller)
						{
							*Lines[i].pCaller = '\0';
							OffHook(i);
							StartSigCheck(i);
							fprintf(fp_log, "Channel:%d DTMF receive ok.\n", i);
							Lines[i].State = ST_OFFHOOK;
							fprintf(fp_log, "Channel:%d State:ST_OFFHOOK\n", i);
						}
					}
					else
					{
						bool bOffHook = false;
						
						int a = GetCallerIDStrEx(i, Lines[i].StrTime, Lines[i].StrCaller, Lines[i].StrUser);
						if(a == 3 || a == 4)
						{
							bOffHook = true;
						}
						//else if(a == 4)
						//{
						//	LoadString(hInst, IDS_CallerID_ERROR, Lines[i].StrCaller, 32);
						//	bOffHook = true;
						//}
						
						if(bOffHook)
						{
							OffHook(i);
							StartSigCheck(i);
							Lines[i].State = ST_OFFHOOK;
							fprintf(fp_log, "Channel:%d State:ST_OFFHOOK\n", i);
						}
					}	
				}
				break;

			case ST_OFFHOOK:
				InitDtmfBuf(i);		//clear DTMF buffer

				// get current time as the sequence number
				GetReqSn(Lines[i].Sn, Lines[i].StrCaller);
				//fprintf(fp_log, "Channel number:%d\n", i);
				fprintf(fp_log, "Channel:%d Set seqence number:%s\n", i, Lines[i].Sn);

				// play welcome 
				ret = StartPlayFile(i, PLAY_WELCOME_FILE, 0L);
				fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_WELCOME_FILE) return:%d\n", i, ret);
				Lines[i].State = ST_WELCOME;
				fprintf(fp_log, "Channel:%d State:ST_WELCOME Playing welcome voice...\n", i);
				break;

			case ST_WELCOME:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);
					
					ret = StartPlayFile(i, PLAY_NAME_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_NAME_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_NAME;
					fprintf(fp_log, "Channel:%d State:ST_PLAY_NAME Playing name voice...\n", i);
				}
				break;

			case ST_PLAY_NAME:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);

					// play name information
					ret = StartPlayFile(i, PLAY_DI_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_DI_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_RECORD_NAME_START;
				}
				break;

			case ST_RECORD_NAME_START:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);
			
					// playing end, start recording building information
					ret = StartRecordFile(i, Lines[i].RecordNameFile, 8000L * gCfg.record_max_sec);	// max recording length is 10 seconds
					fprintf(fp_log, "Channel:%d StartRecordFile(%s) return:%d\n", i, Lines[i].RecordNameFile, ret);
					Lines[i].State = ST_RECORD_NAME;
					isRecordStart = 0;
					fprintf(fp_log, "Channel:%d State:ST_RECORD_NAME Recording name...\n", i);
				}
				break;

			case ST_RECORD_NAME:
				if (CheckRecordEnd(i) || CheckSilence(i, 31 * gCfg.record_name_sec) == 0)
				{
					StopRecordFile(i);
					Lines[i].State = ST_REC_NAME;
					fprintf(fp_log, "Channel:%d State:ST_REC_NAME Recognizing name...\n", i);
				}
				break;

			case ST_REC_NAME:
				// recognize the a-law file	
				if (SWIrecognizer(Lines[i].RecordNameFile, 3, Lines[i].RecNameFile, Lines[i].XmlNameFile) == 0
					&& xml_parser(Lines[i].XmlNameFile, Lines[i].Name) == 0)
				{
					// play place information
					ret = StartPlayFile(i, PLAY_TROUBLE_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_TROUBLE_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_TROUBLE;
					fprintf(fp_log, "Channel:%d State:ST_PLAY_TROUBLE Playing trouble voice...\n", i);
				}
				else
				{
					// play name information
					ret = StartPlayFile(i, PLAY_NAME_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_NAME_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_NAME;
				}
				break;

			case ST_PLAY_PLACE:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);

					// play di...
					ret = StartPlayFile(i, PLAY_DI_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_DI_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_RECORD_PLACE_START;
				}
				break;

			case ST_RECORD_PLACE_START:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);
			
					// playing end, start recording place information
					ret = StartRecordFile(i, Lines[i].RecordPlaceFile, 8000L * gCfg.record_max_sec);	// max recording length is 10 seconds
					fprintf(fp_log, "Channel:%d StartRecordFile(%s) return:%d\n", i, Lines[i].RecordPlaceFile, ret);
					Lines[i].State = ST_RECORD_PLACE;
					isRecordStart = 0;
					fprintf(fp_log, "Channel:%d State:ST_RECORD_PLACE Recording place...\n", i);
				}
				break;

			case ST_RECORD_PLACE:
				if (CheckRecordEnd(i) || CheckSilence(i, 31 * gCfg.record_building_sec) == 0)
				{
					StopRecordFile(i);
					Lines[i].State = ST_REC_PLACE;
					fprintf(fp_log, "Channel:%d State:ST_REC_PLACE Recognizing place...\n", i);
				}
				break;

			case ST_REC_PLACE:
				// recognize the a-law file	
				if (SWIrecognizer(Lines[i].RecordPlaceFile, 6, Lines[i].RecPlaceFile, Lines[i].XmlPlaceFile) == 0
					&& xml_parser(Lines[i].XmlPlaceFile, Lines[i].Place) == 0)
				{
					CutStrNumBlanks(Lines[i].Place);

					// play trouble information
					ret = StartPlayFile(i, PLAY_TROUBLE_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_TROUBLE_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_TROUBLE;
					fprintf(fp_log, "Channel:%d State:ST_PLAY_TROUBLE Playing trouble voice...\n", i);
				}
				else
				{
					// play place information
					ret = StartPlayFile(i, PLAY_PLACE_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_PLACE_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_PLACE;
				}
				break;

			case ST_PLAY_TROUBLE:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);

					// play trouble information
					ret = StartPlayFile(i, PLAY_DI_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_DI_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_RECORD_TROUBLE_START;
				}
				break;

			case ST_RECORD_TROUBLE_START:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);
			
					// playing end, start recording trouble information
					ret = StartRecordFile(i, Lines[i].RecordTroubleFile, 8000L * gCfg.record_max_sec);	// max recording length is 10 seconds
					fprintf(fp_log, "Channel:%d StartRecordFile(%s) return:%d\n", i, Lines[i].RecordTroubleFile, ret);
					Lines[i].State = ST_RECORD_TROUBLE;
					isRecordStart = 0;
					fprintf(fp_log, "Channel:%d State:ST_RECORD_TROUBLE Recording trouble...\n", i);
				}
				break;

			case ST_RECORD_TROUBLE:
				if (CheckRecordEnd(i) || CheckSilence(i, 31 * gCfg.record_trouble_sec) == 0)
				{
					StopRecordFile(i);
					Lines[i].State = ST_REC_TROUBLE;
					fprintf(fp_log, "Channel:%d State:ST_REC_TROUBLE Recognizing trouble...\n", i);
				}
				break;

			case ST_REC_TROUBLE:
				// recognize the a-law file	
				if (SWIrecognizer(Lines[i].RecordTroubleFile, 5, Lines[i].RecTroubleFile, Lines[i].XmlTroubleFile) == 0
					&& xml_parser(Lines[i].XmlTroubleFile, Lines[i].Trouble) == 0)
				{
					//make wav file of the input audio
					fprintf(fp_log, "Channel:%d Make wav file...", i);
					ret = MakeWavFile(&Lines[i]);
					fprintf(fp_log, "Channel:%d MakeWavFile() return:%d\n", i, ret);

					// get recgonization result
					fprintf(fp_log, "Channel:%d Get recognizing result...\n", i);
					fprintf(fp_log, "Channel:%d name:%s place:%s trouble:%s\n", i, Lines[i].Name, Lines[i].Place, Lines[i].Trouble);

					// save to database, start a requestion.
					fprintf(fp_log, "Channel:%d Write information into database...\n", i);
					ret = DataBaseInsert(Lines[i].Name, Lines[i].Place, Lines[i].Trouble, Lines[i].Sn);
					fprintf(fp_log, "Channel:%d DataBaseInsert() return:%d\n", i, ret);

					// play goodbye information
					ret = StartPlayFile(i, PLAY_GOODBYE_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_GOODBYE_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_GOODBYE;
					fprintf(fp_log, "Channel:%d State:ST_GOODBYE Playing goodbye voice...\n", i);
				}
				else
				{
					// play trouble information
					ret = StartPlayFile(i, PLAY_TROUBLE_FILE, 0L);
					fprintf(fp_log, "Channel:%d StartPlayFile(PLAY_TROUBLE_FILE) return:%d\n", i, ret);
					Lines[i].State = ST_PLAY_TROUBLE;
				}
				break;

			case ST_GOODBYE:
				if(CheckPlayEnd(i))
				{
					StopPlayFile(i);
					Lines[i].State = ST_WAIT_ONHOOK;
					fprintf(fp_log, "Channel:%d State:ST_WAIT_ONHOOK Waiting onhook...\n", i);
				}
				break;

			case ST_WAIT_ONHOOK:
				if(!RingDetect(i))
				{
					StartPlaySignal(i,SIG_STOP);
					yzResetChannel(i);

					chValidCnt--;
					fprintf(fp_log, "Channel:%d State:ST_FREE ResetChannel and check busy signal...\n", i);
					//fprintf(fp_log, "==================================================\n");
					//fclose(fp_log);
					//fp_log = NULL;				
				}
				break;

			default:
				break;
		}//end switch

		if(Lines[i].nType == CHTYPE_TRUNK && Lines[i].State != ST_FREE)
		{
			if (Sig_CheckBusy(i))
			{
				switch(Lines[i].State)
				{
					case ST_WELCOME:
					case ST_PLAY_NAME:
					case ST_RECORD_NAME_START:
					case ST_PLAY_PLACE:
					case ST_RECORD_PLACE_START:
					case ST_PLAY_TROUBLE:
					case ST_RECORD_TROUBLE_START:
					case ST_GOODBYE:
						StopPlayFile(i);
						fprintf(fp_log, "Channel:%d Playing stoped.\n", i);
						break;
					case ST_RECORD_NAME:
						StopRecordFile(i);
						fprintf(fp_log, "Channel:%d Recording stoped.\n", i);

						// recognize the a-law file
						SWIrecognizer(Lines[i].RecordNameFile, 3, Lines[i].RecNameFile, Lines[i].XmlNameFile) == 0
							&& xml_parser(Lines[i].XmlNameFile, Lines[i].Name);
						
						//make wav file of the input audio
						fprintf(fp_log, "Channel:%d Make wav file...", i);
						ret = MakeWavFile(&Lines[i]);
						fprintf(fp_log, "Channel:%d MakeWavFile() return:%d\n", i, ret);

						// get recgonization result
						fprintf(fp_log, "Channel:%d Get recognizing result...\n", i);
						fprintf(fp_log, "Channel:%d name:%s place:%s trouble:%s\n", i, Lines[i].Name, Lines[i].Place, Lines[i].Trouble);

						// save to database, start a requestion.
						fprintf(fp_log, "Channel:%d Write information into database...\n", i);
						ret = DataBaseInsert(Lines[i].Name, Lines[i].Place, Lines[i].Trouble, Lines[i].Sn);
						fprintf(fp_log, "Channel:%d DataBaseInsert() return:%d\n", i, ret);
						break;

					case ST_RECORD_PLACE:
						StopRecordFile(i);
						fprintf(fp_log, "Channel:%d Recording stoped.\n", i);

						// recognize the a-law file	
						SWIrecognizer(Lines[i].RecordPlaceFile, 6, Lines[i].RecPlaceFile, Lines[i].XmlPlaceFile) == 0
							&& xml_parser(Lines[i].XmlPlaceFile, Lines[i].Place);

						//make wav file of the input audio
						fprintf(fp_log, "Channel:%d Make wav file...", i);
						ret = MakeWavFile(&Lines[i]);
						fprintf(fp_log, "Channel:%d MakeWavFile() return:%d\n", i, ret);

						// get recgonization result
						fprintf(fp_log, "Channel:%d Get recognizing result...\n", i);
						fprintf(fp_log, "Channel:%d name:%s place:%s trouble:%s\n", i, Lines[i].Name, Lines[i].Place, Lines[i].Trouble);

						// save to database, start a requestion.
						fprintf(fp_log, "Channel:%d Write information into database...\n", i);
						ret = DataBaseInsert(Lines[i].Name, Lines[i].Place, Lines[i].Trouble, Lines[i].Sn);
						fprintf(fp_log, "Channel:%d DataBaseInsert() return:%d\n", i, ret);
						break;

					case ST_RECORD_TROUBLE:
						StopRecordFile(i);
						fprintf(fp_log, "Channel:%d Recording stoped.\n", i);

						// recognize the a-law file	
						SWIrecognizer(Lines[i].RecordTroubleFile, 5, Lines[i].RecTroubleFile, Lines[i].XmlTroubleFile) == 0
							&& xml_parser(Lines[i].XmlTroubleFile, Lines[i].Trouble);

						//make wav file of the input audio
						fprintf(fp_log, "Channel:%d Make wav file...", i);
						ret = MakeWavFile(&Lines[i]);
						fprintf(fp_log, "Channel:%d MakeWavFile() return:%d\n", i, ret);

						// get recgonization result
						fprintf(fp_log, "Channel:%d Get recognizing result...\n", i);
						fprintf(fp_log, "Channel:%d name:%s place:%s trouble:%s\n", i, Lines[i].Name, Lines[i].Place, Lines[i].Trouble);

						// save to database, start a requestion.
						fprintf(fp_log, "Channel:%d Write information into database...\n", i);
						ret = DataBaseInsert(Lines[i].Name, Lines[i].Place, Lines[i].Trouble, Lines[i].Sn);
						fprintf(fp_log, "Channel:%d DataBaseInsert() return:%d\n", i, ret);
						break;
					default:
						break;
				}
				yzResetChannel(i);

				chValidCnt--;
				fprintf(fp_log, "Channel:%d State:ST_FREE ResetChannel and check busy signal...\n", i);
				//fprintf(fp_log, "==================================================\n");
				//fclose(fp_log);
				//fp_log = NULL;
			}
		}//end if

		if (chValidCnt == 0 && fp_log != NULL)
		{
			fprintf(fp_log, "==================================================\n");
			fclose(fp_log);
			fp_log = NULL;
		}
	}//end for
}

void GetVoicePath(void)
{
	char	FileName[100];

	GetWindowsDirectory(FileName, 100);
	strcat(FileName, "\\tc08a-v.ini");
	GetPrivateProfileString("SYSTEM", "InstallDir", NULL, VoicePath, 100, FileName);
	strcat(VoicePath, "voc\\");
}