#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
class InvalidRangeException
{
};
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRangeList;

class CRange
{
  public:
    CRange( long long loIn, long long hiIn);
    CRange ( long long loIn ) : lo(loIn), hi(loIn){}
    friend class CRangeList;
    friend CRangeList operator + ( const CRange & oneC, const CRange & twoC );

    friend ostream & operator << ( ostream & out, const CRange & one );
  private:
    long long lo;
    long long hi;
};

CRange::CRange ( long long loIn, long long hiIn )
{
  if ( loIn > hiIn )
    throw InvalidRangeException();
  else
  {
    lo = loIn;
    hi = hiIn;
  }
}

ostream & operator << ( ostream & out, const CRange & one )
{
  if (one.lo==one.hi)
    return out << one.lo;
  else
    return  out << "<" << one.lo << ".." << one.hi << ">" ;
}




class CRangeList
{
  public:
    // constructor
    CRangeList(){}
    ~CRangeList(){ delVec(); }
    CRangeList(const CRangeList & one);
    CRangeList(const CRange & one);
    // Includes long long / range
    bool Includes ( const long long & num ) const ;
    bool Includes ( const CRange    & rangeC ) const;
    // += range / range list
    CRangeList & operator += ( const CRange     & rangeC  );
    CRangeList & operator += ( const CRangeList & rangeListC );
    // -= range / range list
    CRangeList & operator -= ( const CRange   & rangeC );
    CRangeList &  operator -= ( const CRangeList & rangeListC );
    // = range / range list
    CRangeList & operator = (const  CRange           & rangeC );
    CRangeList & operator = ( const CRangeList & rangeListC );
    // operator ==
    bool operator == ( const CRangeList & one ) const;
    // operator !=
    bool operator != ( const CRangeList & one ) const;
    // operator <<
    static bool cmpLoCRange( const CRange * one, const CRange * two );
    static bool cmpHiCRange( const CRange * one, const CRange * two );
    static bool cmpLoLessCRange( const CRange * one, const CRange * two );
    friend ostream & operator << ( ostream & stream, const CRangeList & one );
    // operator +

    friend CRangeList operator + ( const CRange & oneC, const CRange & twoC );
    friend CRangeList operator - ( const CRange & oneC, const CRange & twoC );

    CRangeList & operator + ( const CRange & oneC );
    CRangeList & operator - ( const CRange & oneC );
  private:
    void delVec();
    vector <CRange*> listRange;
};

CRangeList::CRangeList(const CRangeList & one)
{
  *this = one;
}
CRangeList::CRangeList(const CRange & one)
{
  if ( one.lo > one.hi )
    throw InvalidRangeException();
  *this+=one;
}

bool CRangeList::Includes ( const long long & num ) const
{
  CRange a(num);
  return Includes(a);
}
bool CRangeList::Includes ( const CRange  & rangeC ) const
{
  if (listRange.size() == 0 )
    return false;

  vector <CRange*> :: const_iterator iter = lower_bound( listRange.begin(), listRange.end(), &rangeC, cmpLoCRange );

  if (iter == listRange.end())
    --iter;

  if ( iter == listRange.begin())
  {
    if ((*iter)->lo <= rangeC.lo && (*iter)->hi >= rangeC.hi)
      return true;
    else
      return false;
  }
  if ( (*iter)->lo <= rangeC.lo && (*iter)->hi >= rangeC.hi )
    return true;
  if ( (*iter)->lo <= rangeC.lo && (*iter)->hi < rangeC.hi )
    return false;
  if ( (*iter)->lo > rangeC.lo )
  {
    if ( (*(iter-1))->hi >= rangeC.hi && (*(iter-1))->lo <= rangeC.lo )
      return true;
    else
      return false;
  }
  return false;
}



bool CRangeList::operator == ( const CRangeList & one ) const
{
  vector <CRange*> :: const_iterator iterOne = listRange.begin();
  vector <CRange*> :: const_iterator iterTwo = one.listRange.begin() ;

  if ( iterOne == iterTwo )
    return true;

  while (true)
  {
    if ( iterOne == listRange.end() && iterTwo == one.listRange.end() )
      return true;
    else if( iterOne == listRange.end() || iterTwo == one.listRange.end())
      return false;

    if ( (*iterOne)->lo == (*iterTwo)->lo && (*iterOne)->hi == (*iterTwo)->hi )
    {
      ++iterOne;
      ++iterTwo;
    }
    else
      return false;
  }
}

