#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
    	usleep(130); 
        // 次のビットを送る前に、(1 / 1000)秒間、一時停止。 
    }
}

void	fd_putstr(char *str, int fd)
{
    while (*str)
    	write (fd, str++, 1);
}

void	putnbr(int n)
{
    char c;

    if (n > 9)
    {
        putnbr(n / 10);
    }
	c = '0' + (n % 10);
    write(1, &c, 1);
}

void	signal_handle(int signum, siginfo_t *info, void *dummy)
{
    static int bit_count = 0;
    static int n = 0;

	if (signum == SIGUSR1)
    	n |= 1 << bit_count;
    bit_count++;
    if (bit_count == 32)
    {
		fd_putstr("Output:", 1);
		putnbr(n);
        fd_putstr(" bytes\n", 1);
		n = 0;
        bit_count = 0;
        return;
    }
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

	/*--------- sigactionを使うための設定 -----------*/
    /*各設定を入れるための構造体を宣言*/
    struct sigaction	act1; 
    struct sigaction	act2;

	/*初期化*/
	memset(&act1, 0, sizeof(sigaction));
    memset(&act2, 0, sizeof(sigaction));
    /*ハンドラの設定*/
    act1.sa_sigaction = signal_handle;
	act2.sa_sigaction = signal_handle;
    /*ブロックするシグナルの設定*/
    sigaddset(&(act1.sa_mask), SIGUSR2);
    sigaddset(&(act2.sa_mask), SIGUSR1);
    /*使ってもらうハンドラをsigactionに指定*/
	act1.sa_flags = SA_SIGINFO;
    act2.sa_flags = SA_SIGINFO;
	/*シグナルハンドラ関数*/
	sigaction(SIGUSR1, &act1, (struct sigaction *)0);
    sigaction(SIGUSR2, &act2, (struct sigaction *)0);

	/*------------- サーバーへの送信 -------------------------*/	
    /*1文字ずつに分割*/
	while (*(argv[2]) != '\0')
		send_char(pid, *(argv[2]++));
    /*null文字も送信*/
	if (*(argv[2]) == '\0')
    	send_char(pid, *(argv[2]));


	/*--------------- サーバーからの送信 -----------------------*/

    /*サーバー側からの送信を待つ*/
	while (1)
    {
        pause();
    }

	return (0);
}
