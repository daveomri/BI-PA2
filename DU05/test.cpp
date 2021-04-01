#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
using namespace std;

class CTimeStamp
{
  public:
                   CTimeStamp                              ( int               year,
                                                             int               month,
                                                             int               day,
                                                             int               hour,
                                                             int               minute,
                                                             double            sec );
    int            Compare                                 ( const CTimeStamp & x ) const;
    friend ostream & operator <<                           ( ostream          & os,
                                                             const CTimeStamp & x );
  private:
    int year, month, day, hour, minute;
    double sec;
};
CTimeStamp::CTimeStamp                  ( int               year,
                                          int               month,
                                          int               day,
                                          int               hour,
                                          int               minute,
                                          double            sec ) : year(year), month(month), day(day), hour(hour), minute(minute), sec(sec){}
class CMail
{
  public:
                   CMail                                   ( const CTimeStamp & timeStamp,
                                                             const string     & from,
                                                             const string     & to,
                                                             const string     & subject );
    int            CompareByTime                           ( const CTimeStamp & x ) const;
    int            CompareByTime                           ( const CMail      & x ) const;
    const string & From                                    ( void ) const;
    const string & To                                      ( void ) const;
    const string & Subject                                 ( void ) const;
    const CTimeStamp & TimeStamp                           ( void ) const;
    friend ostream & operator <<                           ( ostream          & os,
                                                             const CMail      & x );
  private:
    CTimeStamp timeStamp;
    string from, to, subject;
};
CMail::CMail                            ( const CTimeStamp & timeStamp,
                                          const string     & from,
                                          const string     & to,
                                          const string     & subject ):
                                          timeStamp(timeStamp), from(from), to(to), subject(subject)
{}

/**
  * Function compares two int numbers
  * @in[const int &] first input number
  * @in[const int &] second input number
  * @out[int] if a = b return 0, if a < b return <0, if a > b return >0
**/
inline int comThis( const int  & a, const int & b)
{
  if ( a == b )
    return 0;
  else
    return a - b;
}

/**
  * Function compares two double numbers
  * @in[const double &] first number to compare
  * @in[const double &] second number to compare
  * @out[int] if a = b return 0, else if a < b return -1 else if a > b return 1
**/
inline int comThis (const double & a, const double & b)
{
  const double EPSILON = 1e-10;
  const double diffD = a - b;
  const double lessDiffD = (diffD<0)?(-1*diffD):(diffD);
  if ( lessDiffD < EPSILON )
    return 0;
  else if (diffD < (double)0)
    return -1;
  else
    return 1;
}



/**
  * This function prints out the content of CTimeStamp class
  * @in ostream & output stream
  * @in const CTimeStamp & class to print
  * @out ostream & output stream
**/
ostream & operator << ( ostream & os, const CTimeStamp & x )
{
  return os <<  std::setw(4) << x.year << "-" <<
                std::setw(2) << std::setfill('0') << std::internal << x.month << "-" <<
                std::setw(2) << std::setfill('0') << std::internal << x.day << " " <<
                std::setw(2) << std::setfill('0') << std::internal << x.hour << ":" <<
                std::setw(2) << std::setfill('0') << std::internal << x.minute << ":" <<
                std::setw(6) << std::setfill('0') << std::setprecision(3) << std::fixed << (x.sec);
}


/**
  * This method compare two CTimeStamp by their values
  * @in[const CTimeStamp &] second class for comparation
  * @out[int] if a < b return -1, else if a > b return 1 else return 0
**/
int CTimeStamp::Compare ( const CTimeStamp & x ) const
{
  if ( comThis(year, x.year) )
    return comThis(year, x.year);

  if ( comThis(month, x.month) )
    return comThis(month, x.month);

  if ( comThis(day, x.day) )
      return comThis(day, x.day);

  if ( comThis(hour, x.hour) )
        return comThis(hour, x.hour);

  if ( comThis(minute, x.minute) )
        return comThis(minute, x.minute);

  return comThis(sec, x.sec);
}
// -- end of CTimeStamp methods ----------------------------------------------------------------


/**
  * This function prints out the content of CMail class
  * @in ostream & output stream
  * @in const CMail & class to print
  * @out ostream & output stream
**/
ostream & operator << ( ostream & os, const CMail & x )
{
  return os << x.timeStamp << " " << x.from << " -> " << x.to << ", subject: " << x.subject;
}


