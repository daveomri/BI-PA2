// Omrai David, 2019
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

const uint16_t ENDIAN_LITTLE = 0x4949;
const uint16_t ENDIAN_BIG    = 0x4d4d;

#endif /* __PROGTEST__ */

class Picture{
  /**
    * Structure that the program uses for storing a position of pixels in a pixel array
    * also the program use this for quick sort algorithm
  ***/
  struct pointPix{
    int idPix;          // helpful value for sort algorithm
    size_t fromIn;      // form index
    size_t toIn;        // to index
    pointPix() : idPix(0), fromIn(0), toIn(0) {}
  };
  /**
    * Structure that is used for quick sort algorithm
  ***/
  struct pointMap{
    int idMap;
    pointPix * thereIs;
    pointMap() : idMap(0), thereIs(NULL) {}
  };
// - - - - - image info - - - - - - -
  uint16_t endian;
  uint16_t width;
  uint16_t heigth;
  uint16_t format;
// - - - - - program logic  - - - - -
  int * data;           // binary pixel map
  char * fullData;      // pixel map
  uint32_t lengthIn;    // size of image, in char
  uint16_t bpp;         // bits peer pixel
// - - - - - - - - - - - - - - - - - -
  pointPix * horizP;    // pointer used for flipping the image horizonatally
  pointMap * verticP;   // pointer used for flipping the image vertically
public:
    Picture();

    ~Picture();

    static double                     absVal(double a);

    bool                              setPic(const char * srcFile );

    bool                              testData();

    bool                              testFormat();

    bool                              readData(const char * srcFile);

    bool                              setVar();

    uint16_t                          setVarChoose(const char & a, const char & b);

    bool                              setArray();

    bool                              makePixMap();

    bool                              fillPixMap();

    static bool                       compareHorizontalImage( const pointPix & a, const pointPix & b );

    bool                              flipHorizontalImage();

    static bool                       compareVerticalImage( const pointMap& a, const pointMap & b );

    bool                              flipVerticalImage();

    bool                              outData(const char * dstFile);

    bool                              fillData();

    bool                              writeData(const char * dstFile);

    static uint16_t                   realWidth(uint16_t num );

    void                              fillBitPixMap();
};

/**
  * Constructor, used to set all necessary values
***/
Picture::Picture() :
                    endian(0), width(0), heigth(0), format(0),
                    data(NULL), fullData(NULL), lengthIn(0), bpp(0),
                    horizP(NULL), verticP(NULL)
{
}

/**
  * Destructor, used to delete all allocated data
***/
Picture::~Picture()
{
  if (data != NULL)
    delete [] data;
  if (fullData!=NULL)
    delete [] fullData;
  if (verticP != NULL)
  {
    for (size_t i = 0; i < heigth; i++)
      delete [] verticP[i].thereIs;
    delete [] verticP;
  }
}

/**
  * Method used to decode input data
  * @in[a] first input value
  * @in[b] second input value
  * @return it returns decoded number
***/
uint16_t Picture::setVarChoose( const char & a, const char & b )
{
  return (uint16_t)((((int)((uint8_t)a))<<8) + (int)((uint8_t)b)) ;
}

/**
  * Method compares two pointPix structures
  * @in[a] first input structure to compare
  * @in[b] second input structure to compare
  * @return true if a < b, else false
***/
bool Picture::compareHorizontalImage( const pointPix & a, const pointPix & b )
{
  return (a.idPix < b.idPix);
}

/**
  * Method compares two pointMap structures
  * @in[a] first input structure to compare
  * @in[b] second input structure to compare
  * @return true if a < b, else false
***/
bool Picture::compareVerticalImage( const pointMap& a, const pointMap & b )
{
  return (a.idMap < b.idMap);
}

/**
  * Method returns the value of the last bit in num
  * @in[num] uint16_t number
  * @return uint16_t number
***/
uint16_t Picture::realWidth( uint16_t num )
{
  int tmp = 0;
  while ( num != 1 )
  {
  tmp++;
  num >>= 1;
  }
  num <<= ( tmp + 1 );
  return num;
}

/**
  * Method returns the absollute value of given number
  * @in[a] double number
  * @return absollute double value
***/
double Picture::absVal( double a )
{
  return (a<0)?(a*-1):(a);
}

// - - - - - - -  output data  - - - - - - - - - - -
/**
  * Method creates the output file and fill it with datas
  * @in[dstFile] const char *, name of file to store output data
  * @return false if problems with file occurs, true otherwise
***/
bool Picture::writeData(const char * dstFile)
{
  ofstream ofYouGo(dstFile, ios::binary);

  if ( ! ofYouGo.is_open() )
    return false;

  if ( ! ofYouGo.good() )
    return false;

  if ( ofYouGo.fail())
    return false;

  if ( ! (ofYouGo.write(fullData, lengthIn)) )
    return false;

  ofYouGo.close();

  if (! ofYouGo.good())
    return false;

  if ( ofYouGo.fail())
    return false;

  if ( ofYouGo.is_open())
    return false;

  return true;
}

