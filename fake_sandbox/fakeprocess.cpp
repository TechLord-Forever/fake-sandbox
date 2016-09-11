#include "fakeprocess.h"

bool DirectoryExists(char* szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool FileExists(char *szPath)
{
	HANDLE hFile = CreateFile(szPath,
		GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	bool retn = ((hFile == INVALID_HANDLE_VALUE) || (hFile == NULL)) ? false : true;

	CloseHandle(hFile);

	return retn;
}

void CFakeProc::Initalize()
{	
	if(!DirectoryExists("services"))
	{
		if (!CreateDirectoryA("services", NULL))
		{
			MessageBox(GetForegroundWindow(), "Create a folder \"services\" and restart the application.", "Error creating a folder", MB_ICONERROR);
			ExitProcess(1);
		}
	}

	if (!FileExists("services\\dummy.exe"))
	{
		MessageBox(GetForegroundWindow(), "An important component is missing (dummy.exe), the download may have been corrupted.\nPlease re-download the full packege again and make sure to whitelist 'fake_sandbox.exe' and 'dummy.exe'.", "File missing", MB_ICONEXCLAMATION);
		ExitProcess(1);
	}

	HANDLE hDummy = NULL;

	int success = 0;
	int fails = 0;

	for (int i = 0; i < /*15*/ sizeof(szFake)/sizeof(szFake[0]); i++) 
	{
		if (CopyFileA("services\\dummy.exe", szFake[i], false) == NULL)
		{
			printf("[-] Error creating dummy file '%s' from 'services\\dummy.exe' - Errorcode: 0x%X\n", GetLastError());
			fails++;
			continue;
		}
		
		Sleep(50);

		printf("[+] Created dummy file: %s\n", szFake[i]);
		success++;
	}
	
	printf("[-] Failed to create %i object(s).\n", fails);
	printf("[+] %i object(s) have been created successfully.\n", success);

	getchar();
}

void CFakeProc::Run()
{
	HANDLE hFile = NULL;
	STARTUPINFO pStartup;
	PROCESS_INFORMATION pInfo;
	
	pStartup.cb = sizeof(STARTUPINFO);
	
	BOOL bStartProcess = FALSE;
	int success = 0;
	int failed = 0;

	printf("-----------------------------------------------\n");
	printf("[+] Running fake processes...\n");
	
	for (int i = 0; i < sizeof(szFake) / sizeof(szFake[0]); i++)
	{
		hFile = CreateFile(szFake[i], GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("[-] File %s was not found. Skipping to next file...\n", szFake[i]);
			continue;
		}

		if (hFile == INVALID_HANDLE_VALUE || hFile == NULL)
			continue;

		if (GetLastError() != 0)
			continue;

		CloseHandle(hFile);

		bStartProcess = CreateProcessA(szFake[i], NULL, NULL, NULL, false, 0, NULL, NULL, &pStartup, &pInfo);

		if (bStartProcess)
		{
			fakeProcesses[success].dwProcessID = pInfo.dwProcessId;
			fakeProcesses[success].hProcess = pInfo.hProcess;

			printf("[+] Started '%s' successfully.\n    ProcessID: %i\n    ThreadID: %i\n    Handle to Process: 0x%i\n    Handle to Thread: 0x%i\n\n", szFake[i], pInfo.dwProcessId, pInfo.dwThreadId, pInfo.hProcess, pInfo.hThread);
			success++;
		}
		else
		{
			printf("Couldn't start '%s'\n", szFake[i]);
			continue;
		}
	}

	printf("[-] Failed to start %i object(s)\n", failed);
	printf("[+] %i object(s) are now running as a fake-process.\n", success);
}

void CFakeProc::Stop()
{
	printf("-----------------------------------------------\n");
	printf("[+] Terminating fake process...\n");

	char szBuffer[MAX_PATH];

	for (int i = 0; i < sizeof(fakeProcesses); i++)
	{
		if (fakeProcesses[i].dwProcessID == NULL)
			continue;

		GetProcessNameByID(fakeProcesses[i].dwProcessID, szBuffer);

		if (strlen(szBuffer) <= 0)
			continue;

		if (fakeProcesses[i].hProcess == NULL || fakeProcesses[i].hProcess == INVALID_HANDLE_VALUE)
		{
			fakeProcesses[i].hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, false, fakeProcesses[i].dwProcessID);
			Sleep(5);
			if (fakeProcesses[i].hProcess == NULL || fakeProcesses[i].hProcess == INVALID_HANDLE_VALUE)
			{
				printf("[-] Termination of '%s' failed. Process is unable to create a handle to the child process.\n", szBuffer);
			}
		}

		if (TerminateProcess(fakeProcesses[i].hProcess, 0))
			printf("[+] Service '%s' terminated successfully.\n", szBuffer);
		else
			printf("[-] Service '%s' couldn't be killed.\n");
	}
}

void CFakeProc::GetProcessNameByID(DWORD dwPID, char *szBuffer)
{
	PROCESSENTRY32 PE32;
	PE32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPID);

	if (Process32First(hSnapProcess, &PE32))
	{
		do {

			if (dwPID == PE32.th32ProcessID)
				break;

		} while (Process32Next(hSnapProcess, &PE32));
	}

	if (hSnapProcess != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapProcess);

	memcpy(szBuffer, PE32.szExeFile, MAX_PATH);
}
