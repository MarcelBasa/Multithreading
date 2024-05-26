#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int n, w;
double wynik=1;
pthread_mutex_t mutex;

void* routine(void* arg)
{
	int x = n/w; //166
	int th = *(int*)arg+1;
	pthread_t self = pthread_self();
	double result=1;
	int i;
	if(th == 1)
	{
		for(i=1; i<(x*th); i++)
		{
			result = result * ((2.*i)*(2.*i))/((2.*i-1)*(2.*i+1));
		}
		printf("Thread #%ld size=%d first=%d\n", self, th*x, 1);
	}
	else
	{
		int i;
		for(i=x*(th-1); i<(th*x); i++)
		{
			result = result * ((2.*i)*(2.*i))/((2.*i-1)*(2.*i+1));
		}
		printf("Thread #%ld size=%d first=%d\n", self, x*(th-1), th*x);
	}
	free(arg);
	printf("Thread #%ld prod=%.20f\n", self, result);
	pthread_mutex_lock(&mutex);
	wynik = wynik * result;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[])
{
	sleep(1);
	if(argc != 3)
	{
		printf("Zla ilosc argumetow\n");
		return 1;
	}
	n = atoi(argv[1]);
	w = atoi(argv[2]);
	pthread_t th[w];
	pthread_mutex_init(&mutex, NULL);
	clock_t clk1 = clock();
	int i;
	for(i=0; i<w; i++)
	{
		int* a=malloc(sizeof(int));
		*a=i;
		if(pthread_create(&th[i], NULL, &routine, a) != 0)
		{
			perror("Nie udalo sie stworzyc watku");
			return 1;
		}	
	}
	for(i=0; i<w; i++)
	{
		if(pthread_join(th[i], NULL) != 0)
			return 2;
	}
	pthread_mutex_destroy(&mutex);
	wynik = wynik * 2;
	double duration = ((double)clk1)/CLOCKS_PER_SEC;	
	printf("wynik =  %.20f time=%fs\n", wynik, duration);
	
	clock_t clk2 = clock();
	wynik = 1;
	for(int i=1; i<n; i++)
	{
		wynik = wynik * ((2.*i)*(2.*i))/((2.*i-1)*(2.*i+1));
	}
	wynik = wynik * 2;
	duration = ((double)clk2)/CLOCKS_PER_SEC;	
	printf("wynik =  %.20f time=%fs\n", wynik, duration);
	return 0;
}
