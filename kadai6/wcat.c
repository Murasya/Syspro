
/*
  wcat.c -- テキストを扱うHTTPクライアント
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */
#include <string.h>	/* memset(), memcpy() */
#include <sys/types.h>	/* socket() */
#include <sys/socket.h>	/* socket() */
#include <netinet/in.h>	/* struct sockaddr_in */
#include <netdb.h>	/* getaddrinfo() */
#include <string.h>	/* strlen() */
#include <unistd.h>	/* close() */
#define N 1024
#define PORTNO_BUFSIZE 30

extern void http_send_request( char *host, char *file, FILE *out );
extern	int tcp_connect( char *server, int portno );
extern  int fdopen_sock( int sock, FILE **inp, FILE **outp );

int
main( int argc, char *argv[] )
{
	char *host, *file, message[N] = {'\0'};
  int port, sock;
  FILE *in, *out;

	if( argc != 4 ) {
		fprintf(stderr,"Usage: %s host port file\n",argv[0] );
		exit( 1 );
	}
	host = argv[1];
  port = strtol(argv[2], 0, 10);
	file = argv[3];

  sock = tcp_connect( host, port );
	if( sock<0 )
		return( 1 );
	if( fdopen_sock(sock,&in,&out) < 0 )
	{
		fprintf(stderr,"fdooen()\n");
		close( sock );
		return( 1 );
	}

	http_send_request( host, file, out );
  
  while (fgets(message, N, in) != NULL ) {
    printf("%s", message);
  }
}

void
http_send_request( char *host, char *file, FILE *out ) {
  fprintf(out,"GET %s HTTP/1.0\r\nHost: %s\r\n\r\n", file, host);
}

int
tcp_connect( char *server, int portno )
{
	struct addrinfo hints, *ai;
	char portno_str[PORTNO_BUFSIZE];
	int s, err;
	snprintf( portno_str,sizeof(portno_str),"%d",portno );
	memset( &hints, 0, sizeof(hints) );
	hints.ai_socktype = SOCK_STREAM;
	if( (err = getaddrinfo( server, portno_str, &hints, &ai )) )
	{
		fprintf(stderr,"unknown server %s (%s)\n",server,
			gai_strerror(err) );
		goto error0;
	}
	if( (s = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0 )
	{
		perror("socket");
		goto error1;
	}
	if( connect(s, ai->ai_addr, ai->ai_addrlen) < 0 )
	{
		perror( server );
		goto error2;
	}
	freeaddrinfo( ai );
	return( s );
error2:
	close( s );
error1:
	freeaddrinfo( ai );
error0:
	return( -1 );
}

int
fdopen_sock( int sock, FILE **inp, FILE **outp )
{
	int sock2 ;
	if( (sock2=dup(sock)) < 0 )
	{
		return( -1 );
	}
	if( (*inp = fdopen( sock2, "r" )) == NULL )
	{
		close( sock2 );
		return( -1 );
	}
	if( (*outp = fdopen( sock, "w" )) == NULL )
	{
		fclose( *inp );
		*inp = 0 ;
		return( -1 );
	}
	setvbuf(*outp, (char *)NULL, _IONBF, 0);
	return( 0 );
}
