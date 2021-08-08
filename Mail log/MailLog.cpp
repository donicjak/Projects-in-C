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
};
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
};
// your code will be compiled in a separate namespace
namespace MysteriousNamespace {
#endif /* __PROGTEST__ */
//----------------------------------------------------------------------------------------
class CMailLog
{
  public:

    //~CMailLog ( );
    int            ParseLog                                ( istream          & in );

    list<CMail>    ListMail                                ( const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;

    set<string>    ActiveUsers                             ( const CTimeStamp & from,
                                                             const CTimeStamp & to ) const;
                                                             
    int getMonth ( const string & month );
                                                         
  private:
  vector < CMail > holder;
  map <string, pair <string, string> > mapaLog;
};
//----------------------------------------------------------------------------------------



int CMailLog::getMonth ( const string & month )
{


    if ( month == "Jan" )
    return 1;

    if ( month == "Feb" )
    return 2;

    if ( month == "Mar" )
    return 3;

    if  ( month == "Apr" )
    return 4;

    if ( month == "May" )
    return 5;

    if ( month == "Jun" )
    return 6;

    if ( month == "Jul" )
    return 7;

    if ( month == "Aug" )
    return 8;

    if ( month == "Sep" )
    return 9;

    if ( month == "Oct" )
    return 10;

    if ( month == "Nov" )
    return 11;

    if ( month == "Dec" )
    return 12;

    return -1;
    
}

bool lowerComparator ( const CMail & left, const CTimeStamp & right )
{   
   if ( left . CompareByTime ( right ) > 0 )
   return false;

   return true;
}

bool upperComparator ( const CMail & left, const CMail & right )
{   
   if ( right . CompareByTime ( left ) <= 0 )
   return false;

   return true;
}

bool upperComparator2 ( const CTimeStamp & left, const CMail & right )
{   
   if ( right . CompareByTime ( left ) <= 0 )
   return false;

   return true;
}

int CMailLog::ParseLog ( istream & in )
{
  string line;
  int counter = 0, day, year, hour, minute;
  char dvojtecka = ':';
  double second;
  stringstream ss;
  string month, skip, id, message;
  

  while ( getline ( in, line, '\n' ) )
  {
    ss . str (line);
    ss >> month >> day >> year >> hour >> dvojtecka >> minute >> dvojtecka >> second >> skip >> id >> message;
    if ( !(ss.rdbuf()->in_avail() == 0) )
    {
      do 
      {
        string pom;
        getline ( ss, pom );
        message += "" + pom;

      } while ( !(ss.rdbuf()->in_avail() == 0) );
    }
   // cout << month << " " << day << " " << year << " " << hour << ":" << minute << ":" << second << " " << skip << " " << id << message << endl;

    string isFrom = message . substr ( 0, 5 );
    string isSubject = message . substr ( 0, 8 );
    string isTo = message . substr ( 0, 3 );

    if ( isFrom == "from=" )
    {
    //  cout << "Je from" << endl;
      mapaLog . insert ( pair <string, pair<string, string > > ( id, make_pair ( message . substr ( 5 ), "") ) );
    }

    if ( isSubject == "subject=" )
    {
    //  cout << "Je subject "<< endl;
      auto it = mapaLog . find ( id );
      if ( it != mapaLog . end() );
      ( it ) -> second . second = message . substr ( 8 ); 
  

    }

    if ( isTo == "to=" )
    {
     // cout << " je to =" << endl;
      CMail tmp ( CTimeStamp ( year, getMonth(month), day, hour, minute, second ), mapaLog . find ( id ) -> second . first, message . substr ( 3 ),  mapaLog . find ( id ) -> second . second );
      
     // cout << tmp << endl;
      auto it = upper_bound ( holder . begin (), holder . end (), tmp, upperComparator );
      
      holder . insert ( it, tmp );
      counter++;
    }

    ss . clear();
  }
  
   // cout << " Counter je: " << counter << endl;
    
   // cout << "---------------------------------------------------" << endl;
    /*for(map<string, pair<string,string> >::const_iterator it = mapaLog.begin();
    it != mapaLog.end(); ++it)
  {
    std::cout << it->first << " " << it->second.first << " " << it->second.second << "\n";
  }*/

    return counter;
}

list<CMail> CMailLog::ListMail ( const  CTimeStamp & from, const CTimeStamp & to ) const
{
    
    auto it = lower_bound ( holder . begin (), holder . end (), from, lowerComparator );
    auto it2 = upper_bound ( holder . begin (), holder . end (), to, upperComparator2 );

   

    list <CMail> listek;


    std::copy( it, it2, back_inserter (listek) );



    return listek;

}


set< string > CMailLog::ActiveUsers ( const  CTimeStamp & from, const CTimeStamp & to ) const
{
    auto it = lower_bound ( holder . begin (), holder . end (), from, lowerComparator );

    auto it2 = upper_bound ( holder . begin (), holder . end (), to, upperComparator2 );

    set < string > res;

    while ( it != it2  )
    {
        res . insert ( it -> From ());
        res . insert ( it -> To ());
        it++;
    }


    return res;
}

#ifndef __PROGTEST__
} //namespace
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
  users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 23, 59, 59 ) );
  assert ( printUsers ( users ) == "CEO@fit.cvut.cz, CIO@fit.cvut.cz, HR-department@fit.cvut.cz, PR-department@fit.cvut.cz, archive@fit.cvut.cz, boss13@fit.cvut.cz, dean@fit.cvut.cz, office13@fit.cvut.cz, person3@fit.cvut.cz, user76@fit.cvut.cz, vice-dean@fit.cvut.cz" );
  users = m . ActiveUsers ( CTimeStamp ( 2019, 3, 28, 0, 0, 0 ),
                            CTimeStamp ( 2019, 3, 29, 13, 59, 59 ) );
  assert ( printUsers ( users ) == "person3@fit.cvut.cz, user76@fit.cvut.cz" );
  return 0;
}
#endif /* __PROGTEST__ */