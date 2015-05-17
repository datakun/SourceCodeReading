// List6.8

/*
 *  Stopwatch.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

/*
 * 時間測定用Stopwatchクラスの実装（Linux, MacOS用）
 */

#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Stopwatch.h"

/*-------------------------------------------------------------------------
 * コンストラクタ
 */
Stopwatch::Stopwatch()
{
    memset(&mStart, 0, sizeof(timeval)); 
    memset(&mStop, 0, sizeof(timeval));
}

/*-------------------------------------------------------------------------
 * デストラクタ
 */
Stopwatch::~Stopwatch()
{
}

/*-------------------------------------------------------------------------
 * 時間計測を開始する
 */
void
Stopwatch::start()
{
    if (gettimeofday(&mStart, NULL) != 0) {
        perror("gettimeofday()");
        return;
    }
}

/*-------------------------------------------------------------------------
 * 時間計測を終了する
 */
void
Stopwatch::stop()
{
    if (gettimeofday(&mStop, NULL) != 0) {
        perror("gettimeofday()");
        return;
    }
    
    uint64_t diffSec;
    uint64_t diffMicroSec;
    uint64_t startSec      = mStart.tv_sec;
    uint64_t startMicroSec = mStart.tv_usec;
    uint64_t stopSec       = mStop.tv_sec;
    uint64_t stopMicroSec  = mStop.tv_usec;
    
    if (stopMicroSec < startMicroSec) {
        diffSec      = stopSec - startSec - 1;
        diffMicroSec = stopMicroSec + 1000000LL - startMicroSec;
    } else {
        diffSec      = stopSec - startSec;
        diffMicroSec = stopMicroSec - startMicroSec;
    }

    mElapsed = (diffSec * 1000000LL) + diffMicroSec;
}

/*-------------------------------------------------------------------------
 *　結果を表示する
 */
void
Stopwatch::printResult(std::ostream& os) const
{
    os << "total: " << elapseTotalMilliSec() << " (ms)" << std::endl;
    os << "(" << elapseTotalMicroSec() << " (us))" << std::endl;
}

/*-------------------------------------------------------------------------
 * 経過時間をms単位で返す
 */
uint64_t
Stopwatch::elapseTotalMilliSec() const
{
    return ((mElapsed + 500LL) / 1000LL);
}

/*-------------------------------------------------------------------------
 * 経過時間をus単位で返す
 */
uint64_t
Stopwatch::elapseTotalMicroSec() const
{
    return mElapsed;
}
