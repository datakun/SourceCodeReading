//
//  main.cpp
//  OpenCL_01
//
//  Created by kimdata on 2015. 3. 19..
//  Copyright (c) 2015년 kimdata. All rights reserved.
//

#include <stdio.h>

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#else
#include <CL/cl.h>
#endif // __APPLE__

static void printPlatformInfo(const cl_platform_id platform_id);

int main(int argc, const char * argv[])
{
    cl_platform_id platforms[10];
    cl_uint num_platforms;
    cl_int status;
    
    status = clGetPlatformIDs(sizeof(platforms) / sizeof(*platforms), platforms, &num_platforms);
    
    if (status != CL_SUCCESS)
    {
        fprintf(stderr, "clGetPlatformIds failed with status %d\n", status);
        
        return 1;
    }
    
    printf("Number of platform(s) : %d\n", num_platforms);
    for (int i=0; i<(int)num_platforms; i++)
    {
        printPlatformInfo(platforms[i]);
    }
    
    return 0;
}

static void printPlatformInfo(const cl_platform_id platform_id)
{
    char buffer[1024];
    cl_int status;
    
    status = clGetPlatformInfo(platform_id, CL_PLATFORM_PROFILE, sizeof(buffer)-1, buffer, NULL);
    
    printf("Platform profile : ");
    if (status == CL_SUCCESS)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error: clGetPlatformInfo failed with status %d\n", status);
    }
    
    status = clGetPlatformInfo(platform_id, CL_PLATFORM_VERSION, sizeof(buffer)-1, buffer, NULL);
    
    printf("Platform version    : ");
    if (status == CL_SUCCESS)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error: clGetPlatformInfo failed with status %d\n", status);
    }
    
    status = clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, sizeof(buffer)-1, buffer, NULL);
    
    printf("Platform name       : ");
    if (status == CL_SUCCESS)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error: clGetPlatformInfo failed with status %d\n", status);
    }
    
    status = clGetPlatformInfo(platform_id, CL_PLATFORM_VENDOR, sizeof(buffer)-1, buffer, NULL);
    
    printf("Platform vendor     : ");
    if (status == CL_SUCCESS)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error: clGetPlatformInfo failed with status %d\n", status);
    }
    
    status = clGetPlatformInfo(platform_id, CL_PLATFORM_EXTENSIONS, sizeof(buffer)-1, buffer, NULL);
    
    printf("Platform extensions : ");
    if (status == CL_SUCCESS)
    {
        printf("%s\n", buffer);
    }
    else
    {
        printf("Error: clGetPlatformInfo failed with status %d\n", status);
    }
}