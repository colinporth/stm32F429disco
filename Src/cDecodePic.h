// cDecodePic.h - read .gif,.png,.jpg,.bmp file
#pragma once
#include "cPngPic.h"
#include "cGifPic.h"
#include "cJpegPic.h"

class cDecodePic : public iPic {
public:
  cDecodePic()  {}
  virtual ~cDecodePic() {}
  void* operator new (std::size_t size) { return smallMalloc (size, "cDecodePic"); }
  void operator delete (void *ptr) { smallFree (ptr); }

  // iPic
  virtual uint16_t getWidth() { return mWidth; }
  virtual uint16_t getHeight() { return mHeight; }
  virtual uint16_t getComponents() { return mComponents; }
  virtual uint8_t* getPic() { return mPic; }
  //{{{
  virtual void setPic (uint8_t* buffer, int size) {
    if ((buffer[0] == 'G') && (buffer[1] == 'I') && (buffer[2] == 'F'))  {
      //{{{  load gif
      cGifPic gif;
      gif.setPic (buffer, size);
      if (gif.readHeader() == cGifPic::GIF_OK) {
        mWidth = gif.getWidth();
        mHeight = gif.getHeight();
        mComponents = gif.getComponents();
        if (gif.decodeBody (0) == cGifPic::GIF_OK)
          mPic = gif.getPic();
        }
      }
      //}}}
    else if ((buffer[0] == 0x89) && (buffer[1] == 'P') && (buffer[2] == 'N') && (buffer[3] == 'G'))  {
      //{{{  load png
      cPngPic png (buffer);
      if (png.readHeader()) {
        mWidth = png.getWidth();
        mHeight = png.getHeight();
        mComponents = png.getComponents();
        mPic = png.decodeBody();
        }
      }
      //}}}
    else if ((buffer[0] == 0xFF) && (buffer[1] == 0xD8)) {
      //{{{  load jpeg
      cJpegPic jpeg (buffer);
      if (jpeg.readHeader()) {
        mWidth = jpeg.getWidth();
        mHeight = jpeg.getHeight();
        mComponents = jpeg.getComponents();
        mPic = jpeg.decodeBody (0, 3);
        }
      }
      //}}}
    else if ((buffer[0] == 'B') && (buffer[1] == 'M')) {
      //{{{  load bmp
      mWidth = buffer[0x12];
      mHeight = buffer[0x16];
      mComponents = 3;
      mPic = buffer + 54;
      }
      //}}}
    }
  //}}}

  //{{{
  bool setFileName (std::string fileName) {

  #ifdef WIN32
    HANDLE mFileHandle = CreateFileA (fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    int mFileSize = (int)GetFileSize (mFileHandle, NULL);
    uint8_t* mSrcBuffer = (uint8_t*)MapViewOfFile (CreateFileMapping (mFileHandle, NULL, PAGE_READONLY, 0, 0, NULL), FILE_MAP_READ, 0, 0, 0);
    setPic (mSrcBuffer, mFileSize);
    UnmapViewOfFile (mSrcBuffer);
    CloseHandle (mFileHandle);
    return true;
  #else
    FILE* file = fopen (fileName.c_str(), "rb");
    if (file) {
      //printf ("loading %s\n", fileName.c_str());
      //int fileSize = ;
      //uint8_t* mSrcBuffer = (uint8_t*)bigMalloc (size, "fileBuf");
      //fread (mSrcBuffer, 1, (unsigned long)0xFFFFFFFF, file);
      //fclose (file);
      //setPic (mSrcBuffer, mFileSize);
      //bigFree (mSrcBuffer);
      //mSrcBuffer = nullptr;
      return true;
      }
    return false;
  #endif
    }
  //}}}

private:
  uint16_t mWidth = 0;
  uint16_t mHeight = 0;
  uint16_t mComponents = 0;
  uint8_t* mPic = nullptr;
  };
