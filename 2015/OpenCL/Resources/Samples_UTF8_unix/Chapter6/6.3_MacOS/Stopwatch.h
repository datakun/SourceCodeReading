// List6.5

/*
 *  Stopwatch.h (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <stdint.h>
#include <iostream>

/*-------------------------------------------------------------------------
 * 時間測定用Stopwatchクラス（MacOS用）
 */
class Stopwatch {
public:
    Stopwatch();
    virtual ~Stopwatch();
    
    void start();
    void stop();
    void printResult(std::ostream& os = std::cout) const;

private:
    uint64_t elapseTotalMilliSec() const;
    uint64_t elapseTotalMicroSec() const;

private:
    uint64_t mStart;
    uint64_t mStop;
    uint64_t mElapsed;
};

#endif // __STOPWATCH_H___
