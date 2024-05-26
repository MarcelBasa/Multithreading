#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

int n, w;
double wynik=0;
HANDLE mutex;

DWORD WINAPI routine(void* arg)
{
	int x = n/w;
	int th = *(int*)arg+1;;
	double result=0;
	int i;
	if(th == 1)
	{
		for(i=0; i<(x*th); i++)
		{
			result = result + (pow(-1., i)/(2.*i+1));
		}
		printf("Thread #%ld size=%d first=%d\n", GetCurrentThreadId(), th*x, 1);
	}
	else
	{
		int i;
		for(i=x*(th-1); i<(th*x); i++)
		{
			result = result + (pow(-1., i)/(2.*i+1));
		}
		printf("Thread #%ld size=%d first=%d\n", GetCurrentThreadId(), x*(th-1), th*x);
	}
	free(arg);
	printf("Thread #%ld prod=%.20f\n", GetCurrentThreadId(), result);
	WaitForSingleObject(mutex, INFINITE);
	wynik = wynik + result;
	ReleaseMutex(mutex);
}

int main(int argc, char* argv[])
{
	Sleep(1);
	if(argc != 3)
	{
		printf("Zla ilosc argumetow\n");
		return 1;
	}
	n = atoi(argv[1]);
	w = atoi(argv[2]);
	HANDLE th[w];
	DWORD TID;
	mutex = CreateMutex(NULL, FALSE, NULL);
	clock_t clk1 = clock();
	int i;
	for(i=0; i<w; i++)
	{
		int* a=malloc(sizeof(int));
		*a=i;
		th[i] = CreateThread(
				0,
				0,
				routine,
				a,
				0,
				&TID);
	}
	for(i=0; i<w; i++)
	{
		WaitForSingleObject(th[i], INFINITE);
	}
	for(i=0; i<w; i++)
	{
		CloseHandle(th[i]);
	}
	wynik = wynik * 4;
	double duration = ((double)clk1)/CLOCKS_PER_SEC;	
	printf("wynik =  %.20f time=%fs\n", wynik, duration);
	
	clock_t clk2 = clock();
	wynik = 0;
	for(int i=0; i<n; i++)
	{
		wynik = wynik + (pow(-1., i)/(2.*i+1));
	}
	wynik = wynik * 4;
	duration = ((double)clk2)/CLOCKS_PER_SEC;	
	printf("wynik =  %.20f time=%fs\n", wynik, duration);
	return 0;
}
