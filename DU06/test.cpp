#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */

// -- component --
class CComponent{
  public:
    CComponent          () {}
    virtual ~CComponent () {}
    virtual void        print ( ostream & os, string branch ) const = 0;
    virtual             CComponent * clone() const = 0;
};

// -- leafs --
class CCPU : public CComponent{
  public:
    CCPU                ( int   coreNum, int clockNum );
    CCPU                ( const CCPU & cpu );
    ~CCPU               ();
    CCPU &              operator =   ( const CCPU & cpu );
    void                print( ostream & os, string branch ) const;
    CCPU *              clone() const;
  protected:
    int                 coreNum;
    int                 clockNum;
};

class CMemory : public CComponent{
  public:
    CMemory             ( int   sizeMem );
    CMemory             ( const CMemory & mine );
    ~CMemory            ();
    CMemory &           operator = ( const CMemory & cmem );
    void                print( ostream & os, string branch ) const;
    CMemory *           clone() const;
  protected:
    int                 sizeMem;
};

class CDisk : public CComponent{
  public:
    enum diskType       { MAGNETIC = 0, SSD = 1 };
    CDisk               ( diskType typeDisk, int diskSize );
    CDisk               ( const CDisk & diskC );
    ~CDisk              ();
    CDisk &             operator = ( const CDisk & cdi );
    void                print ( ostream & os, string branch ) const;
    CDisk *             clone () const ;
    CDisk &             AddPartition( int size, const string & id );
  protected:
    class partition{
      public:
        partition       ( const string & id, int size );
        partition       ( const partition & part );
        void            print ();
        string          id;
        int             size;
    };
    diskType            typeDisk;
    int                 diskSize;
    int                 sizeAvailable;
    vector              <partition*> part;
};

// -- composite --
class CComputer : public CComponent {
  public:
    CComputer                      ( const string & compName );
    CComputer                      ( const CComputer & comp );
    ~CComputer                     ();
    void              addCom       ( CComponent * compon );
    string            getName      () const;
    CComputer &       AddComponent ( const CCPU & cpu );
    CComputer &       AddComponent ( const CMemory & cme );
    CComputer &       AddComponent ( const CDisk & cdi );
    CComputer &       AddAddress   ( const string & address );
    CComputer &       operator =   ( const CComputer & comp );
    void              print        ( ostream & os, string branch ) const;
    friend ostream &  operator <<  ( ostream & os, const CComputer & c );
    CComputer *       clone() const;
  private:
    string                compName;
    vector<CComponent *>  compIn;
    vector<string>        addressLog;
};

// -- client --
class CNetwork{
  public:
    CNetwork          ( const string & netName );
    CNetwork          ( const CNetwork & net );
    ~CNetwork         ();
    void              print()  {}
    friend ostream &  operator << ( ostream & os, const CNetwork & n );
    CNetwork &        operator = ( CNetwork & net );
    CNetwork  &       AddComputer   ( const string & compName );
    CNetwork  &       AddComputer   ( const CComputer & comp );
    CComputer *       FindComputer  ( const string & compName );
  private:
    map <string, CComputer*>     findComp;
    vector<CComputer*>           baseCom;
    string                       netName;
};
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CComputer::CComputer( const string & compName ) : compName( compName )
{
}

CComputer::CComputer( const CComputer & comp )
{
  compName = comp.compName;
  for ( auto iter : comp.addressLog )
    addressLog.emplace_back( iter );
  for ( auto iter : comp.compIn ){
    compIn.emplace_back( iter->clone() );
  }
}

CComputer::~CComputer()
{
  for ( auto iter : compIn )
    delete iter;
}

CComputer * CComputer::clone() const
{
  return new CComputer(*this);
}

void CComputer::addCom ( CComponent * compon )
{
  compIn.emplace_back( compon );
}

CComputer & CComputer::AddComponent( const CCPU & cpu )
{
  CComponent * a = new CCPU(cpu);
  addCom(a);
  return *this;
}

CComputer & CComputer::CComputer::AddComponent( const CMemory & cme )
{
  CComponent * a = new CMemory(cme);
  addCom(a);
  return *this;
}

CComputer & CComputer::AddComponent( const CDisk & cdi )
{
  CComponent * a = new CDisk(cdi);
  addCom(a);
  return *this;
}

string      CComputer::getName() const
{
  return compName;
}