/**
  * Method generates output data from pixel map
  * @return true if no problems occurs, false otherwise
***/
bool Picture::fillData()
{
  uint16_t dataP = 0;
  size_t verI = 0;
  size_t horI = 0;
  int opMade = 0;
  size_t moveIn = verticP[horI].thereIs[verI].fromIn;

  for (size_t i = 8; i < lengthIn; i++)
  {
    opMade = 0;
    dataP = 0;
    while ( opMade != 8 )
    {
      dataP <<= 1;
      dataP += data[moveIn];
      opMade++;
      if ( moveIn == verticP[horI].thereIs[verI].toIn )
      {
        verI += 1;
        if ( verI == width && ((horI+1) != (size_t)(heigth)) )
        {
          verI = 0;
          horI+=1;
          moveIn = verticP[horI].thereIs[verI].fromIn;
        }
        else if ( verI != width )
        {
          moveIn = verticP[horI].thereIs[verI].fromIn;
        }
      }
      else
        moveIn++;
    }
    fullData[i] = (char)dataP;
  }
  return true;
}

/**
  * Method checks if output data are successfully written
  * @return false if not, true otherwise
***/
bool Picture::outData(const char * dstFile)
{
  if (! fillData())
    return false;

  if ( ! writeData(dstFile) )
    return false;

  return true;
}

// - - - - - - -  input data  - - - - - - - - - - -

/**
  * Method flips pixel map vertically
  * @return true if no problems occurs
***/
bool Picture::flipVerticalImage()
{
  sort(verticP, verticP+heigth, compareVerticalImage);
  return true;
}

/**
  * Method flips pixel map horizonatally
  * @return true if everything is ok
***/
bool Picture::flipHorizontalImage()
{
  for (size_t i = 0; i < heigth; i++ )
  {
    sort(verticP[i].thereIs, verticP[i].thereIs+width, compareHorizontalImage);
  }
  return true;
}

/**
  * Method stores data to pixel map
  * @return true if no problems occurs
***/
bool Picture::fillPixMap()
{
  size_t wai = 0;

  for (size_t vin = 0; vin < heigth; vin++)
  {
    verticP[vin].idMap = (heigth-1)-vin;

    for (size_t lin = 0; lin < width; lin++)
    {
      verticP[vin].thereIs[lin].idPix = (width-1)-lin;
      verticP[vin].thereIs[lin].fromIn = wai;
      wai = wai + bpp;
      verticP[vin].thereIs[lin].toIn = wai-1;
    }
  }
  return true;
}

/**
  * Method creates a pixel map
  * @return false if map couldn't be created, true otherwise
***/
bool Picture::makePixMap()
{
  verticP = new pointMap[heigth];
  if (verticP == NULL)
    return false;
  for (size_t i = 0; i < heigth; i++){
    verticP[i].thereIs = new pointPix[width];
    if (verticP[i].thereIs == NULL)
      return false;
  }
  return true;
}

/**
  * Method reads data from fullData and it appends them into data (binary representation of image)
    and fullData (where I store binary info after this function)
  * @return true if no fail happends, fail otherwise
***/
bool Picture::setArray()
{
  int * bitArr = new int [(int)(((int)(lengthIn)-8)*8)];
  size_t iba = 0;
  for (size_t i = 8; i < lengthIn; i++){
    for (int k = 7; k >= 0; k--){
      bitArr[iba++] = (int)(((int)(fullData[i])>>k)& 1);
    }
  }
  data = bitArr;
  return true;
}

/**
  * Method reads data from fullData and stores it in variables
  * @return false if endian is not right, true otherwise
***/
bool Picture::setVar()
{
  endian = (uint16_t)( (((uint8_t)fullData[0])<<8) + (uint8_t)fullData[1] );

  if ( endian == ENDIAN_LITTLE )
  {
    width   = setVarChoose(fullData[3], fullData[2]);
    heigth  = setVarChoose(fullData[5], fullData[4]);
    format  = setVarChoose(fullData[7], fullData[6]);
  }
  else if ( endian == ENDIAN_BIG )
  {
    width   = setVarChoose(fullData[2], fullData[3]);
    heigth  = setVarChoose(fullData[4], fullData[5]);
    format  = setVarChoose(fullData[6], fullData[7]);
  }
  else
    return false;
  return true;
}

/**
  * Method tests format of proportions from header
  * @return false if data are incorrect, true otherwise
***/
bool Picture::testData()
{
  if ( (width*heigth) == 0 )
    return false;

  const double ALPHACON = 1e-100;

  if ( absVal((((lengthIn-8)*8)/(double)bpp)/(double)width - (double)heigth) > ALPHACON )
    return false;
  return true;
}

