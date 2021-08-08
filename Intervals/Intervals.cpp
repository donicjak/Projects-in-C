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

class CRange
{
  public:
    // constructor
    CRange ( void ) {}
    CRange ( long long int lo, long long int hi );
    ~CRange ( void ) {}

    long long int lo;
    long long int hi;
  
  private:

    // todo
};
class CRangeList
{
  public:
    // constructor
    CRangeList ( void ) {};
    CRangeList ( const CRange & interval );
    CRangeList ( const CRangeList &c2 );
    ~CRangeList ( void )
    {
        for ( auto & i : CRangeListVector )
        delete i;
    }

    // Includes long long / range
    // += range / range list
    CRangeList & operator += ( const CRange & interval );
    CRangeList & operator += ( const CRangeList & tmp );
    // -= range / range list
    CRangeList & operator -= ( const CRange & interval );
    CRangeList & operator -= ( const CRangeList & tmp );
    //CRangeList & operator -= ( const CRangeList & tmp );
    // = range / range list
    CRangeList & operator = ( const CRange & interval );
    CRangeList & operator = ( const CRangeList & tmp );
    // operator ==

    // operator !=
       // operator <<
    friend ostream & operator << ( ostream & os, const CRangeList & x );

    friend CRangeList & operator + ( const CRangeList & tmp, const CRange & interval );
    friend CRangeList  & operator + ( const CRange & first, const CRange & second );
    friend CRangeList & operator - ( const CRange & first, const CRange & second );
    friend CRangeList & operator - ( const CRangeList & tmp, const CRange & interval );

    bool  operator == ( const CRangeList & x ) const;
    bool  operator != ( const CRangeList & x ) const;

    void mergeIntervals (  );

    ostream & printList ( ostream & os ) const;

    bool Includes ( long long int number ) const;
    bool Includes ( const CRange & interval ) const; 

  private:
  vector < CRange* > CRangeListVector;

};

CRangeList::CRangeList ( const CRangeList &c2 )
{
  for ( auto it = c2 . CRangeListVector . begin (); it != c2 . CRangeListVector . end(); ++it)
  {
    CRange * tmp = new CRange ( ( *it ) -> lo, ( *it ) -> hi );
    CRangeListVector . push_back ( tmp );
  }
}


bool isValid ( long long int lo, long long int hi )
{
    if ( lo > hi )
    return false;
    
    return true;
}

CRange::CRange ( long long int lo, long long int hi )
{
    if ( !isValid (lo, hi ) )
        throw InvalidRangeException();
    
    this -> lo = lo;
    this -> hi = hi;
} 



bool lowerComparator ( const CRange * left, const long long int &right )
{
    return left -> lo < right;
}

bool lowerComparatorX ( const CRange * left, const long long int &right )
{
    return left -> hi < right;
}

bool upperComparator ( const long long int &left, const CRange * right )
{
    return left < right -> hi;
}

bool upperComparatorX ( const long long int &left, const CRange * right )
{
    return left < right -> lo;
}
 
CRangeList & CRangeList::operator = ( const CRange & interval )
{
    CRangeListVector . erase ( CRangeListVector . begin(), CRangeListVector . end() );

    CRange * tmpCRange = new CRange ( interval . lo, interval . hi );
    CRangeListVector . push_back ( tmpCRange );

    return *this;
}

CRangeList & CRangeList::operator = ( const CRangeList & tmp )
{
    CRangeListVector . erase ( CRangeListVector . begin(), CRangeListVector . end() );

    CRangeListVector . insert ( CRangeListVector . end(), tmp . CRangeListVector . begin (), tmp . CRangeListVector . end () );
    return *this;
}

void CRangeList::mergeIntervals (  )
{
   vector < CRange* > tmp;
   vector <CRange*>::iterator it = CRangeListVector . begin ();
   auto current = ( *it++ );  

   while ( it != CRangeListVector . end() )
   {
       if (  current -> hi + 1  >= (  *it ) -> lo  )
       {  
           current -> hi = max(current -> hi, ( *it ) -> hi);
       }
       else
       {
        tmp . push_back ( current );
        current = (*it);
       }
       it++;
   } 
    tmp . push_back ( current );
    CRangeListVector . erase ( CRangeListVector . begin (), CRangeListVector . end() );

    for ( auto to : tmp )
    {
      CRangeListVector . push_back ( to );
    }
}