CComputer &  CComputer::AddAddress( const string & address )
{
  addressLog.emplace_back( address );
  return *this;
}

ostream & operator << ( ostream & os, const CComputer & c )
{
  c.print( os, "" );
  return os;
}

CComputer & CComputer::operator = ( const CComputer & comp )
{
  if ( this == &comp )
    return *this;

  compName = comp.compName;

  for ( auto iter : compIn )
    delete iter;

  compIn.clear();

  addressLog.clear();

  for ( auto iter : comp.addressLog )
    addressLog.emplace_back( iter );
  for ( auto iter : comp.compIn )
    compIn.emplace_back( iter->clone() );
  return *this;
}

void CComputer::print( ostream & os, string branch ) const
{
  // comp name
  os << "Host: " << compName << endl;

  // address
  size_t i = 0;
  if ( addressLog.size() >= 1 )
  {
    for ( i = 0; i < addressLog.size() - 1; ++i )
    {
      os << branch << "+-" << addressLog[i] << endl;
    }
    if ( compIn.size() == 0 )
      os << branch << "\\" << addressLog[i] << endl;
    else
      os << branch << "+-" << addressLog[i] << endl;
  }
  // end of address formating

  //component
  if ( compIn.size() >= 1 )
  {
    for ( i = 0; i < compIn.size()-1; ++i )
    {
      os << branch << "+-";
      compIn[i]->print(os, branch + "| ");
    }
    os << branch << "\\-";
    compIn[i]->print(os, branch + "  ");
  }

}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CNetwork::CNetwork ( const string & netName ) : netName(netName)
{
}

CNetwork::CNetwork ( const CNetwork & net )
{
  netName = net.netName;
  CComputer * tmp;
  for ( auto iter : net.baseCom )
  {
    tmp = iter->clone();
    findComp[iter->getName()] = tmp;
    baseCom.emplace_back( tmp );
  }
}

CNetwork::~CNetwork ()
{
  for ( auto iter : baseCom )
    delete iter;
}

CNetwork & CNetwork::AddComputer  ( const string & compName )
{
  if ( ! findComp[compName] )
  {
    baseCom.emplace_back( new CComputer( compName ) );
  }
  return *this;
}

CNetwork & CNetwork::AddComputer  ( const CComputer & comp )
{
  if ( ! findComp[comp.getName()] )
  {
    CComputer * tmp = comp.clone();
    findComp[tmp->getName()] = tmp;
    baseCom.emplace_back( tmp );
  }
  return *this;
}

CComputer * CNetwork::FindComputer  ( const string & compName )
{
  return findComp[compName];
}

CNetwork & CNetwork::operator = ( CNetwork & net )
{
  if ( this == &net )
    return *this;

  netName = net.netName;

  for ( auto iter : baseCom )
    delete iter;
  baseCom.clear();

  findComp.clear();

  CComputer * tmp;
  for ( auto iter : net.baseCom )
  {
    tmp = iter->clone();
    findComp[iter->getName()] = tmp;
    baseCom.emplace_back( tmp );
  }
  return *this;
}

