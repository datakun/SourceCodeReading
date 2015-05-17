// List4.3 (List4.4, List4.5)

/*
 *  main.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono All rights reserved.
 */

#include <stdio.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

static const int NumElements = 10000000;
static const int MaxDevices  = 10;
static const int MaxLogSize  = 5000;

static float In1[NumElements];
static float In2[NumElements];
static float Out[NumElements];

static void printBuildLog(const cl_program program, const cl_device_id device);
static void printError(const cl_int err);


/*-------------------------------------------------------------------------
 *
 */
int
main()
{
    cl_int status;
    
    // プラットフォームIDを取得する
    cl_platform_id platforms[10];
    cl_uint num_platforms;
    status = clGetPlatformIDs(sizeof(platforms) / sizeof(platforms[0]),
                              platforms,
                              &num_platforms);
    if (status != CL_SUCCESS || num_platforms <= 0) {
        fprintf(stderr, "clGetPlatformIDs failed.\n");
        printError(status);
        return 1;
    }
    
    // 最初の要素として返されたプラットフォームIDを、プロパティにセットする
    cl_context_properties properties[]
    = {CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[0], 0};
    
    // 1. コンテキストの作成
    cl_context context;
    context = clCreateContextFromType(properties,
                                      CL_DEVICE_TYPE_GPU,
                                      NULL,
                                      NULL,
                                      &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateContextFromType failed.\n");
        printError(status);
        return 1;
    }
    
    // 2. コンテキストに含まれるデバイスを取得
    cl_device_id devices[MaxDevices];
    size_t size_return;
    status = clGetContextInfo(context,
                              CL_CONTEXT_DEVICES,
                              sizeof(devices),
                              devices,
                              &size_return);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clGetContextInfo failed.\n");
        printError(status);
        return 2;
    }
    
    // 3. コマンドキューの作成
    cl_command_queue queue;
    queue = clCreateCommandQueue(context,
                                 devices[0],
                                 0,
                                 &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreaetCommandQueue failed.\n");
        printError(status);
        return 3;
    }
    
    // 4. プログラムオブジェクトの作成
    static const char *sources[] = {
        "__kernel void\n\
        addVector(__global const float *in1,\n\
        __global const float *in2,\n\
        __global float *out)\n\
        {\n\
        int index = get_global_id(0);\n\
        out[index] = in1[index] + in2[index];\n\
    }\n"};
    
    cl_program program;
    program = clCreateProgramWithSource(context,
                                        1,
                                        (const char**)&sources,
                                        NULL,
                                        &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateProgramWithSource failed.\n");
        printError(status);
        return 4;
    }
    
    // 5. プログラムのビルド
    status = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clBuildProgram filaed.\n");
        printError(status);
        printBuildLog(program, devices[0]);
        return 5;
    }
    clUnloadCompiler();
    
    // 6. カーネルの作成
    cl_kernel kernel;
    kernel = clCreateKernel(program, "addVector", &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateKernel failed.\n");
        printError(status);
        return 6;
    }
    
    // 7. メモリオブジェクトの作成
    for (int i = 0; i < NumElements; i++) {
        // データをセット
        In1[i] = (float)i * 100.0f;
        In2[i] = (float)i / 100.0f;
        Out[i] = 0.0f;
    }
    
    cl_mem memIn1;
    memIn1 = clCreateBuffer(context,
                            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(cl_float) * NumElements,
                            In1,
                            &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateBuffer for memIn1 failed.\n");
        printError(status);
        return 7;
    }
    
    cl_mem memIn2;
    memIn2 = clCreateBuffer(context,
                            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                            sizeof(cl_float) * NumElements,
                            In2,
                            &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateBuffer for memIn2 failed.\n");
        printError(status);
        return 7;
    }
    
    cl_mem memOut;
    memOut = clCreateBuffer(context,
                            CL_MEM_WRITE_ONLY,
                            sizeof(cl_float) * NumElements,
                            NULL,
                            &status);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clCreateBuffer for memOut failed.\n");
        printError(status);
        return 7;
    }
    
    // 8. カーネル関数引数のセット
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memIn1);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clSetKernelArg for memIn1 failed.\n");
        printError(status);
        return 8;
    }
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memIn2);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clSetKernelArg for memIn2 failed.\n");
        printError(status);
        return 8;
    }
    status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memOut);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clSetKernelArg for memOut failed.\n");
        printError(status);
        return 8;
    }
    
    // 9. カーネル実行のリクエスト
    size_t globalSize[] = {NumElements};
    status = clEnqueueNDRangeKernel(queue,
                                    kernel,
                                    1,
                                    NULL,
                                    globalSize,
                                    0, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clEnqueueNDRangeKernel.\n");
        printError(status);
        return 9;
    }
    
    // 10. 結果の取得
    status = clEnqueueReadBuffer(queue,
                                 memOut,
                                 CL_TRUE,
                                 0,
                                 sizeof(cl_float) * NumElements,
                                 Out, 0, NULL, NULL);
    if (status != CL_SUCCESS) {
        fprintf(stderr, "clEnqueueReadBuffer.\n");
        printError(status);
        return 10;
    }
    
    // 結果の表示（一部）
    printf("(In1, In2, Out)\n");
    for (int i = 0; i < 100; i++) {
        printf("%f, %f, %f (%f)\n", In1[i], In2[i], Out[i], In1[i] + In2[i]);
    }
    
    // 11. リソースの解放
    clReleaseMemObject(memOut);
    clReleaseMemObject(memIn2);
    clReleaseMemObject(memIn1);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    
    return 0;
}

