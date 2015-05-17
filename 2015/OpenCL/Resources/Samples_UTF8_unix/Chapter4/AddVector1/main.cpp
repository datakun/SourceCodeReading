// List4.1

/*
 *  main.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono All rights reserved.
 */

#include <stdio.h>

static const size_t NumElements = 10000;  // 配列の要素数

int
main()
{
    float in1[NumElements];
    float in2[NumElements];
    float out[NumElements];

    // ダミーの値をセット
    for (int i = 0; i < NumElements; i++) {
        in1[i] = float(i) * 10.0f;
        in2[i] = float(i) / 10.0f;
        out[i] = 0.0f;
    }

    // 配列の要素毎の加算 (逐次的に実行される)
    for (int index = 0; index < NumElements; index++) {
        out[index] = in1[index] + in2[index];          // ここを並列化する
    }

    // 結果の一部を表示
    printf("index: in1, in2, out\n");
    for (int i = 0; i < 100; i++) {
        printf("%d: %f, %f, %f\n", i, in1[i], in2[i], out[i]);
    }

    return 0;
}
