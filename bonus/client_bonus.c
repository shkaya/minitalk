#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "../libft/libft.h"

#ifndef SLEEP_US
# define SLEEP_US 100
#endif

// sig_atomic_t: シグナルハンドラ関数内で使用するための特別なint型。
// volatile: シグナルハンドラ関数などの外部関数内側での変数の変更を失わせない。
typedef struct s_ack
{
	pid_t					pid;
    volatile sig_atomic_t	flg;
}			t_ack;

static t_ack	g_ack;

/* エラーハンドリング関数 */
static void	err_exit(char *str)
{
    ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}

/* 1文字をさらにbit単位(8つ)に分解して送信する関数 */
/* ACTを使って通信をより堅実に */
static void	send_msg(pid_t pid, char *msg)
{
    int	i;

	g_ack.flg = 1; // flg=1であることが①での脱出手段
    while (1)
    {
        i = 0;
    	while (i < 8)
    	{
            while (!g_ack.flg)
            	pause(); // ハンドラ関数内でserverPIDが正しいことがわかったら(flg=1)脱出。 ー①
            g_ack.flg = 0;
            usleep(SLEEP_US);
    	    if (1 & (*msg >> i))
            {
				if (kill(pid, SIGUSR1) == -1)
                	err_exit("Error: Kill functions Failed\n");
            }
    	    else
        	{
				if (kill(pid, SIGUSR2) == -1)
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

#include <stdio.h>
/* シグナルハンドラ関数 */
static void	signal_handler(int signum, siginfo_t *info, void *dummy)
{
    (void)signum;
    (void)dummy;
    // serverのPIDをチェックし、正しかったらflgを立てる
	if (g_ack.pid == info->si_pid || info->si_pid == 0)
    	g_ack.flg = 1;
    return;
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
	g_ack.pid = ft_atoi(argv[1]);

	/*エラーハンドリング2*/
    if (g_ack.pid < 100 | 99998 < g_ack.pid)
    	err_exit("Error: A Valid PID is 100 ~ 99998\n");

    /*------- sigaction関数を使うにあたっての設定 ---------*/
    struct sigaction	sa;
    ft_memset(&sa, 0, sizeof(sigaction));
    sigemptyset(&(sa.sa_mask));
    sa.sa_sigaction = signal_handler;
	sigaddset(&(sa.sa_mask), SIGUSR1);
    sigaddset(&(sa.sa_mask), SIGUSR2);
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &sa, NULL) < 0)
    	exit(EXIT_FAILURE);

    /*------- serverに文字列を送信する ---------*/
	send_msg(g_ack.pid, argv[2]);
    return (EXIT_SUCCESS);
}
