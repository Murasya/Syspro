
/*
     echo-server-nofork-fdopen.c -- 受け取った文字列をそのまま返すサーバ(fork無し版)
     ~yas/syspro/ipc/echo-server-nofork-fdopen.c
*/
#include <stdio.h>
#include <stdlib.h>     /* exit() */
#include <sys/types.h>  /* socket(), wait4() */
#include <sys/socket.h> /* socket() */
#include <netinet/in.h> /* struct sockaddr_in */
#include <sys/resource.h> /* wait4() */
#include <sys/wait.h>   /* wait4() */
#include <netdb.h>      /* getnameinfo() */
#include <string.h>     /* strlen() */
#include <unistd.h>     /* getpid(), gethostname() */

extern  void echo_server( int portno, int ip_version );
extern  void echo_receive_request_and_send_reply( int com );
extern  int  echo_receive_request( char *line, size_t size, FILE *in );
extern  void echo_send_reply( char *line, FILE *out );
extern  void print_my_host_port( int portno );
extern  void tcp_sockaddr_print( int com );
extern  void tcp_peeraddr_print( int com );
extern  void sockaddr_print( struct sockaddr *addrp, socklen_t addr_len );
extern  int  tcp_acc_port( int portno, int pf );
extern  int  fdopen_sock( int sock, FILE **inp, FILE **outp );

int
main( int argc, char *argv[] )
{
   int portno, ip_version;

   if( !(argc == 2 || argc==3) ) {
       fprintf(stderr,"Usage: %s portno {ipversion}\n",argv[0] );
       exit( 1 );
   }
   portno = strtol( argv[1],0,10 );
   if( argc == 3 )
        ip_version = strtol( argv[2],0,10 );
    else
        ip_version = 46; /* Both IPv4 and IPv6 by default */
    echo_server( portno, ip_version );
}