ostream & operator << ( ostream & os, const CNetwork & n )
{
  os << "Network: " << n.netName << endl;

  if (n.baseCom.size() >= 1)
  {
    size_t i = 0;
    for ( i = 0; i < n.baseCom.size()-1; i++ ){
      os << "+-";
      n.baseCom[i]->print(os, "| ");
    }
    os << "\\-";
    n.baseCom[i]->print(os, "  ");
  }

  return os;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CCPU::CCPU    ( int   coreNum, int clockNum ) : coreNum(coreNum),
                                                clockNum(clockNum)
{
}

CCPU::CCPU    ( const CCPU & cpu )
{
  coreNum = cpu.coreNum;
  clockNum = cpu.clockNum;
}

CCPU::~CCPU()
{
}

CCPU * CCPU::clone() const
{
  return new CCPU(*this);
}

CCPU & CCPU::operator = ( const CCPU & cpu )
{
  coreNum = cpu.coreNum;
  clockNum = cpu.clockNum;
  return *this;
}

void CCPU::print( ostream & os, string branch ) const
{
  os << "CPU, " << coreNum << " cores" << " @ " << clockNum << "MHz" << endl;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CMemory::CMemory( int sizeMem ) : sizeMem(sizeMem)
{
}

CMemory::CMemory( const CMemory & mine ) : sizeMem(mine.sizeMem)
{
}

CMemory::~CMemory()
{
}

CMemory * CMemory::clone() const
{
  return new CMemory(*this);
}

CMemory & CMemory::operator = ( const CMemory & cmem )
{
    sizeMem = cmem.sizeMem;
    return *this;
}

void CMemory::print( ostream & os, string branch ) const
{
  os << "Memory, " << sizeMem << " MiB" << endl;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


CDisk::CDisk( diskType typeDisk, int diskSize ) : typeDisk(typeDisk),
                                                  diskSize(diskSize),
                                                  sizeAvailable(diskSize)
{
}

CDisk::CDisk( const CDisk & diskC ) : typeDisk(diskC.typeDisk),
                                      diskSize(diskC.diskSize),
                                      sizeAvailable(diskC.sizeAvailable)
{
  for ( auto iter : diskC.part )
    part.emplace_back( new partition( *iter ) );
}

CDisk::~CDisk()
{
  for ( auto iter : part )
    delete iter;
}

CDisk * CDisk::clone() const
{
  return new CDisk(*this);
}

void CDisk::print( ostream & os, string branch ) const
{
  os << ( (typeDisk)?("SSD"):("HDD") ) << ", " << diskSize << " GiB" << endl;
  if ( part.size() >= 1 ){
    size_t i = 0;
    for ( i = 0; i < part.size()-1; ++i )
      os << branch << "+-" << "[" << i << "]: " << part[i]->size << " GiB, " << part[i]->id << endl;
    os << branch << "\\-" << "[" << i << "]: " << part[i]->size << " GiB, " << part[i]->id << endl;
  }
}

CDisk::partition::partition       ( const string & id, int size ) :id(id), size(size)
{
}

CDisk::partition::partition       ( const partition & part ) :id(part.id), size(part.size)
{
}

CDisk & CDisk::operator = ( const CDisk & cdi )
{
  if ( this == &cdi )
    return *this;

  typeDisk = cdi.typeDisk;
  diskSize = cdi.diskSize;
  sizeAvailable = cdi.sizeAvailable;

  for ( auto iter : part )
    delete iter;
  part.clear();

  for ( size_t i = 0; i < cdi.part.size(); ++i )
    part.emplace_back( new partition(*cdi.part[i]) );
  return *this;
}

CDisk & CDisk::AddPartition( int size, const string & id )
{
  if ( size <= sizeAvailable )
  {
    sizeAvailable -= size;
    part.emplace_back( new partition( id, size ) );
  }
  return *this;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


#ifndef __PROGTEST__
template<typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

void test0() // test for = operator
{
  CNetwork n ( "Home network" );

  CComputer a("Lenovo Z500");

  a .AddAddress("192.168.0.9")
    .AddComponent( CCPU(8, 2400))
    .AddComponent( CCPU(8, 1200))
    .AddComponent( CDisk( CDisk::SSD, 256 )
      .AddPartition(128, "linux/")
      .AddPartition(128, "windows/"))
    .AddComponent( CDisk( CDisk::MAGNETIC, 1024)
      .AddPartition( 1024, "media/"))
    .AddComponent( CMemory(16000));

  CComputer b("Lenovo Z400");

  b .AddAddress("192.168.0.9")
    .AddComponent( CCPU(8, 2400))
    .AddComponent( CCPU(8, 1200))
    .AddComponent( CDisk( CDisk::SSD, 256 )
      .AddPartition(128, "linux/")
      .AddPartition(128, "windows/"))
    .AddComponent( CDisk( CDisk::SSD, 256 )
      .AddPartition(128, "anime/")
      .AddPartition(128, "other/"))
    .AddComponent( CDisk( CDisk::MAGNETIC, 1024)
      .AddPartition( 1024, "documents/"))
    .AddComponent( CMemory(16000));

  n.AddComputer(a).AddComputer(a).AddComputer(b). AddComputer(a) . AddComputer(b);

  CNetwork netos("notoe");

  CComputer c ("pc");

  c = b;
  c = b;

  c = c;

  assert( toString(c) ==
  "Host: Lenovo Z400\n"
  "+-192.168.0.9\n"
  "+-CPU, 8 cores @ 2400MHz\n"
  "+-CPU, 8 cores @ 1200MHz\n"
  "+-SSD, 256 GiB\n"
  "| +-[0]: 128 GiB, linux/\n"
  "| \\-[1]: 128 GiB, windows/\n"
  "+-SSD, 256 GiB\n"
  "| +-[0]: 128 GiB, anime/\n"
  "| \\-[1]: 128 GiB, other/\n"
  "+-HDD, 1024 GiB\n"
  "| \\-[0]: 1024 GiB, documents/\n"
  "\\-Memory, 16000 MiB\n");

  netos = n;
  netos = n;

  netos = netos;

  assert( toString(netos) ==
  "Network: Home network\n"
  "+-Host: Lenovo Z500\n"
  "| +-192.168.0.9\n"
  "| +-CPU, 8 cores @ 2400MHz\n"
  "| +-CPU, 8 cores @ 1200MHz\n"
  "| +-SSD, 256 GiB\n"
  "| | +-[0]: 128 GiB, linux/\n"
  "| | \\-[1]: 128 GiB, windows/\n"
  "| +-HDD, 1024 GiB\n"
  "| | \\-[0]: 1024 GiB, media/\n"
  "| \\-Memory, 16000 MiB\n"
  "\\-Host: Lenovo Z400\n"
  "  +-192.168.0.9\n"
  "  +-CPU, 8 cores @ 2400MHz\n"
  "  +-CPU, 8 cores @ 1200MHz\n"
  "  +-SSD, 256 GiB\n"
  "  | +-[0]: 128 GiB, linux/\n"
  "  | \\-[1]: 128 GiB, windows/\n"
  "  +-SSD, 256 GiB\n"
  "  | +-[0]: 128 GiB, anime/\n"
  "  | \\-[1]: 128 GiB, other/\n"
  "  +-HDD, 1024 GiB\n"
  "  | \\-[0]: 1024 GiB, documents/\n"
  "  \\-Memory, 16000 MiB\n");
}

int main ( void )
{
  test0();
  CNetwork n ( "FIT network" );

  n . AddComputer (
        CComputer ( "progtest.fit.cvut.cz" ) .
          AddAddress ( "147.32.232.142" ) .
          AddComponent ( CCPU ( 8, 2400 ) ) .
          AddComponent ( CCPU ( 8, 1200 ) ) .
          AddComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
            AddPartition ( 50, "/" ) .
            AddPartition ( 5, "/boot" ).
            AddPartition ( 1000, "/var" ) ) .
          AddComponent ( CDisk ( CDisk::SSD, 60 ) .
            AddPartition ( 60, "/data" )  ) .
          AddComponent ( CMemory ( 2000 ) ).
          AddComponent ( CMemory ( 2000 ) ) ) .
      AddComputer (
        CComputer ( "edux.fit.cvut.cz" ) .
          AddAddress ( "147.32.232.158" ) .
          AddComponent ( CCPU ( 4, 1600 ) ) .
          AddComponent ( CMemory ( 4000 ) ).
          AddComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
            AddPartition ( 100, "/" )   .
            AddPartition ( 1900, "/data" ) ) ) .
      AddComputer (
        CComputer ( "imap.fit.cvut.cz" ) .
          AddAddress ( "147.32.232.238" ) .
          AddComponent ( CCPU ( 4, 2500 ) ) .
          AddAddress ( "2001:718:2:2901::238" ) .
          AddComponent ( CMemory ( 8000 ) ) );

  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: edux.fit.cvut.cz\n"
    "| +-147.32.232.158\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  CNetwork x = n;
  auto c = x . FindComputer ( "imap.fit.cvut.cz" );
  assert ( toString ( *c ) ==
    "Host: imap.fit.cvut.cz\n"
    "+-147.32.232.238\n"
    "+-2001:718:2:2901::238\n"
    "+-CPU, 4 cores @ 2500MHz\n"
    "\\-Memory, 8000 MiB\n" );
  c -> AddComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
         AddPartition ( 100, "system" ) .
         AddPartition ( 200, "WWW" ) .
         AddPartition ( 700, "mail" ) );
  assert ( toString ( x ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: edux.fit.cvut.cz\n"
    "| +-147.32.232.158\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  +-Memory, 8000 MiB\n"
    "  \\-HDD, 1000 GiB\n"
    "    +-[0]: 100 GiB, system\n"
    "    +-[1]: 200 GiB, WWW\n"
    "    \\-[2]: 700 GiB, mail\n" );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: edux.fit.cvut.cz\n"
    "| +-147.32.232.158\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  return 0;
}
#endif /* __PROGTEST__ */
