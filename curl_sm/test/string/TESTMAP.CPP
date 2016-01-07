#include <pthread.h>
#include "util.h"

class CMyMap
{

private:
  map < CGenString, int > m_mymap;
  typedef map < CGenString, int > GenStringIntMap;

public:  

  void add ( const char *str, int i )
  {
    m_mymap[str] = i;
  }

  void remove ( const char *str )
  {
    FAST_STRING(s,str);
    m_mymap.erase ( s );
  }

  bool find ( const char *str, int *res )
  {
    map < CGenString, int >::const_iterator ci;   

    FAST_STRING(s,str);
    ci = m_mymap.find(s);

    bool found = ( ci != m_mymap.end() ); 

    if ( found )
      *res = ci->second;

    return found;
  }
};

int main ( int argc, char **argv )
{
  CMyMap mymap;

  // add an element to the map    

  printf ( "***** adding an element...\n" );
  mymap.add("something1", 7);
  printf ( "***** done adding\n" );

  int res;

  // find an element

  printf ( "***** finding an element...\n" );
  mymap.find("something1", &res );
  printf ( "***** done finding\n" );

  // remove an element

  printf ( "***** removing an element...\n" );
  mymap.remove ( "something1" );
  printf ( "***** done removing...\n" );

  return 0;
}
