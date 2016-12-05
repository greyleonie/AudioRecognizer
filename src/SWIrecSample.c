#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>

#include "include/SWIrecAPI.h"
#include "include/SWIepAPI.h"

/* Windows NT */
#include <windows.h>
#include <process.h>


typedef unsigned int (__stdcall *ThreadFunc)(void* userData);

#define RETURN_TYPE unsigned int
#define MAX_AUDIO_BUF_LEN    2000   
#define MAX_EP_AUDIO_BUF_LEN 2000
#define usleep(x) Sleep(x / 1000)
#define DEFINE_THREAD_FUNC(funcName, userData) \
        unsigned int (__stdcall funcName)(void *userData)
#define Thread HANDLE
//设置语法
#define GRAM					L"./grammar/trouble_report.grxml"
#define GRAM_BUILDING			L"./grammar/building.grxml"
#define GRAM_DEPARTMENT			L"./grammar/department.grxml"
#define GRAM_NAME				L"./grammar/name.grxml"
#define GRAM_NUMBER				L"./grammar/number.grxml"
#define GRAM_TROUBLE			L"./grammar/trouble.grxml"
#define	GRAM_PLACE				L"./grammar/place.grxml"


/*  "audio/basic" 8-bit 8 KHz u-law encoding [unsigned char *]
	"audio/x-alaw-basic" 8-bit 8 KHz A-law encoding [unsigned char *]
	"audio/L16;rate=8000" 16-bit 8 KHz linear encoding [short *]*/
//#define TYPE L"audio/basic"
#define	TYPE			L"audio/x-alaw-basic"		//8-bit 8 kHz A-law encoding 

//#define RECORD_ALAW		"src/output/record.alaw"
//#define	RESULT_XML		"src/output/result.xml"
//#define	RESULT_ULAW		"src/output/result.ulaw"


int ThreadCreate(Thread* thread, ThreadFunc start_function, void* arg)
{
  *thread = (Thread)_beginthreadex(NULL, 0, start_function, arg, 0, NULL);
  return 0;
}

int ThreadJoin(Thread* thread, void* status)
{
  int rc;

  switch (WaitForSingleObject(*thread, INFINITE)) {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
      rc = 0;
      break;
    case WAIT_TIMEOUT:
    default:
      rc = 1;
      break;
  }
  CloseHandle(*thread);
  return rc;
}

typedef struct {
  SWIrecRecognizer *rec;
  const char *filename;
  SWIrecFuncResult ret;
} audioThreadData;

SWIrecFuncResult start_audio_write(SWIrecRecognizer *, Thread *,
    const char *, audioThreadData **);
SWIrecFuncResult finish_audio_write(Thread *, audioThreadData *);
DEFINE_THREAD_FUNC(audio_write, rec);

/**************************************************************************/

void check_rc (char * func_name, int rc)
{
  switch (rc)  {
    case SWIrec_SUCCESS:
      printf("\n Function %s returned SUCCESS.\n", func_name);
      break;
    case SWIrec_ERROR_GENERIC_FAILURE:
      printf("\n Function %s returned FAILURE.\n", func_name);
      break;
    case SWIrec_ERROR_GENERIC_ERROR:
      printf("\n Function %s returned ERROR.\n", func_name);
      break;
    case SWIrec_ERROR_GENERIC_FATAL_ERROR:
      printf("\n Function %s returned FATAL ERROR.\n", func_name);
      break;
	case SWIrec_ERROR_NO_DATA:
	  printf("\n No data.\n", func_name);
      break;
    default:
      printf ("\n Function %s returned %d. \n", func_name, rc);
	  break;
  }

  if (rc != SWIrec_SUCCESS) {
    printf ("\n An Error Was Returned.  Exit Test Application. \n");
    //exit(1);
  }
}

