#include "main.h"

CFakeProc fake;

bool bHide = false;
bool bHasCreated = false;

int main()
{
restart:
	system("cls");
	int option = 0;

	SetConsoleTitle("Fake Sandbox");

	printf("1. Start the fake sandbox\n");
	printf("2. Stop the fake sandbox processes\n");
	printf("3. Exit from the UI\n");
	printf("\n\nOption: ");
	scanf_s("%i", &option);

	switch(option)
	{
	case 1: 
		if (bHasCreated)
		{
			MessageBox(GetForegroundWindow(), "Services are already running.\n", "Error", MB_ICONEXCLAMATION);
			goto restart;
			break;
		}
		system("cls");
		printf("[+] Attemping to start fake sandbox module...\n");
		fake.Initalize();
		fake.Run();
		bHasCreated = true;
		printf("\n\n\n ### This window hides after 5 seconds ###\n ### To show this window up again, press F9 ###");
		Sleep(5000);
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		bHide = true;
		break;
	case 2:
		if (!bHasCreated)
		{
			MessageBox(GetForegroundWindow(), "No service running.\n", "Error", MB_ICONEXCLAMATION);
			goto restart;
			break;
		}
		system("cls");
		printf("[+] Attemping to kill all fakebox services...\n");
		fake.Stop();
		printf("\n\n\n ### This window clears itself after 5 seconds ###\n");
		Sleep(5000);
		break;
	case 3:
		ExitProcess(0);
		break;
	default:
		MessageBox(GetForegroundWindow(), "Invalid operation.", "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	while (bHide)
	{
		if (GetAsyncKeyState(VK_F9))
		{
			bHide = !bHide;
			ShowWindow(GetConsoleWindow(), SW_SHOW);
			break;
		}
		Sleep(1000);
	}

	if (!bHide)
		goto restart;

	getchar();
}