
__kernel void cl_invertgamma(__global float4 *out,
                             __global const float4 *in)
{
    int id  = get_global_id(0);
    float4 in_v  = in[id];
    float4 out_v;
    out_v.xyz = 1.0f - in_v.xyz;
    out_v.w = in_v.w;
    out[id] = out_v;
}
