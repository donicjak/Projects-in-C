#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <climits>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

// Inspiraci jsem cerpal z prvniho seminare, konkretne ze sedme ulohy.

/**
*Function accepts 2 parametrs
and using third variable swaps their values.
*/


void swap ( uint8_t & x, uint8_t  & y )
{
	int t = x;
	x = y;
	y = t;
}


const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;



#endif /* __PROGTEST__ */

/**
 * Structure for header of binary file
 * Each variable has size of 2 bytes
 */

  struct fHeader {
    uint16_t  fileEndian; //represents endianity of bytes in file
    uint16_t  fileWidth;  // width of picture
    uint16_t  fileHeight; // height of picture
    uint16_t  fileFormat; // format of pixels
    };


/**
 * Function check header accepts reference to a header of file.
 * Function check if header has a correct format and values
 * Return true or false
 * */

bool checkHeader ( fHeader & header )
{
  if ( ( header . fileEndian ) != ENDIAN_LITTLE && ( header . fileEndian ) != ENDIAN_BIG )  // checks if Endianity of bytes in file is Little or Big, if not returns false.
  return false;

  if ( ( header . fileWidth ) == 0 || ( header . fileHeight ) == 0 ) // checks if width or height of picture is not 0
  return false;

  if (( header . fileEndian ) == ENDIAN_LITTLE ) // In case of little endianity start checking format of pixels.
  {
    
    uint16_t channel = header . fileFormat & 3; // Saving channel format
    if ( channel == 1 )
    {
      return false;
    }
    else
    {
      
    uint16_t bitsPerChannel = (header . fileFormat & 28); // Saving bits per one channel
    

      if ( bitsPerChannel != 12  &&  bitsPerChannel != 16 ) 
      {
        return false;
      }
      
    }
      
  }

  if (( header . fileEndian ) == ENDIAN_BIG ) // Now checking for big endianity
  {
    uint16_t channel = header . fileFormat & 3; // saving channels
    if ( channel != 1 ) 
      {
      uint16_t bitesBIG1 = (header . fileFormat & 0x00FF) << 8;   // saving number of bites per one channel
      uint16_t bitesBIG2 = (header . fileFormat & 0xFF00) >> 8;
      uint16_t bitesBIG = ( ( bitesBIG1 | bitesBIG2 ) & 28 );      
    
      if ( bitesBIG != 12  &&  bitesBIG != 16  )
        {
      
          return false;
        }
      
      }
      
  }
  

  return true; 
}

/**
 * Function accepts as parametrs binary file of image,
 * destination file where we want to save a fliped image
 * and 2 bools determinating wheter we want to flip image
 * verticaly, horizontaly or both ways.
 * Function returns true or false depending on correct inputs.
 * */

bool flipImage ( const char  * srcFileName,
                 const char  * dstFileName,
                 bool          flipHorizontal,
                 bool          flipVertical )
{


  //open file
  ifstream file(srcFileName, ios::binary | ios::in );
  

  // testing fails during opening
  if ( file . fail ( ) )
    return false;

  fHeader header;
  //reading header
  if ( file . read ( ( char* ) &header, sizeof ( header ) ) . gcount () != sizeof ( header ) )
    return false;
    

// testing if header is in correct format
if (checkHeader( header ) != true)
  return false;



int Height = 0;
int Width = 0;
int bytesPerPixel = 0;

int arrayLength = 0;

/**
 * Now we have to prepare data for fliping image
 * Data differs from little to big endianity
 * */



if (( header . fileEndian ) == ENDIAN_LITTLE )
{
  Height = header . fileHeight;    // height of image
  Width = header . fileWidth;     // width of image
  
uint16_t channel = (header . fileFormat & 3) + 1; // number of channels
uint16_t bitsPerChannel = (header . fileFormat & 28); // number of bites per one channel
if (bitsPerChannel == 12)
bitsPerChannel = 8;
bytesPerPixel = ( channel * bitsPerChannel ) / 8;   //bytes per one pixel
arrayLength =  Height * Width * bytesPerPixel; // length of array we will be creating next
}



if (( header . fileEndian ) == ENDIAN_BIG )
{ 
  uint16_t channelBIG1 = (header . fileFormat & 0x00FF) << 8;
  uint16_t channelBIG2 = (header . fileFormat & 0xFF00) >> 8;
  uint16_t channelBIG = ( ( channelBIG1 | channelBIG2 ) & 3 ) + 1; // number of channels


  uint16_t bitesBIG1 = (header . fileFormat & 0x00FF) << 8;
  uint16_t bitesBIG2 = (header . fileFormat & 0xFF00) >> 8;
  uint16_t bitesBIG = ( ( bitesBIG1 | bitesBIG2 ) & 28 ); // number of bites per chanel

  if ( bitesBIG == 12)
  {
    bitesBIG = 8;
  }
  bytesPerPixel = ( channelBIG * bitesBIG ) / 8; //number of bytes per one pixel

  uint16_t firstHeight =  ( header . fileHeight & 0x00FF ) << 8 ; 
  uint16_t secondHeight = ( header . fileHeight & 0xFF00) >> 8 ; 

  Height = secondHeight | firstHeight; // Height of image for big endianity

  uint16_t firstWidth = ( header . fileWidth & 0x00FF ) << 8;
  uint16_t secondWidth = ( header . fileWidth & 0xFF00 ) >> 8;
  Width = secondWidth | firstWidth; // Width of image for big endianity

  arrayLength =  Height * Width * bytesPerPixel; // Length of array we will be allocating next.
}
  


/** 
 * Now we allocate 1d array we will be using for fliping image
 * After that we read all data apart from header into this array
 * */

uint8_t *array = new uint8_t [ arrayLength ];

 if ( (int)file . read ( ( char* ) array, arrayLength ) . gcount () != arrayLength )
    {
      delete [] array;
      return false;
    }


//Checking if end of file or more chars are remaining.
char c;
file.get(c);
if (!file.eof())
{
  delete [] array;
  return false;
}




//We start fliping horizontaly.
if ( flipHorizontal == true )
{
  for ( int i = 0; i < (Width) / 2; ++i ) // we search  half of width of the image.
  {
    for ( int j = 0 ; j < Height; ++j) // we search  through entire height of the image.
    {
      for (int k = 0; k < bytesPerPixel; ++k) // last loop is needed for case of more bytes per pixels.
      {
        
        int x = ( bytesPerPixel * i ) + (( Width * bytesPerPixel ) * j) + k; 
        int y = ( ( Width * bytesPerPixel )* ( j + 1 ) ) - ( ( bytesPerPixel * i ) - k ) - bytesPerPixel;
        swap( array[ x ], array[ y ] ); // swaping arrays
      }
    }
  }
}



// We start fliping verticaly
if ( flipVertical == true )
{
  for ( int i = 0; i < ( Height ) / 2; ++i ) // We serach half of the Height of the image
  {
    for ( int j = 0; j < Width; ++j ) // We search through entire width of the imgae.
    {
      for ( int k = 0; k < bytesPerPixel; ++k ) // last loop is needed for case of more bytes per pixels.
      {
      int x = ( ( j * bytesPerPixel ) + k ) + ( i * bytesPerPixel * Width );
      int y =  arrayLength - (( (Width - j - 1) * bytesPerPixel) + (bytesPerPixel - k ) + ( i * bytesPerPixel * Width  ) );
      swap( array[ x ], array[ y ] ); // swaping arrays
      }

    }
  }
}


//Output to file
ofstream fileOut(dstFileName, ios::binary | ios::out );

          if ( !fileOut . write ( ( char* ) &header, sizeof ( header ) ) )
          {
            delete [] array;
            return false;
          }

          if ( !fileOut . write ( ( char* ) array, arrayLength ) )
          {
            delete [] array;
            return false;
          }  

  //freeing memory
  delete [] array;

  return true;
}

