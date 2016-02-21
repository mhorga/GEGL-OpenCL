static const char *invertgamma =
"__kernel void cl_invertgamma(__global float4 *out,\n"
"                             __global const float4 *in)\n"
"{\n"
"    int id  = get_global_id(0);\n"
"    float4 in_v  = in[id];\n"
"    float4 out_v;\n"
"    out_v.xyz = 1.0f - in_v.xyz;\n"
"    out_v.w = in_v.w;\n"
"    out[id] = out_v;\n"
"}\n"
;