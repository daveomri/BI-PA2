#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */


// -- insides of database --
struct LandOfMine{
  string region;
  string city;
  string addr;
  unsigned int id;
  string owner;
  LandOfMine * next;
  LandOfMine * prev;
  LandOfMine () : region(""), city(""), addr(""), id(0), owner(""), next(NULL), prev(NULL) {}
};
struct nextOne
{
  LandOfMine * inBase;
  nextOne * next;

  nextOne() : inBase(NULL), next(NULL) {}
};
// -- end of insides of database --



class CIterator
{
  public:
    bool                     AtEnd                         ( void ) const;
    void                     Next                          ( void );
    string                   City                          ( void ) const;
    string                   Addr                          ( void ) const;
    string                   Region                        ( void ) const;
    unsigned                 ID                            ( void ) const;
    string                   Owner                         ( void ) const;
    CIterator (vector <LandOfMine*> iter, size_t vecLen, size_t iV = 0) : iter(iter), vecLen(vecLen), iV(0) {}
  private:
    vector <LandOfMine*> iter;
    size_t vecLen;
    size_t iV;
};


class CLandRegister
{
  public:
    bool                     Add                           ( const string    & city,
                                                             const string    & addr,
                                                             const string    & region,
                                                             unsigned int      id );

    bool                     Del                           ( const string    & city,
                                                             const string    & addr );

    bool                     Del                           ( const string    & region,
                                                             unsigned int      id );

    bool                     GetOwner                      ( const string    & city,
                                                             const string    & addr,
                                                             string          & owner ) const;

    bool                     GetOwner                      ( const string    & region,
                                                             unsigned int      id,
                                                             string          & owner ) const;

    bool                     NewOwner                      ( const string    & city,
                                                             const string    & addr,
                                                             const string    & owner );

    bool                     NewOwner                      ( const string    & region,
                                                             unsigned int      id,
                                                             const string    & owner );

    unsigned                 Count                         ( const string    & owner ) const;

    CIterator                ListByAddr                    ( void ) const;

    CIterator                ListByOwner                   ( const string    & owner ) const;

// - - - - - - - - - - - - - - - my methods - - - - - - - - - - - - - - -
    bool                     inBase(const string & city, const string & address);
    bool                     inBase(const string & region, unsigned int id);

    static bool              cmpBase( const LandOfMine * first, const LandOfMine * second );

    void                     logOwner(const string & owner,  LandOfMine * & inDase);

    void                     logNewRegId(const string & region,  const unsigned int id, LandOfMine * & inDase);

    void                     logNewCityAddr(const string & city, const string & address, LandOfMine * & inDase);

    LandOfMine * findBase      (  const string & city, const string & addr  )                       const ;
    LandOfMine * findBase      (  const string & region, unsigned int id  )                         const ;
    void         delOwner      (  const string & owner, const LandOfMine * insBase );
    void         delAddr       (  const LandOfMine * insBase  );
    void         delCityAddr   (  const string & city, const string & addr  );
    void         delRegionId   (  const string & region, unsigned int id) ;


    CLandRegister ()
    {
      landBase = NULL;
      sizeBase = 0;

      ownerArr = new ownerArray * [769];
      ownArrSize = 769;
      for ( size_t i = 0; i < ownArrSize; i++ )
        ownerArr[i] = NULL;

      regIdArr = new nextOne * [769];
      regIdArrSize = 769;
      for ( size_t i = 0; i < regIdArrSize; i++ )
        regIdArr[i] = NULL;

      cityAddrArr = new nextOne * [769];
      cityAddrArrSize = 769;
      for ( size_t i = 0; i < cityAddrArrSize; i++ )
        cityAddrArr[i] = NULL;
    }
    ~CLandRegister()
    {
      LandOfMine * tmpland = NULL;
      while (landBase)
      {
        tmpland = landBase->next;
        delete landBase;
        landBase = tmpland;
      }
      nextOne * tmp = NULL;
      for ( size_t i = 0; i < cityAddrArrSize; i++ )
      {
        tmp = cityAddrArr[i];
        while ( tmp )
        {
          tmp = cityAddrArr[i]->next;
          delete cityAddrArr[i];
          cityAddrArr[i] = tmp;
        }
      }
      delete [] cityAddrArr;

      tmp = NULL;
      for ( size_t i = 0; i < regIdArrSize; i++ )
      {
        tmp = regIdArr[i];
        while ( tmp )
        {
          tmp = regIdArr[i]->next;
          delete regIdArr[i];
          regIdArr[i] = tmp;
        }
      }
      delete [] regIdArr;

      ownerArray * tmpl = NULL;
      for ( size_t i = 0; i < ownArrSize; i++ )
      {
        tmpl = ownerArr[i];
        while ( tmpl )
        {
          tmpl = ownerArr[i]->nextOwner;
          delete ownerArr[i];
          ownerArr[i] = tmpl;
        }
      }
      delete [] ownerArr;


    }

  private:

    struct ownerArray
    {
      string owner;
      ownerArray * nextOwner;
      size_t vecSize;
      vector<LandOfMine*> ownLands;
      ownerArray () : owner(""), nextOwner(NULL), vecSize(0) { }
    };

    LandOfMine    *  landBase;
    size_t        sizeBase;

    vector <LandOfMine*> addrVector;

    // - - - - - - hash function - - - - - - -

    uint32_t  oneKeyHashFunc   (const string & key) const;
    uint32_t  twoKeysHashFunc  (const string & key1, const string & key2) const;
    uint32_t  twoKeysHashFunc  (const string & key1, const unsigned int & key2) const;

