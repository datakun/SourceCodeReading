// List5.19
/*
 *  calc.cl (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

__kernel void
processPixel(const unsigned int width,
             __global const unsigned char* orgDataR,
             __global const unsigned char* orgDataG,
             __global const unsigned char* orgDataB,
             __global unsigned char* dataR,
             __global unsigned char* dataG,
             __global unsigned char* dataB)
{
    size_t x = get_global_id(0);
    size_t y = get_global_id(1);

    int line0 = width * (y + 0);
    int line1 = width * (y + 1);
    int line2 = width * (y + 2);

    int intValR = (int)(  orgDataR[line0 + x + 0]
                        + orgDataR[line0 + x + 1]
                        + orgDataR[line0 + x + 2]
                        + orgDataR[line1 + x + 0]
                        + orgDataR[line1 + x + 1]
                        + orgDataR[line1 + x + 2]
                        + orgDataR[line2 + x + 0]
                        + orgDataR[line2 + x + 1]
                        + orgDataR[line2 + x + 2]);
    intValR /= 9;
    dataR[line1 + x + 1] = convert_uchar_sat(intValR);

    int intValG = (int)(  orgDataG[line0 + x + 0]
                        + orgDataG[line0 + x + 1]
                        + orgDataG[line0 + x + 2]
                        + orgDataG[line1 + x + 0]
                        + orgDataG[line1 + x + 1]
                        + orgDataG[line1 + x + 2]
                        + orgDataG[line2 + x + 0]
                        + orgDataG[line2 + x + 1]
                        + orgDataG[line2 + x + 2]);
    intValG /= 9;
    dataG[line1 + x + 1] = convert_uchar_sat(intValG);
    int intValB = (int)(  orgDataB[line0 + x + 0]
                        + orgDataB[line0 + x + 1]
                        + orgDataB[line0 + x + 2]
                        + orgDataB[line1 + x + 0]
                        + orgDataB[line1 + x + 1]
                        + orgDataB[line1 + x + 2]
                        + orgDataB[line2 + x + 0]
                        + orgDataB[line2 + x + 1]
                        + orgDataB[line2 + x + 2]);
    intValB /= 9;
    dataB[line1 + x + 1] = convert_uchar_sat(intValB);
}
