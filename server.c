#include <signal.h> //シグナル関連のヘッダファイル
#include <stdio.h>
#include <unistd.h>

/*
    ---- Available function ----
    	#include <signal.h>
	1.signal
    >> 指定したシグナルが発生した時に実行されるシグナルハンドラを設定。
	2.sigemptyset
    >> 指定したシグナルセットを空にする。
    3.sigaddset
    >> 指定したシグナルをシグナルセットに追加する。
    // シグナルとシグナルセットが存在する？
    4.sigaction (?)
    >> シグナルハンドラを設定するために使用される。この関数はシグナル処理のため
    の詳細なオプションを提供する。(?)(?)(?)
	5.kill
    >> 指定したプロセスID(PID)にシグナルを送信する。

    	#include <unistd.h>
    6.getpid
    >> 現在のプロセスID(PID)を返す。
	7.pause
    >> シグナルを受信するまでプロセスを一時停止にする。シグナルを受信すると
    、シグナルハンドラが実行される。
    8.sleep
    >> 指定した秒数だけプロセスを一時停止する。
    9.usleep
    >> 指定したマイクロ秒(1秒の100万分の１)だけプロセスを一時停止する。

	What 'プロセス' is
    >> プロセスは、コンピュータの基本的な実行単位。
    実行中のプログラム(実際に調理中の料理)。
    What 'PID' is
    >> OS内で実行されている各プロセスを一意に識別するための番号。
    [PID追加情報]
    親プロセスID(PPID): 親プロセスとは現在のプロセスを生成したプロセス。
	PID 1: システム起動時に最初に生成されるプロセス(intプロセスやsystemdプロセス)
    がPID 1を持つ。
*/

int main(void)
{
    int pid; // What pid_t is

    pid = getpid(); // get pid?
    printf("Sever PID: %d\n", pid);
    return (0);
}