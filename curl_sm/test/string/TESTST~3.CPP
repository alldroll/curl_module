#include <pthread.h>
#include "util.h"

unsigned int createStrings ( int n )
{
  static char *samples[] = { "something1","something2","something3",
                             "something4" };

  printf ( "Thread <%d>: Creating/destroying %d strings...\n", 
           pthread_self(), n );

  unsigned int start = utilGetTickCount ();

  for ( int i=0; i<n; i++ )
  {
    CGenString s ( samples[i%4] );
  }

  unsigned int end = utilGetTickCount ();

  printf ( "Thread <%d>: total time=%d millisecs, average=%f\n", 
           pthread_self(), end-start, (float)(end-start)/n );

  return ( end - start );
}

int main ( int argc, char **argv )
{
  if ( argc > 2 )
  {
    printf ( "teststring1 <n_of_strings_to_create> \n" );
    return 0;
  }

  int n = 100000;

  if ( argc > 1 )
    n = atoi ( argv[1] );

  createStrings ( n ); 
  
  exit(0);
}
