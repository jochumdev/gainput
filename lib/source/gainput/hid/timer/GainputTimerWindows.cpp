/*
 * Copyright (c) 2017-2022 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma comment(lib, "winmm.lib")  // Need to link to this library otherwise timeGetTime has no reference

#include <stdint.h>
#include <time.h>

#include "GainputTimer.h"
// clang-format off
#include <windows.h>
#include <timeapi.h>
// clang-format on
#include <assert.h>
#include <process.h>  // _beginthreadex

#define INIT_CALL_ONCE_GUARD INIT_ONCE_STATIC_INIT
typedef INIT_ONCE CallOnceGuard;

/**
 * @brief Function pointer for function that will be called only once
 */
typedef void (*CallOnceFn)(void);

/*
 * Function pointer can't be casted to void*,
 * so we wrap it with a struct
 */
typedef struct CallOnceFnWrapper {
    CallOnceFn fn;
} CallOnceFnWrapper;

static BOOL callOnceImpl(PINIT_ONCE initOnce, PVOID pWrapper, PVOID* ppContext) {
    CallOnceFn fn = ((CallOnceFnWrapper*)pWrapper)->fn;
    if (fn) fn();

    return TRUE;
}

void callOnce(CallOnceGuard* pGuard, CallOnceFn fn) {
    CallOnceFnWrapper wrapper = {fn};
    InitOnceExecuteOnce(pGuard, callOnceImpl, &wrapper, NULL);
}

/************************************************************************/
// Time Related Functions
/************************************************************************/
uint32_t getSystemTime() { return (uint32_t)timeGetTime(); }

uint32_t getTimeSinceStart() { return (uint32_t)time(NULL); }

static CallOnceGuard timeInitGuard   = INIT_CALL_ONCE_GUARD;
static int64_t highResTimerFrequency = 0;
static int64_t highResTimerStart     = 0;

static bool alwaysSimpleMulDiv = false;
static int64_t timerToUSecMul  = 0;
static int64_t timerToUSecDiv  = 0;

static int64_t timeGCD(int64_t a, int64_t b) { return (a == 0) ? b : timeGCD(b % a, a); }

static void initTime(void) {
    LARGE_INTEGER frequency;
    BOOL qpcResult = QueryPerformanceFrequency(&frequency);
    assert(qpcResult);
    if (qpcResult)  //-V547
    {
        highResTimerFrequency = frequency.QuadPart;
    } else {
        highResTimerFrequency = 1000LL;
    }

    LARGE_INTEGER counter;
    qpcResult = QueryPerformanceCounter(&counter);
    assert(qpcResult);
    if (qpcResult)  //-V547
    {
        highResTimerStart = counter.QuadPart;
    } else {
        highResTimerStart = 0;
    }

    timerToUSecMul        = (int64_t)1e6;  // 1 second = 1,000,000 microseconds
    timerToUSecDiv        = highResTimerFrequency;
    const int64_t divisor = timeGCD(timerToUSecMul, timerToUSecDiv);
    timerToUSecMul /= divisor;
    timerToUSecDiv /= divisor;

    // If the multiplier is 1, there's no way our "simple" formula will overflow.
    // If the divisor is 1, then we still might overflow, but int64MulDiv wouldn't prevent it.
    alwaysSimpleMulDiv = (timerToUSecMul == 1) || (timerToUSecDiv == 1);
}

static void ensureTimeInit() {
    // Make sure time constants are initialized before anyone tries to use them
    callOnce(&timeInitGuard, initTime);
}

int64_t getTimerFrequency() {
    ensureTimeInit();

    return highResTimerFrequency;
}

// The `precise` param is being used to specify the way in which the usec is calculated.
// If it's false, then a normal unsafe multiplication and division operations are made.
// If it's true, a special int64MulDiv function is called that avoids the overflow.
int64_t getUSec(bool precise) {
    ensureTimeInit();

    LARGE_INTEGER counter;
    BOOL qpcResult = QueryPerformanceCounter(&counter);
    assert(qpcResult);
    counter.QuadPart -= highResTimerStart;

    if (alwaysSimpleMulDiv || !precise) {
        return counter.QuadPart * timerToUSecMul / timerToUSecDiv;
    } else {
        return int64MulDiv(counter.QuadPart, timerToUSecMul, timerToUSecDiv);
    }
}
