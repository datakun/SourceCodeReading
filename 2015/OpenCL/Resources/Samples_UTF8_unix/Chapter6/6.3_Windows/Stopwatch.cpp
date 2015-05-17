// List6.10

/*
 *  Stopwatch.cpp (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

/*
 * 時間測定用Stopwatchクラスの実装（Windows用）
 */

#include <iostream>
#include <Windows.h>
#include "Stopwatch.h"

/*-------------------------------------------------------------------------
 * コンストラクタ
 */
Stopwatch::Stopwatch()
{
    mStart.QuadPart   = 0L;
    mStop.QuadPart    = 0L;
    mElapsed.QuadPart = 0L;
    mFreq.QuadPart    = 0L;

    if (QueryPerformanceFrequency(&mFreq) == 0) {
        std::cerr << "Error: QueryPerformanceFrequency failed"
                  << std::endl << std::flush;
    }
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
    
    if (!QueryPerformanceCounter(&mStart)) {
        std::cerr << "Error: QueryPerformanceCounter failed"
                  << std::endl << std::flush;
    }
}

/*-------------------------------------------------------------------------
 * 時間計測を終了する
 */
void
Stopwatch::stop()
{
    if (!QueryPerformanceCounter(&mStop)) {
        std::cerr << "Error: QueryPerformanceCounter failed"
                  << std::endl << std::flush;
    }

    mElapsed.QuadPart = mStop.QuadPart - mStart.QuadPart;
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
double
Stopwatch::elapseTotalMilliSec() const
{
    double milli = ((double)mElapsed.QuadPart * 1000.0f) /
                   (double)mFreq.QuadPart;
    return milli;
}

/*-------------------------------------------------------------------------
 * 経過時間をus単位で返す
 */
double
Stopwatch::elapseTotalMicroSec() const
{
    double micro = ((double)mElapsed.QuadPart * 1000000.0f) /
                   (double)mFreq.QuadPart;
    return micro;
}
