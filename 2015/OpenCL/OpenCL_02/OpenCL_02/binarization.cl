const sampler_t MySampler = CLK_NORMALIZED_COORDS_FALSE |
                            CLK_ADDRESS_NONE |
                            CLK_FILTER_NEAREST;

__kernel void
softFocus(__read_only  image2d_t src_image,
          __write_only image2d_t dst_image)
{
    size_t x = get_global_id(0) + 1;
    size_t y = get_global_id(1) + 1;

    float4 dstValue = (float4)0;
    int2 coord = (int2)(x, y);
    dstValue = read_imagef(src_image, MySampler, coord);
    
    float value = (float)((dstValue.x + dstValue.y + dstValue.z) / (float)3.0);
    // 0.0 = 0, 1.0 = 255;
    if (value > (float)0.5)
    {
        dstValue = (float4)(255, 255, 255, 255);
    }
    else
    {
        dstValue = (float4)(0, 0, 0, 255);
    }

    write_imagef(dst_image, (int2)(x, y), dstValue);
}
