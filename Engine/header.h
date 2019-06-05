#pragma once

typedef          short  i8;
typedef unsigned short ui8;
typedef          int   i16;
typedef unsigned int  ui16;
typedef          long  i32;
typedef unsigned long ui32;
typedef          long long  i64;
typedef unsigned long long ui64;

typedef ui8 b;

typedef float  f32;
typedef double f64;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "jobManager.h"
#include "logging.h"
#include "time.h"
#include "rendering.h"
//#include "data.h"

struct ApplicationManager
{
	JobManager* Job = 0;
	logging::Logger* Log = 0;
	TimeManager* Time = 0;
	//Rendering
	//Data
};

extern ApplicationManager Manager;