    void  reHash(nextOne ** & arrOr, uint32_t & numOr, int mode);
    void  reHashOwner();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



    ownerArray ** ownerArr;
    uint32_t      ownArrSize;

    nextOne ** regIdArr;
    uint32_t    regIdArrSize;


    nextOne ** cityAddrArr;
    uint32_t    cityAddrArrSize;

// - - - - - - end of hash function - - -
friend class CIterator;
};


uint32_t CLandRegister::oneKeyHashFunc(const string & key) const
{
  uint32_t hashOut  = 0;
  uint32_t primNum   = 29;

  for (size_t i = 0; i < key.size(); i++)
    hashOut += ((uint32_t)((uint8_t)key[i])) * primNum + hashOut;

  return hashOut;
}


uint32_t CLandRegister::twoKeysHashFunc(const string & key1, const string & key2) const
{
  return ( oneKeyHashFunc(key1) * 7 + oneKeyHashFunc(key2) * 3 ); // multiplying by prime number to avoid collisions like key1=text1 key2=text2, key1=text2 key2=text1
}


uint32_t CLandRegister::twoKeysHashFunc(const string & key1, const unsigned int & key2) const
{
  return ( oneKeyHashFunc(key1) + key2 );
}


void  CLandRegister::reHash(nextOne ** & arrOr, uint32_t & numOr, int mode)
{
  nextOne               **  newArr;
  uint32_t                  newNum;
  uint32_t                  curPlace;
  nextOne               *   tmp;
  nextOne               *   saveThere;
  newNum = numOr * 2 + 1;
  newArr = new nextOne * [newNum];

  for (size_t i = 0; i < newNum; i++)
    newArr[i] = NULL;

  for (size_t i = 0; i < numOr; i++)
  {
    tmp = arrOr[i];
    while (tmp)
    {
      if (mode == 1)
        curPlace = twoKeysHashFunc(tmp->inBase->region, tmp->inBase->id)%newNum;
      else
        curPlace = twoKeysHashFunc(tmp->inBase->city, tmp->inBase->addr)%newNum;

      saveThere = newArr[curPlace];
      if (! saveThere )
      {
        newArr[curPlace] = tmp;
        saveThere = newArr[curPlace];
      }
      else
      {
        while (saveThere)
        {
          if ( ! saveThere->next )
          {
            saveThere->next = tmp;
            saveThere = saveThere->next;
            break;
          }
          else
            saveThere = saveThere->next;
        }
      }
      tmp = tmp->next;
      saveThere->next = NULL;
    }
  }
  delete [] arrOr;
  arrOr = newArr;
  numOr = newNum;
}

void  CLandRegister::reHashOwner()
{
  ownerArray            **  newArr;
  uint32_t                  newNum;
  uint32_t                  curPlace;

  ownerArray            *   tmp;
  ownerArray            *   saveThere;

  newNum = ownArrSize * 2 + 1;
  newArr = new ownerArray * [newNum];

  for (size_t i = 0; i<newNum; i++)
    newArr[i] = NULL;

  for (size_t i = 0; i < ownArrSize; i++)
  {
    tmp = ownerArr[i];
    while (tmp)
    {
      curPlace = oneKeyHashFunc(tmp->owner)%newNum;

      saveThere = newArr[curPlace];
      if (! saveThere )
      {
        newArr[curPlace] = tmp;
        saveThere = tmp;
      }
      else
      {
        while (saveThere)
        {
          if ( ! saveThere->nextOwner )
          {
            saveThere->nextOwner = tmp;
            saveThere = tmp;
            break;
          }
          else
            saveThere = saveThere->nextOwner;
        }
      }
      tmp = tmp->nextOwner;
      saveThere->nextOwner = NULL;


    }
  }

  delete [] ownerArr;
  ownerArr = newArr;
  ownArrSize = newNum;
}


bool                     CLandRegister::inBase           (const string & city, const string & address)
{
  size_t inDa = twoKeysHashFunc(city, address)%cityAddrArrSize;

  if ( ! cityAddrArr[inDa] )
    return false;

  nextOne * tmp = cityAddrArr[inDa];

  while ( tmp )
  {
    if ( tmp->inBase->city == city && tmp->inBase->addr == address )
      return true;
    tmp = tmp->next;
  }

  return false;
}

bool                     CLandRegister::inBase           (const string & region, unsigned int id)
{

  size_t inDa = twoKeysHashFunc(region, id)%regIdArrSize;
  if ( !  regIdArr[inDa] )
    return false;

  nextOne * tmp = regIdArr[inDa];
  while (  tmp )
  {
    if (  (tmp->inBase->region == region) &&  (tmp->inBase->id == id) )
      return true;
    tmp =  tmp->next;
  }
  return false;
}

bool operator > (const string & one, const string & two)
{
  if ( (one.compare(two)) < 0 )
    return true;
  return false;
}

string stringLower( string smallStr )
{

  for (size_t i = 0; i < smallStr.size(); i++)
  {
    if ( smallStr[i] >= 'A' && smallStr[i] <= 'Z' )
      smallStr[i] = 'z' - ( 'Z' - smallStr[i] );
  }
  return smallStr;
}

