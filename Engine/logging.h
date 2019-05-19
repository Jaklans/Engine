#pragma once
#include "threadingConstructs.h"

namespace threading {
	struct atomicCounter;
}
namespace logging {
	enum verbosity {
		FATAL,
		ERRORS,
		WARNINGS,
		INFORMATION,
		DEBUG
	};

	class Logger {
	public:
		Logger();
		~Logger();

		void HandleMessages();
		void log(const char* message, const char* verbosity, ui32 verbosityLength);
	private:
		char* messageBuffer;
		ui32 messageBufferSize;
		ui32 halfBufferSize;
		volatile ui32 currentPosition;
		bool bufferState;
		threading::atomicCounter semaphore;
	};

	static Logger* LoggingManager;
}

#define DISPATCH_LOGS() logging::LoggingManager->HandleMessages();

#define LOG_ERROR(message) logging::LoggingManager->log(message, "[ERROR]: ", 9)
#define LOG_INFORMATION(message) //Logger::logger::log(message, verbosity::ERRORS)
#define LOG_DEBUG(message) logging::LoggingManager->log(message, "[DEBUG]: ", 9)
