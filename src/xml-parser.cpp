
#include <stdio.h>
#include <string.h>


#define	MAX_BUFFER_LENGTH			2048

#define	XML_HEADER					"<?xml version='1.0'?>"
#define	XML_RESULT_BEGIN			"<result>"
#define	XML_RESULT_END				"</result>"
#define	XML_INTERPRETATION_BEGIN	"<interpretation grammar=\"Digits_Grammar\" confidence=\""
#define	XML_INTERPRETATION_END		"</interpretation>"
#define	XML_INPUT_BEGIN				"<input mode=\"speech\">"
#define	XML_INPUT_END				"</input>"
#define	XML_NOMATCH_BEGIN			"<nomatch>"
#define	XML_NOMATCH_END				"</nomatch>"
#define	XML_RESULT_ON				"<RESULT confidence="
#define	XML_RESULT_OFF				"</RESULT>"
#define	XML_RESULT_OFFSET			"\">"


int
xml_parser(
	char const	*filename,
	char		*result
	)
{
	FILE	*fp_xml;
	char	buffer[MAX_BUFFER_LENGTH];
	char	*begin, *end, *temp1, *temp2;
	size_t	cnt = 0;
	int		length = 0;

	fp_xml = fopen(filename, "r");
	if (fp_xml == NULL)
		return -1;

	cnt = fread(buffer, MAX_BUFFER_LENGTH, 1, fp_xml);
	fclose(fp_xml);

	begin = buffer;
	// check xml file header
	if (strstr(begin, XML_HEADER) == NULL)
		return -1;
	begin += strlen(XML_HEADER);

	// check <result> flag
	if ((begin = strstr(begin, XML_RESULT_BEGIN)) == NULL
		|| strstr(begin, XML_RESULT_END) == NULL)
		return -1;
	begin += strlen(XML_RESULT_BEGIN);

	// check <interpretation>
	if ((begin = strstr(begin, XML_INTERPRETATION_BEGIN)) == NULL
		|| strstr(begin, XML_INTERPRETATION_END) == NULL)
		return -1;
	begin += strlen(XML_INTERPRETATION_BEGIN);

	// check <input>
	if ((begin = strstr(begin, XML_INPUT_BEGIN)) == NULL
		|| (end = strstr(begin, XML_INPUT_END)) == NULL)
		return -1;
	begin += strlen(XML_INPUT_BEGIN);

	// check <RESULT>
	if (strstr(begin, XML_RESULT_ON) != NULL
		&& strstr(begin, XML_RESULT_OFF) != NULL)
	{
		begin = strstr(begin, XML_RESULT_ON);
		begin += strlen(XML_RESULT_ON);
		end = strstr(begin, XML_RESULT_OFF);
		
		if ((begin = strstr(begin, XML_RESULT_OFFSET)) != NULL)
			begin += strlen(XML_RESULT_OFFSET);
	}

	
	// check <nomatch>
	if ((temp1 = strstr(begin, XML_NOMATCH_BEGIN)) != NULL
		&& (temp2 = strstr(begin, XML_NOMATCH_END)) != NULL)
	{
		begin = temp1 + strlen(XML_NOMATCH_BEGIN);
		end = temp2;
	}

	while(begin != end)
		*result++ = *begin++;

	*result = '\0';


	return 0;
}