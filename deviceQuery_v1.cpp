/*
 * deviceQuery.cpp
 *
 *  Created on: Aug 5, 2013
 *      Author: arjun
 */

#include <iostream>
using namespace std;

//OpenCL includes
#include <CL/cl.h>


int main(int argc, char **argv)
{

	//platform details
	cl_uint devicecount=NULL;
	cl_uint NumPlatforms;
	clGetPlatformIDs (0, NULL, &NumPlatforms);

	cl_platform_id* PlatformIDs;
	PlatformIDs = new cl_platform_id[NumPlatforms];
	clGetPlatformIDs(NumPlatforms, PlatformIDs, NULL);

	cl_device_id cdDevice;
	clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &cdDevice, &devicecount);
	
	
	cout << NumPlatforms << endl;
	cout << PlatformIDs << endl;
	cout << cdDevice << endl;
	cout << devicecount << endl;
	return 0;
}
