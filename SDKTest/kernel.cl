#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable
__constant char hw[] = "Hello World\n";
__kernel void hello(__global char *out)
{
size_t id= get_global_id(0);
out[id]= hw[id];
}