/**
  * Method tests format from header
  * @return false if the format is incorrect, true otherwise
***/
bool Picture::testFormat()
{
  switch(format)
  {
//----one channel----
    case 0:   bpp = 1;
              break;
    case 12:  bpp = 8;
              break;
    case 16:  bpp = 16;
              break;
//----three chanels--
    case 2:   bpp = 3;
              break;
    case 14:  bpp = 24;
              break;
    case 18:  bpp = 48;
              break;
//----four chanels---
    case 3:   bpp = 4;
              break;
    case 15:  bpp = 32;
              break;
    case 19:  bpp = 64;
              break;
    default: return false;
  }
  return true;
}

/**
  * Method reads the image, and works with data from it
  * @in[srcFile] name of file
  * @in[fullData] pointer defining what data to read
  * @return true if there is no problem with file, false otherwise
***/
bool Picture::readData(const char * srcFile)
{
  ifstream file (srcFile, ios::binary);

  if (! file.is_open())
    return false;
  if (! file.good())
    return false;
  if( file.fail())
    return false;

  file.seekg(0, file.end);
  lengthIn = file.tellg();
  file.seekg(0);

  if (lengthIn <= 8)
    return false;

  fullData = new char [lengthIn];
  if (fullData == NULL)
    return false;

  if (! file.read(fullData, lengthIn))
    return false;

  file.close();

  if (! file.good())
    return false;
  if ( file.is_open())
    return false;
  if ( file.fail())
    return false;

  return true;
}

/**
  * Method sets all necessary data from input to class Picture
  * @in[srcFile] name of input file
  * @return true if everything goes fine, false otherwise
***/
bool Picture::setPic(const char * srcFile)
{
  if (! readData(srcFile))
    return false;
  if ( ! setVar() )
    return false;

  if (! testFormat())
    return false;

  if(! testData())
    return false;


  if (! setArray() )
    return false;

  if (! makePixMap())
    return false;

  if (! fillPixMap())
    return false;

  return true;
}

/**
  * Main function of this program, from where the program controls everything
  * @in[srcFileName]    name of input file
  * @in[dstFileName]    name of output file
  * @in[flipHorizontal] indicator of horizonatal image flip
  * @in[flipVertical]   second indicator of vertical image flip
  * @return             true if no problems with data or file accure, false otherwise
***/
bool flipImage ( const char  * srcFileName,
                 const char  * dstFileName,
                 bool          flipHorizontal,
                 bool          flipVertical )
{
  Picture pix;
  if (! pix.setPic(srcFileName))
    return false;
  if (flipHorizontal)
    if (! pix.flipHorizontalImage())
      return false;

  if (flipVertical)
    if (! pix.flipVerticalImage())
      return false;

  if (! pix.outData(dstFileName))
    return false;

  return true;
}


#ifndef __PROGTEST__

bool identicalFiles ( const char * fileName1,
                      const char * fileName2 )
{
  ifstream file1 (fileName1, ios::binary);
  ifstream file2 (fileName2, ios::binary);

  if (! file1.is_open())
    return false;
  if (! file1.good())
    return false;
  if (! file2.is_open())
    return false;
  if (! file2.good())
    return false;

  file1.seekg(0, file1.end);
  size_t lengthFile1 = file1.tellg();
  file1.seekg(0);

  file2.seekg(0, file2.end);
  size_t lengthFile2 = file2.tellg();
  file2.seekg(0);

  if (lengthFile1 != lengthFile2)
    return false;

 char * dataFile1 = new char [lengthFile1];
 char * dataFile2 = new char [lengthFile2];

  if ( ! file1.read(dataFile1, lengthFile1))
    return false;

  if (! file2.read(dataFile2, lengthFile2))
    return false;

  file1.close();
  file2.close();

  if (! file1.good())
    return false;
  if (file1.is_open())
    return false;

  if (! file2.good())
    return false;
  if (file2.is_open())
    return false;

  for (size_t i = 0; i<lengthFile1; i++){
    if ( dataFile1[i] != dataFile2[i] )
        return false;
  }
  delete [] dataFile1;
  delete [] dataFile2;

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
           /* // I still don't have program for these asserts
  assert ( flipImage ( "extra_input_08.img", "extra_out_08.img", true, false )
           && identicalFiles ( "extra_out_08.img", "extra_ref_08.img" ) );
  assert ( flipImage ( "extra_input_09.img", "extra_out_09.img", false, true )
           && identicalFiles ( "extra_out_09.img", "extra_ref_09.img" ) );
  assert ( flipImage ( "extra_input_10.img", "extra_out_10.img", true, false )
           && identicalFiles ( "extra_out_10.img", "extra_ref_10.img" ) );
  assert ( flipImage ( "extra_input_11.img", "extra_out_11.img", false, true )
           && identicalFiles ( "extra_out_11.img", "extra_ref_11.img" ) );
  */
  return 0;
}
#endif /* __PROGTEST__ */
