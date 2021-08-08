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

  static unsigned int counterClass = 0;

bool compareStrings(string i, string j)
{
  return (i < j);
}

class Land
{
public:
  Land(const string &city, const string &addr, const string &region, unsigned int id, const string &owner)
      : city(city), addr(addr), region(region), id(id), owner(owner), counterLoc(counterClass++) {}

  Land(const Land &land) : city(land.city), addr(land.addr), region(land.region), id(land.id), owner(land.owner), counterLoc(counterClass) {}

  Land(void) {}

  Land(const string &city, const string &addr)
  : city(city), addr(addr) {}

  Land(const string &region, unsigned int id)
  : region(region), id(id) {}

  ~Land(void) {}

  void PrintLand(void) const;

  string city;
  string addr;
  string region;
  unsigned int id;
  string owner = "";
  int unsigned counterLoc;

private:
};

class CIterator
{
public:
  bool AtEnd(void) const;
  void Next(void);
  string City(void) const;
  string Addr(void) const;
  string Region(void) const;
  unsigned ID(void) const;
  string Owner(void) const;

  CIterator(const vector<Land*> pointer, bool flag);

private:
 
  int unsigned counter;
  const vector<Land*> pointer;
  bool flag;
};

class CLandRegister
{
public:
  bool Add(const string &city,
           const string &addr,
           const string &region,
           unsigned int id);

  bool Del(const string &city,
           const string &addr);

  bool Del(const string &region,
           unsigned int id);

  bool GetOwner(const string &city,
                const string &addr,
                string &owner) const;

  bool GetOwner(const string &region,
                unsigned int id,
                string &owner) const;

  bool NewOwner(const string &city,
                const string &addr,
                const string &owner);

  bool NewOwner(const string &region,
                unsigned int id,
                const string &owner);

  unsigned Count(const string &owner) const;

  CIterator ListByAddr(void) const;

  CIterator ListByOwner(const string &owner) const;
  void Print(void) const;
    

 ~CLandRegister()
 {
   for(auto & i : LandVector)
   delete i;
 }
  
private:
  vector<Land*> LandVector;
  vector<Land*> RegionID;
  vector<Land*> OwnerSorted;
  vector<Land*> OwnerVectorSorted;
 
};

void Land::PrintLand(void) const
{
  cout << city << " " << addr << " " << region << " " << id << " " << owner;
}

bool pairComparator(const Land *left, const pair<string, string> &right)
{

  if (left -> city == right . first)
  {
    return left -> addr < right . second;
  }
  return left -> city < right . first;
}

bool pairComparator2(const pair<string, string> &left, const Land *right)
{

  if (left . first == right -> city)
  {
    return left . second < right -> addr;
  }
  return left . first < right -> city;
}

bool regionIDComparator(const Land *left, const pair<string, unsigned int> &right)
{

  if (left -> region == right . first)
  {
    return left -> id < right . second;
  }
  return  left -> region < right . first;
}

bool regionIDComparator2(const pair<string, unsigned int> &left, const Land *right)
{

  if (left . first == right -> region)
  {
    return left . second < right -> id;
  }
  return  left . first < right -> region;
}

