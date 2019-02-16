#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <fstream>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#else if
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct 
{ 
    WORD   bfType; 
    DWORD  bfSize; 
    WORD   bfReserved1; 
    WORD   bfReserved2; 
    DWORD  bfOffBits; 
}
BITMAPFILEHEADER; 

typedef struct
{
    DWORD  biSize; 
    LONG   biWidth; 
    LONG   biHeight; 
    WORD   biPlanes; 
    WORD   biBitCount; 
    DWORD  biCompression; 
    DWORD  biSizeImage; 
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
}
BITMAPINFOHEADER; 
#endif

unsigned char* LoadBitmap(const char* filename){

  BITMAPFILEHEADER* bmpHeader = nullptr;
  BITMAPINFOHEADER* bmpInfo = nullptr;

  uint8_t* databuffer[2] = {nullptr, nullptr};
  std::ifstream infile(filename, std::ios::binary);
  if(!infile){
    std::cout << "Failed to load file" << std::endl;
    return nullptr;
  }
  databuffer[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
  databuffer[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

  infile.read((char*)databuffer[0], sizeof(BITMAPFILEHEADER));
  infile.read((char*)databuffer[1], sizeof(BITMAPINFOHEADER));

  bmpHeader = (BITMAPFILEHEADER*) databuffer[0];
  bmpInfo   = (BITMAPINFOHEADER*) databuffer[1];

  if(bmpHeader->bfType != 0x4D42)
    {
      std::cout << "File \"" << filename << "\" isn't a bitmap file\n";
      return nullptr;
    }
  unsigned int width = bmpInfo->biWidth;
  unsigned int height = bmpInfo->biHeight;
  int imageSize = width * height * 3;
  unsigned char* buffer = new unsigned char[imageSize];
  infile.seekg(bmpHeader->bfOffBits);
  infile.read((char*)buffer, imageSize);
  return buffer;
}

#endif