/**
  * Function compares two time stamps
  * @in[const CTimeStamp &] second class to compare
  * @out[int] return 1 if a < b, return -1 if a > b, return 0 if a == b
**/
int CMail::CompareByTime ( const CTimeStamp & x ) const
{
  return timeStamp.Compare( x );
}


/**
  * Function compares two Cmail classes
  * @in[const CMail &] second class to compare
  * @out[int] return 1 if a < b, return -1 if a > b, return 0 if a == b
**/
int CMail::CompareByTime ( const CMail & x ) const
{
  return timeStamp.Compare( x.timeStamp );
}


/**
  * Function returns from item form class
  *@out[string]
**/
const string & CMail::From                                    ( void ) const
{
  return from;
}


/**
  * Function returns to item form class
  *@out[string]
**/
const string & CMail::To                                      ( void ) const
{
  return to;
}


/**
  * Function returns subject item form class
  *@out[string]
**/
const string & CMail::Subject                                 ( void ) const
{
  return subject;
}


/**
  * Function returns timeStamp item form class
  *@out[CTimeStamp]
**/
const CTimeStamp & CMail::TimeStamp                           ( void ) const
{
  return timeStamp;
}



// your code will be compiled in a separate namespace
namespace MysteriousNamespace {
  #endif  /*__PROGTEST__*/
//----------------------------------------------------------------------------------------
class CMailLog
{
  public:
    ~CMailLog();
    int            ParseLog                                ( istream          & in );

    list<CMail>    ListMail                                ( const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;

    set<string>    ActiveUsers                             ( const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;
    static bool comThis                                    (CMail * one, CMail * two);
    static bool findMail                                   (CMail * one, CMail * two);
    void appFrom                                           ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message );

    void appCon                                            ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message );

    void appLog                                            ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message );

  private:
    struct itemVe
    {
      itemVe( CTimeStamp * timeAr, string & relay_name, string & mailID, string & message) : timeAr(timeAr), relay_name(relay_name), mailID(mailID), message(message) {}
      ~itemVe(){ delete timeAr; }
      CTimeStamp * timeAr;
      string relay_name, mailID, message;
    };

    map <string, std::unique_ptr<itemVe>>  fromPer;
    map <string, std::unique_ptr<itemVe>>  contentPer;
    vector <CMail*>        mailLog;
    vector <CTimeStamp*>   garbage;
};

CMailLog::~CMailLog()
{
  for ( auto iter : mailLog )
    delete iter;
  mailLog.clear();
  for ( auto iter : garbage )
    delete iter;
  garbage.clear();
}

/**
  * Method for comparation two CMail classes
  * @in[CMail *] first class to compare
  * @in[CMail *] second class to compare
  * @out[bool] if one < two return true, else false
**/
bool CMailLog::comThis (CMail * one, CMail * two)
{
  int i = one->CompareByTime(*two);

  if ( i >= 0 )
    return false;
  else
    return true;
}

/**
  * Method for comparation two CMail classes
  * @in[CMail *] first class to compare
  * @in[CMail *] second class to compare
  * @out[bool] if one <= two return true, else false
**/
bool CMailLog::findMail (CMail * one, CMail * two)
{
  int i = one->CompareByTime(*two);

  if ( i > 0 )
    return false;
  else
    return true;
}

/**
  * Method for insertion into fromPer map
  * @in[CTimeStamp *] time
  * @in[string &] more parameters for input
**/
void CMailLog::appFrom ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message )
{
  fromPer[mailID] = std::unique_ptr<itemVe> ( new itemVe( inSt, relay_name, mailID, message ) );
}


/**
  * Method for insertion into contentPer map
  * @in[CTimeStamp *] time
  * @in[string &] more parameters for input
**/
void CMailLog::appCon ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message )
{
  contentPer[mailID] = std::unique_ptr<itemVe> ( new itemVe( inSt, relay_name, mailID, message ));
}


/**
  * Method for insertion into garbage and mailLog vectors
  * @in[CTimeStamp *] time
  * @in[string &] more parameters for input
**/
void CMailLog::appLog ( CTimeStamp * inSt, string & relay_name, string & mailID, string & message )
{
  string conT = "";
  if ( contentPer.count(mailID) )
    conT = contentPer[mailID]->message;

  garbage.emplace_back(inSt);

  CMail * tmp = new CMail( *inSt, fromPer[mailID]->message, message, conT);

  auto iter = lower_bound( mailLog.begin(), mailLog.end(), tmp, findMail );

/*
  while ( iter != mailLog.end() )
  {
    cout << "here is me" << endl;
    if ( ! (*iter)->CompareByTime( *tmp ) )
      iter++;
    else
      break;
  }
*/
  iter = mailLog.insert(iter, tmp);
}