char tolowercase (char c)
{
    if ( c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}



bool upperComparator(const string & left, const Land  *right)
{
    string s = left;
    for ( char& c : s)
    {
        c = tolowercase(c);
    }

    string s2 = right -> owner;
    for ( char& c : s2)
    {
        c = tolowercase(c);
    } 
    return  s  < s2;

}

bool lowerComparator(const Land * left, const string & right)
{
    string s = right;
    for ( char& c : s)
    {
        c = tolowercase(c);
    }

    string s2 = left -> owner;
    for ( char& c : s2)
    {
        c = tolowercase(c);
    }
    return s2  < s;

}




bool CLandRegister::Add(const string &city, const string &addr, const string &region, unsigned int id)
{
  
  Land * tmpLand = new Land(city, addr, region, id, "");
  
  auto index3 = upper_bound(OwnerSorted . begin (), OwnerSorted . end (), "", upperComparator);

  auto index = lower_bound(LandVector.begin(), LandVector.end(), make_pair(city,addr), pairComparator);

  auto index2 = lower_bound(RegionID.begin(), RegionID.end(), make_pair(region,id), regionIDComparator);
  

    if ( ( index == LandVector. end() || (*index) -> city != city || (*index) -> addr != addr ) && ( index2 == RegionID . end() || (*index2) -> region != region || (*index2) -> id != id ) )
    {
        LandVector . insert(index, tmpLand);
        RegionID . insert(index2, tmpLand);
        OwnerSorted . insert(index3, tmpLand);

        return true;
    }

    if ( (*index) -> city == city && (*index) -> addr == addr)
    {
    delete tmpLand;
    
    return false;
    }


    if ( (*index2) -> region == region && (*index2) -> id == id )
    {
    delete tmpLand;
    
    return false;
    }

    return false;
}

bool CLandRegister::Del(const string &city, const string &addr)
{
  auto index = lower_bound( LandVector . begin(), LandVector . end(), make_pair(city, addr), pairComparator );
  if ( index == LandVector.end() || (*index) -> city != city || (*index) -> addr != addr)
  {
      return false;
  }

  
      string getregion = ( *index ) -> region;
      string getname = ( *index ) -> owner;
      unsigned int getid = ( *index ) -> id;
    

  auto index2 = lower_bound ( RegionID . begin(), RegionID . end(), make_pair( getregion, getid), regionIDComparator );

  auto index3 = lower_bound( OwnerSorted . begin(), OwnerSorted . end(), getname, lowerComparator );

  auto index4 = upper_bound ( OwnerSorted . begin(), OwnerSorted . end(), getname, upperComparator );

  for ( auto it = index3; it != index4; ++it )
  {
      if ( ( *it )  -> city == ( *index ) -> city && ( *it ) -> addr == ( *index ) -> addr )
      {
          OwnerSorted . erase( it );
          break;
      }
  }
  delete(*index);
  LandVector . erase ( index );
  RegionID . erase ( index2 );

  return true;

}

bool CLandRegister::Del(const string &region, unsigned int id)
{
  auto index = lower_bound( RegionID . begin(), RegionID . end(), make_pair(region, id), regionIDComparator );
  if ( index == RegionID . end() || (*index) -> region != region || (*index) -> id != id)
  {
      return false;
  }


      string getcity = ( *index ) -> city;
      string getname = ( *index ) -> owner;
      string getaddr = ( *index ) -> addr;
    

  auto index2 = lower_bound ( LandVector . begin(), LandVector . end(), make_pair( getcity, getaddr), pairComparator );

  auto index3 = lower_bound( OwnerSorted . begin(), OwnerSorted . end(), getname, lowerComparator );

  auto index4 = upper_bound ( OwnerSorted . begin(), OwnerSorted . end(), getname, upperComparator );

  for ( auto it = index3; it != index4; ++it )
  {
      if ( ( *it )  -> region == ( *index ) -> region && ( *it ) -> id == ( *index ) -> id )
      {
          OwnerSorted . erase( it );
          break;
      }
  }
  delete(*index);
  RegionID . erase ( index );
  LandVector . erase ( index2 );

  return true;

}

bool CLandRegister::NewOwner(const string &city, const string &addr, const string &owner)
{

  auto index = lower_bound( LandVector . begin(), LandVector . end(), make_pair(city, addr), pairComparator );
 
  if ( index == LandVector . end() ||  ( *index ) -> city != city || ( *index ) -> addr != addr ) 
  {
      return false;
  }

  
  string region = (*index) -> region;
  unsigned id = (*index) -> id;
  string name = (*index) -> owner;
  
  if  ( ( *index ) -> city == city && ( *index ) -> addr == addr && ( *index ) -> owner == owner )
  {
      return false;
  }

  Land * tmpLand = new Land( city, addr, region, id, owner );


  auto index2 = lower_bound( RegionID . begin(), RegionID . end(), make_pair(region, id), regionIDComparator );
  if ( index2 == RegionID . end() || ( *index2 ) -> region != region || ( *index2 ) -> id != id)
  {   
      delete tmpLand;
      return false;
  }

  auto index3 = lower_bound( OwnerSorted . begin(), OwnerSorted . end(), name, lowerComparator);
  if ( index3 == OwnerSorted . end())
  {
      delete tmpLand;
      return false;
  }

  auto index4 = upper_bound(OwnerSorted . begin(), OwnerSorted . end(), name, upperComparator);
     
  for ( auto  it = index3; it != index4; ++it )
  {
      if (  ( *it ) -> city == ( *index ) -> city && ( *it ) -> addr == ( *index ) -> addr )
      {   
      OwnerSorted.erase(it);
      break;
      }
  }
  delete (*index2);
  LandVector . erase( index );
  RegionID . erase( index2 );
 
auto index5 = upper_bound( OwnerSorted . begin(), OwnerSorted . end(), owner, upperComparator );

RegionID.insert( index2, tmpLand );
LandVector.insert( index, tmpLand );
OwnerSorted . insert( index5, tmpLand );

return true;
}


bool CLandRegister::NewOwner(const string &region, unsigned int id, const string &owner)
{

  auto index = lower_bound( RegionID . begin(), RegionID . end(), make_pair(region, id), regionIDComparator );
 
  if ( index == RegionID . end() ||  ( *index ) -> region != region || ( *index ) -> id != id ) 
  {
      return false;
  }

  
  string city = (*index) -> city;
  string addr = (*index) -> addr;
  string name = (*index) -> owner;
  
  if  ( ( *index ) -> region == region && ( *index ) -> id == id && ( *index ) -> owner == owner )
  {
      return false;
  }

  Land * tmpLand = new Land( city, addr, region, id, owner );


  auto index2 = lower_bound( LandVector . begin(), LandVector . end(), make_pair(city, addr), pairComparator );
  if ( index2 == LandVector . end() || ( *index2 ) -> city != city || ( *index2 ) -> addr != addr)
  {   
      delete tmpLand;
      return false;
  }

  auto index3 = lower_bound( OwnerSorted . begin(), OwnerSorted . end(), name, lowerComparator);
  if ( index3 == OwnerSorted . end())
  {
      delete tmpLand;
      return false;
  }

  auto index4 = upper_bound(OwnerSorted . begin(), OwnerSorted . end(), name, upperComparator);
     
  for ( auto  it = index3; it != index4; ++it )
  {
      if (  ( *it ) -> region == ( *index ) -> region && ( *it ) -> id == ( *index ) -> id )
      {   
      OwnerSorted.erase(it);
      break;
      }
  }
  delete (*index2);
  RegionID . erase( index );
  LandVector . erase( index2 );
 
auto index5 = upper_bound( OwnerSorted . begin(), OwnerSorted . end(), owner, upperComparator );

RegionID.insert( index, tmpLand );
LandVector.insert( index2, tmpLand );
OwnerSorted . insert( index5, tmpLand );

return true;
}

bool CLandRegister::GetOwner(const string &city,
                             const string &addr,
                             string &owner) const
{
  

  auto index = lower_bound( LandVector . begin(), LandVector . end(), make_pair(city, addr), pairComparator );
  if ( index == LandVector . end() ||  ( *index ) -> city != city || ( *index ) -> addr != addr ) 
  {
      return false;
  }

  auto index2 = upper_bound( LandVector . begin(), LandVector . end(), make_pair(city, addr), pairComparator2 );  

  for ( auto it = index; it != index2; ++it)
     {
        if ( (*it) -> city == city && (*it) -> addr == addr)
       {
        owner = (*it) -> owner;
        break;
       }
     }
  return true;
}

bool CLandRegister::GetOwner(const string &region,
                             unsigned int id,
                             string &owner) const
{
  auto index = lower_bound( RegionID . begin(), RegionID . end(), make_pair(region, id), regionIDComparator );
  if ( index == RegionID . end() ||  ( *index ) -> region != region || ( *index ) -> id != id ) 
  {
      return false;
  }

  auto index2 = upper_bound( RegionID . begin(), RegionID . end(), make_pair(region, id), regionIDComparator2 );  

  for ( auto it = index; it != index2; ++it)
     {
        if ( (*it) -> region == region && (*it) -> id == id)
       {
        owner = (*it) -> owner;
        break;
       }
     }
  return true;
}


unsigned CLandRegister::Count(const string &owner) const
{

    
  unsigned int pocitadlo = 0;


  auto index = lower_bound(OwnerSorted . begin(), OwnerSorted . end (), owner, lowerComparator);

  auto index2 = upper_bound(OwnerSorted . begin(), OwnerSorted . end (), owner, upperComparator);
  
  for ( auto it = index; it != index2; ++it)
  {
    string name = (*it) -> owner;
    for ( char& c : name)
    {
        c = tolowercase(c);
    } 

    string name2 = owner;
    for ( char& c : name2 )
    {
        c = tolowercase(c);
    }
    if ( name == name2 )

    pocitadlo++;
  }

  return pocitadlo;
}


CIterator CLandRegister::ListByAddr(void) const
{
  return CIterator( LandVector, false );
}


CIterator CLandRegister::ListByOwner(const string &owner) const
{
    
    vector<Land*> OwnerVectorSorted;

    

   for (auto & i : OwnerSorted)
  {
    string s = i -> owner;  
    for ( char& c : s)
    {
        c = tolowercase(c);
    }

    string s2 = owner;
    for ( char& c : s2)
    {
        c = tolowercase(c);
    }
    if (s == s2) {
      OwnerVectorSorted . push_back(i);
    }
  }

    return CIterator( OwnerVectorSorted, true );
}

CIterator::CIterator(const vector<Land*> pointer, bool flag)
    : counter(0), pointer(pointer), flag(flag) {}

bool CIterator::AtEnd(void) const
{
  return !(counter < pointer . size());
}
void CIterator::Next(void)
{
  counter++;
}
string CIterator::City(void) const
{
  return (pointer . at(counter) -> city);
}
string CIterator::Addr(void) const
{
  return (pointer . at(counter) -> addr);
}

string CIterator::Region(void) const
{
  return (pointer . at(counter) -> region);
}

unsigned CIterator::ID(void) const
{
  return (pointer . at(counter) -> id);
}

string CIterator::Owner(void) const
{
  return (pointer . at(counter) -> owner);
}


void CLandRegister::Print(void) const
{ 
  cout << "==========================CITYADRESS=================" << endl;

  for (const auto &it : LandVector)
  {
    it -> PrintLand();
    cout << endl;
  }

  cout << "==========================REGIONID=================" << endl;
  for (const auto &ti : RegionID)
  {
    ti -> PrintLand();
    cout << endl;
  }

  cout << "==========================OWNER=================" << endl;
  for (const auto &to : OwnerSorted)
  {
    to -> PrintLand();
    cout << endl;
  }
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

int main ( void )
{
  test0();
  test1();
  return 0;
}
#endif /* __PROGTEST__ */