bool CRangeList::operator != ( const CRangeList & one ) const
{
  return !(*this==one);
}

CRangeList operator + ( const CRange & oneC, const CRange & twoC )
{
  CRangeList a;
  a += oneC;
  a += twoC;
  return a;
}
CRangeList operator - ( const CRange & oneC, const CRange & twoC )
{
  CRangeList a;
  a += oneC;
  a -= twoC;
  return a;
}

CRangeList & CRangeList::operator + ( const CRange & oneC )
{
  *this += oneC;
  return *this;
}
CRangeList & CRangeList::operator - ( const CRange & oneC )
{
  *this -= oneC;
  return *this;
}

CRangeList & CRangeList::operator -= ( const CRangeList & rangeListC )
{
  if ( listRange.size() == 0)
    return *this;
  if ( *this == rangeListC)
    delVec();
  else
  {
    for ( auto iter : rangeListC.listRange )
    {
      *this -= *iter;
    }
  }
  return *this;
}

CRangeList & CRangeList::operator += ( const CRangeList & rangeListC )
{
  if ( *this == rangeListC )
    return *this;
  for ( auto iter : rangeListC.listRange )
    *this += *iter;
  return *this;
}

void CRangeList::delVec()
{
  for ( size_t i = 0 ; i < listRange.size(); ++i )
  {
    delete listRange[i];
  }
  listRange.clear();
}

ostream & operator << ( ostream & stream, const CRangeList & one )
{
  stream << "{";
  for ( size_t i = 0; i < one.listRange.size(); ++i )
  {
    stream << * one.listRange[i] << ( (i != one.listRange.size() -1 )? (","):("") ) ;
  }
  stream << "}";
  return stream;
}

bool CRangeList::cmpLoCRange( const CRange * one, const CRange * two )
{
  if ( one->lo > two->lo )
    return false;
  return true;
}

bool CRangeList::cmpHiCRange( const CRange * one, const CRange * two )
{
  if ( one->hi > two->hi )
    return false;
  return true;
}

CRangeList & CRangeList::operator = ( const CRange & rangeC )
{
  if ( listRange.size() != 0)
    delVec();
  CRange * tmp = new CRange(rangeC.lo, rangeC.hi);
  listRange.emplace_back(tmp);
  return *this;
}


CRangeList & CRangeList::operator = ( const CRangeList & rangeListC )
{
  if ( *this == rangeListC )
    return *this;
  if ( listRange.size() != 0 )
    delVec();
  CRange * tmp = NULL;
  for ( size_t i = 0 ; i < rangeListC.listRange.size(); ++i )
  {
    tmp = new CRange(rangeListC.listRange[i]->lo, rangeListC.listRange[i]->hi);
    //listRange[i] = tmp;
    listRange.emplace_back(tmp);
  }
  return *this;
}

bool CRangeList::cmpLoLessCRange( const CRange * one, const CRange * two )
{
  if ( one->lo > two->hi )
    return false;
  return true;
}