bool CLandRegister::cmpBase( const LandOfMine * first, const LandOfMine * second )
{
  if ( first->city != second->city )
  {
    if ( first->city > second->city )
    {
      return true;
    }
    else
    {
      return false;
    }

    /*if (  first->city > second->city )
      return true;*/
  }
  if ( first->addr != second->addr )
  {
    if ( first->addr > second->addr )
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}


void CLandRegister::logOwner(const string & owner, LandOfMine * & inDase)
{
  if ( ((double)(sizeBase) / ownArrSize ) > 0.7   )
    reHashOwner();
  string smallOwner = stringLower(owner);


  size_t inDa = oneKeyHashFunc(smallOwner)%ownArrSize;
  if ( ! ownerArr[inDa] )
  {
    ownerArr[inDa] = new ownerArray;
    ownerArr[inDa]->ownLands.push_back(inDase);
    ownerArr[inDa]->owner = smallOwner;
    ownerArr[inDa]->vecSize++;
  }
  else
  {
    ownerArray * tmp = ownerArr[inDa];
    while ( tmp )
    {
      if ( tmp->owner == smallOwner )
      {
        tmp->ownLands.push_back(inDase);
        tmp->vecSize++;
        break;
      }
      if ( tmp->nextOwner == NULL )
      {
        tmp->nextOwner = new ownerArray;
        tmp = tmp->nextOwner;
        tmp->ownLands.push_back(inDase);
        tmp->owner = smallOwner;
        tmp->vecSize++;
        break;
      }
      else
        tmp = tmp->nextOwner;
    }
  }
}

void                     CLandRegister::logNewRegId(const string & region,  const unsigned int id, LandOfMine * & inDase)
{
  if ( ((double)(sizeBase) / regIdArrSize ) > 0.7   )
    reHash(regIdArr, regIdArrSize, 1);
  size_t inDa = twoKeysHashFunc(region, id)%regIdArrSize;
  if ( ! regIdArr[inDa] )
  {
      regIdArr[inDa] = new nextOne;
      regIdArr[inDa]->inBase = inDase;
  }
  else
  {
    nextOne * tmp = regIdArr[inDa];
    while ( tmp )
    {
      if ( tmp->next ==  NULL )
      {
        tmp->next = new nextOne;
        tmp->next->inBase = inDase;
        break;
      }
      else
        tmp = tmp->next;
    }
  }
}

void                     CLandRegister::logNewCityAddr(const string & city, const string & address, LandOfMine * & inDase)
{
  if ( ((double)(sizeBase) / cityAddrArrSize ) > 0.7   )
      reHash(cityAddrArr, cityAddrArrSize, 0);

  size_t inDa = twoKeysHashFunc(city, address)%cityAddrArrSize;
  if ( ! cityAddrArr[inDa] )
  {
      cityAddrArr[inDa] = new nextOne;
      cityAddrArr[inDa]->inBase = inDase;

  }
  else
  {
    nextOne * tmp = cityAddrArr[inDa];
    while ( tmp )
    {
      if ( tmp->next ==  NULL )
      {
        tmp->next = new nextOne;
        tmp->next->inBase = inDase;
        break;
      }
      else
        tmp = tmp->next;
    }
  }
}

bool                     CLandRegister::Add              ( const string    & city,
                                                         const string    & addr,
                                                         const string    & region,
                                                         unsigned int      id )
{
  if ( inBase(city, addr) || inBase(region, id) )
    return false;

  LandOfMine * newOne = new LandOfMine;
  newOne->city = city;
  newOne->addr = addr;
  newOne->region = region;
  newOne->id = id;

  if ( landBase )
    {
      //newOne->prev = landBase->prev;
      newOne->next = landBase;
      landBase->prev = newOne;
      landBase = newOne;
    }
  else
    landBase = newOne;

  if ( addrVector.size() > 0 )
  {
    vector<LandOfMine*> :: iterator iter = lower_bound ( addrVector.begin(), addrVector.end(), newOne, cmpBase );
    iter = addrVector.insert(iter, newOne);
  }
  else
    addrVector.push_back(newOne);

  logOwner("", newOne);
  logNewRegId(region, id, newOne);
  logNewCityAddr(city, addr, newOne);

  sizeBase++;

  return true;
}

LandOfMine * CLandRegister::findBase(const string & city, const string & addr) const
{
  uint32_t yek = twoKeysHashFunc(city, addr)%cityAddrArrSize;
  nextOne * tmp = cityAddrArr[yek];
  while (tmp)
  {
    if ( ! tmp )
      return NULL;
    if ( tmp->inBase->city == city && tmp->inBase->addr == addr )
      return tmp->inBase;
    tmp = tmp->next;
  }
  return NULL;
}

LandOfMine * CLandRegister::findBase(const string & region, unsigned int id) const
{
  uint32_t yek = twoKeysHashFunc(region, id)%regIdArrSize;
  nextOne * tmp = regIdArr[yek];
  while (tmp)
  {
    if ( ! tmp )
      return NULL;
    if ( tmp->inBase->region == region && tmp->inBase->id == id )
      return tmp->inBase;
    tmp = tmp->next;
  }
  return NULL;
}

bool                     CLandRegister::GetOwner         ( const string    & city,
                                                         const string    & addr,
                                                         string          & owner ) const
{
  LandOfMine * rightBase = findBase(city, addr);
  if ( ! rightBase )
    return false;
  owner = rightBase->owner;
  return true;
}

bool                     CLandRegister::GetOwner         ( const string    & region,
                                                         unsigned int      id,
                                                         string          & owner ) const
{
  LandOfMine * rightBase = findBase(region, id);
  if ( ! rightBase )
    return false;
  owner = rightBase->owner;
  return true;
}


void CLandRegister::delOwner ( const string & owner, const LandOfMine * insBase )
{
  string smallOwner = stringLower(owner);
  uint32_t yek = oneKeyHashFunc( smallOwner ) % ownArrSize;

  ownerArray * tmp = ownerArr[yek];
  ownerArray * nextThere = NULL;


  if ( tmp )
  {
    while (tmp)
    {
      if ( tmp->owner == smallOwner )
      {
        if ( tmp->ownLands.size() > 0 )
        {
          vector<LandOfMine*>::iterator iter = (tmp->ownLands).begin();
          for ( size_t i = 0; i < tmp->ownLands.size(); i++)
          {
            if ( insBase == (*iter) )
            {
              tmp->ownLands.erase(iter);
              break;
            }
            if ( i != ( tmp->ownLands.size() - 1 ) )
            iter++;
          }
          tmp->vecSize --;
          break;
        }
      }
      else
      {
        nextThere = tmp;
        tmp = tmp->nextOwner;
      }
    }
    if ( tmp )
    {
      if ( tmp -> ownLands.size() == 0 )
      {
        if( ownerArr[yek] == tmp )
        {
          ownerArr[yek] = tmp->nextOwner;
          delete tmp;
        }
        else
        {
          nextThere->nextOwner = tmp->nextOwner;
          delete tmp;
        }
      }
    }

  }
}
// * *

void CLandRegister::delAddr(const LandOfMine * insBase)
{
  if ( addrVector.size() > 0 )
  {
    vector<LandOfMine*> :: iterator iter = lower_bound ( addrVector.begin(), addrVector.end(), insBase, cmpBase );

    if ( insBase != (*iter) ){
      iter++;
    }
    addrVector.erase(iter);
  }
}

void CLandRegister::delCityAddr(const string & city, const string & addr)
{
  uint32_t yek = twoKeysHashFunc(city, addr)%cityAddrArrSize;


  if ( cityAddrArr[yek] )
  {
    nextOne * tmp = cityAddrArr[yek];
    nextOne * nextThere = NULL;

    while (tmp)
    {
      if ( tmp->inBase->city == city && tmp->inBase->addr == addr )
        break;
      else
      {
        nextThere = tmp;
        tmp = tmp->next;
      }
    }

    if( cityAddrArr[yek] == tmp && tmp != NULL )
    {
      cityAddrArr[yek] = tmp->next;
      delete tmp;
    }
    else if ( tmp != NULL && nextThere != NULL)
    {
      nextThere->next = tmp->next;
      delete tmp;
    }
  }
}

void CLandRegister::delRegionId(const string & region, unsigned int id)
{
  uint32_t yek = twoKeysHashFunc(region, id)%regIdArrSize;


  if ( regIdArr[yek] )
  {
    nextOne * tmp = regIdArr[yek];
    nextOne * nextThere = NULL;

    while ( tmp )
    {
      if ( tmp->inBase->region == region && tmp->inBase->id == id )
        break;
      else
      {
        nextThere = tmp;
        tmp = tmp->next;
      }
    }

    if( regIdArr[yek] == tmp && tmp != NULL )
    {
      regIdArr[yek] = tmp->next;
      delete tmp;
    }
    else if ( tmp != NULL && nextThere != NULL )
    {
      nextThere->next = tmp->next;
      delete tmp;
    }
  }
}


bool                     CLandRegister::Del              ( const string    & city,
                                                         const string    & addr )
{


  string owner;

  if ( ! GetOwner( city, addr, owner ) )
    return false;

  LandOfMine * baseMine = findBase(city, addr);

  if ( ! baseMine )
    return false;

  string region = baseMine->region;
  unsigned int id = baseMine->id;


  delOwner(owner, baseMine);

  delAddr(baseMine);

  delCityAddr(city, addr);

  delRegionId(region, id);

  if ( baseMine->prev == NULL )
    landBase = baseMine->next;
  if (baseMine->next)
    baseMine->next->prev = baseMine->prev;
  if (baseMine->prev)
    baseMine->prev->next = baseMine->next;
  delete baseMine;

  return true;
}

bool                     CLandRegister::Del              ( const string    & region,
                                                         unsigned int      id )
{
    string owner;
    if ( ! GetOwner( region, id, owner ) )
      return false;

    LandOfMine * baseMine = findBase(region, id);

    if ( ! baseMine )
      return false;

    string city = baseMine->city;
    string addr = baseMine->addr;



    delOwner(owner, baseMine);

    delAddr(baseMine);

    delCityAddr(city, addr);

    delRegionId(region, id);

    if ( baseMine->prev == NULL )
      landBase = baseMine->next;
    if (baseMine->next)
      baseMine->next->prev = baseMine->prev;
    if (baseMine->prev)
      baseMine->prev->next = baseMine->next;
    delete baseMine;

    return true;
}



unsigned                 CLandRegister::Count                         ( const string    & owner ) const
{
  string smallOwner = stringLower(owner);
  uint32_t yek = oneKeyHashFunc( smallOwner ) % ownArrSize;
  unsigned int outVal = 0;

  ownerArray * tmp = ownerArr[yek];

  if ( tmp )
  {
    while (tmp)
    {
      if ( tmp->owner == smallOwner )
      {
        outVal = (unsigned int)(tmp->ownLands.size());
        break;
      }
      else
      {
        tmp = tmp->nextOwner;
      }
    }
  }
  return outVal;
}



bool                     CLandRegister::NewOwner             ( const string    & city,
                                                             const string    & addr,
                                                             const string    & owner )
{
  string oldOwner;
  if ( ! GetOwner(city, addr, oldOwner) )
    return false;
  if (stringLower(oldOwner) == stringLower(owner))
    return false;

  LandOfMine * base = findBase(city, addr);

  if ( ! base )
    return false;

  delOwner(oldOwner, base);

  logOwner(owner, base);

  base->owner = owner;

  return true;
}

bool                     CLandRegister::NewOwner                      ( const string    & region,
                                                             unsigned int      id,
                                                             const string    & owner )
{
  string oldOwner;
  if ( ! GetOwner(region, id, oldOwner) )
    return false;
  if (stringLower(oldOwner) == stringLower(owner))
    return false;

  LandOfMine * base = findBase(region, id);

  if ( ! base )
    return false;

  delOwner(oldOwner, base);

  logOwner(owner, base);

  base->owner = owner;

  return true;

}




CIterator                CLandRegister::ListByAddr                    ( void ) const
{
  vector <LandOfMine*> iter = addrVector;
  CIterator outThis (iter, addrVector.size());
  return outThis;
}
// **

CIterator                CLandRegister::ListByOwner                   ( const string    & owner ) const
{
  size_t inDa = oneKeyHashFunc(stringLower(owner))%ownArrSize;
  string lowOwner = stringLower(owner);

  ownerArray * tmp = ownerArr[inDa];
  while ( tmp )
  {
    if ( tmp->owner == lowOwner )
      break;

    else
      tmp = tmp->nextOwner;
  }

  vector <LandOfMine*> iter;
  size_t numVec = 0;
  if (tmp)
  {
    iter = tmp->ownLands;
    numVec = tmp->ownLands.size();
  }

  CIterator outMe (iter, numVec);

  return outMe;
}



bool                     CIterator::AtEnd                         ( void ) const
{
  if (vecLen == iV)
    return true;
  return false;
}
void                     CIterator::Next                          ( void )
{
  iV++;
}
string                   CIterator::City                          ( void ) const
{
  string city = iter[iV]->city;
  return city;
}
string                   CIterator::Addr                          ( void ) const
{
  return iter[iV]->addr;
}
string                   CIterator::Region                        ( void ) const
{
  return iter[iV]->region;
}
unsigned                 CIterator::ID                            ( void ) const
{
  return iter[iV]->id;
}
string                   CIterator::Owner                         ( void ) const
{
  return iter[iV]->owner;
}




#ifndef __PROGTEST__
static void test0 ( void )
{
  CLandRegister x;
  string owner;

  assert ( x . Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . Add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . Add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( x . Add ( "Plzen", "Evropska", "Plzen mesto", 78901 ) );
  assert ( x . Add ( "Liberec", "Evropska", "Librec", 4552 ) );
  CIterator i0 = x . ListByAddr ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Liberec"
           && i0 . Addr () == "Evropska"
           && i0 . Region () == "Librec"
           && i0 . ID () == 4552
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Plzen"
           && i0 . Addr () == "Evropska"
           && i0 . Region () == "Plzen mesto"
           && i0 . ID () == 78901
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Evropska"
           && i0 . Region () == "Vokovice"
           && i0 . ID () == 12345
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Technicka"
           && i0 . Region () == "Dejvice"
           && i0 . ID () == 9873
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Thakurova"
           && i0 . Region () == "Dejvice"
           && i0 . ID () == 12345
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( i0 . AtEnd () );

  assert ( x . Count ( "" ) == 5 );
  CIterator i1 = x . ListByOwner ( "" );
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Prague"
           && i1 . Addr () == "Thakurova"
           && i1 . Region () == "Dejvice"
           && i1 . ID () == 12345
           && i1 . Owner () == "" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Prague"
           && i1 . Addr () == "Evropska"
           && i1 . Region () == "Vokovice"
           && i1 . ID () == 12345
           && i1 . Owner () == "" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Prague"
           && i1 . Addr () == "Technicka"
           && i1 . Region () == "Dejvice"
           && i1 . ID () == 9873
           && i1 . Owner () == "" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Plzen"
           && i1 . Addr () == "Evropska"
           && i1 . Region () == "Plzen mesto"
           && i1 . ID () == 78901
           && i1 . Owner () == "" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Liberec"
           && i1 . Addr () == "Evropska"
           && i1 . Region () == "Librec"
           && i1 . ID () == 4552
           && i1 . Owner () == "" );
  i1 . Next ();
  assert ( i1 . AtEnd () );

  assert ( x . Count ( "CVUT" ) == 0 );
  CIterator i2 = x . ListByOwner ( "CVUT" );
  assert ( i2 . AtEnd () );

  assert ( x . NewOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( x . NewOwner ( "Dejvice", 9873, "CVUT" ) );
  assert ( x . NewOwner ( "Plzen", "Evropska", "Anton Hrabis" ) );
  assert ( x . NewOwner ( "Librec", 4552, "Cvut" ) );
  assert ( x . GetOwner ( "Prague", "Thakurova", owner ) && owner == "CVUT" );
  assert ( x . GetOwner ( "Dejvice", 12345, owner ) && owner == "CVUT" );
  assert ( x . GetOwner ( "Prague", "Evropska", owner ) && owner == "" );
  assert ( x . GetOwner ( "Vokovice", 12345, owner ) && owner == "" );
  assert ( x . GetOwner ( "Prague", "Technicka", owner ) && owner == "CVUT" );
  assert ( x . GetOwner ( "Dejvice", 9873, owner ) && owner == "CVUT" );
  assert ( x . GetOwner ( "Plzen", "Evropska", owner ) && owner == "Anton Hrabis" );
  assert ( x . GetOwner ( "Plzen mesto", 78901, owner ) && owner == "Anton Hrabis" );
  assert ( x . GetOwner ( "Liberec", "Evropska", owner ) && owner == "Cvut" );
  assert ( x . GetOwner ( "Librec", 4552, owner ) && owner == "Cvut" );
  CIterator i3 = x . ListByAddr ();
  assert ( ! i3 . AtEnd ()
           && i3 . City () == "Liberec"
           && i3 . Addr () == "Evropska"
           && i3 . Region () == "Librec"
           && i3 . ID () == 4552
           && i3 . Owner () == "Cvut" );
  i3 . Next ();
  assert ( ! i3 . AtEnd ()
           && i3 . City () == "Plzen"
           && i3 . Addr () == "Evropska"
           && i3 . Region () == "Plzen mesto"
           && i3 . ID () == 78901
           && i3 . Owner () == "Anton Hrabis" );
  i3 . Next ();
  assert ( ! i3 . AtEnd ()
           && i3 . City () == "Prague"
           && i3 . Addr () == "Evropska"
           && i3 . Region () == "Vokovice"
           && i3 . ID () == 12345
           && i3 . Owner () == "" );
  i3 . Next ();
  assert ( ! i3 . AtEnd ()
           && i3 . City () == "Prague"
           && i3 . Addr () == "Technicka"
           && i3 . Region () == "Dejvice"
           && i3 . ID () == 9873
           && i3 . Owner () == "CVUT" );
  i3 . Next ();
  assert ( ! i3 . AtEnd ()
           && i3 . City () == "Prague"
           && i3 . Addr () == "Thakurova"
           && i3 . Region () == "Dejvice"
           && i3 . ID () == 12345
           && i3 . Owner () == "CVUT" );
  i3 . Next ();
  assert ( i3 . AtEnd () );

  assert ( x . Count ( "cvut" ) == 3 );
  CIterator i4 = x . ListByOwner ( "cVuT" );
  assert ( ! i4 . AtEnd ()
           && i4 . City () == "Prague"
           && i4 . Addr () == "Thakurova"
           && i4 . Region () == "Dejvice"
           && i4 . ID () == 12345
           && i4 . Owner () == "CVUT" );
  i4 . Next ();
  assert ( ! i4 . AtEnd ()
           && i4 . City () == "Prague"
           && i4 . Addr () == "Technicka"
           && i4 . Region () == "Dejvice"
           && i4 . ID () == 9873
           && i4 . Owner () == "CVUT" );
  i4 . Next ();
  assert ( ! i4 . AtEnd ()
           && i4 . City () == "Liberec"
           && i4 . Addr () == "Evropska"
           && i4 . Region () == "Librec"
           && i4 . ID () == 4552
           && i4 . Owner () == "Cvut" );
  i4 . Next ();
  assert ( i4 . AtEnd () );

  assert ( x . NewOwner ( "Plzen mesto", 78901, "CVut" ) );
  assert ( x . Count ( "CVUT" ) == 4 );
  CIterator i5 = x . ListByOwner ( "CVUT" );
  assert ( ! i5 . AtEnd ()
           && i5 . City () == "Prague"
           && i5 . Addr () == "Thakurova"
           && i5 . Region () == "Dejvice"
           && i5 . ID () == 12345
           && i5 . Owner () == "CVUT" );
  i5 . Next ();
  assert ( ! i5 . AtEnd ()
           && i5 . City () == "Prague"
           && i5 . Addr () == "Technicka"
           && i5 . Region () == "Dejvice"
           && i5 . ID () == 9873
           && i5 . Owner () == "CVUT" );
  i5 . Next ();
  assert ( ! i5 . AtEnd ()
           && i5 . City () == "Liberec"
           && i5 . Addr () == "Evropska"
           && i5 . Region () == "Librec"
           && i5 . ID () == 4552
           && i5 . Owner () == "Cvut" );
  i5 . Next ();
  assert ( ! i5 . AtEnd ()
           && i5 . City () == "Plzen"
           && i5 . Addr () == "Evropska"
           && i5 . Region () == "Plzen mesto"
           && i5 . ID () == 78901
           && i5 . Owner () == "CVut" );
  i5 . Next ();
  assert ( i5 . AtEnd () );

  assert ( x . Del ( "Liberec", "Evropska" ) );
  assert ( x . Del ( "Plzen mesto", 78901 ) );
  assert ( x . Count ( "cvut" ) == 2 );
  CIterator i6 = x . ListByOwner ( "cVuT" );
  assert ( ! i6 . AtEnd ()
           && i6 . City () == "Prague"
           && i6 . Addr () == "Thakurova"
           && i6 . Region () == "Dejvice"
           && i6 . ID () == 12345
           && i6 . Owner () == "CVUT" );
  i6 . Next ();
  assert ( ! i6 . AtEnd ()
           && i6 . City () == "Prague"
           && i6 . Addr () == "Technicka"
           && i6 . Region () == "Dejvice"
           && i6 . ID () == 9873
           && i6 . Owner () == "CVUT" );
  i6 . Next ();
  assert ( i6 . AtEnd () );

  assert ( x . Add ( "Liberec", "Evropska", "Librec", 4552 ) );
}

static void test1 ( void )
{
  CLandRegister x;
  string owner;

  assert ( x . Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
  assert ( x . Add ( "Prague", "Evropska", "Vokovice", 12345 ) );
  assert ( x . Add ( "Prague", "Technicka", "Dejvice", 9873 ) );
  assert ( ! x . Add ( "Prague", "Technicka", "Hradcany", 7344 ) );
  assert ( ! x . Add ( "Brno", "Bozetechova", "Dejvice", 9873 ) );
  assert ( !x . GetOwner ( "Prague", "THAKUROVA", owner ) );
  assert ( !x . GetOwner ( "Hradcany", 7343, owner ) );
  CIterator i0 = x . ListByAddr ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Evropska"
           && i0 . Region () == "Vokovice"
           && i0 . ID () == 12345
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Technicka"
           && i0 . Region () == "Dejvice"
           && i0 . ID () == 9873
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Prague"
           && i0 . Addr () == "Thakurova"
           && i0 . Region () == "Dejvice"
           && i0 . ID () == 12345
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( i0 . AtEnd () );

  assert ( x . NewOwner ( "Prague", "Thakurova", "CVUT" ) );
  assert ( ! x . NewOwner ( "Prague", "technicka", "CVUT" ) );
  assert ( ! x . NewOwner ( "prague", "Technicka", "CVUT" ) );
  assert ( ! x . NewOwner ( "dejvice", 9873, "CVUT" ) );
  assert ( ! x . NewOwner ( "Dejvice", 9973, "CVUT" ) );
  assert ( ! x . NewOwner ( "Dejvice", 12345, "CVUT" ) );
  assert ( x . Count ( "CVUT" ) == 1 );
  CIterator i1 = x . ListByOwner ( "CVUT" );
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Prague"
           && i1 . Addr () == "Thakurova"
           && i1 . Region () == "Dejvice"
           && i1 . ID () == 12345
           && i1 . Owner () == "CVUT" );
  i1 . Next ();
  assert ( i1 . AtEnd () );

  assert ( ! x . Del ( "Brno", "Technicka" ) );
  assert ( ! x . Del ( "Karlin", 9873 ) );
  assert ( x . Del ( "Prague", "Technicka" ) );
  assert ( ! x . Del ( "Prague", "Technicka" ) );
  assert ( ! x . Del ( "Dejvice", 9873 ) );
}


/* wrong compare of strings
static void test2 ( void )
{
  CLandRegister x;
  string owner;

  assert ( x. Add ("Sokolov", "Jiriho z Podebrad", "Sokolov", 35601) );
  assert ( ! x. Add ("Sokolov", "Jiriho z Podebrad", "Sokolov", 35601) );
  assert ( x. Add ("Sokolov", "Jiri z Podebrd", "Sokolovv", 35601) );
  assert ( x. Add ("Sokol", "Jiriho z Podebrad", "Sok", 35601) );
  assert ( x. Add ("sokol", "Jiriho z Podebrad", "sok", 35601) );
  assert ( x. Add ("sokolov", "jiri z Podebrd", "sokolov", 35601) );

  CIterator i0 = x . ListByAddr ();

  assert ( ! i0 . AtEnd ()
           && i0 . City () == "sokol"
           && i0 . Addr () == "Jiriho z Podebrad"
           && i0 . Region () == "sok"
           && i0 . ID () == 35601
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "Sokol"
           && i0 . Addr () == "Jiriho z Podebrad"
           && i0 . Region () == "Sok"
           && i0 . ID () == 35601
           && i0 . Owner () == "" );
  i0 . Next ();
  assert ( ! i0 . AtEnd ()
           && i0 . City () == "sokolov"
           && i0 . Addr () == "jiri z Podebrd"
           && i0 . Region () == "sokolov"
           && i0 . ID () == 35601
           && i0 . Owner () == "" );
  i0.Next();
  assert ( ! i0 . AtEnd ()
          && i0 . City () == "Sokolov"
          && i0 . Addr () == "Jiri z Podebrd"
          && i0 . Region () == "Sokolovv"
          && i0 . ID () == 35601
          && i0 . Owner () == "" );
  i0.Next();
  assert ( ! i0 . AtEnd ()
         && i0 . City () == "Sokolov"
         && i0 . Addr () == "Jiriho z Podebrad"
         && i0 . Region () == "Sokolov"
         && i0 . ID () == 35601
         && i0 . Owner () == "" );
  i0 . Next ();
  assert ( i0 . AtEnd () );


  assert ( x. NewOwner ("Sokolov", "Jiriho z Podebrad", "El") );
  assert ( ! x. NewOwner ("Sokolov", 35601, "El") );
  assert ( x. NewOwner ("Sokolovv", 35601, "Eel") );

  assert ( x. NewOwner ("Sokol", "Jiriho z Podebrad", "Eleven") );
  assert ( x. NewOwner ("sokol", "Jiriho z Podebrad", "eL") );
  assert ( x. NewOwner ("sokolov", 35601, "el") );

  assert ( x.Count("El") == 3 );

  CIterator i1 = x . ListByOwner ("EL");
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "Sokolov"
           && i1 . Addr () == "Jiriho z Podebrad"
           && i1 . Region () == "Sokolov"
           && i1 . ID () == 35601
           && i1 . Owner () == "El" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "sokol"
           && i1 . Addr () == "Jiriho z Podebrad"
           && i1 . Region () == "sok"
           && i1 . ID () == 35601
           && i1 . Owner () == "eL" );
  i1 . Next ();
  assert ( ! i1 . AtEnd ()
           && i1 . City () == "sokolov"
           && i1 . Addr () == "jiri z Podebrd"
           && i1 . Region () == "sokolov"
           && i1 . ID () == 35601
           && i1 . Owner () == "el" );
  i1 . Next ();
  assert ( i1 . AtEnd () );
}

static void test3 ( void )
{
  CLandRegister x;

  for (size_t i = 0; i < 1000; i++)
  {
    assert (   x .   Add ( "Prague", "Thakurova", "Dejvice", 12345 ) );
    assert (   x .   Del ( "Prague", "Thakurova" ) );
    assert ( ! x .   Del ( "Dejvice", 12345 ) );
  }

  assert ( x.Count("") == 0 );

  assert ( x . Add ("Brno", "Kdo Vi", "Tady", 12468) );

  assert (   x . NewOwner ("Brno", "Kdo Vi", "Ofrie") );
  assert ( ! x . NewOwner ("Tady", 12468, "Ofrie") );

  assert ( ! x . NewOwner ("Brno", "Kdo Vi", "OfRiE") );
  assert ( ! x . NewOwner ("Tady", 12468, "oFrIe") );

  for (size_t i = 0; i < 1000; i ++)
  {
    assert (   x . NewOwner ("Brno", "Kdo Vi", "olaf") );
    assert (   x . NewOwner ("Tady", 12468, "falo") );
  }

  assert ( x.Count("falo") == 1 );

}

static void test4( void )
{
  CLandRegister x;
  string owner;
  assert (   x .   Add ( "0", "0", "", 0 ) );
  assert ( !x.NewOwner("       ", "               ", "Dave") );
  assert ( !x.NewOwner("", "", "") );
  assert ( x . GetOwner("0", "0", owner) );
  assert ( owner == "" );

  CIterator i1 = x . ListByOwner ("");
  assert ( !i1 . AtEnd ());

}

static void test5 ( void )
{
  CLandRegister x;
  string owner;
  x.Add("adfkjaflkajflajflkadjflsadlkfjadslkfj jdlkfjlads jfldsaj lfjsdalfkj43543 5435 34", "afjkdoaiuf9034u4ijdsflk4092k" , "ljdfaoasfjidf9u390u 9ujfd", 2394329);
  x.Add(" jdlkfjlads jfldsaj lfjsdalfkj43543 5435 34", "" , " 9ujfd", 10000000);
  x.Add("io", "oi", "io", 1234);
  x.NewOwner("iio", "io", "dave");
}

static void test6 ( void )
{
  CLandRegister x;
  string owner;
  assert ( x . Add("S okolo i", "Jirky1", "SSokollov", 12345) );
  assert ( x . Add("s okolo   ", "Jirky", "Sokolossv", 123456) );
  assert ( x . Add("SOkolov ", "Jirky1", "Sossskolov", 12345) );
  assert ( x . Add("Sokolov", "Jirky", "Sokosslov", 123456) );
  assert ( x . Add("SokoloV ", "Jirky1", "Sosskolov", 12345) );
  assert ( x . Add("SoKoLoV", "Jirky", "Sssokolov", 123456) );
  CIterator i0 = x . ListByAddr();

  assert ( ! i0 . AtEnd() &&
          i0.City() == "s okolo   " &&
          i0.Addr() == "Jirky"      &&
          i0.Region() == "Sokolossv"&&
          i0.ID() == 123456 );
  i0.Next();
  assert ( ! i0 . AtEnd() &&
          i0.City() == "S okolo i" &&
          i0.Addr() == "Jirky1"      &&
          i0.Region() == "SSokollov"&&
          i0.ID() == 12345 );
  i0.Next();
  assert ( ! i0 . AtEnd() &&
          i0.City() == "Sokolov" &&
          i0.Addr() == "Jirky"      &&
          i0.Region() == "Sokosslov"&&
          i0.ID() == 123456 );
  i0.Next();
  assert ( ! i0 . AtEnd() &&
          i0.City() == "SokoloV " &&
          i0.Addr() == "Jirky1"      &&
          i0.Region() == "Sosskolov"&&
          i0.ID() == 12345 );
  i0.Next();
  assert ( ! i0 . AtEnd() &&
          i0.City() == "SoKoLoV" &&
          i0.Addr() == "Jirky"      &&
          i0.Region() == "Sssokolov"&&
          i0.ID() == 123456 );
  i0.Next();
  assert ( ! i0 . AtEnd() &&
          i0.City() == "SOkolov " &&
          i0.Addr() == "Jirky1"      &&
          i0.Region() == "Sossskolov"&&
          i0.ID() == 12345 );
  i0.Next();
  assert ( i0.AtEnd() );
}
*/


static void test4( void )
{
  CLandRegister x;
  string owner;
  assert (   x .   Add ( "Town", "this", "now", 0 ) );
  assert (   x .   Add ( "town2", "this2", "now1", 6 ) );
  assert ( x.NewOwner("Town", "this", "Wovdua=") );
  assert ( x.NewOwner("now1", 6, "Wovdua=") );

  assert ( x.Count("Wovdua=") == 2);

}

static void test6(void)
{
  CLandRegister x;
  string owner, town, region;
  town = "thisTown";
  region = "thisRegion";
  for (size_t i = 0; i < 10; i++)
  {
    assert (   x .   Add ( town, "this", region, 0 ) );

    if ( i % 2 )
    {
      assert ( x.NewOwner(region, 0, "WoVdua=") );
    }
    else
    {
      assert ( x . NewOwner (town, "this", "Wovdua ") );
    }

    town += "=";
    region += "=";
  }

  assert ( x.Count("WoVdua=") == 5 );
  assert ( x.Count("Wovdua ") == 5 );
}


int main ( void )
{
  test0();
  test1();
  //test2();
  //test3();
  test4();
  //test5();
  test6();
  return 0;
}
#endif /* __PROGTEST__ */
