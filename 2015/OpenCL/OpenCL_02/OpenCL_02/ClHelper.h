#ifndef __CL_HELPER_H__
#define __CL_HELPER_H__

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif //__APPLE__

class ClHelper {
public:
    ClHelper();
    virtual ~ClHelper();

    void preloadProgram(const char* filename);
    cl_context getContext() const {return mContext;}
    cl_device_id getDevice() const {return mDeviceId;}
    cl_command_queue getCommandQueue() const {return mCommandQueue;}
    cl_program getProgram() const {return mProgram;}
    cl_kernel getKernel() const {return mKernel;}

    static void printError(const cl_int err);

private:
    void showBuildLog(const cl_program program, const cl_device_id device) const;
    char* loadProgramSource(const char *filename);

private:
    cl_context       mContext;
    cl_device_id     mDeviceId;
    cl_command_queue mCommandQueue;
    cl_program       mProgram;
    cl_kernel        mKernel;
};

#endif // __CL_HELPER_H__
