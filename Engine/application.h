#pragma once
#include "jobManager.h"
#include "logging.h"
#include "time.h"

class JobManager;
namespace logging {
	class Logger;
}

struct App
{
	JobManager* jobManager;
	logging::Logger* loggingManager;
	TimeManager* timeManager;
};

extern App* app;