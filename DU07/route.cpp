#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>

#if defined ( __cplusplus ) && __cplusplus > 199711L /* C++ 11 */
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#endif /* C++ 11 */

using namespace std;
#endif /* __PROGTEST__ */

class NoRouteException
{
};



template <typename _T, typename _E>
class CRoute
{
  public:
    CRoute                    () = default;
    ~CRoute                   ();
    CRoute &                  Add( const _T & fromT, const _T & toT, const _E & paramT );
    template <typename _F>
    list<_T>                  Find( const _T & fromT, const _T & toT, _F func);
    list<_T>                  Find( const _T & fromT, const _T & toT );

    class CNode
    {
      public:
        CNode( _T label );
        ~CNode();
        void setNeighbour     ( CNode * neighbour, const _E & param );
        _T                    label;
        list<_T>              path;
        std::vector<std::pair<CNode*, _E*>> neighbourhood;
        };

      private:
        std::map<_T, CNode*>   treeMap;
};

template <typename _T, typename _E>
CRoute<_T, _E>::CNode::CNode(_T label) :label(label) {}

template <typename _T, typename _E>
CRoute<_T, _E>::CNode::~CNode(){
  for ( auto iter : neighbourhood)
    delete iter.second;
}

template <typename _T, typename _E>
CRoute<_T,_E>::~CRoute(){
  for ( auto iter : treeMap )
    delete iter.second;
}

template <typename _T, typename _E>
void CRoute<_T, _E>::CNode::setNeighbour( CNode * neighbour, const _E & param ){
    neighbourhood.emplace_back(std::pair<CNode*, _E*>(neighbour, new _E(param)));
}

template < typename _T, typename _E >
CRoute<_T,_E > & CRoute<_T,_E >::Add( const _T & fromT, const _T & toT, const _E & paramT ){
  CRoute<_T,_E>::CNode * fromNode, * toNode;

  fromNode  = treeMap[fromT];

  if ( ! (fromNode) )
    fromNode  =     new CRoute<_T,_E>::CNode(fromT);

  toNode    = treeMap[toT];

  if ( ! (toNode) )
    toNode    =     new CRoute<_T,_E>::CNode(toT);

  fromNode  ->setNeighbour    ( toNode,   paramT );
  toNode    ->setNeighbour    ( fromNode, paramT );

  treeMap[fromT]  = fromNode;
  treeMap[toT]    = toNode;
  return *this;
}


template <typename _T, typename _E>
template <typename _F>
list<_T> CRoute<_T, _E>::Find( const _T & fromT, const _T & toT, _F func ){

  map <CNode*, bool> statusNode;
  std::queue<CNode*> queueNode;
  CNode * nodeTmp = treeMap[fromT];
  CNode * tmp;

  if ( ! nodeTmp ){
    if ( fromT == toT){
      list <_T> outList;
      outList.emplace_back(fromT);
      return outList;
    }
    else
      throw NoRouteException();
  }

  nodeTmp->path.clear();
  statusNode[nodeTmp] = 1;
  queueNode.push(nodeTmp);

  while ( true ){
    if ( queueNode.empty() )
      break;
    nodeTmp = queueNode.front();
    queueNode.pop();

    nodeTmp->path.emplace_back(nodeTmp->label);

    if ( nodeTmp->label == toT )
      break;

    for ( auto iter : nodeTmp->neighbourhood ){
      tmp = iter.first;
      if ( ! statusNode[tmp] and func(*iter.second) ){
        tmp->path.clear();

        for ( auto cont : nodeTmp->path )
          tmp->path.emplace_back(cont);

        statusNode[tmp] = 1;
        queueNode.push(tmp);
      }
    }
  }

  if (nodeTmp->label == toT)
    return nodeTmp->path;
  else
    throw NoRouteException();
}

template <typename _T, typename _E>
list<_T> CRoute<_T, _E>::Find( const _T & fromT, const _T & toT ){

  map <CNode*, bool> statusNode;
  std::queue<CNode*> queueNode;
  CNode * nodeTmp = treeMap[fromT];
  CNode * tmp;

  if ( ! nodeTmp ){
    if ( fromT == toT){
      list <_T> outList;
      outList.emplace_back(fromT);
      return outList;
    }
    else
      throw NoRouteException();
  }

  nodeTmp->path.clear();
  statusNode[nodeTmp] = 1;
  queueNode.push(nodeTmp);

  while ( true ){
    if ( queueNode.empty() )
      break;
    nodeTmp = queueNode.front();
    queueNode.pop();

    nodeTmp->path.emplace_back(nodeTmp->label);

    if ( nodeTmp->label == toT )
      break;

    for ( auto iter : nodeTmp->neighbourhood ){
      tmp = iter.first;
      if ( ! statusNode[tmp] ){
        tmp->path.clear();

        for ( auto cont : nodeTmp->path )
          tmp->path.emplace_back(cont);

        statusNode[tmp] = 1;
        queueNode.push(tmp);
      }
    }
  }
  if (nodeTmp->label == toT)
    return nodeTmp->path;
  else
    throw NoRouteException();
}


