#include <unistd.h>
#include <signal.h>
#include "../libft/libft.h"

typedef struct	s_msg
{
	char	c;
    int		bit_count;
}				t_msg;

static void	signal_handler(int signum, siginfo_t *info, void *dummy)
{
    (void)info;
    (void)dummy;

	static t_msg	msg = {0};

	if (signum == SIGUSR1)
		msg.c |= (1 << msg.bit_count);
    else if (signum == SIGUSR2)
    	;
    msg.bit_count++;
    if (msg.bit_count == 8)
    {
        if (msg.c != '\0')
        	write(STDOUT_FILENO, &(msg.c), 1);
    	msg.c = 0;
        msg.bit_count = 0;
    }
}

/* struct sigaction (man sigaction)*/
/*
	typedef struct sigaction
    {
		void	(*sa_handler)(int);
        void	(*sa_sigaction)(int, siginfo_t *, void *);
        sigset	sa_mask;
        int		sa_flags;
    };
*/
/* clientからbit単位に分解された文字列をserverで組み立てて出力するプログラム */
int main(void)
{
	/*---------- PID -------------*/
	ft_putstr_fd("PID: ", 1);
    ft_putnbr_fd(getpid(), 1);
    ft_putstr_fd("\n", 1);

	/*---------- sigaction関数を使うにあたっての設定 ------------*/
	// 設定情報を格納する構造体を宣言
    struct sigaction	sa;

    // 構造体の初期化
    ft_memset(&sa, 0, sizeof(sigaction));
    // シグナルセットの初期化
    sigemptyset(&(sa.sa_mask));
    // シグナルハンドラ関数のポインタの設定
    sa.sa_sigaction = signal_handler;
    // 処理中にブロックするシグナルの設定
    sigaddset(&(sa.sa_mask), SIGUSR1);
    sigaddset(&(sa.sa_mask), SIGUSR2);
    // シグナルハンドラの詳細設定
    sa.sa_flags = SA_SIGINFO; //ハンドラ関数で受け取る引数を3つに

	// シグナルハンドラの設定
    if (sigaction(SIGUSR1, &sa, NULL) < 0)
    	exit(EXIT_FAILURE);
    if (sigaction(SIGUSR2, &sa, NULL) < 0)
    	exit(EXIT_FAILURE);

    while (1)
    	pause();
    return (EXIT_SUCCESS);
}