void check_rec_status (SWIrecRecognizerStatus status)
{
  switch (status)  {
    case SWIrec_SUCCESS:
      printf("\n Recognizer status returned was SUCCESS.\n");
      break;
    case SWIrec_STATUS_NO_MATCH:
      printf("\n Recognizer status returned was NO_MATCH.\n");
      break;
    case SWIrec_STATUS_INCOMPLETE:
      printf("\n Recognizer status returned was INCOMPLETE.\n");
      break;
    case SWIrec_STATUS_NON_SPEECH_DETECTED:
      printf("\n Recognizer status returned was NON_SPEECH_DETECTED.\n");
      break;
    case SWIrec_STATUS_SPEECH_DETECTED:
      printf("\n Recognizer status returned was SPEECH_DETECTED.\n");
      break;
    case SWIrec_STATUS_SPEECH_COMPLETE:
      printf("\n Recognizer status returned was SPEECH_COMPLETE.\n");
      break;
    case SWIrec_STATUS_MAX_CPU_TIME:
      printf("\n Recognizer status returned was MAX_CPU_TIME.\n");
      break;
    case SWIrec_STATUS_MAX_SPEECH:
      printf("\n Recognizer status returned was MAX_SPEECH.\n");
      break;
    case SWIrec_STATUS_STOPPED:
      printf("\n Recognizer status returned was STOPPED.\n");
      break;
    default: /* should never get here */
      printf ("\n Internal error: invalid status from Compute()!\n");
      break;
  }
  return;
}

void check_parse_status (SWIrecParseStatus status)
{
  switch (status)  {
    case SWIrec_PARSE_COMPLETE:
      printf("\n Recognizer status returned was COMPLETE.\n");
      break;
    case SWIrec_PARSE_VALID_PREFIX:
      printf("\n Recognizer status returned was VALID PREFIX.\n");
      break;
    case SWIrec_PARSE_INCOMPLETE:
      printf("\n Recognizer status returned was INCOMPLETE.\n");
      break;
    case SWIrec_PARSE_INVALID:
      printf("\n Recognizer status returned was INVALID.\n");
      break;
    default: /* should never get here */
      printf ("\n Internal error: invalid status from ParseDTMF()!\n");
      break;
  }
  return;
}

int char2wchar(const char *str, wchar_t *wstr, int len)
{
  int l;
  int i;

  if (!str) {
    return -1;
  }

  l = strlen(str);

  if (l >= len) {
    return -1;
  }

  for (i = 0; i < l + 1; i++) {
    wstr[i] = str[i];
  }

  return 0;
}

char *W2C(const wchar_t *pw , char *pc)
{
	*pc++ = *(char *)pw >> 8;
	*pc = *(char *)pw;

	return 0 ;
} 

int wchar2char(const wchar_t *pwstr , char *pcstr, size_t len)
{
	char *ptemp = pcstr;

    if(pwstr!=NULL && pcstr!=NULL)
    {  
		size_t wstr_len = wcslen(pwstr);
	
		len = (len > wstr_len ) ? wstr_len : len;
	
		while( len-- > 0)
		{
			W2C(pwstr, pcstr);
			pwstr++;
			pcstr +=2;      
		}
        *pcstr = '\0';

        return 0;
	}

	return 0;
}

wchar_t *get_sample_baseline_cfg(void)
{
   wchar_t *env;
   static wchar_t recParams[1024];
   static wchar_t envbuff[1024];

   env = _wgetenv(L"SWISRSDK");
   if(!env) {
     printf ("\n ERROR. SWISRSDK environment variable not defined.\n");
     return NULL;
   }
   wcsncpy(envbuff, env, sizeof(envbuff)/2-1);
   wcscat(envbuff,L"\\config\\Baseline.xml");
   wcsncpy(recParams, envbuff, sizeof(recParams)/2-1);

   return recParams;
}

SWIrecFuncResult do_recognizer_compute(SWIrecRecognizer *rec,
    SWIrecResultData **resultData, int *finalStatus)
{
  SWIrecFuncResult rc;
  const int maxComputeTime = -1;
  SWIrecResultType type;
  SWIrecRecognizerStatus status;


  do {
    type = SWIrec_RESULT_TYPE_COMPLETE;
    status = SWIrec_STATUS_INCOMPLETE;
    *finalStatus = SWIrec_STATUS_NO_MATCH;
    rc = SWIrecRecognizerCompute(rec, maxComputeTime, &status, &type,
                                 resultData);
    if (rc != SWIrec_SUCCESS)
	{
      return rc;
	}
    printf ("\n Compute Status = %d", status);
    printf ("\n Compute Type = %d\n", type);
	} while ((status != SWIrec_STATUS_SUCCESS) &&
            (status != SWIrec_STATUS_NO_MATCH) &&
            (status != SWIrec_STATUS_STOPPED) &&
            (status != SWIrec_STATUS_MAX_CPU_TIME) &&
            (status != SWIrec_STATUS_MAX_SPEECH)
          ); /* These are recognizer termination statuses */
	return rc;
}

