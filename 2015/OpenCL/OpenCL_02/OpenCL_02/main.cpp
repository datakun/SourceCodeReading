#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Bitmap.h"
#include "MyError.h"
#include "ClHelper.h"

// TODO: 경로는 적절하게 변경
static const char* INFILE  = "/Users/kimdata/Pictures/input.bmp";
static const char* OUTFILE = "/Users/kimdata/Pictures/output.bmp";

static void
binarizationWithOpenCLImage(unsigned char* srcData,
                         unsigned char* dstData,
                         const unsigned int width,
                         const unsigned int height)
{
    ClHelper clHelper;
    // 커널 로드
    // TODO: 경로는 적절하게 변경
    clHelper.preloadProgram("/Users/kimdata/SourceCodeReading/2015/OpenCL/OpenCL_02/OpenCL_02/binarization.cl");

    // 컨텍스트
    cl_context       context = clHelper.getContext();
    // 커맨드 큐
    cl_command_queue queue   = clHelper.getCommandQueue();
    // 프로그램 오브젝트
    cl_program       program = clHelper.getProgram();
    if (program == (cl_program)0) {
        throw MyError("program bug, program object is not loaded", __FUNCTION__);
    }

    cl_int status;

    cl_image_format format;
    format.image_channel_data_type = CL_UNORM_INT8; // 0-255を[0-1]に正規化
    format.image_channel_order     = CL_BGRA;       // BGRAの順に並んでいる

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

    // 커널 오브젝트 생성
    cl_kernel kernel;
    kernel = clCreateKernel(program, "softFocus", &status);
    if (kernel == (cl_kernel)0) {
        ClHelper::printError(status);
        throw MyError("failed to create kernel", __FUNCTION__);
    }

    // 커널 오브젝트 매개변수 설정
    status += clSetKernelArg(kernel, 0, sizeof(src_mem), (void *)&src_mem);
    status += clSetKernelArg(kernel, 1, sizeof(dst_mem), (void *)&dst_mem);
    if (status != 0) {
        printf("clSetKernelArg failed\n");
        throw MyError("failed to set kernel arguments.", __FUNCTION__);
    }

    // 인덱스 공간 설정
    cl_uint work_dim = 2; // x, y
    size_t global_work_size[] = {width - 2, height - 2};

    status = clEnqueueNDRangeKernel(queue, kernel, work_dim, NULL,
                                    global_work_size, 0, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        ClHelper::printError(status);
        throw MyError("clEnqueueNDRangeKernel failed.", __FUNCTION__);
    }

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

    clReleaseMemObject(dst_mem);
    clReleaseMemObject(src_mem);
}

int main()
{
    bool isOpenCL = false;
    
    if (isOpenCL)
    {
        // OenCL
        try {
            Bitmap bm;
            bm.readFile(INFILE);
            
            const int width  = bm.getWidth();
            const int height = bm.getHeight();
            const int absHeight = height > 0 ? height : height * (-1);
            const int pixelSize = 4;
            const int bufferSize = width * absHeight * pixelSize;
            
            unsigned char* srcData = new unsigned char[bufferSize];
            unsigned char* dstData = new unsigned char[bufferSize];
            
            bm.getBgraData(srcData);
            
            memcpy(dstData, srcData, bufferSize);
            
            binarizationWithOpenCLImage(srcData, dstData, width, absHeight);
            
            Bitmap outBm;
            outBm.createFromBgra(width, height, dstData);
            outBm.writeFile(OUTFILE);
            
            delete [] dstData;
            delete [] srcData;
            
        } catch (MyError err) {
            fprintf(stderr, "Error: %s\n", err.cstr());
            return 1;
        }
    }
    else
    {
        // non-OpenCL
        Bitmap bm;
        bm.readFile(INFILE);
        
        int width  = bm.getWidth();
        int height = bm.getHeight();
        unsigned const int absHeight = height > 0 ? height : height * -1;
        unsigned const int datasize = width * absHeight;
        
        unsigned char* dataR = new unsigned char[datasize];
        unsigned char* dataG = new unsigned char[datasize];
        unsigned char* dataB = new unsigned char[datasize];
        
        bm.getRgbData(dataR, dataG, dataB);
        
        // binarization
        for (int i=0; i<datasize; i++) {
            unsigned char value = (dataR[i] + dataG[i] + dataB[i]) / 3;
            if (value > 128)
            {
                dataR[i] = dataG[i] = dataB[i] = 255;
            }
            else
            {
                dataR[i] = dataG[i] = dataB[i] = 0;
            }
        }
        
        Bitmap outBm;
        outBm.create(width, height, dataR, dataG, dataB);
        outBm.writeFile(OUTFILE);
        
        delete [] dataB;
        delete [] dataG;
        delete [] dataR;
    }

    return 0;
}

