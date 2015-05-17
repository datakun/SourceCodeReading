// List6.9

/*
 *  Stopwatch.h (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <iostream>
#include <Windows.h>

/*-------------------------------------------------------------------------
 * Stopwatchクラス （Windows用）
 */
class Stopwatch {
public:
    Stopwatch();
    virtual ~Stopwatch();
    
    void start();
    void stop();
    void printResult(std::ostream& os = std::cout) const;

private:
    double elapseTotalMilliSec() const;
    double elapseTotalMicroSec() const;

private:
    LARGE_INTEGER mStart;
    LARGE_INTEGER mStop;
    LARGE_INTEGER mElapsed;
    LARGE_INTEGER mFreq;
};

#endif // __STOPWATCH_H__
