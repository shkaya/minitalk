#include <signal.h> //シグナル関連のヘッダファイル
#include <stdio.h>
#include <unistd.h>

void	signal_handle(int signum)
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
        if (c == '\0')
        	write(1, "\n", 1);
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
int main(void)
{
	// pid_t = int ... PIDを一貫して扱うための標準的な型
    pid_t pid;

    pid = getpid();
    printf("Sever PID: %d\n", pid);

    // シグナルハンドラの設定
    signal(SIGUSR1, signal_handle); // ON(1)が送られてきた時
    signal(SIGUSR2, signal_handle); // OFF(0)が送られてきた時

	while (1)
    {
    	pause(); // シグナルを待機
        // printf("シグナルを受信しました。\n");
    }

    return (0);
}