// List.4.4
/*-------------------------------------------------------------------------
 * ビルドログを表示する
 * program: ビルドをおこなったプログラムオブジェクト
 * device:  ビルドのターゲットに用いたデバイスID
 */
static void
printBuildLog(const cl_program program, const cl_device_id device)
{
    cl_int status;
    size_t size_ret;
    
    char buffer[MaxLogSize + 1];
    status = clGetProgramBuildInfo(program,
                                   device,
                                   CL_PROGRAM_BUILD_LOG,
                                   MaxLogSize,
                                   buffer,
                                   &size_ret);
    if (status == CL_SUCCESS) {
        buffer[size_ret] = '\0';
        printf(">>> build log <<<\n");
        printf("%s\n", buffer);
        printf(">>> end of build log <<<\n");
    } else {
        printf("clGetProgramInfo failed.\n");
        printError(status);
    }
}

// List4.5
/*-------------------------------------------------------------------------
 * エラーコードに対応するメッセージを出力する
 * err: エラーコード
 */
static void
printError(const cl_int err)
{
    switch (err) {
        case CL_BUILD_PROGRAM_FAILURE:
            fprintf(stderr, "Program build failed\n");
            break;
        case CL_COMPILER_NOT_AVAILABLE:
            fprintf(stderr, "OpenCL compiler is not available\n");
            break;
        case CL_DEVICE_NOT_AVAILABLE:
            fprintf(stderr, "Device is not available\n");
            break;
        case CL_DEVICE_NOT_FOUND:
            fprintf(stderr, "Device not found\n");
            break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:
            fprintf(stderr, "Image format is not supported\n");
            break;
        case CL_IMAGE_FORMAT_MISMATCH:
            fprintf(stderr, "Image format mismatch\n");
            break;
        case CL_INVALID_ARG_INDEX:
            fprintf(stderr, "Invalid arg index\n");
            break;
        case CL_INVALID_ARG_SIZE:
            fprintf(stderr, "Invalid arg size\n");
            break;
        case CL_INVALID_ARG_VALUE:
            fprintf(stderr, "Invalid arg value\n");
            break;
        case CL_INVALID_BINARY:
            fprintf(stderr, "Invalid binary\n");
            break;
        case CL_INVALID_BUFFER_SIZE:
            fprintf(stderr, "Invalid buffer size\n");
            break;
        case CL_INVALID_BUILD_OPTIONS:
            fprintf(stderr, "Invalid build options\n");
            break;
        case CL_INVALID_COMMAND_QUEUE:
            fprintf(stderr, "Invalid command queue\n");
            break;
        case CL_INVALID_CONTEXT:
            fprintf(stderr, "Invalid context\n");
            break;
        case CL_INVALID_DEVICE:
            fprintf(stderr, "Invalid device\n");
            break;
        case CL_INVALID_DEVICE_TYPE:
            fprintf(stderr, "Invalid device type\n");
            break;
        case CL_INVALID_EVENT:
            fprintf(stderr, "Invalid event\n");
            break;
        case CL_INVALID_EVENT_WAIT_LIST:
            fprintf(stderr, "Invalid event wait list\n");
            break;
        case CL_INVALID_GL_OBJECT:
            fprintf(stderr, "Invalid OpenGL object\n");
            break;
        case CL_INVALID_GLOBAL_OFFSET:
            fprintf(stderr, "Invalid global offset\n");
            break;
        case CL_INVALID_HOST_PTR:
            fprintf(stderr, "Invalid host pointer\n");
            break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
            fprintf(stderr, "Invalid image format descriptor\n");
            break;
        case CL_INVALID_IMAGE_SIZE:
            fprintf(stderr, "Invalid image size\n");
            break;
        case CL_INVALID_KERNEL:
            fprintf(stderr, "Invalid kernel\n");
            break;
        case CL_INVALID_KERNEL_ARGS:
            fprintf(stderr, "Invalid kernel args\n");
            break;
        case CL_INVALID_KERNEL_DEFINITION:
            fprintf(stderr, "Invalid kernel definition\n");
            break;
        case CL_INVALID_KERNEL_NAME:
            fprintf(stderr, "Invalid kernel name\n");
            break;
        case CL_INVALID_MEM_OBJECT:
            fprintf(stderr, "Invalid memory object\n");
            break;
        case CL_INVALID_MIP_LEVEL:
            fprintf(stderr, "Invalid MIP level\n");
            break;
        case CL_INVALID_OPERATION:
            fprintf(stderr, "Invalid operation\n");
            break;
        case CL_INVALID_PLATFORM:
            fprintf(stderr, "Invalid platform\n");
            break;
        case CL_INVALID_PROGRAM:
            fprintf(stderr, "invalid program\n");
            break;
        case CL_INVALID_PROGRAM_EXECUTABLE:
            fprintf(stderr, "invalid program executable\n");
            break;
        case CL_INVALID_QUEUE_PROPERTIES:
            fprintf(stderr, "invalid queue properties\n");
            break;
        case CL_INVALID_SAMPLER:
            fprintf(stderr, "invalid sampler\n");
            break;
        case CL_INVALID_VALUE:
            fprintf(stderr, "invalid value\n");
            break;
        case CL_INVALID_WORK_DIMENSION:
            fprintf(stderr, "invalid work dimension\n");
            break;
        case CL_INVALID_WORK_GROUP_SIZE:
            fprintf(stderr, "Invalid work group size\n");
            break;
        case CL_INVALID_WORK_ITEM_SIZE:
            fprintf(stderr, "Invalid work item size\n");
            break;
        case CL_MAP_FAILURE:
            fprintf(stderr, "Memory mapping failed\n");
            break;
        case CL_MEM_COPY_OVERLAP:
            fprintf(stderr, "Copying overlapped memory address\n");
            break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:
            fprintf(stderr, "Memory object allocation failure\n");
            break;
        case CL_OUT_OF_HOST_MEMORY:
            fprintf(stderr, "Out of host memory\n");
            break;
        case CL_OUT_OF_RESOURCES:
            fprintf(stderr, "Out of resources\n");
            break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:
            fprintf(stderr, "Profiling info is not available\n");
            break;
        case CL_SUCCESS:
            fprintf(stderr, "Succeeded\n");
            break;
        default:
            fprintf(stderr, "Unknown error code: %d\n", err);
            break;
    }
}
