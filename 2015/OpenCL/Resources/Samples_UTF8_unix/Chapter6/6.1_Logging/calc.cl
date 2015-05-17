// List6.2

/*
 *  calc.cl (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

/* 出力するメッセージを constant空間に配置しておく */
__constant char Message[] = "Sample message.";
__constant char Msg1[]    = "case 0";
__constant char Msg2[]    = "case 1";
__constant char Msg3[]    = "case 2";
__constant char Msg4[]    = "Unexpected case";

/*------------------------------------------------------------------------
 * ログデータを格納するために用いる構造体
 */
struct __attribute__ ((packed)) logdata {
    int   ival;
    float fval;
    char  msg[128];
};
typedef struct logdata LogDataType;

/*------------------------------------------------------------------------
 * 与えられた文字列をログにセットする(以前にセットされている値は上書きされる)
 * パラメータ:
 *     log   : ログを格納するバッファへのポインタ
 *     index : ログの書き込み位置を指定するためのインデックス
 *     str   : ログへ出力する文字列
 */
void
setMsg(__global LogDataType *log, const int index, __constant char *str)
{
    int i=0;
    while (str[i] != '\0') {
        log[index].msg[i] = str[i];
        i++;
    }
    log[index].msg[i] = '\0';
}

/*------------------------------------------------------------------------
 * 与えられた文字列をログに追加する
 * パラメータ:
 *     log   : ログを格納するバッファへのポインタ
 *     index : ログの書き込み位置を指定するためのインデックス
 *     str   : ログへ出力する文字列
 */
void
appendMsg(__global LogDataType *log, const int index, __constant char *str)
{
    int i = 0;
    while (log[index].msg[i] != '\0') {
        i++;
    }

    /* メッセージの区切りとしてカンマを挿入 */
    log[index].msg[i++] = ',';
    log[index].msg[i++] = ' ';

    int j = 0;
    while (str[j] != '\0') {
        log[index].msg[i] = str[j];
        i++;
        j++;
    }
    log[index].msg[i] = '\0';    
}

/*------------------------------------------------------------------------
 * 2つのfloat値を加算するカーネル関数
 * パラメータ:
 *     in1 : 加算されるfloat値を保持するバッファへのポインタ
 *     in2 : 加算されるfloat値を保持するバッファへのポインタ
 *     out : 加算された結果を格納するためのバッフへのポインタ
 *     log : ログ用バッファへのポインタ
 */
__kernel void
addVector(__global const float *in1,
          __global const float *in2,
          __global float *out,
          __global LogDataType *log)
{
    // グローバルIDの取得
    int index = get_global_id(0);

    // ログを格納するためのインデックスを計算する
    // (この例では、グローバルIDをインデックスとして使用している)
    int logindex = index;

    // 配列の加算
    out[index] = in1[index] + in2[index];

    // 整数値をログに記録
    log[logindex].ival = index;

    // float値をログに記録
    log[logindex].fval = in2[index];

    // 以降のコードは、ログへの文字列出力方法を示すための例
    setMsg(log, logindex, Message);

    switch (index % 5) {
        case 0:
            appendMsg(log, logindex, Msg1); // 実行経路を知るために文字列を出力
            break;
        case 1:
            appendMsg(log, logindex, Msg2); // 同様に文字列を出力
            break;
        case 2:
            appendMsg(log, logindex, Msg3); // 同様に文字列を出力
            break;
        default:
            appendMsg(log, logindex, Msg4); // 同様に文字列を出力
            break;
    }
}
