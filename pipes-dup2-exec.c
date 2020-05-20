#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    
	int fd[2];  
	pipe(fd);
	pid_t pid = fork();
	
	if (pid == 0)		
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		execlp("ls", "ls", "-1", (char*)NULL);
		exit(1);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(fd[0], 0);
			close(fd[1]);
			close(fd[0]);
			execlp("cut", "cut", "-f1", "-d.", (char *)NULL);
			exit(-1);
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[0], 0);
				close(fd[1]);
				close(fd[0]);
				execlp("sort", "sort", (char*)NULL);
				exit(-1);
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					dup2(fd[0], 0);
					close(fd[1]);
					close(fd[0]);
					execlp("uniq", "uniq", (char*)NULL);
					exit(-1);
				}
				else
				{
					pid = fork();
					if (pid == 0)
					{
						dup2(fd[0], 0);
						close(fd[1]);
						close(fd[0]);
						execlp("tee", "tee", "dirents", (char*)NULL);
						exit(-1);
					}
					else
					{
						int status;
						close(fd[0]);
						close(fd[1]);
						waitpid(pid, &status, 0);
					}
				}
			}
		}
	}
	return 0;
}

