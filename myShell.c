#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "local_lib/local_lib.h"
#define ARG_MAX 1000
char** segInput(int *size, char* input, char *delim);
char* getInput();
void killShell(int argc, char** argv);
void freeStringArray(char** array, int size) ;
void exeInput(int argc, char** argv);

int main()
{
	int pid, stat, cpid, argc;
	while(1)
	{
		char *input;
		char **argv;
		
		input = getInput(); //getting input
				
		argv = segInput(&argc, input, " "); //segmenting the input
		killShell(argc, argv); //terminating keyword will exit the programme
		exeInput(argc, argv); //executing commands	
	}
	
}

void killShell(int argc, char** argv)
{
	for(int i = 0; i < argc; i++)
		if(!strcmp(argv[i], "leave"))
		{
			printf("BYE BYE!\n");
			printf("pid=%d, ppid=%d\n", getpid(), getppid());
			exit(0);
		}
}
		
void exeInput(int argc, char** argv)
{
	printf("my_shell->\n");
	int pid, stat, pathArrSz;
	char** pathArray, cmdPath[ARG_MAX];
	if((pid = fork()) == 0)
	{
	//printf("In child\n");
		pathArray = segInput(&pathArrSz, getenv("PATH"), ":");
		for(int i = 0; i < pathArrSz; i++)
		{ 
			sprintf(cmdPath, "%s/%s", pathArray[i], argv[0]);
			puts(cmdPath);
			if(!access(cmdPath, X_OK))
			{
				execv(cmdPath, argv);
				printf("exec failed\n"); //execv failed if returned;
				exit(-1); 
			}
		}
		printf("command not found\n");
		exit(-1);
	}
	else
	{
		printf("In parent, cpid = %d\n",wait(&stat));
	}		
}

char* getInput()
{
	char *input = (char*)malloc(ARG_MAX*sizeof(char));
	int len;
	fgets(input,ARG_MAX, stdin);
	len = strlen(input);
	
	if(input[len-1] = '\n')
		input[len-1] = '\0';
	
	return input;
}

char** segInput(int *numOfArgs, char* input, char* delim)
{
	*numOfArgs = 0;	
	char **argv = (char**)malloc(ARG_MAX*sizeof(char*));
	char* t = strtok(input, delim);
	while(t)
	{
		argv[(*numOfArgs)++] = t;		
		t = strtok(NULL, delim);						
	}
	argv[*numOfArgs] = NULL;
	return argv;
}
void freeStringArray(char** array, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        free(array[i]);
    }
    free(array);
}


 
