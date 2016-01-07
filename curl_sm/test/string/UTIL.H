// 
//  util.h
//
//  utility classes
//
//  CGenString class uses SString code by Bruce Eckel

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <dlfcn.h>
#endif

using namespace std;

// String template for three types of strings:
// heap  ( with alloc/copy )
// stack ( automatic fixed size array, just copy )
// just a pointer ( no alloc/copy, fastest )

template<int bufsize>
class CGenStringT
{

public:

  CGenStringT ( const char *S = "", int bAlloc = 1 ) : m_s(m_buf)
  {
    if ( !bufsize )
    {
      if ( !( m_bAlloc = bAlloc ) )
      {
        // fast version without allocation
        m_s = (char *) S;
      }
      else
      {
        // make on heap
        m_s = new char [ strlen(S)+1 ];
        strcpy ( m_s, S );
      }
    }
    else
    {
      // make on stack
      m_buf[bufsize] = 0;
      strncpy ( m_s, S, bufsize );
    }

#ifdef MEM_ALLOC_DEBUG
    printf ("CGenString constructor called: this=%x allocation=%s"
            " ptr=%x\n",
            this, (!bufsize && bAlloc ) ? "yes":"no", m_s );
#endif

  }

  CGenStringT ( const CGenStringT& rv ): m_s(m_buf)
  {
    if ( !bufsize )
    {
      if ( m_bAlloc = rv.m_bAlloc )
      {
        // make on heap
        m_s = new char [ strlen(rv.m_s)+1 ];
        strcpy ( m_s, rv.m_s );
      }
      else
        m_s = rv.m_s;
    }
    else
    {
      // make on stack
      m_buf[bufsize] = 0;
      strncpy ( m_s, rv.m_s, bufsize );
    }

#ifdef MEM_ALLOC_DEBUG
    printf ("CGenString copy constructor called: this=%x allocation=%s"
            " ptr=%x\n",
            this, (!bufsize && m_bAlloc ) ? "yes":"no", m_s );
#endif

  }

  CGenStringT& operator=(const CGenStringT& rv )
  {

#ifdef MEM_ALLOC_DEBUG
    printf ("CGenString assignment op called: this=%x allocation=%s\n",
             this, (!bufsize && m_bAlloc ) ? "yes":"no" );
#endif

    // check for self-assignment
    if ( &rv == this )
      return *this;

    if ( !bufsize )
    {
      if ( m_bAlloc )
      {
        // manage heap
        delete [] m_s;
        m_s = new char [ strlen(rv.m_s)+1 ];
        strcpy ( m_s, rv.m_s );
      }
      else
        m_s = rv.m_s;
    }
    else
    {
      // stack version
      m_buf[bufsize] = 0;
      strncpy ( m_s, rv.m_s, bufsize );
    }

    return *this;
  }

  ~CGenStringT()
  {

#ifdef MEM_ALLOC_DEBUG
    printf ("CGenString destructor called: this=%x\n", this);
#endif

    if ( !bufsize && m_bAlloc) 
      delete m_s;
  }

  int operator==(const CGenStringT& rv) const
  {
    return !strcmp ( m_s, rv.m_s );
  }

  int operator!=(const CGenStringT& rv) const
  {
    return strcmp ( m_s, rv.m_s );
  }

  int operator>(const CGenStringT& rv) const
  {
    return strcmp ( m_s, rv.m_s )>0;
  }

  int operator<(const CGenStringT& rv) const
  {
    return strcmp ( m_s, rv.m_s )<0;
  }

  const char* str() const
  {
    return m_s;
  }

  friend ostream& operator<<(ostream& os, const CGenStringT<bufsize>& S )
  {
    return os << S.m_s;
  }

private:

  char *m_s;
  int   m_bAlloc;
  char  m_buf[bufsize+1];
};

typedef CGenStringT<0> CGenString;
typedef CGenStringT<100> CStackString100;
#define FAST_STRING(id,str) CGenString id(str,0);

// return current time in millisecs with 24 hours reset
//-----------------------------------------------------------------------------
inline unsigned int utilGetTickCount ()
//-----------------------------------------------------------------------------
{

#ifdef WIN32
  return GetTickCount();
#else
  struct timeval t;

  gettimeofday ( &t, NULL );
  t.tv_sec %= 8640; // one day ticks 24*60*60

  return ( t.tv_sec*1000 + t.tv_usec/1000 );
#endif

}

#endif // _UTIL_H_

// Local Variables:
// mode:C++
// End:
