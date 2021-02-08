# Linux Shell
A simple Linux Shell written in C
Commands for cd, help and exit are built in. More custom commands can be added!

## Help 
Help function can be called by using `myhelp`

![alt text](https://github.com/vedantsawant/LinuxShell/blob/master/images/1.png "Help Image")

## Forking
Creating a new process using fork()
```C
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 
void forkexample() 
{ 
	// child process because return value zero 
	if (fork() == 0) 
		printf("Hello from Child!\n"); 

	// parent process because return value non-zero. 
	else
		printf("Hello from Parent!\n"); 
} 
int main() 
{ 
	forkexample(); 
	return 0; 
} 
```
#### Result
![alt text](https://github.com/vedantsawant/LinuxShell/blob/master/images/2.png "Help Image")

```C
#include<stdio.h> 
int main() 
{ 
	for(int i=0;i<5;i++) // loop will run n times (n=5) 
	{ 
		if(fork() == 0) 
		{ 
			printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
			exit(0); 
		} 
	} 
	for(int i=0;i<5;i++) // loop will run n times (n=5) 
	wait(NULL); 
} 
```
#### Result
![alt text](https://github.com/vedantsawant/LinuxShell/blob/master/images/5.png "Help Image")
