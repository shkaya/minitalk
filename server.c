#include <signal.h> //シグナル関連のヘッダファイル
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void	send_to_sourse(int pid, int size)
{
	for (int i = 0; i < 32; i++)
    {
        if (1 & (size >> i))
        	kill(pid, SIGUSR1);
        else
        	kill(pid, SIGUSR2);
        usleep(100);
    }
    return;
}

volatile unsigned int g_char_count = 0;

void	signal_handle(int signum, siginfo_t *info, void *dummy)
{
	static int bit_count = 0;
    static char c;
	// if (signum == SIGUSR1)
    // 	printf("SIGUSR1を受信しました。\n");
    // else if (signum == SIGUSR2)
    // 	printf("SIGUSR2を受信しました。\n");

	// SIGUSR1(ON)を受信した場合、確かな場所に1を設定。
    if (signum == SIGUSR1)
		c |= (1 << bit_count); //cのbit_count番目のビットを1にする
    else if (signum == SIGUSR2)
    	; // SIGUSR2(OFF)を受信した時、何もしない。(デフォが0)

    bit_count++;
	// 一バイト分(8ビット)受信したら、一文字を出力
    if (bit_count == 8)
    {
        g_char_count++;
        if (c == '\0')
        {
        	write(1, "\n", 1);
			send_to_sourse(info->si_pid, g_char_count - 1);
            g_char_count = 0;
        }
        else
    		write(1, &c, 1);
        // リセット。
        c = 0;
        bit_count = 0;
    }
}

/*
	#define SIGUSR1 30 -> ONの時
    #define SIGUSR2 31 -> OFFの時
    とする。
*/
/*
	typedef struct sigaction
    {
		void		(*sa_handler)(int); // シグナルハンドラ関数のポインタ
		void		(*sa_sigaction)(int, siginfo_t *, void *) // 拡張シグナルハンドラ
        sigset_t	sa_mask; // シグナルハンドラ実行中にブロックするシグナル。
        int			sa_flag; // シグナルハンドリングの動作を制御するフラグ
        void		(*sa_restorer)(void); // 使用しない
    };
*/
int main(void)
{
	// pid_t = int ... PIDを一貫して扱うための標準的な型
    pid_t pid;
    pid = getpid();
    printf("Sever PID: %d\n", pid);

	/*-------------- sigactionを使うための各設定 ------------------*/
    struct sigaction	act1; // SIGUSR1をハンドリングする用を宣言
    struct sigaction	act2; // SIGUSR2をハンドリングする用を宣言

	memset(&act1, 0, sizeof(sigaction)); // 0で初期化
    memset(&act2, 0, sizeof(sigaction)); // 0で初期化
    /*シグナルハンドラ関数のポインタを設定*/
    act1.sa_sigaction = signal_handle;
    act2.sa_sigaction = signal_handle;
    /*ブロックするシグナルを設定*/
	sigaddset(&(act1.sa_mask), SIGUSR2); // SIGUSR1実行中は,SIGUSR2をブロック。
    sigaddset(&(act2.sa_mask), SIGUSR1); // SIGUSR2実行中は,SIGUSR1をブロック。
    /*拡張ハンドラを使ってもらえるように設定*/
    /*--> ハンドラ関数で受け取る構造体、infoに送信元のpidがある！*/
    act1.sa_flags = SA_SIGINFO;
    act2.sa_flags = SA_SIGINFO;

	// シグナルハンドラ
    sigaction(SIGUSR1, &act1, (struct sigaction *)0); // ON(1)が送られてきた時
    sigaction(SIGUSR2, &act2, (struct sigaction *)0); // OFF(0)が送られてきた時

	while (1)
    {
    	pause(); // シグナルを待機
        // printf("シグナルを受信しました。\n");
    }

    return (0);
}