CRangeList & CRangeList::operator += ( const CRange & interval )
{
    CRange * tmpCRange = new CRange ( interval . lo, interval . hi );

    if ( interval . lo == LLONG_MIN && interval . hi == LLONG_MAX )
    {
      CRangeListVector . erase ( CRangeListVector . begin(), CRangeListVector . end ());
      CRangeListVector . push_back ( tmpCRange );
      return *this;
    }

    auto index = lower_bound( CRangeListVector . begin (), CRangeListVector . end(), interval . lo, lowerComparator );
    
    CRangeListVector . insert ( index, tmpCRange);

    mergeIntervals (  );
    return  *this;
}

CRangeList & CRangeList::operator += ( const CRangeList & tmp )
{
    if ( tmp . CRangeListVector . empty () )
    return *this;

    for ( const auto it : tmp . CRangeListVector )
    {
    if ( (*it) . lo == LLONG_MIN && (*it) . hi == LLONG_MAX )
    {
      CRangeListVector . erase ( CRangeListVector . begin (), CRangeListVector . end());
      CRangeListVector . push_back ( it );
      return *this;
    }

    auto index = lower_bound( CRangeListVector . begin (), CRangeListVector . end(), it -> lo, lowerComparator );
    CRangeListVector . insert ( index, it);
    }

    mergeIntervals (  );
    return *this;
}

 CRangeList & operator + ( const CRange & first, const CRange & second )
{   
    CRangeList *tmp = new CRangeList();
   ( *tmp ) = first;
   ( *tmp ) += second;

    return *tmp;  
}

CRangeList & operator - ( const CRange & first, const CRange & second )
{
  CRangeList *result = new CRangeList();
  ( *result ) = first;
  ( *result ) -= second;

  return ( *result );
}

 CRangeList & operator + ( const CRangeList & tmp, const CRange & interval )
{
   CRangeList *result = new CRangeList();
   ( *result ) = tmp;
   ( *result ) += interval;

   return ( *result );
}

CRangeList & operator - ( const CRangeList & tmp, const CRange & interval )
{
  CRangeList * result = new CRangeList();
  ( *result ) = tmp;
  ( *result ) -= interval;

  return ( *result ); 
}

CRangeList & CRangeList::operator -= ( const CRange & interval )
{
    int counter = 0;
    if  ( interval . lo == LLONG_MIN && interval . hi == LLONG_MAX )
    {
      CRangeListVector . erase ( CRangeListVector . begin (), CRangeListVector . end ());
      return *this;
    }

    if  (CRangeListVector . empty () )
    return *this;

    CRangeList *tmpList = new CRangeList();

  //  cout << "Interval je: " << interval . lo << " " << interval . hi << endl;

    auto index = lower_bound ( CRangeListVector . begin (), CRangeListVector . end (), interval . lo, lowerComparatorX );
    auto index2 = upper_bound ( CRangeListVector . begin (), CRangeListVector . end (), interval . hi, upperComparator );

    if ( index2 == CRangeListVector . end() )
    index2 = CRangeListVector . end ( ) - 1;

//    cout << " Indexy jsou: " << ( *index ) -> lo << " " << ( *index ) -> hi << " a " << ( *index2 ) -> lo << " " << ( *index2 ) -> hi << endl;

    for ( index = index; index != index2 - counter + 1; ++index )
    {
      if ( ( *index ) -> lo > interval . hi )
      break;

      if ( ( *index ) -> lo < interval . lo && ( ( *index ) -> hi > interval  . hi ) )
      {
    //  cout << " Vstupuji sem" << endl;
        CRange * tmp = new CRange ( ( *index) -> lo, interval . lo - 1);
        CRange * tmp2 = new CRange (  interval . hi + 1, ( *index ) -> hi);

//      cout << " Nove intervaly jsou: " << ( *index ) -> lo << " " << interval . lo -1 << " a " << interval . hi + 1 << " " << ( *index ) -> hi << endl;
        if ( index == CRangeListVector . end() - 1 + counter )
        CRangeListVector . pop_back();
        
        else
        CRangeListVector . erase ( index );
          
      

        ( *tmpList ) = ( *tmp );
        ( *tmpList ) += ( *tmp2 );

      //cout << " UZ tady se to dojebe:" << endl;
       /* for ( auto ata : (*tmpList) . CRangeListVector )
        {
          cout << ( *ata ) . lo << " " <<  ( *ata ) . hi << endl;
        }*/
        break;
      }


      else if ( ( *index) -> lo >= interval . lo && ( ( *index ) -> hi > interval . hi ) )
      {
      //  cout << "Vstupuji i zde" << endl;
        CRange * tmp = new CRange ( interval . hi + 1, ( *index ) -> hi );


        if ( index == CRangeListVector . end() - 1 + counter ) 
        CRangeListVector . pop_back();
    
        else
        CRangeListVector . erase ( index );
            
        ( *tmpList ) += ( *tmp );
        
        break;
      }


      else if ( ( *index) -> lo < interval . lo && ( ( *index ) -> hi <= interval . hi ) )
      {
   //   cout << "Vstupuji i zde2" << endl;

        CRange * tmp = new CRange ( ( *index) -> lo, interval . lo - 1);

        if ( index == CRangeListVector . end() )
        CRangeListVector . pop_back();

        else
        {
        CRangeListVector . erase ( index );
        index--;
        counter++;
        }

        ( *tmpList ) = ( *tmp );
      }

      else if ( ( ( *index ) -> lo >= interval . lo ) && ( ( *index ) -> hi <= interval . hi ) )
      {
          
    //   cout << "nebo sem" << endl;
        if ( index == CRangeListVector . end() - 1  )
       {
        CRangeListVector . pop_back();
        }
       
        else
        {
            CRangeListVector . erase ( index );
            counter++;
            index--;
        }
      }  

    }
    
    if ( CRangeListVector . empty() && (( *tmpList ) . CRangeListVector . empty()) )
     return *this;

    else
    {
    for ( auto it : ( *tmpList) . CRangeListVector )
      {
    auto ot = lower_bound( CRangeListVector . begin (), CRangeListVector . end(), it -> lo, lowerComparator );

    CRangeListVector . insert ( ot, it);
    
        mergeIntervals (  );

      }
    }
return *this;
}

