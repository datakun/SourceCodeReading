// List6.7

/*
 *  Stopwatch.h (rev1.1, 28.Nov.2010)
 *  Copyright 2010 Takashi Okuzono. All rights reserved.
 */

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include <stdint.h>
#include <sys/time.h>
#include <iostream>

/*-------------------------------------------------------------------------
 * Stopwatchクラス（Linux, MacOS用）
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
    timeval mStart;
    timeval mStop;
    uint64_t mElapsed;    // 経過時間 (us)
};

#endif // __STOPWATCH_H__
