
/*
  print-http-request-fixed.c -- HTTPの要求を画面に表示する(枠組みだけ)
  ~yas/syspro/ipc/print-http-request-fixed.c
  Created on: 2014/07/24 17:33:10
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */

extern void send_http_request_fixed( FILE *out );

int
main( int argc, char *argv[] )
{
	send_http_request_fixed( stdout );
}

void
send_http_request_fixed( FILE *out ) {
    	/* この関数の内容を変更しなさい。
	   "Change this\n" の部分を変更しなさい。
	   printf() を使わないで fprintf() を使いなさい。
	   stdout, stderr を直接使わないで、引数の out を使いなさい。
	   必要なパラメタは、引数から取る。fgets() は使わない。
	 */
    fprintf(out, "GET /~syspro/index.html HTTP/1.0\nHost: www.coins.tsukuba.ac.jp\n\n");
}
