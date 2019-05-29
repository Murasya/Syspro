
/*
  print-http-request.c -- HTTPの要求を画面に表示する(引き通付き、枠組みだけ)
  ~yas/syspro/ipc/print-http-request.c
  Created on: 2014/07/24 17:43:35
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */

extern void http_send_request( char *host, char *file, FILE *out );

int
main( int argc, char *argv[] )
{
	char *host, *file;
	if( argc != 3 ) {
		fprintf(stderr,"Usage: %s host file\n",argv[0] );
		exit( 1 );
	}
	host = argv[1];
	file = argv[2];
	http_send_request( host, file, stdout );
}

void
http_send_request( char *host, char *file, FILE *out ) {
    	/* この関数の内容を変更しなさい。
	   "Change this\n" の部分を変更しなさい。
	   printf() を使わないで fprintf() を使いなさい。
	   stdout, stderr を直接使わないで、引数の out を使いなさい。
	   必要なパラメタは、引数から取る。fgets() は使わない。
	 */
    	fprintf(out,"GET %s HTTP/1.0\nHost: %s\n\n", file, host);
}
