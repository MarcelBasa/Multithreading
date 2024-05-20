#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

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

	pid_t pid1 = fork();
	if(pid1 == 0)
	{
		n=n-1;
		char digit[4];
		sprintf(digit, "%d", n); 
		execlp(argv[0], argv[0], digit, NULL);
		return 0;
	}

	pid_t pid2 = fork();
	if(pid2 == 0)
	{
		n=n-2;
		char digit[4];
		sprintf(digit, "%d", n); 
		execlp(argv[0], argv[0], digit, NULL);
		return 0;
	}
	int w1, w2;
	waitpid(pid1, &w1, 0);
	waitpid(pid2, &w2, 0);
	
	printf("%d  %d   %d  %d\n",getpid(), pid1, n-1, WEXITSTATUS(w1));
	printf("%d  %d   %d  %d\n",getpid(), pid2, n-2, WEXITSTATUS(w2));
	printf("%d             %d\n\n", getpid(), WEXITSTATUS(w1)+WEXITSTATUS(w2));
	return WEXITSTATUS(w1)+WEXITSTATUS(w2);
}