void write_file(const char *name, void *buffer, size_t length)
{
   FILE *fp = fopen(name, "wb");
   if (fp == NULL)
   {
     printf("\nERROR. Cannot open output file %s\n", name);
	 return;
   }
   fwrite(buffer, length, 1, fp);
   fclose(fp);
}

int do_swirec_sample(
	char const	*in_audio_file, 
	int			grammar, 
	char const	*out_audio_file,
	char const	*out_xml_file
	)
{
  /* Results */
//  wchar_t **keyList = NULL;
//  unsigned int numKeys = 0;
//  unsigned int asSize =0;
//  unsigned int numAnswers = 0;
//  unsigned int grammarSize = 0;

  /* audio state */
//  static char asBuffer[1000];

  /* session parameters */
  wchar_t *channelName = L"1";
  wchar_t *applicationName = L"SWIrecSample";

  /* swirec */
  SWIrecFuncResult rc;
  SWIrecRecognizer *rec = NULL;
  SWIrecGrammarData digits_grammar = {L"uri", GRAM, NULL };
  SWIrecResultData *resultData = NULL;
  int status = -1;

  /* audio thread */
  Thread audio_thread;
//  const char *test_filename;
  /* audio thread data */
  audioThreadData *atd = NULL;

  /* waveform */
  unsigned char *waveBuf;
  unsigned int waveLen = 0;

  /* XML result string */
  wchar_t *myXML = NULL;
  FILE		*fp_xml;
  int		ret = 0;

//load语法
	
  switch(grammar)
  {
	  case 1:
			digits_grammar.data = GRAM_BUILDING;
			break;
	  case 2:
			digits_grammar.data = GRAM_DEPARTMENT;
			break;
	  case 3:
			digits_grammar.data = GRAM_NAME;
			break;
	  case 4:
			digits_grammar.data = GRAM_NUMBER;
			break;
	  case 5:
			digits_grammar.data = GRAM_TROUBLE;
			break;
	  case 6:
			digits_grammar.data = GRAM_PLACE;
			break;
	  default:
			digits_grammar.data = GRAM;
			break;
  }


  printf ("\n Calling SWIrecRecognizerCreate.\n");
  rc = SWIrecRecognizerCreate(&rec, NULL, NULL);
  check_rc ("SWIrecRecognizerCreate", rc);

  printf ("\n Calling SWIrecSessionStart.\n");
  rc = SWIrecSessionStart(rec, channelName, L"");
  check_rc ("SWIrecSessionStart", rc);

  printf ("\n Calling SWIrecGrammarLoad on digits.\n");
  rc = SWIrecGrammarLoad(rec, &digits_grammar);
  check_rc ("SWIrecGrammarLoad", rc);


  /* Make digits active always */
  printf ("\n Calling SWIrecGrammarActivate on digits grammar.\n");
  rc = SWIrecGrammarActivate(rec, &digits_grammar, 1, "Digits_Grammar");
  check_rc ("SWIrecGrammarActivate", rc);

//	test_filename = in_filename;
    printf ("\n Calling SWIrecAcousticStateReset. \n");
    rc = SWIrecAcousticStateReset(rec);
    check_rc ("SWIrecAcousticStateReset", rc);

    printf ("\n Calling SWIrecRecognizerStart.\n");
    rc = SWIrecRecognizerStart(rec);
    check_rc ("SWIrecRecognizerStart", rc);

    printf ("\n Calling start_audio_write.\n");
    rc = start_audio_write(rec, &audio_thread, in_audio_file, &atd);
    check_rc ("start_audio_write", rc);

    printf ("\n Calling SWIrecRecognizerCompute.\n");
    rc = do_recognizer_compute(rec, &resultData, &status);
    check_rc ("SWIrecRecognizerCompute", rc);

    printf ("\n Calling SWIrecGetWaveform. \n");
    rc = SWIrecGetWaveform(resultData, NULL, (void **)&waveBuf, &waveLen);
    check_rc ("SWIrecGetWaveform", rc);
	if (rc == SWIrec_ERROR_NO_DATA)
	{
		ret = 1;
		goto ERROR_PROCESS;
	}
    printf("\n Waveform Len = %d \n", waveLen);
    write_file(out_audio_file, waveBuf, waveLen);

    printf ("\n Calling SWIrecGetXMLResult.\n");
    rc = SWIrecGetXMLResult(resultData, L"application/x-vnd.speechworks.emma+xml", &myXML);
    check_rc ("SWIrecGetXMLResult", rc);

    //wprintf(L"\n XML result is = %ls \n", myXML);

	fp_xml = fopen(out_xml_file, "w");
	if (fp_xml == NULL)
		return -1;
	fprintf(fp_xml, "%ls", myXML);
	fclose(fp_xml);

    printf ("\n Calling finish_audio_write.\n");
    rc = finish_audio_write(&audio_thread, atd);
    check_rc ("finish_audio_write", rc);
	if (atd) 
	{
	  free(atd);
	  atd = NULL;
	}

  printf ("\n Calling SWIrecLogEvent.\n");
  rc = SWIrecLogEvent(rec, L"SWIendp", L"BRGN|BTIM|UCPU=0|SCPU=0");
  check_rc ("SWIrecLogEvent", rc);

ERROR_PROCESS:

  printf ("\n Calling SWIrecGrammarDeactivate on digits grammar.\n");
  rc = SWIrecGrammarDeactivate(rec, &digits_grammar);
  check_rc ("SWIrecGrammarDeactivate", rc);


  printf ("\n Calling SWIrecGrammarFree on digits.\n");
  rc = SWIrecGrammarFree(rec, &digits_grammar);
  check_rc ("SWIrecGrammarFree", rc);

  printf ("\n Calling SWIrecRecognizerDestroy.\n");
  rc = SWIrecRecognizerDestroy(rec);
  check_rc ("SWIrecRecognizerDestroy", rc);

  return ret;
}

