// List5.18

/*
 *  main.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#include <string.h>
#include "Bitmap.h"

static const char* INFILE  = "../../input.bmp";  // 入力ビットマップファイル
static const char* OUTFILE = "../../output.bmp"; // 出力ビットマップファイル

template<class T> static T min(T a, T b);
template<class T> static T max(T a, T b);
static unsigned char convert_uchar_sat(int val);

/*-------------------------------------------------------------------------
 * x, yで指定された画素についてソフトフォーカス処理をおこなう.
 *
 * x, y                           : 処理対象となる画素
 * width                          : 画面の幅
 * origDataR, origDataG, origDataB: 処理前のデータ
 * dataR, dataG, dataB            : 処理後のデータ
 */
static void processPixel(const int x, const int y,
                         const unsigned int width,
                         const unsigned char* orgDataR,
                         const unsigned char* orgDataG,
                         const unsigned char* orgDataB,
                         unsigned char* dataR,
                         unsigned char* dataG,
                         unsigned char* dataB)
{
    int line0 = width * (y + 0);
    int line1 = width * (y + 1);
    int line2 = width * (y + 2);

    // red データの処理
    int intValR = (int)(  orgDataR[line0 + x + 0]
                        + orgDataR[line0 + x + 1]
                        + orgDataR[line0 + x + 2]
                        + orgDataR[line1 + x + 0]
                        + orgDataR[line1 + x + 1]
                        + orgDataR[line1 + x + 2]
                        + orgDataR[line2 + x + 0]
                        + orgDataR[line2 + x + 1]
                        + orgDataR[line2 + x + 2]);
    intValR /= 9;
    dataR[line1 + x + 1] = convert_uchar_sat(intValR);

    // green データの処理
    int intValG = (int)(  orgDataG[line0 + x + 0]
                        + orgDataG[line0 + x + 1]
                        + orgDataG[line0 + x + 2]
                        + orgDataG[line1 + x + 0]
                        + orgDataG[line1 + x + 1]
                        + orgDataG[line1 + x + 2]
                        + orgDataG[line2 + x + 0]
                        + orgDataG[line2 + x + 1]
                        + orgDataG[line2 + x + 2]);
    intValG /= 9;
    dataG[line1 + x + 1] = convert_uchar_sat(intValG);

    // blue データの処理
    int intValB = (int)(  orgDataB[line0 + x + 0]
                        + orgDataB[line0 + x + 1]
                        + orgDataB[line0 + x + 2]
                        + orgDataB[line1 + x + 0]
                        + orgDataB[line1 + x + 1]
                        + orgDataB[line1 + x + 2]
                        + orgDataB[line2 + x + 0]
                        + orgDataB[line2 + x + 1]
                        + orgDataB[line2 + x + 2]);
    intValB /= 9;
    dataB[line1 + x + 1] = convert_uchar_sat(intValB);
}

/*-------------------------------------------------------------------------
 * ソフトフォーカス処理 (OpenCLを使わない実装)
 *
 * dataR, dataG, dataB: 処理対象となる画像データ (R, G, B).
 * width:  画像の幅
 * height: 画像の高さ
 */
static void softfocus(unsigned char* dataR,
                      unsigned char* dataG,
                      unsigned char* dataB,
                      const unsigned int width,
                      const unsigned int height)
{
    unsigned int datasize = width * height;
    unsigned char* origR = new unsigned char[datasize];
    unsigned char* origG = new unsigned char[datasize];
    unsigned char* origB = new unsigned char[datasize];

    // 元データをorigR, origG, origBとして保持する.
    memcpy(origR, dataR, datasize);
    memcpy(origG, dataG, datasize);
    memcpy(origB, dataB, datasize);

    // (1, 1) から (height - 1, width - 1) までのピクセルを処理する
    for (int y = 0; y < height - 2; y++) {  
        for (int x = 0; x < width - 2; x++) {
            processPixel(x, y, width, origR, origG, origB, dataR, dataG, dataB);
        }
    }

    delete [] origB;
    delete [] origG;
    delete [] origR;
}

/*-------------------------------------------------------------------------
 * ソフトフォーカス処理をおこなうプログラム
 */
int main()
{
    // ビットマップファイルの読み込み
    Bitmap bm;
    bm.readFile(INFILE);

    // ビットマップ情報の取得
    int width  = bm.getWidth();
    int height = bm.getHeight();    
    unsigned const int absHeight = height > 0 ? height : height * -1;
    unsigned const int datasize = width * absHeight;

    // データを格納する配列を確保
    unsigned char* dataR = new unsigned char[datasize];
    unsigned char* dataG = new unsigned char[datasize];
    unsigned char* dataB = new unsigned char[datasize];

    // ピクセルデータの取得
    bm.getRgbData(dataR, dataG, dataB);    

    // ソフトフォーカス処理
    softfocus(dataR, dataG, dataB, width, absHeight);

    // ビットマップファイルへの書き出し
    Bitmap outBm;
    outBm.create(width, height, dataR, dataG, dataB);
    outBm.writeFile(OUTFILE);

    delete [] dataB;
    delete [] dataG;
    delete [] dataR;

    return 0;
}

/*-------------------------------------------------------------------------
 * a, b の小さい方を返す
 */
template<class T>
static T
min(T a, T b)
{
    return ( a < b ) ? a : b;
}

/*-------------------------------------------------------------------------
 * a, b の大きい方を返す
 */
template<class T>
static T
max(T a, T b)
{
    return ( a > b ) ? a : b;
}

/*-------------------------------------------------------------------------
 * 飽和演算付きでunsigned charの値に変換する
 */
static unsigned char
convert_uchar_sat(int val)
{
    return max(0, min(val, 0xff));
}