/**
  * Method for parsing the input
  * @in[istream &] input stream
  * @out[int] number of created emails, passed to mailLog vector
**/
int CMailLog::ParseLog ( istream & in )
{
  map <string, int> months ;
  string allMon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  for ( int i = 0; i<12; ++i )
    months[allMon[i]] = i+1;

  int year, day, hour, minute, arem = 0;
  double sec;
  string month, relay_name, mailID, message, lineRead;
  char q;
  std::getline(in, lineRead);


  while ( ! in.eof() )
  {
    istringstream strIn;
    strIn.str(lineRead);

    strIn >> month >> ws >> day >> ws >> year >> ws >> hour >> q >> minute >> q >> sec >> ws >> relay_name >> ws >> mailID >> ws;
    lineRead = strIn.str();
    message = lineRead.substr(strIn.tellg());

    if ( message.length() >= 5 && message.substr(0, 5) == "from=" )
    {
      message = message.substr(5);
      appFrom ( (new CTimeStamp( year, months[month], day, hour, minute, sec )),
                relay_name, mailID, message );  // append map
    }
    else if ( message.length() >= 8 && message.substr(0, 8) == "subject=" )
    {
      message = message.substr(8);
      appCon  ( (new CTimeStamp( year, months[month], day, hour, minute, sec )),
                relay_name, mailID, message ); // append map
    }
    else if ( message.length() >= 3 && message.substr(0, 3) == "to=" )
    {
      arem ++;
      message = message.substr(3);
      appLog  ( (new CTimeStamp( year, months[month], day, hour, minute, sec )),
                relay_name, mailID, message ); // append vector
    }

    std::getline(in, lineRead);
  }
  return arem;
}


/**
  * Method for listing mails by given time
  * @in[CTimeStamp &] time from
  * @in[CTimeStamp &] time to
  * @out[list<CMail>] output list of mails in given time range
**/
list<CMail>    CMailLog::ListMail                      ( const CTimeStamp & from,
                                                         const CTimeStamp & to ) const
{
  list<CMail> data;
  CMail tmpFrom(from, "", "", "");
  CMail tmpTo(to, "", "", "");

  auto fromIt = lower_bound( mailLog.begin(), mailLog.end(), &tmpFrom, comThis );
  auto toIt = lower_bound( mailLog.begin(), mailLog.end(), &tmpTo, findMail );

  while ( fromIt != toIt )
  {
    data.emplace_back( *(*fromIt) );
    fromIt++;
  }
  return data;
}


/**
  * Method for listing emails by given time
  * @in[CTimeStamp &] time from
  * @in[CTimeStamp &] time to
  * @out[set<string>] output set of mails in given time range
**/
set<string>    CMailLog::ActiveUsers                             ( const CTimeStamp & from,
                                                         const CTimeStamp & to ) const
{
  set<string> data;

  CMail fromTime (from, "", "", ""), toTime(to, "", "", "");

  auto fromIt = lower_bound( mailLog.begin(), mailLog.end(), &fromTime, comThis );
  auto toIt = lower_bound( mailLog.begin(), mailLog.end(), &toTime, findMail );

  while ( fromIt != toIt )
  {
    data.insert( (*fromIt)->From() );
    data.insert( (*fromIt)->To() );
    ++fromIt;
  }

  return data;
}