int SWIOpen(void)
{
	SWIrecFuncResult rc;

	setlocale(LC_ALL,"Chinese_China");

	/* Set the Baseline configuration file */
	printf ("\n Calling SWIrecInit.\n");
	rc = SWIrecInit(get_sample_baseline_cfg());
	check_rc ("SWIrecInit", rc);

	printf ("\n Calling SWIepInit.\n");
	if(SWIepInit() != SWIep_RESULT_SUCCESS) 
	{
		printf("\n SWIepInit failed\n");
		return -1;
	}

	return 0;
}

int SWIClose(void)
{
	SWIrecFuncResult rc;

	printf ("\n Calling SWIrecTerminate.\n");
	rc = SWIrecTerminate();
	check_rc ("SWIrecTerminate", rc);

	return 0;
}

int SWIrecognizer(
	char const	*in_audio_file, 
	int			grammar, 
	char const	*out_audio_file,
	char const	*out_xml_file
	)
{
	return do_swirec_sample(in_audio_file, grammar, out_audio_file, out_xml_file);
}

/**************************************************************************/
/*** Async audio ****/

/* doc that thread create/destroy for recognition is optional (see PIM page XXXX-XX) */

SWIrecFuncResult start_audio_write(SWIrecRecognizer *rec, Thread *audio_thread,
    const char *filename, audioThreadData **atdpp)
{

  audioThreadData *atd = calloc(sizeof(audioThreadData), 1);
  atd->rec = rec;
  atd->filename = filename;
  ThreadCreate(audio_thread, audio_write, atd);
  *atdpp = atd;
  return SWIrec_SUCCESS;
}

SWIrecFuncResult finish_audio_write(Thread *audio_thread, audioThreadData *atd)
{
  ThreadJoin(audio_thread, &atd);
  return SWIrec_SUCCESS;
}

