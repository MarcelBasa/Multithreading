#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    if(argc != 2) 
	{
		printf("Niepoprawna ilosc argumentow\n");
		return 1;
	}
	int n = atoi(argv[1]);
	if(n == 0)
	{
		printf("to nie jest liczba naturalna\n");
		return 2;
	}
	if(n < 1 || n> 13)
	{
		printf("Liczba spoza przedzialu <1...13>\n");
		return 2;
	}
	if(n==1 || n==2) 
		return 1;

    STARTUPINFO si1;
    PROCESS_INFORMATION pi1;
    ZeroMemory(&si1, sizeof(si1));
    ZeroMemory(&pi1, sizeof(pi1));
    char arr1[50];
    sprintf(arr1, "%s %d", argv[0], n - 1);
    BOOL bCreateProcess1 = CreateProcessA(
                            argv[0], 
                            arr1, 
                            NULL, 
                            NULL, 
                            FALSE, 
                            0, 
                            NULL, 
                            NULL, 
                            &si1, 
                            &pi1
                        );
    if (bCreateProcess1 == FALSE) 
    {
            printf("Blad przy tworzeniu procesu\n");
            return 1;
    }   

    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si2, sizeof(si2));
    ZeroMemory(&pi2, sizeof(pi2));
    char arr2[50];
    sprintf(arr2, "%s %d", argv[0], n - 2);
    BOOL bCreateProcess2 = CreateProcessA(
                            argv[0], 
                            arr2, 
                            NULL, 
                            NULL, 
                            FALSE, 
                            0, 
                            NULL, 
                            NULL, 
                            &si2, 
                            &pi2
                        );
    if (bCreateProcess2 == FALSE) 
    {
            printf("Blad przy tworzeniu procesu\n");
            return 1;
    }  

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    DWORD exitCode1, exitCode2;

    BOOL bExit1 = GetExitCodeProcess(pi1.hProcess, &exitCode1);
    if (bExit1==FALSE)
    {
        printf("Blad przy uzyskaniu kodu wyjsciowego procesu\n");
        return 6;
    }

    BOOL bExit2 = GetExitCodeProcess(pi2.hProcess, &exitCode2);
    if (bExit2 == FALSE)
    {
        printf("Blad przy uzyskaniu kodu wyjsciowego procesu\n");
        return 6;
    }

    printf("%d  %d   %d  %d\n", GetCurrentProcessId(), pi1.dwProcessId, n-1, exitCode1);
    printf("%d  %d   %d  %d\n", GetCurrentProcessId(), pi2.dwProcessId, n-2, exitCode2);
    printf("%d  %d\n\n", GetCurrentProcessId(), exitCode1 + exitCode2);
    return exitCode1 + exitCode2; 
}