#include <windows.h>
#include <string.h>
#include <stdio.h>


int
make_zip(
	char const	*zip_exe,
	char const	*sn,
	char const	*src_path,
	char const	*dst_path,
	char const	*zip_file
	)
{
	char	cmd_line[512];

	// make command line 

	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	int	ret;
	DWORD	dwExitCode;

	sprintf(cmd_line, "%s %s %s%s%s %s%s", zip_exe, "a -tzip", dst_path, sn, ".zip", src_path, zip_file);

	// Set up members of STARTUPINFO structure.
	memset(&piProcInfo, 0, sizeof(PROCESS_INFORMATION));
	memset(&siStartInfo, 0, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.lpReserved = NULL;
	siStartInfo.lpReserved2 = NULL;
	siStartInfo.cbReserved2 = 0;
	siStartInfo.lpDesktop = NULL;
	siStartInfo.dwFlags = 0;

	ret = CreateProcess(NULL, cmd_line, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &siStartInfo, &piProcInfo);
	if (ret == 0)
	{
		dwExitCode = GetLastError();
		HANDLE hProcess = GetCurrentProcess();//get current process
		return dwExitCode;
	}

	DWORD rc = WaitForSingleObject(
		piProcInfo.hProcess, // process handle
		INFINITE); 


	return 0;
}