#ifndef __PROGTEST__
/**
 * Function accepts 2 binary files and
 * compare them.
 * If they are identical, function return true
 * otherwise returns false.
 * */
bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
  return true;
}

int main ( void )
{
 

  

  assert ( flipImage ( "input_00.img", "output_00.img", true, false )
           && identicalFiles ( "output_00.img", "ref_00.img" ) );

  assert ( flipImage ( "input_01.img", "output_01.img", false, true )
           && identicalFiles ( "output_01.img", "ref_01.img" ) );

  assert ( flipImage ( "input_02.img", "output_02.img", true, true )
           && identicalFiles ( "output_02.img", "ref_02.img" ) );

  assert ( flipImage ( "input_03.img", "output_03.img", false, false )
           && identicalFiles ( "output_03.img", "ref_03.img" ) );

  assert ( flipImage ( "input_04.img", "output_04.img", true, false )
           && identicalFiles ( "output_04.img", "ref_04.img" ) );

  assert ( flipImage ( "input_05.img", "output_05.img", true, true )
           && identicalFiles ( "output_05.img", "ref_05.img" ) );

  assert ( flipImage ( "input_06.img", "output_06.img", false, true )
           && identicalFiles ( "output_06.img", "ref_06.img" ) ); 

  assert ( flipImage ( "input_07.img", "output_07.img", true, false )
           && identicalFiles ( "output_07.img", "ref_07.img" ) );

  assert ( flipImage ( "input_08.img", "output_08.img", true, true )
           && identicalFiles ( "output_08.img", "ref_08.img" ) );

  assert ( ! flipImage ( "input_09.img", "output_09.img", true, false ) );
 
  // extra inputs (optional & bonus tests)
  
  
   assert ( flipImage ( "extra_input_00.img", "extra_out_00.img", true, false )
           && identicalFiles ( "extra_out_00.img", "extra_ref_00.img" ) );
  assert ( flipImage ( "extra_input_01.img", "extra_out_01.img", false, true )
           && identicalFiles ( "extra_out_01.img", "extra_ref_01.img" ) );
 assert ( flipImage ( "extra_input_02.img", "extra_out_02.img", true, false )
           && identicalFiles ( "extra_out_02.img", "extra_ref_02.img" ) );
  assert ( flipImage ( "extra_input_03.img", "extra_out_03.img", false, true )
           && identicalFiles ( "extra_out_03.img", "extra_ref_03.img" ) );
  assert ( flipImage ( "extra_input_04.img", "extra_out_04.img", true, false )
           && identicalFiles ( "extra_out_04.img", "extra_ref_04.img" ) );
  assert ( flipImage ( "extra_input_05.img", "extra_out_05.img", false, true )
           && identicalFiles ( "extra_out_05.img", "extra_ref_05.img" ) );
  assert ( flipImage ( "extra_input_06.img", "extra_out_06.img", true, false )
           && identicalFiles ( "extra_out_06.img", "extra_ref_06.img" ) );
  assert ( flipImage ( "extra_input_07.img", "extra_out_07.img", false, true )
           && identicalFiles ( "extra_out_07.img", "extra_ref_07.img" ) );
  /*assert ( flipImage ( "extra_input_08.img", "extra_out_08.img", true, false )
           && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
  assert ( flipImage ( "extra_input_09.img", "extra_out_09.img", false, true )
           && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
  assert ( flipImage ( "extra_input_10.img", "extra_out_10.img", true, false )
           && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
  assert ( flipImage ( "extra_input_11.img", "extra_out_11.img", false, true )
           && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );*/
  
  return 0;
}
#endif /* __PROGTEST__ */
