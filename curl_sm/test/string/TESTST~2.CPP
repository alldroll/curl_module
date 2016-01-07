#include <pthread.h>
#include "util.h"

void *createStrings ( void *n_void )
{
  int n = (int) n_void;
  static char *samples[] = { "something1","something2","something3",
                             "something4" };

  printf ( "Thread <%d>: Creating/destroying %d strings...\n", 
           pthread_self(), n );

  unsigned int start = utilGetTickCount ();

  for ( int i=0; i<n; i++ )
  {
    string s ( samples[i%4] );
  }

  unsigned int end = utilGetTickCount ();

  printf ( "Thread <%d>: total time=%d millisecs, average=%f\n", 
           pthread_self(), end-start, (float)(end-start)/n );

  return NULL;
}

int main ( int argc, char **argv )
{
  if ( argc > 3 )
  {
    printf ( 
       "teststring1 [<n_of_strings_to_create>] [<n_threads>]\n" );
    return 0;
  }

  int n = 100000;
  int n_thr = 10;

  if ( argc > 1 )
    n = atoi ( argv[1] );

  if ( argc > 2 )
    n_thr = atoi ( argv[2] );

  pthread_attr_t attr;
  pthread_attr_init ( &attr );

  pthread_t *tid = new pthread_t[n_thr];

  int i;

  // create threads
  for  ( i=0; i<n_thr; i++ )
  {
    if ( pthread_create ( &tid[i], NULL, createStrings, (void *) n ) )
    {
      printf ( "ERROR: ptread_create for thread <%d>\n", i );
      exit(-1);
    }
  }

  // wait for all threads to terminate
  for  ( i=0; i<n_thr; i++ )
    pthread_join ( tid[i], 0 );
  
  exit(0);
}
