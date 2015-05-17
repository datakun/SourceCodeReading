// List6.4

/*
 *  calc.cl (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

/*------------------------------------------------------------------------
 * 2つのfloat値を加算するカーネル関数
 * パラメータ:
 *     in1 (入力): 加算されるfloat値を保持するバッファへのポインタ
 *     in2 (入力): 加算されるfloat値を保持するバッファへのポインタ
 *     out (出力): 加算された結果を格納するためのバッフへのポインタ
 */
__kernel void
addVector(__global const float *in1,
          __global const float *in2,
          __global float *out)
{
    // グローバルIDの取得
    int index = get_global_id(0);

    // 配列の加算
    out[index] = in1[index] + in2[index];
}