#ifndef __PROGTEST__
//=================================================================================================
class CTrain
{
  public:
                             CTrain                        ( const string    & company,
                                                             int               speed )
                             : m_Company ( company ),
                               m_Speed ( speed )
    {
    }
    //---------------------------------------------------------------------------------------------
    string                   m_Company;
    int                      m_Speed;
};
//=================================================================================================
class TrainFilterCompany
{
  public:
                             TrainFilterCompany            ( const set<string> & companies )
                             : m_Companies ( companies )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain & train ) const
    {
      return m_Companies . find ( train . m_Company ) != m_Companies . end ();
    }
    //---------------------------------------------------------------------------------------------
  private:
    set <string>             m_Companies;
};
//=================================================================================================
class TrainFilterSpeed
{
  public:
                             TrainFilterSpeed              ( int               min,
                                                             int               max )
                             : m_Min ( min ),
                               m_Max ( max )
    {
    }
    //---------------------------------------------------------------------------------------------
    bool                     operator ()                   ( const CTrain    & train ) const
    {
      return train . m_Speed >= m_Min && train . m_Speed <= m_Max;
    }
    //---------------------------------------------------------------------------------------------
  private:
    int                      m_Min;
    int                      m_Max;
};
//=================================================================================================
bool               NurSchnellzug                           ( const CTrain    & zug )
{
  return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
}
//=================================================================================================
static string      toText                                  ( const list<string> & l )
{
  ostringstream oss;

  auto it = l . cbegin();
  oss << *it;
  for ( ++it; it != l . cend (); ++it )
    oss << " > " << *it;
  return oss . str ();
}
//=================================================================================================

int main ( void )
{
  CRoute<string,CTrain> lines;
  lines . Add ( "Berlin", "Prague", CTrain ( "DB", 120 ) )
        . Add ( "Berlin", "Prague", CTrain ( "CD",  80 ) )
        . Add ( "Berlin", "Dresden", CTrain ( "DB", 160 ) )
        . Add ( "Dresden", "Munchen", CTrain ( "DB", 160 ) )
        . Add ( "Munchen", "Prague", CTrain ( "CD",  90 ) )
        . Add ( "Munchen", "Linz", CTrain ( "DB", 200 ) )
        . Add ( "Munchen", "Linz", CTrain ( "OBB", 90 ) )
        . Add ( "Linz", "Prague", CTrain ( "CD", 50 ) )
        . Add ( "Prague", "Wien", CTrain ( "CD", 100 ) )
        . Add ( "Linz", "Wien", CTrain ( "OBB", 160 ) )
        . Add ( "Paris", "Marseille", CTrain ( "SNCF", 300 ))
        . Add ( "Paris", "Dresden",  CTrain ( "SNCF", 250 ) );

  list<string> r1 = lines . Find ( "Berlin", "Linz" );
  assert ( toText ( r1 ) == "Berlin > Prague > Linz" );

  list<string> r2 = lines . Find ( "Linz", "Berlin" );
  assert ( toText ( r2 ) == "Linz > Prague > Berlin" );

  list<string> r3 = lines . Find ( "Wien", "Berlin" );
  assert ( toText ( r3 ) == "Wien > Prague > Berlin" );

  list<string> r4 = lines . Find ( "Wien", "Berlin", NurSchnellzug );
  assert ( toText ( r4 ) == "Wien > Linz > Munchen > Dresden > Berlin" );

  list<string> r5 = lines . Find ( "Wien", "Munchen", TrainFilterCompany ( set<string> { "CD", "DB" } ) );
  assert ( toText ( r5 ) == "Wien > Prague > Munchen" );

  list<string> r6 = lines . Find ( "Wien", "Munchen", TrainFilterSpeed ( 120, 200 ) );
  assert ( toText ( r6 ) == "Wien > Linz > Munchen" );


  list<string> r7 = lines . Find ( "Wien", "Munchen", [] ( const CTrain & x ) { return x . m_Company == "CD"; } );
  assert ( toText ( r7 ) == "Wien > Prague > Munchen" );


  list<string> r8 = lines . Find ( "Munchen", "Munchen" );
  assert ( toText ( r8 ) == "Munchen" );

  list<string> r9 = lines . Find ( "Marseille", "Prague" );
  assert ( toText ( r9 ) == "Marseille > Paris > Dresden > Berlin > Prague"
           || toText ( r9 ) == "Marseille > Paris > Dresden > Munchen > Prague" );

  try
  {
    list<string> r10 = lines . Find ( "Marseille", "Prague", NurSchnellzug );
    assert ( "Marseille > Prague connection does not exist!!" == NULL );
  }
  catch ( const NoRouteException & e )
  {
  }

  list<string> r11 = lines . Find ( "Salzburg", "Salzburg" );
  assert ( toText ( r11 ) == "Salzburg" );

  list<string> r12 = lines . Find ( "Salzburg", "Salzburg", [] ( const CTrain & x ) { return x . m_Company == "SNCF"; }  );
  assert ( toText ( r12 ) == "Salzburg" );

  try
  {
    list<string> r13 = lines . Find ( "London", "Oxford" );
    assert ( "London > Oxford connection does not exist!!" == NULL );
  }
  catch ( const NoRouteException & e )
  {
  }
  return 0;
}
#endif  /* __PROGTEST__ */