CRangeList & CRangeList::operator -= ( const CRange & rangeC )
{

  if ( listRange.size() == 0 )
  {
    return *this;
  }

  CRange * tmp = new CRange(rangeC.lo, rangeC.hi);
  // smaller
  vector <CRange*> :: iterator iterLo = lower_bound( listRange.begin(), listRange.end(), tmp, cmpLoCRange );
  // higher
  vector <CRange*> :: iterator iterHi = lower_bound( listRange.begin(), listRange.end(), tmp, cmpHiCRange );

  delete tmp;

  if ( iterLo == listRange.end() )
  {
    --iterLo;
    //cout << endl << (*iterLo)->lo << " - " << (*iterLo)->hi << endl;

    if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
    {
      delete * iterLo;
      listRange.erase(iterLo);
    }
    else if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->lo <= rangeC.hi && (*iterLo)->hi > rangeC.hi )
      (*iterLo)->lo = ( rangeC.hi + 1);
    else if ( (*iterLo)->hi >= rangeC.lo && (*iterLo)->hi <= rangeC.hi)
      (*iterLo)->hi = ( rangeC.lo - 1);
    else if ( (*iterLo)->lo < rangeC.lo && (*iterLo)->hi > rangeC.hi )
    {
      //cout << "this one "<< endl;
      tmp = new CRange((*iterLo)->lo, rangeC.lo-1);
      (*iterLo)->lo = rangeC.hi+1;
      iterLo = listRange.insert(iterLo, tmp);
    }
  }

  else
  {
    if ( (*iterLo)->lo > rangeC.lo && iterLo != listRange.begin() )
      --iterLo;
    if ( iterHi == listRange.end() )
      --iterHi;

    else if ( iterHi != listRange.begin() && iterHi!=iterLo)
    {
      if ( (*iterHi)->lo <= rangeC.hi && (*iterHi)->hi > rangeC.hi )
      {
        (*iterHi)->lo = rangeC.hi+1;
        --iterHi;
      }
      else if ( (*iterHi)->lo > rangeC.hi )
        --iterHi;
    }

    while ( iterLo != iterHi )
    {
      delete * iterHi;
      listRange.erase(iterHi);
      --iterHi;
    }
  //  cout << "worry not "<< *this << endl;

    if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
    {
      delete * iterLo;
      listRange.erase(iterLo);
    }

    else if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->lo <= rangeC.hi && (*iterLo)->hi > rangeC.hi )
      (*iterLo)->lo = ( rangeC.hi + 1);

    else if ( (*iterLo)->hi >= rangeC.lo && (*iterLo)->hi <= rangeC.hi)
      (*iterLo)->hi = ( rangeC.lo - 1);

    else if ( (*iterLo)->lo < rangeC.lo && (*iterLo)->hi > rangeC.hi )
    {
      //cout << "this one "<< endl;
      tmp = new CRange((*iterLo)->lo, rangeC.lo-1);
      (*iterLo)->lo = rangeC.hi+1;
      iterLo = listRange.insert(iterLo, tmp);
    }
/*
    if ( iterLo == listRange.begin()  ) // && (*iterLo)->lo >= rangeC.lo
    {

      if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
      {
        delete * iterLo;
        listRange.erase(iterLo);
      }
      else if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi > rangeC.hi )
        (*iterLo)->lo = ( rangeC.hi + 1);
      else if ( (*iterLo)->hi >= rangeC.lo && (*iterLo)->hi <= rangeC.hi)
        (*iterLo)->hi = ( rangeC.lo - 1);
      else if ( (*iterLo)->lo < rangeC.lo && (*iterLo)->hi > rangeC.hi )
      {
        //cout << "this one "<< endl;
        tmp = new CRange((*iterLo)->lo, rangeC.lo-1);
        (*iterLo)->lo = rangeC.hi+1;
        iterLo = listRange.insert(iterLo, tmp);
      }


     if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
      {
        delete *iterLo;
        listRange.erase(iterLo);
      }
      else if ( (*iterLo)->lo <= rangeC.hi )
        (*iterLo)->lo = rangeC.hi + 1;
    }
    else
    {

      if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
      {
        delete * iterLo;
        listRange.erase(iterLo);
      }

      else if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi > rangeC.hi )
        (*iterLo)->lo = ( rangeC.hi + 1);

      else if ( (*iterLo)->hi >= rangeC.lo && (*iterLo)->hi <= rangeC.hi)
        (*iterLo)->hi = ( rangeC.lo - 1);

      else if ( (*iterLo)->lo < rangeC.lo && (*iterLo)->hi > rangeC.hi )
      {
        //cout << "this one "<< endl;
        tmp = new CRange((*iterLo)->lo, rangeC.lo-1);
        (*iterLo)->lo = rangeC.hi+1;
        iterLo = listRange.insert(iterLo, tmp);
      }
      if ( (*iterLo)->lo < rangeC.lo && (*iterLo)->hi > rangeC.hi )
      {
        tmp = new CRange((*iterLo)->lo, rangeC.lo-1);
        (*iterLo)->lo = rangeC.hi+1;
        iterLo = listRange.insert(iterLo, tmp);
      }

      else if ( ( *(iterLo) )->lo < rangeC.lo && (*iterLo)->hi >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
      {
        (*(iterLo))->hi = rangeC.lo - 1 ;
      }
      else if ( ( *(iterLo) )->lo >= rangeC.lo && (*iterLo)->hi > rangeC.hi )
      {
        (*(iterLo))->lo = rangeC.hi + 1 ;
      }
      else if ( (*iterLo)->lo >= rangeC.lo && (*iterLo)->hi <= rangeC.hi )
      {
        delete * iterLo;
        listRange.erase(iterLo);
      }

    }*/
  }
  return *this;
}