//----------------------------------------------------------------------------------------
#ifndef __PROGTEST__
} // namespace
string             printMail                               ( const list<CMail> & all )
{
  ostringstream oss;
  for ( const auto & mail : all )
    oss << mail << endl;
  return oss . str ();
}
string             printUsers                              ( const set<string> & all )
{
  ostringstream oss;
  bool first = true;
  for ( const auto & name : all )
  {
    if ( ! first )
      oss << ", ";
    else
      first = false;
    oss << name;
  }
  return oss . str ();
}
int                main                                    ( void )
{
  MysteriousNamespace::CMailLog m;
  list<CMail> mailList;
  set<string> users;
  istringstream iss;

  iss . clear ();
  iss . str (
    "Mar 29 2019 12:35:32.233 relay.fit.cvut.cz ADFger72343D: from=user1@fit.cvut.cz\n"
    "Mar 29 2019 12:37:16.234 relay.fit.cvut.cz JlMSRW4232Df: from=person3@fit.cvut.cz\n"
    "Mar 29 2019 12:55:13.023 relay.fit.cvut.cz JlMSRW4232Df: subject=New progtest homework!\n"
    "Mar 29 2019 13:38:45.043 relay.fit.cvut.cz Kbced342sdgA: from=office13@fit.cvut.cz\n"
    "Mar 29 2019 13:36:13.023 relay.fit.cvut.cz JlMSRW4232Df: to=user76@fit.cvut.cz\n"
    "Mar 29 2019 13:55:31.456 relay.fit.cvut.cz KhdfEjkl247D: from=PR-department@fit.cvut.cz\n"
    "Mar 29 2019 14:18:12.654 relay.fit.cvut.cz Kbced342sdgA: to=boss13@fit.cvut.cz\n"
    "Mar 29 2019 14:48:32.563 relay.fit.cvut.cz KhdfEjkl247D: subject=Business partner\n"
    "Mar 29 2019 14:58:32.000 relay.fit.cvut.cz KhdfEjkl247D: to=HR-department@fit.cvut.cz\n"
    "Mar 29 2019 14:25:23.233 relay.fit.cvut.cz ADFger72343D: mail undeliverable\n"
    "Mar 29 2019 15:02:34.231 relay.fit.cvut.cz KhdfEjkl247D: to=CIO@fit.cvut.cz\n"
    "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=CEO@fit.cvut.cz\n"
    "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=dean@fit.cvut.cz\n"
    "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=vice-dean@fit.cvut.cz\n"
    "Mar 29 2019 15:02:34.230 relay.fit.cvut.cz KhdfEjkl247D: to=archive@fit.cvut.cz\n" );
  assert ( m . ParseLog ( iss ) == 8 );
  mailList = m . ListMail ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 23, 59, 59 ) );
  assert ( printMail ( mailList ) ==
    "2019-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!\n"
    "2019-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz, subject: \n"
    "2019-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner\n"
    "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> CEO@fit.cvut.cz, subject: Business partner\n"
    "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> dean@fit.cvut.cz, subject: Business partner\n"
    "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> vice-dean@fit.cvut.cz, subject: Business partner\n"
    "2019-03-29 15:02:34.230 PR-department@fit.cvut.cz -> archive@fit.cvut.cz, subject: Business partner\n"
    "2019-03-29 15:02:34.231 PR-department@fit.cvut.cz -> CIO@fit.cvut.cz, subject: Business partner\n" );
  mailList = m . ListMail ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 14, 58, 32 ) );

  assert ( printMail ( mailList ) ==
    "2019-03-29 13:36:13.023 person3@fit.cvut.cz -> user76@fit.cvut.cz, subject: New progtest homework!\n"
    "2019-03-29 14:18:12.654 office13@fit.cvut.cz -> boss13@fit.cvut.cz, subject: \n"
    "2019-03-29 14:58:32.000 PR-department@fit.cvut.cz -> HR-department@fit.cvut.cz, subject: Business partner\n" );
  mailList = m . ListMail ( CTimeStamp ( 2019, 3, 30, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 30, 23, 59, 59 ) );
  assert ( printMail ( mailList ) == "" );
  mailList = m . ListMail ( CTimeStamp ( 2018, 3, 30, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 13, 36, 13.022 ) );
  assert ( printMail ( mailList ) == "" );
  users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 23, 59, 59 ) );
  assert ( printUsers ( users ) == "CEO@fit.cvut.cz, CIO@fit.cvut.cz, HR-department@fit.cvut.cz, PR-department@fit.cvut.cz, archive@fit.cvut.cz, boss13@fit.cvut.cz, dean@fit.cvut.cz, office13@fit.cvut.cz, person3@fit.cvut.cz, user76@fit.cvut.cz, vice-dean@fit.cvut.cz" );
  users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 13, 59, 59 ) );
  assert ( printUsers ( users ) == "person3@fit.cvut.cz, user76@fit.cvut.cz" );
  return 0;
}
#endif /* __PROGTEST__ */
