#pragma once

#include "main.h"

typedef struct fakeProcInfo_t
{
	DWORD dwProcessID;
	HANDLE hProcess;

}fakeProcInfo[15];

class CFakeProc
{
public:

	void Initalize();
	void Run();
	void Stop();

private: 

	fakeProcInfo fakeProcesses;

	void GetProcessNameByID(DWORD dwPID, char *szBuffer);

	const char *szFake[15] = {"services\\wireshark.exe", 
		"services\\vmacthlp.exe", "services\\VBoxService.exe",
		"services\\VBoxTray.exe", "services\\procmon.exe",
		"services\\ollydbg.exe", "services\\vmware-tray.exe",
		"services\\idag.exe", "services\\ImmunityDebugger.exe", 
		"services\\idaq.exe", "services\\idaq64.exe", 
		"services\\idaw.exe", "services\\idaw64.exe",
	    "services\\apimonitor-x86.exe", "services\\apimonitor-x64.exe"};
};