CRangeList & CRangeList::operator += ( const CRange & rangeC  )
{
  CRange * tmp = new CRange(rangeC.lo, rangeC.hi);
  // smaller
  vector <CRange*> :: iterator iterLo = lower_bound( listRange.begin(), listRange.end(), tmp, cmpLoCRange );
  // higher
  vector <CRange*> :: iterator iterHi = lower_bound( listRange.begin(), listRange.end(), tmp, cmpHiCRange );

  if (listRange.size() == 0)
    listRange.emplace_back(tmp);
  else if ( ( iterLo == listRange.begin() && rangeC.lo <= (*iterLo)->lo) && iterHi == listRange.end())
  {
    delVec();
    listRange.clear();
    listRange.emplace_back(tmp);
  }
  else if ( iterLo == listRange.end() )
  {
    if ( ( ( (*(iterLo-1))->hi != LLONG_MAX && (((*(iterLo-1))->hi + 1) ) >= rangeC.lo) || (((*(iterLo-1))->hi) >= rangeC.lo && (*(iterLo-1))->hi == LLONG_MAX )  ) )
    {
      if ( (*(iterLo-1))->lo > rangeC.lo )
        (*(iterLo-1))->lo = rangeC.lo;
      if ( (*(iterLo-1))->hi < rangeC.hi )
        (*(iterLo-1))->hi = rangeC.hi;
      delete tmp;
    }
    else if ( (*(iterLo-1))->hi  < rangeC.lo  )
    {
      listRange.emplace_back(tmp);
    }
    else
      delete tmp;
  }
  else
  {
      if ( iterHi == listRange.end() )
      --iterHi;

      if ( iterHi == iterLo )
      {
        if ( iterLo == listRange.begin() )
        {
          if ( rangeC.hi == LLONG_MAX )
          {
            if ( (rangeC.hi) > (*iterLo)->hi )
              (*iterLo)->hi = rangeC.hi;
            if ( (*iterLo)->lo > rangeC.lo )
              (*iterLo)->lo = rangeC.lo;
            delete tmp;
          }
          else
          {
            if ( (rangeC.hi+1) < (*iterLo)->lo )
              iterLo = listRange.insert(iterLo, tmp);
            else
            {
              if ( (*iterLo)->lo > rangeC.lo )
                (*iterLo)->lo = rangeC.lo;
              if ((*iterLo)->hi < rangeC.hi)
                (*iterLo)->hi = rangeC.hi;
              delete tmp;
            }
          }
        }
        else if ( ( rangeC.lo <= ((*(iterLo-1))->hi)  ) || ( ( * ( iterLo - 1 ) )->hi != LLONG_MAX && rangeC.lo <= ((*(iterLo-1))->hi + 1 ) ))
        {
          if ( ( (rangeC.hi+1) >= (*iterLo)->lo && rangeC.hi != LLONG_MAX) || ( (rangeC.hi) >= (*iterLo)->lo)  )
          {
            if ( (*(iterLo-1))->lo > rangeC.lo )
              (*(iterLo-1))->lo = rangeC.lo;
            (*(iterLo-1))->hi = (*iterLo)->hi;
            delete *iterLo;
            delete tmp;
            listRange.erase(iterLo);
          }
          else
          {
            if ( (*(iterLo-1))->lo > rangeC.lo )
              (*(iterLo-1))->lo = rangeC.lo;
            (*(iterLo-1))->hi = rangeC.hi;
            delete tmp;
          }
        }

        else if ( ((rangeC.hi+1) >= ( ( *iterLo )->lo ) && rangeC.hi != LLONG_MAX ) || ( (rangeC.hi) >= ( ( *iterLo )->lo )) )
        {
          if ( (*iterLo)->lo > rangeC.lo )
            (*iterLo)->lo = rangeC.lo;
          delete tmp;
        }
        else
        {
          iterLo = listRange.insert(iterLo, tmp);
        }

      }

      else
      {
        delete tmp;
        if ( iterLo == listRange.begin() )
        {
          if ( (rangeC.lo) < (*iterLo)->lo )
            (*iterLo)->lo = rangeC.lo;
        }
        else
        {
          if ( ( ((* (iterLo-1) )->hi ) != LLONG_MAX && ((* (iterLo-1) )->hi + 1 ) >= rangeC.lo )|| ((* (iterLo-1) )->hi) >= rangeC.lo )
            --iterLo;
        }
        if ( ( rangeC.hi != LLONG_MAX && (rangeC.hi+1) >= (*iterHi)->lo ) || (rangeC.hi) >= (*iterHi)->lo )
        {
          if (rangeC.hi >= (*iterHi)->hi)
            (*iterLo)->hi = rangeC.hi;
          else
            (*iterLo)->hi = (*iterHi)->hi;
        }
        else
        {
          --iterHi;
          (*iterLo)->hi = rangeC.hi;
        }
        while (iterLo != iterHi  )
        {
          delete * iterHi;
          listRange.erase(iterHi);
          --iterHi;
        }
      }
    }
  return *this;
}

