#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
	#define SIGUSR1 31 -> ONの時
    #define SIGUSR2 30 -> OFFの時
*/
//　一文字の中身のビット8つを順番に参照していきたい。
void	send_char(pid_t pid, char c)
{
	for (int i = 0; i < 8; i++)
    {
		if (1 & (c >> i))
        {
            // printf("%d: send SIGUSR1\n", i);
			kill(pid, SIGUSR1); //ONの時
        }
        else
        {
            // printf("%d: send SIGUSR2\n", i);
        	kill(pid, SIGUSR2); //OFFの時
        }
    	usleep(100); 
        // 次のビットを送る前に、(1 / 1000)秒間、一時停止。 
    }
}

void	fd_putstr(char *str, int fd)
{
    while (*str)
    	write (fd, str++, 1);
}

int main(int argc, char *argv[])
{
    pid_t	pid;

	if (argc != 3)
    {
    	fd_putstr("Usage: <sever_pid> <message>\n", 2);
        exit(EXIT_FAILURE);
    }
	pid = atoi(argv[1]);
	while (*(argv[2]) != '\0')
		send_char(pid, *(argv[2]++));
	if (*(argv[2]) == '\0')
    	send_char(pid, *(argv[2]));
	return (0);
}
