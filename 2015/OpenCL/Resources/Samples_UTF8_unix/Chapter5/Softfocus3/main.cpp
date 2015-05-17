// List5.21

/*
 *  main.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Bitmap.h"
#include "MyError.h"
#include "ClHelper.h"

static const char* INFILE  = "../../input.bmp";  // 入力bitmapファイル
static const char* OUTFILE = "../../output.bmp"; // 出力bitmapファイル
/*-------------------------------------------------------------------------
 * ソフトフォーカス処理をおこなう（OpenCLを使った実装, イメージオブジェクトを使う)
 */
static void
softfocusWithOpenCLImage(unsigned char* srcData,
                         unsigned char* dstData,
                         const unsigned int width,
                         const unsigned int height)
{
    ClHelper clHelper;
    clHelper.preloadProgram("../../calc.cl");  // カーネルプログラムの読み込み

    cl_context       context = clHelper.getContext();      // コンテキストの取得
    cl_command_queue queue   = clHelper.getCommandQueue(); // コマンドキューの取得
    cl_program       program = clHelper.getProgram();      // プログラムの取得
    if (program == (cl_program)0) {
        throw MyError("program bug, program object is not loaded", __FUNCTION__);
    }

    cl_int status;

    // イメージフォーマットの定義
    cl_image_format format;
    format.image_channel_data_type = CL_UNORM_INT8; // 0-255を[0-1]に正規化
    format.image_channel_order     = CL_BGRA;       // BGRAの順に並んでいる

    // 元のイメージデータを保持するイメージオブジェクトの作成
    cl_mem src_mem = clCreateImage2D(context,
                                     CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                                     &format,
                                     width, height,
                                     0,
                                     srcData,
                                     &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create image memory object 1", __FUNCTION__);
    }

    // 結果を保持するイメージオブジェクトの作成
    cl_mem dst_mem = clCreateImage2D(context,
                                     CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR,
                                     &format,
                                     width, height,
                                     0,
                                     dstData,
                                     &status);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to create image memory object 2", __FUNCTION__);
    }

    // カーネルの作成
    cl_kernel kernel;
    kernel = clCreateKernel(program, "softFocus", &status);
    if (kernel == (cl_kernel)0) {
        ClHelper::printError(status);
        throw MyError("failed to create kernel", __FUNCTION__);
    }

    // カーネル関数引数のセット
    status += clSetKernelArg(kernel, 0, sizeof(src_mem), (void *)&src_mem);
    status += clSetKernelArg(kernel, 1, sizeof(dst_mem), (void *)&dst_mem);
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

    // 画像データの取得
    const size_t origin[] = {0, 0, 0};
    const size_t region[] = {width, height, 1};
    status = clEnqueueReadImage(queue,
                                dst_mem,
                                CL_TRUE,
                                origin,
                                region,
                                0, 0,
                                dstData,
                                NULL, 0, NULL);

    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("failed to read image buffer", __FUNCTION__);
    }
                                
    // リソースの解放
    clReleaseMemObject(dst_mem);
    clReleaseMemObject(src_mem);
}

/*-------------------------------------------------------------------------
 * ソフトフォーカス処理をおこなうプログラム (イメージオブジェクトを使用)
 */
int main()
{
    try {
        // ビットマップファイルを読み込む
        Bitmap bm;
        bm.readFile(INFILE);

        // BGRAデータのアロケート
        const int width  = bm.getWidth();
        const int height = bm.getHeight();
        const int absHeight = height > 0 ? height : height * (-1);
        const int pixelSize = 4;        
        const int bufferSize = width * absHeight * pixelSize;

        unsigned char* srcData = new unsigned char[bufferSize];
        unsigned char* dstData = new unsigned char[bufferSize];

        // BGRAの順に並んだ、ビットマップデータを取得する
        bm.getBgraData(srcData);

        // 処理後のイメージデータを格納するバッファに元データをコピーしておく
        memcpy(dstData, srcData, bufferSize);

        // ソフトフォーカス処理
        softfocusWithOpenCLImage(srcData, dstData, width, absHeight);

        // 結果のビットマップファイルへの書き出し
        Bitmap outBm;
        outBm.createFromBgra(width, height, dstData);
        outBm.writeFile(OUTFILE);

        delete [] dstData;
        delete [] srcData;

    } catch (MyError err) {
        // エラー発生
        fprintf(stderr, "Error: %s\n", err.cstr());
        return 1;
    }

    return 0;
}
