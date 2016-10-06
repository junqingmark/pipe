#include <unistd.h>
#include <stdio.h>
#include <string.h>

//#define MAXLINE 10

int main(int argc, char* argv[])
{
	int n;
	int fd[2];
	pid_t pid;
	//char line[MAXLINE];
	if (pipe(fd) < 0)
	{
		printf("Fail to create pipe!\n");
		return 1;
	}

	long pipeBufSize;
	//pipeBufSize = sysconf(_PC_PIPE_BUF);
	//PIPE_BUF is the atomic size of writing
	pipeBufSize = fpathconf(fd[0], _PC_PIPE_BUF);
	printf("The capacity of pipe buf is %ld\n", pipeBufSize);
	char line[pipeBufSize];

	//pipe father========>child, father close fd[0], child close fd[1]
	if ((pid = fork()) < 0)
	{
		printf("fork error!\n");
		return 1;
	}
	else if (pid > 0)
	{
		const char* msg = "Hello World!\n";
		close(fd[0]);
		write(fd[1], (const void*)msg, strlen(msg));
	}
	else
	{
		close(fd[1]);
		n = read(fd[0], line, pipeBufSize);
		write(STDOUT_FILENO, line, n);
	}
	return 0;
}