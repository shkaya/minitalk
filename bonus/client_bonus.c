#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "../libft/libft.h"

#ifndef SLEEP_US
# define SLEEP_US 100
#endif

void	err_exit(char *str)
{
    ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}

/* 1文字をさらにbit単位(8つ)に分解して送信する関数 */
void	send_msg(pid_t pid, char *msg)
{
    int	i;

    while (1)
    {
        i = 0;
    	while (i < 8)
    	{
    	    if (1 & (*msg >> i))
            {
    	    	if (kill (pid, SIGUSR1) == -1)
                	err_exit("Error: Kill functions Failed\n");
            }
    	    else
            {
    	    	if (kill (pid, SIGUSR2) == -1)
                	err_exit("Error: Kill functions Failed\n");
            }
    	    usleep(SLEEP_US);
    	    i++;
    	}
        if (*msg == '\0')
        	return;
        msg++;
    }
}

/* 受け取った文字列をbit単位に分解して、serverに送信するプログラム */
int main(int argc, char *argv[])
{
	int	i;
    /*エラーハンドリング*/
    if (argc != 3)
    	err_exit("[Error] Usage: ./client <PID> <messege>\n");
    i = 0;
    while (argv[1][i] != '\0')
    {
		if (argv[1][i] < '0' || '9' < argv[1][i])
        	err_exit("Error: A Valid PID is 100 ~ 99998\n");
        i++;
    }
    /* ----- pid -----*/
    pid_t	pid;
	pid = ft_atoi(argv[1]);

	/*エラーハンドリング2*/
    if (pid < 100 | 99998 < pid)
    	err_exit("Error: A Valid PID is 100 ~ 99998\n");
    /*------- serverに文字列を送信する ---------*/
	send_msg(pid, argv[2]);
    return (0);
}
