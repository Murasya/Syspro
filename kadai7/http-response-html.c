
/*
  http-response-html.c -- HTTPの応答を画面に表示する(.html専用、枠組みだけ)
  ~yas/syspro/ipc/http-response-html.c
  Created on: 2014/07/28 21:20:09
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */
#include <string.h> /* strrchr() */
#define SIZE 100

extern void http_send_reply( FILE *out, char *filename );
extern void http_send_reply_bad_request( FILE *out );
extern void http_send_reply_not_found( FILE *out );

int
main( int argc, char *argv[] )
{
	char *filename;

	if( argc != 2 ) {
		fprintf(stderr,"Usage: %s filename\n",argv[0] );
		exit( 1 );
	}
	filename = argv[1];
	http_send_reply( stdout, filename );
}

void
http_send_reply( FILE *out, char *filename )
{
	char *ext;
    char *home;
    FILE *fp;
    char text[SIZE];
    char full_filename[SIZE];
    int count;

	ext = strrchr( filename, '.' );
	if( ext == NULL )
	{
		http_send_reply_bad_request( out );
		return;
	}
	else if( strcmp( ext,".html" ) == 0 )
	{
		printf("filename is [%s], and extention is [%s].\n", 
			filename, ext);
        home = getenv("HOME");
        sprintf(full_filename, "%s%s%s", home, "/public_html/htdocs/", filename);
        printf("%s\n", full_filename);
        if ((fp = fopen(full_filename, "r")) == NULL) {
            http_send_reply_not_found( out );
            return ;
        }
        fprintf(out, "HTTP/1.0 200 OK\r\n");
        fprintf(out, "Content-Type: text/html\r\n");
        fprintf(out, "\r\n");
        while ((count = fread(text, sizeof(char), SIZE, fp)) > 0) {
            fwrite(text, sizeof(char), count, out);
        }
        fclose(fp);
		return;
	}
	else
	{
		http_send_reply_bad_request( out );
		return;
	}
}

void
http_send_reply_bad_request( FILE *out )
{
	fprintf(out,"HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n\r\n");
    	fprintf(out,"<html><head></head><body>400 Bad Request</body></html>\n");
}

void
http_send_reply_not_found( FILE *out )
{
	fprintf(out,"HTTP/1.0 404 Not Found\r\nContent-Type: text/html\r\n\r\n");
    	fprintf(out,"<html><head></head><body>404 Not Found</body></html>\n");
}
