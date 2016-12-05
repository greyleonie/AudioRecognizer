#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/cfg.h"

int
GetParaConfig(
	PARAMETER_CONFIG	*cfg
	)
{
	FILE	*fp_cfg;
	char	buffer[256];
	char	ch;
	char	*pch;

	fp_cfg = fopen("./recognizer.cfg", "r");
	if (fp_cfg == NULL)
		return -1;

	pch = buffer;
	while(!feof(fp_cfg))
	{
		ch = fgetc(fp_cfg);
		if (ch != '\n')
		{
			*pch++ = ch;
			continue;
		}
		*pch = '\0';

		
		if ((pch = strstr(buffer, "WORK_DIR=")) != NULL)
		{
			pch += strlen("WORK_DIR=");
			strcpy(cfg->work_dir, pch);
		}
		else if ((pch = strstr(buffer,"BIN_DIR=")) != NULL)
		{
			pch += strlen("BIN_DIR=");
			strcpy(cfg->bin_dir, pch);
		}
		else if ((pch = strstr(buffer,"VOICE_DIR=")) != NULL)
		{
			pch += strlen("VOICE_DIR=");
			strcpy(cfg->voice_dir, pch);
		}
		else if ((pch = strstr(buffer,"OUTPUT_DIR=")) != NULL)
		{
			pch += strlen("OUTPUT_DIR=");
			strcpy(cfg->output_dir, pch);
		}
		else if ((pch = strstr(buffer, "SAVE_DIR=")) != NULL)
		{
			pch += strlen("SAVE_DIR=");
			strcpy(cfg->save_dir, pch);
		}
		else if ((pch = strstr(buffer, "GRAMMAR_DIR=")) != NULL)
		{
			pch += strlen("GRAMMAR_DIR=");
			strcpy(cfg->grammar_dir, pch);
		}
		else if ((pch = strstr(buffer, "DATABASE_CONNECT=")) != NULL)
		{
			pch += strlen("DATABASE_CONNECT=");
			strcpy(cfg->database_connect, pch);
		}
		else if ((pch = strstr(buffer, "DATABASE_SEARCH=")) != NULL)
		{
			pch += strlen("DATABASE_SEARCH=");
			strcpy(cfg->database_search, pch);
		}
		else if ((pch = strstr(buffer, "DATABASE_SEARCH_DEPARTMENT=")) != NULL)
		{
			pch += strlen("DATABASE_SEARCH_DEPARTMENT=");
			strcpy(cfg->database_search_department, pch);
		}
		else if ((pch = strstr(buffer, "DATABASE_SEARCH_BUILDING=")) != NULL)
		{
			pch += strlen("DATABASE_SEARCH_BUILDING=");
			strcpy(cfg->database_search_building, pch);
		}
		else if ((pch = strstr(buffer, "DATABASE_SEARCH_ROOM=")) != NULL)
		{
			pch += strlen("DATABASE_SEARCH_ROOM=");
			strcpy(cfg->database_search_room, pch);
		}
		else if ((pch = strstr(buffer, "ZIP_EXE=")) != NULL)
		{
			pch += strlen("ZIP_EXE=");
			strcpy(cfg->zip_exe, pch);
		}
		else if ((pch = strstr(buffer, "ZIP_FILE=")) != NULL)
		{
			pch += strlen("ZIP_FILE=");
			strcpy(cfg->zip_file, pch);
		}
		else if ((pch = strstr(buffer, "LOG_FILE=")) != NULL)
		{
			pch += strlen("LOG_FILE=");
			strcpy(cfg->log_file, pch);		
		}
		else if ((pch = strstr(buffer, "RECORD_MAX_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_MAX_SECOND=");
			cfg->record_max_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "RECORD_END_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_END_SECOND=");
			cfg->record_end_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "RECORD_NAME_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_NAME_SECOND=");
			cfg->record_name_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "RECORD_BUILDING_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_BUILDING_SECOND=");
			cfg->record_building_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "RECORD_ROOM_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_ROOM_SECOND=");
			cfg->record_room_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "RECORD_TROUBLE_SECOND=")) != NULL)
		{
			pch += strlen("RECORD_TROUBLE_SECOND=");
			cfg->record_trouble_sec = atoi(pch);
		}
		else if ((pch = strstr(buffer, "STAFF_CHANNEL=")) != NULL)
		{
			pch += strlen("STAFF_CHANNEL=");
			cfg->staff_channel = atoi(pch);
		}
		else if ((pch = strstr(buffer, "STUDENT_CHANNEL=")) != NULL)
		{
			pch += strlen("STUDENT_CHANNEL=");
			cfg->student_channel = atoi(pch);
		}
		

		pch = buffer;

	}//end while

	fclose(fp_cfg);

	return 0;
}