CRangeList & CRangeList::operator -= ( const CRangeList & tmp ) 
{
  for ( const auto & iteratorek :  tmp . CRangeListVector )
  {
    ( *this ) -=  (*iteratorek) ;
  }

  return *this;
  
}

bool CRangeList::Includes ( long long int number ) const
{
    auto it = lower_bound ( CRangeListVector . begin (), CRangeListVector . end(), number, lowerComparatorX );
    
    if ( it == CRangeListVector . end ( ) )
    return false;

    if ( (*it) -> lo <= number && ( *it ) -> hi >= number)
    {
      return true;
    }
return false;    
}
    
bool CRangeList::Includes ( const CRange & interval ) const
{
    auto it = lower_bound ( CRangeListVector . begin (), CRangeListVector . end(), interval . lo, lowerComparatorX );
 
    if ( it == CRangeListVector . end ( ) )
    return false;

    if ( ( *it ) -> lo <= interval . lo && ( *it ) -> hi >= interval . hi )
    {
            return true;
    }
    return false;
}

bool CRangeList::operator == ( const CRangeList & x ) const
{
    return ( equal ( this -> CRangeListVector . begin (), this -> CRangeListVector . end (), x . CRangeListVector . begin () ));
}

bool CRangeList::operator != ( const CRangeList & x ) const
{
    return !( *this == x );
}

ostream & operator << ( ostream & os, const CRangeList & x )
{

  
    os << "{";
    //cout << "{";
    for ( auto it = x . CRangeListVector . begin (); it != x . CRangeListVector . end(); ++it )
    {
    if ( ( *it ) -> hi == LLONG_MAX )
     {
        if ( ( *it ) -> lo < ( *it ) -> hi )
        {
           os << "<" << (*it) -> lo << ".." << (*it) -> hi << ">";
        //  cout << "<" << (*it) -> lo << ".." << (*it) -> hi << ">";
        }
        else
        {
      os << (*it) -> lo ;
      //cout <<  (*it) -> lo;
        }
      break;  
     }
     
     if ( ( *it ) -> lo < ( *it ) -> hi )
     {

      if ( it == x . CRangeListVector . end() - 1 )
     {
      os << "<" << (*it) -> lo << ".." << (*it) -> hi << ">";
    //cout << "<" << (*it) -> lo << ".." << (*it) -> hi << ">";

     }
     else
     {
         os << "<" << (*it) -> lo << ".." << (*it) -> hi << ">" << ",";
        //cout<< "<" << (*it) -> lo << ".." << (*it) -> hi << ">" << ",";

     }
     }
     else
     {
      if ( it == x . CRangeListVector . end() - 1 )
     {
      os << (*it) -> lo ;
      //cout <<  (*it) -> lo;

     }
     else
     {
         os <<  (*it) -> lo  << ",";
        //cout<<  (*it) -> lo << ",";
             
     } 
     }
     

    }
    os << "}";
       // cout << "}" << endl;

    return os;
}

/*#ifndef __PROGTEST__*/
string             toString                                ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int                main                                    ( void )
{
  CRangeList a,b;

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
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
  return 0;
}
//#endif /* __PROGTEST__ */
