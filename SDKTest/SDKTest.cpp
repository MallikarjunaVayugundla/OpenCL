#include "SDKTest.hpp"

const string hw("Hello World!");
cl_int err;

//function that handles the case of OpenCL error
void checkErr(cl_int err, string name)
{
	if(err!= CL_SUCCESS)
		cerr<<"error...damn it! " << err << "error at: "<<name<<endl;
}


//SET UP CONTEXT
void setUp()
{
vector<cl::Platform> platformList;
Platform::get(&platformList);
checkErr(platformList.size()!=0 ? CL_SUCCESS : -1, "getPlatform");
cerr<< "No.of platforms is: "<<platformList.size()<<endl;
string platformVendor;
platformList[0].getInfo((cl_platform_info)CL_PLATFORM_VENDOR, &platformVendor);
cerr<< "Platform is by: "<<platformVendor<<endl;
cl_context_properties contextProperties[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platformList[0])(),0};
Context context(CL_DEVICE_TYPE_ALL, contextProperties,NULL,NULL,&err);
checkErr(err, "context creation");

//CREATE BUFFER TO HOLD THE RESULT OF KERNEL
char* outH=new char[hw.length()*1];
Buffer outCL(context, CL_MEM_WRITE_ONLY|CL_MEM_USE_HOST_PTR,hw.length()+1,outH,&err);
checkErr(err, "buffer creation");

//GETTING DEVICE HANDLE FOR PROGRAM COMPILATION AND KERNEL EXECUTION
vector<cl::Device> devices;
devices = context.getInfo<CL_CONTEXT_DEVICES>();
checkErr(devices.size()>0 ? CL_SUCCESS: -1, "in getting devices");

//LOADING PROGRAM FILE
std::ifstream file("/home/arjun/workspace/SDKTest/kernel.cl");
checkErr(file.is_open() ? CL_SUCCESS:-1, "kernel.cl");
std::string prog(std::istreambuf_iterator<char>(file),
(std::istreambuf_iterator<char>()));
cl::Program::Sources source(1,std::make_pair(prog.c_str(), prog.length()+1));
cl::Program program(context, source);
err = program.build(devices,"");//having a problem here in building the program
checkErr(err, "Program::build()");

//CREATING KERNELS (ENTRY POINTS) TO THE PROGRAM
Kernel kernel(program,"hello", &err);
checkErr(err, "creating kernel");
err = kernel.setArg(0,outCL);
checkErr(err,"setting kernel args");


//CREATING COMMAND QUE
cl::CommandQueue queue(context,devices[0],0,&err);
checkErr(err, "in creating a command queue");
cl::Event event;
err = queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(hw.length()+1),cl::NDRange(1,1),NULL,&event);
checkErr(err,"in creating rangekernel");
event.wait();//to make sure kernel execution is finished before reading result back into host memory
err = queue.enqueueReadBuffer(outCL,CL_TRUE,0,hw.length()+1,outH);
checkErr(err,"in reading bufffer");
cerr<<outH;

}




int main(int argc, char** argv)
{
	setUp();
	return EXIT_SUCCESS;
}