#ifndef __PROGTEST__
string             toString                                ( const CRangeList & x )
{
  ostringstream oss;
  oss  << x;
  return oss . str ();
}


void test0()
{
  // testing copy constructor
  CRangeList a = CRange(0, 1);
  CRangeList b = CRange(0,1) + CRange(2,4) - CRange(10, 12);
  CRangeList c = CRange(1,2);
  CRangeList uno = CRange( -1, 1 );
    assert( toString(uno) == "{<-1..1>}");
  uno -= CRange(LLONG_MIN, LLONG_MAX);
  assert( toString(uno) == "{}");
  uno -= CRange(10, 11) + CRange(20, 30) - CRange(19, 29);
  assert( toString(uno) == "{}");
}

void test1()
{
  CRangeList a = CRange(10, 20) + CRange(22, 40) + CRange(42, 50)+ CRange(52, 200);
  assert(toString(a) == "{<10..20>,<22..40>,<42..50>,<52..200>}");
  a += CRange(21) + CRange(41)+CRange(50);
  assert(toString(a) == "{<10..50>,<52..200>}");
  a += CRange(LLONG_MIN, LLONG_MAX);
//  cout << a << endl;
  a -= CRange(20) + CRange(30)+CRange(40) - CRange(30, 40) + CRange(100) + CRange(LLONG_MIN, LLONG_MAX-2);
//  cout << a << endl;
  assert(toString(a) == "{<9223372036854775806..9223372036854775807>}");
  a = a = a;
  assert(toString(a) == "{<9223372036854775806..9223372036854775807>}");

  a-=a;
  a+=a;
//  cout<< a << endl;
  a += CRange(0, 10) + CRange(-2, 20);
  a += CRange(LLONG_MIN);
  a+= CRange(LLONG_MAX);
  a+=CRange(21, LLONG_MAX-2);
  a+=CRange(0);
  //cout << a.Includes(0);
  //cout << a.Includes(LLONG_MAX);
  //cout << a.Includes(LLONG_MAX-1);
  //cout << a << endl;
}


int                main                                    ( void )
{
  test0();
  test1();

  CRangeList a, b;

  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
  a = CRange ( 5, 10 );
  a += CRange ( 25, 100 );

  assert ( toString ( a ) == "{<5..10>,<25..100>}" );
  a += CRange ( -5, 0 );
  a += CRange ( 8, 50 );
  assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( -400, -400 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );

  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == NULL );
  }
  catch ( const InvalidRangeException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == NULL );
  }
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . Includes ( 15 ) );
  assert ( b . Includes ( 2900 ) );
  assert ( b . Includes ( CRange ( 15, 15 ) ) );
  assert ( b . Includes ( CRange ( -350, -350 ) ) );
  assert ( b . Includes ( CRange ( 100, 200 ) ) );
  assert ( !b . Includes ( CRange ( 800, 900 ) ) );
  assert ( !b . Includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . Includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );

#ifdef EXTENDED_SYNTAX
  /*
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
*/
#endif /* EXTENDED_SYNTAX */
  return 0;
}
#endif /* __PROGTEST__ */
