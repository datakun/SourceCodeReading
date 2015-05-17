// List6.6

/*
 *  Stopwatch.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

/*
 * 時間測定用Stopwatchクラスの実装（MacOS用）
 */
#include <iostream>

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

#include "Stopwatch.h"

/*-------------------------------------------------------------------------
 * コンストラクタ
 */
Stopwatch::Stopwatch()
{
    mStart = 0LL;
    mStop = 0LL;    
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
    mStart = mach_absolute_time();
}

/*-------------------------------------------------------------------------
 * 時間計測を終了する
 */
void
Stopwatch::stop()
{
    mStop = mach_absolute_time();

    uint64_t elapsed = mStop - mStart;
    Nanoseconds nanosec = AbsoluteToNanoseconds(*(AbsoluteTime *) &elapsed);
    mElapsed = *(uint64_t *)&nanosec;
}

/*-------------------------------------------------------------------------
 * 結果を表示する
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
    return ((mElapsed + 500000LL) / 1000000LL);
}

/*-------------------------------------------------------------------------
 * 経過時間をus単位で返す
 */
uint64_t
Stopwatch::elapseTotalMicroSec() const
{
    return ((mElapsed + 500LL) / 1000LL);
}
