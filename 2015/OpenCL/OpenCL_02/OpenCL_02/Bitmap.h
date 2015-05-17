#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdio.h>
#include "bitmapspec.h"

class Bitmap {
public:
    Bitmap();
    virtual ~Bitmap();

    void create(const int width, const int height,
                const float* dataR,
                const float* dataG,
                const float* dataB);
    void create(const int width, const int height,
                const unsigned char* dataR,
                const unsigned char* dataG,
                const unsigned char* dataB);
    void createFromBgra(const int width, const int height,
                        const unsigned char* dataBgra);

    void readFile(const char *path);
    void writeFile(const char *path) const;

    void getRgbData(unsigned char* dataR,
                    unsigned char* dataG,
                    unsigned char* dataB) const;
    void getBgraData(unsigned char* dataBgra) const;

    int getWidth() const {return mWidth;}
    int getHeight() const {return mHeight;}
    int getDataSize() const {return mDataSize;}
    int getTotalPixels() const {return mTotalPixels;}

private:
    void initialize(const int width, const int height);

    void fillData(const float* dataR,
                  const float* dataG,
                  const float* dataB);
    void fillData(const unsigned char* dataR,
                  const unsigned char* dataG,
                  const unsigned char* dataB);
    void fillBgraData(const unsigned char *dataBgra);

    int readFileHeader(FILE *ifp);
    int readInfoHeader(FILE *ifp);
    int readBitmapData(FILE *ifp);
    int writeFileHeader(FILE *ofp) const;
    int writeInfoHeader(FILE *ofp) const;
    int writeBitmapData(FILE *ofp) const;
    void initFileHeader();
    void initInfoHeader();

    unsigned char getPixelValue(const float val) const;
    unsigned char getPixelValue(const int val) const;

private:
    BitmapFileHeader mFileHeader;
    BitmapInfoHeader mInfoHeader;

    int mWidth;
    int mHeight;
    unsigned char *mData;
    size_t mDataSize;
    size_t mTotalPixels;
};

#endif // __BITMAP_H__
