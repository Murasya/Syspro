
/*
  http-request-analyze.c -- HTTPのrequest lineを解析する(枠組みだけ)
  ~yas/syspro/ipc/http-request-analyze.c
  Created on: 2010/05/31 21:26:14
*/

#include <stdio.h>  /* fprintf() */
#include <stdlib.h> /* exit() */
#include <string.h> /* strcmp() */

extern int  http_receive_request( FILE *in, char *filename, size_t size );
extern char * chomp( char *str );
extern int  string_split( char *str, char del, int *countp, char ***vecp  );
extern void free_string_vector( int qc, char **vec );
extern int  countchr( char *s, char c );

#define	BUFFERSIZE	1024

int
main( int argc, char *argv[] )
{
	char filename[BUFFERSIZE];
	int res;

	if( argc != 1 ) {
		fprintf(stderr,"Usage: %s < request-filename\n",argv[0] );
		exit( 1 );
	}
	res = http_receive_request( stdin, filename, BUFFERSIZE );
	if( res )
	    printf("filename is [%s].\n", filename );
	else
	    printf("Bad request.\n");
}

int
http_receive_request( FILE *in, char *filename, size_t size )
{
	char requestline[BUFFERSIZE] ;
	char rheader[BUFFERSIZE] ;
    int count;
    char **sp_str;
	
	snprintf( filename, size, "NOFILENAME");
	if( fgets(requestline,BUFFERSIZE,in) <= 0 )
	{
		printf("No request line.\n");
		return( 0 );
	}
	chomp( requestline ); /* remove \r\n */
	printf("requestline is [%s]\n",requestline );
	while( fgets(rheader,BUFFERSIZE,in) )
	{
		chomp( rheader ); /* remove \r\n */
		if( strcmp(rheader,"") == 0 )
			break;
		printf("Ignored: %s\n",rheader );
	}
	if( strchr(requestline,'<') ||
	    strstr(requestline,"..") )
	{
		printf("Dangerous request line found.\n");
		return( 0 );
	}
    // 3 elements
    if (string_split( requestline, ' ', &count, &sp_str) != 3) {
        printf("Requestline needs 3 words.\n");
        free_string_vector(count, sp_str);
        return(0);
    }
    // "GET"
    if (strcmp(sp_str[0], "GET") != 0) {
        printf("First element need \"GET\".\n");
        free_string_vector(count, sp_str);
        return (0);
    }
    // "HTTP/1.0" or "HTTP/1.1"
    if (strcmp(sp_str[2], "HTTP/1.0") != 0 && strcmp(sp_str[2], "HTTP/1.1") != 0) {
        printf("Third element need \"HTTP/1.0\" or \"HTTP/1.1\".\n");
        free_string_vector(count, sp_str);
        return (0);
    }
    // copy filename
	snprintf( filename, size, "%s", sp_str[1] );
	return( 1 );
}

char *
chomp( char *str )
{
	int len ;

	len = strlen( str );
	if( len>=2 && str[len-2] == '\r' && str[len-1] == '\n' )
	{
		str[len-2] = str[len-1] = 0;
	}
	else if( len >= 1 && (str[len-1] == '\r' || str[len-1] == '\n') )
	{
		str[len-1] = 0;
	}
	return( str );
}

int
string_split( char *str, char del, int *countp, char ***vecp  )
{
	char **vec ;
	int  count_max, i, len ;
	char *s, *p ;

	if( str == 0 )
		return( -1 );
	count_max = countchr(str,del)+1 ;
	vec = malloc( sizeof(char *)*(count_max+1) );
	if( vec == 0 )
		return( -1 );

	for( i=0 ; i<count_max ; i++ )
	{
		while( *str == del )
			str ++ ;
		if( *str == 0 )
			break;
		for( p = str ; *p!=del && *p!=0 ; p++ )
			continue;
		/* *p == del || *p=='\0' */
		len =  p - str ;
		s = malloc( len+1 );
		if( s == 0 )
		{
			int j ;
			for( j=0 ; j<i; j++ )
			{
				free( vec[j] );
				vec[j] = 0 ;
			}
			return( -1 );
		}
		memcpy( s, str, len );
		s[len] = 0 ;
		vec[i] = s ;
		str = p ;
	}
	vec[i] = 0 ;
	*countp = i ;
	*vecp = vec ;
	return( i );
}

void
free_string_vector( int qc, char **vec )
{
	int i ;
    	for( i=0 ; i<qc ; i++ )
	{
		if( vec[i] == NULL )
			break;
		free( vec[i] );
	}
	free( vec );
}

int
countchr( char *s, char c )
{
	int count ;
	for( count=0 ; *s ; s++ )
		if( *s == c )
			count ++ ;
	return( count );
}
