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

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* prevent 64-bit overflow when computing relative timestamp
    see https://gist.github.com/jspohr/3dc4f00033d79ec5bdaf67bc46c813e3
*/
/* TODO: Move this function to the Math Library as soon as it's in C */
static inline int64_t int64MulDiv(int64_t value, int64_t numer, int64_t denom)
{
    int64_t q = value / denom;
    int64_t r = value % denom;
    return q * numer + r * numer / denom;
}

// High res timer functions
int64_t getUSec(bool precise);
int64_t getTimerFrequency(void);

// Time related functions
uint32_t getSystemTime(void);
uint32_t getTimeSinceStart(void);


/// Low res OS timer
typedef struct Timer
{
	uint32_t mStartTime;
}Timer;

void		initTimer(Timer* pTimer);
void		resetTimer(Timer* pTimer);
uint32_t	getTimerMSec(Timer* pTimer, bool reset);


/// High-resolution OS timer
#define HIRES_TIMER_LENGTH_OF_HISTORY 60

typedef struct HiresTimer
{
	int64_t		mStartTime = 0;
	int64_t		mHistory[HIRES_TIMER_LENGTH_OF_HISTORY] = {};
	uint32_t	mHistoryIndex = 0;
}HiresTimer;

void		initHiresTimer(HiresTimer* pTimer);
int64_t		getHiresTimerUSec(HiresTimer* pTimer, bool reset);
int64_t		getHiresTimerUSecAverage(HiresTimer* pTimer);
float		getHiresTimerSeconds(HiresTimer* pTimer, bool reset);
float		getHiresTimerSecondsAverage(HiresTimer* pTimer);
void		resetHiresTimer(HiresTimer* pTimer);


#ifdef __cplusplus
}
#endif
