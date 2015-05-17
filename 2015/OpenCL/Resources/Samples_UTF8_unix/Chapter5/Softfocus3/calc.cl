// List5.22

/*
 *  calc.cl (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */
 
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
    for (int offsetY = 0; offsetY < 3; offsetY++) {
        for (int offsetX = 0; offsetX < 3; offsetX++) {
            int2 coord = (int2)((x - 1 + offsetX), (y - 1 + offsetY));
            dstValue += read_imagef(src_image, MySampler, coord);
        }
    }
    dstValue /= 9.0f;

    write_imagef(dst_image, (int2)(x, y), dstValue);
}
