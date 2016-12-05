#ifndef __CFG_H__
#define	__CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _PARAMETER_CONFIG
{
	char	work_dir[128];
	char	bin_dir[128];
	char	voice_dir[128];
	char	output_dir[128];
	char	save_dir[128];
	char	grammar_dir[128];
	char	database_connect[256];
	char	database_search[256];
	char	database_search_department[256];
	char	database_search_building[256];
	char	database_search_room[256];
	char	zip_exe[128];
	char	zip_file[128];
	char	log_file[128];
	int		record_max_sec;
	int		record_end_sec;
	int		record_name_sec;
	int		record_building_sec;
	int		record_room_sec;
	int		record_trouble_sec;
	int		staff_channel;
	int		student_channel;

}PARAMETER_CONFIG;


#ifdef __cplusplus
}
#endif

#endif//__CFG_H__