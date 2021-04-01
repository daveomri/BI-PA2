/**
  @file CText.hpp

  @author David Omrai
  @version 1.0
  @date 22/05/2019
*/

#pragma once
#include "CFile.hpp"
using namespace std;

/**
      \section CText class
      This class represents text file in filesystem.
*/
class CText :                 public CFile{
  public:
    /**
        Class constructor.

        @param extension File extension.
        @param name File name.
        @param location File location.
        @param creation File creation date and time.
        @param owner File owner.
        @param size File size.
        @returns None
    */
    CText(const string &  name,
          const string &  location,
          const string &  creation,
          const string &  owner,
          unsigned int    size,
          const string &  extension);
    /**
        Class constructor.

        @param pattern Class to copy.
        @returns None.
    */
    CText                  ( const CText & pattern );
    /**
        Method prints file type and name.


        @returns None
    */
    void                   printCFile();
    /**
        Method returns type of class

        @returns Type of class.
    */
    string                 getType() const;
    /**
        Method returns clone of this class

        @returns Pointer to new class.
    */
    CFile *                clone() const;
    /**
        Method returns full name of file, name of file + extension

        @returns String representing full name of file.
    */
    string                getFullName()const;
  protected:
    string                extension;/*!< Name of extension */
};
