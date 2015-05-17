// List5.20

/*
 *  main.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include "Bitmap.h"
#include "MyError.h"
#include "ClHelper.h"

static const char* INFILE  = "../../input.bmp";  // 入力bitmapファイル
static const char* OUTFILE = "../../output.bmp"; // 出力bitmapファイル

/*-------------------------------------------------------------------------
 * ソフトフォーカス処理 (OpenCLを使った実装)
 */
static void
softfocusWithOpenCL(unsigned char* dataR,
                    unsigned char* dataG,
                    unsigned char* dataB,
                    const unsigned int width,
                    const unsigned int height)
{
    ClHelper clHelper;
    clHelper.preloadProgram("../../calc.cl");  // カーネルプログラムの読み込み

    unsigned int datasize = width * height;
    unsigned char* origR = new unsigned char[datasize];
    unsigned char* origG = new unsigned char[datasize];
    unsigned char* origB = new unsigned char[datasize];

    // ピクセルデータの保存
    memcpy(origR, dataR, datasize);
    memcpy(origG, dataG, datasize);
    memcpy(origB, dataB, datasize);        

    cl_context       context = clHelper.getContext();      // コンテキストの取得
    cl_command_queue queue   = clHelper.getCommandQueue(); // コマンドキューの取得
    cl_program       program = clHelper.getProgram();      // プログラムの取得
    if (program == (cl_program)0) {
        throw MyError("program bug, program object is not loaded", __FUNCTION__);
    }

    cl_int status;

    // カーネルの作成
    cl_kernel kernel;
    kernel = clCreateKernel(program, "processPixel", &status);
    if (kernel == (cl_kernel)0) {
        printf("clCreateKernel failed\n");
        ClHelper::printError(status);
        throw MyError("failed to create kernel", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （入力R）
    cl_mem inR = clCreateBuffer(context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(cl_uchar) * datasize, origR, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （入力G）
    cl_mem inG = clCreateBuffer(context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(cl_uchar) * datasize, origG, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （入力B）
    cl_mem inB = clCreateBuffer(context,
                                CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                sizeof(cl_uchar) * datasize, origB, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （出力R）
    cl_mem outR = clCreateBuffer(context,
                                 CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(cl_uchar) * datasize, origR, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （出力G）
    cl_mem outG = clCreateBuffer(context,
                                 CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(cl_uchar) * datasize, origG, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // メモリオブジェクトの作成 （出力B）
    cl_mem outB = clCreateBuffer(context,
                                 CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(cl_uchar) * datasize, origB, &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create memory object", __FUNCTION__);
    }

    // カーネル関数引数のセット
    status =  clSetKernelArg(kernel, 0, sizeof(cl_uint), (void *)&width);
    status += clSetKernelArg(kernel, 1, sizeof(inR),  (void *)&inR);
    status += clSetKernelArg(kernel, 2, sizeof(inG),  (void *)&inG);
    status += clSetKernelArg(kernel, 3, sizeof(inB),  (void *)&inB);
    status += clSetKernelArg(kernel, 4, sizeof(outR), (void *)&outR);
    status += clSetKernelArg(kernel, 5, sizeof(outG), (void *)&outG);
    status += clSetKernelArg(kernel, 6, sizeof(outB), (void *)&outB);

    if (status != 0) {
        printf("clSetKernelArg failed\n");
        throw MyError("failed to set kernel arguments.", __FUNCTION__);
    }

    // カーネル実行のリクエスト
    cl_uint work_dim = 2; // x, y
    size_t global_work_size[] = {width - 2, height - 2};

    status = clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                    global_work_size, 0, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("clEnqueueNDRangeKernel failed.", __FUNCTION__);
    }

    // 計算結果の取得 (R)
    status = clEnqueueReadBuffer(queue, outR, CL_TRUE, 0,
                                 sizeof(cl_uchar) * datasize, dataR,
                                 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("clEnqueueReadBuffer failed.", __FUNCTION__);
    }

    // 計算結果の取得 (G)
    status = clEnqueueReadBuffer(queue, outG, CL_TRUE, 0,
                                 sizeof(cl_uchar) * datasize, dataG,
                                 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("clEnqueueReadBuffer failed.", __FUNCTION__);
    }

    // 計算結果の取得 (B)
    status = clEnqueueReadBuffer(queue, outB, CL_TRUE, 0,
                                 sizeof(cl_uchar) * datasize, dataB,
                                 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("clEnqueueReadBuffer failed.", __FUNCTION__);
    }

    // リソースの解放
    clReleaseMemObject(outB);
    clReleaseMemObject(outG);
    clReleaseMemObject(outR);
    clReleaseMemObject(inB);
    clReleaseMemObject(inG);
    clReleaseMemObject(inR);

    delete [] origB;
    delete [] origG;    
    delete [] origR;
}

/*-------------------------------------------------------------------------
 * ソフトフォーカス処理をおこなうプログラム (OpenCLを使った実装)
 */
int main()
{
    try {
        // ビットマップファイルの読み込み
        Bitmap bm;
        bm.readFile(INFILE);

        // ビットマップ情報の取得
        int width  = bm.getWidth();
        int height = bm.getHeight();    
        unsigned const int absHeight = height > 0 ? height : height * -1;
        unsigned const int datasize = width * absHeight;

        // ピクセルバッファのアロケート
        unsigned char* dataR = new unsigned char[datasize];
        unsigned char* dataG = new unsigned char[datasize];
        unsigned char* dataB = new unsigned char[datasize];    

        // ピクセルデータの取得
        bm.getRgbData(dataR, dataG, dataB);    

        // ソフトフォーカス処理
        softfocusWithOpenCL(dataR, dataG, dataB, width, absHeight);

        // 結果のビットマップファイルへの書き出し
        Bitmap outBm;
        outBm.create(width, height, dataR, dataG, dataB);
        outBm.writeFile(OUTFILE);

        delete [] dataB;
        delete [] dataG;
        delete [] dataR;
    } catch (MyError err) {
        // エラー発生
        fprintf(stderr, "Error: %s\n", err.cstr());
        return 1;
    }

    return 0;
}