/* return 1 on any failure
return 0 on success
*/
DEFINE_THREAD_FUNC(audio_write, arg)
{
  audioThreadData *atd = (audioThreadData *)arg;
  SWIrecFuncResult rc;
  FILE *fp;
  unsigned char buf[MAX_AUDIO_BUF_LEN];
  int len;
  SWIrecAudioSamples *samplesStruct;
  SWIepAudioSamples epSamplesStruct;
  SWIepDetector *swiep_handle;
  SWIepFuncResult eprc;
  SWIepState swiep_state;
  int begin, end;
  int num_samples_read = 0;
  int before_speech = 1;
   
  printf("\nAUDIO THREAD=> ENTER.\n");
  samplesStruct = (void *) malloc(sizeof(SWIrecAudioSamples));
  samplesStruct->samples = (void *) malloc(sizeof(unsigned char)*MAX_EP_AUDIO_BUF_LEN);
  samplesStruct->type = TYPE;
         
  if(SWIepDetectorCreate(&swiep_handle) != SWIep_RESULT_SUCCESS) {
    printf("AUDIO THREAD=> ERROR. Cannot create speech detector\n");
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
    return (RETURN_TYPE)1;
  }
  if(SWIepSessionStart(swiep_handle, L"SWIepSampleSession", NULL) != SWIep_RESULT_SUCCESS) {
    printf("AUDIO THREAD=> ERROR. Cannot start session on EP detector\n");
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
    return (RETURN_TYPE)1;
  }
  SWIepAcousticStateReset(swiep_handle);

//设置参数
//     printf ("\n Calling SWIepSetParameter for magic word mode. \n");
//     eprc = SWIepSetParameter(swiep_handle, L"swiep_mode", L"magic_word");
//     check_rc ("SWIepSetParameter", eprc);
//     printf ("\n Calling SWIepSetParameter for min magic word mode. \n");
//     eprc = SWIepSetParameter(swiep_handle, L"swiep_magic_word_min_msec", L"100");
//     check_rc ("SWIepSetParameter", eprc);
//     printf ("\n Calling SWIepSetParameter for max magic word mode. \n");
//     eprc = SWIepSetParameter(swiep_handle, L"swiep_magic_word_max_msec", L"800");
//     check_rc ("SWIepSetParameter", eprc);
//     printf ("\n Calling SWIepSetParameter for incomplete timeout. \n");
//     eprc = SWIepSetParameter(swiep_handle, L"incompletetimeout", L"250");
//     check_rc ("SWIepSetParameter", eprc);
//     eprc = SWIepSetParameter(swiep_handle, L"timeout", L"10000");
//     check_rc ("SWIepSetParameter", eprc);
  
  if(SWIepStart(swiep_handle) != SWIep_RESULT_SUCCESS) {
    printf("AUDIO THREAD=> ERROR. Cannot start endpointer\n");
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
    SWIepDetectorDestroy(swiep_handle);
    return (RETURN_TYPE)1;
  }
  /* Read in Audio File */
  fp = fopen(atd->filename, "rb");
  if (fp == NULL) {
    printf("AUDIO THREAD=> ERROR. Cannot open prompt file %s\n", atd->filename);
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
    SWIepDetectorDestroy(swiep_handle);
    return (RETURN_TYPE)1;
  }
   
  /* loop while there are still buffers to be picked up from file */
  while((len = fread(buf, 1, MAX_AUDIO_BUF_LEN, fp)) > 0) {
     
    /* call ep even after begin so it can adapt to audio */
    epSamplesStruct.samples = (void *) buf;
    epSamplesStruct.len     = len;
    epSamplesStruct.type    = TYPE;
    epSamplesStruct.status  = (0 == num_samples_read) ? SWIrec_SAMPLE_FIRST : SWIrec_SAMPLE_CONTINUE;

    num_samples_read += len;
    printf("AUDIO THREAD=> Read %d samples; total now %d\n", len, num_samples_read);

    /* begin and end values passed back from epWrite means nothing in epRead mode */
    if((eprc = SWIepWrite(swiep_handle,  &epSamplesStruct, &swiep_state, &begin, &end)) != SWIep_RESULT_SUCCESS) {
      printf("AUDIO THREAD=> ERROR. SWIepWrite returned %d\n", eprc);
      atd->ret = SWIrec_ERROR_GENERIC_ERROR;
      SWIepDetectorDestroy(swiep_handle);
      return (RETURN_TYPE)1;
    }

    if(swiep_state == SWIep_TIMEOUT) {
      printf("AUDIO THREAD=> ALERT: beginning of speech not found before BOS_timeout\n");
      atd->ret = SWIrec_ERROR_GENERIC_FAILURE;
      SWIepDetectorDestroy(swiep_handle);
      return (RETURN_TYPE)1;
    } else if(swiep_state == SWIep_AUDIO_ERROR) {
      printf("AUDIO THREAD=> ERROR: swiep audio error\n");
      atd->ret = SWIrec_ERROR_GENERIC_ERROR;
      SWIepDetectorDestroy(swiep_handle);
      return (RETURN_TYPE)1;
    }
    else {

        /* impl ep_read */

      printf ("AUDIO THREAD=>  Calling SWIepRead.\n");
      eprc =  SWIepRead(swiep_handle,samplesStruct,&swiep_state,MAX_EP_AUDIO_BUF_LEN);
      if(swiep_state == SWIep_IN_SPEECH){
        printf("epstate = IN SPEECH\n");
      }
      if(samplesStruct->status == SWIrec_SAMPLE_FIRST){       
        printf("AUDIO THREAD=> Begin of speech found at sample %d\n", begin);
      }
      if (samplesStruct->status == SWIrec_SAMPLE_NEW_CHUNK){
        printf("AUDIO THREAD=> new chunk found. \n");
      }
      if(eprc == SWIep_RESULT_SUCCESS &&
           (samplesStruct->len != 0 || samplesStruct->status != 0) ){
        printf ("AUDIO THREAD=>  Calling SWIrecAudioWrite.\n");
        before_speech = 0;
        rc = SWIrecAudioWrite(atd->rec, samplesStruct);
      }

    }
  }
  fclose(fp);

  /* have to write the end packet for file */
  if(!before_speech){
    /* flush out the epRead buffer */
    while(SWIepRead(swiep_handle,samplesStruct,&swiep_state,MAX_EP_AUDIO_BUF_LEN) == SWIep_RESULT_SUCCESS){
      if(samplesStruct->len != 0 || samplesStruct->status != 0){
        printf ("AUDIO THREAD=>  Calling SWIrecAudioWrite.\n");
        rc = SWIrecAudioWrite(atd->rec, samplesStruct);
      }
      else
        break;
    }
    printf ("AUDIO THREAD=>  Calling SWIrecAudioWrite to indicate end.\n");
    ((unsigned char *)samplesStruct->samples)[0] = 0;
    samplesStruct->len = 1;
    samplesStruct->type = TYPE;
    samplesStruct->status = SWIrec_SAMPLE_LAST;
    printf ("AUDIO THREAD=>  Calling SWIrecAudioWrite.\n");
    rc = SWIrecAudioWrite(atd->rec, samplesStruct);
    atd->ret = rc;
  }
  if(SWIepStop(swiep_handle, SWIep_STOP_SPEECH, 0) != SWIep_RESULT_SUCCESS) {
    printf("AUDIO THREAD=> ERROR. Cannot stop endpointer\n");
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
  }

  /* destroy normally happens when the channel is done, not once per utterance */
  SWIepDetectorDestroy(swiep_handle);

  /* SAMPLE_LAST won't stop the recognizer automatically
   * if we never detected BOS.  This would be true for all-noise
   * utterances, for example.  So, we manually Stop(); otherwise,
   * Compute(-1) will appear to hang waiting for its data.
   */
  if (before_speech) {
    printf("AUDIO THREAD=> ERROR: BOS never found\n");
    atd->ret = SWIrec_ERROR_GENERIC_ERROR;
    SWIrecRecognizerStop(atd->rec, SWIrec_STOP_OTHER);
  }
  /* free struct */
  free(samplesStruct->samples);
  free(samplesStruct);
  return (RETURN_TYPE)0;
}
/**************************************************************************/


