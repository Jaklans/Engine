#pragma once
#include "logging.h"

#include <cstdio>

#define MESSAGE_INTERVAL 1000;

namespace logging {
	Logger::Logger() {
		messageBufferSize = 1000000;
		halfBufferSize = messageBufferSize / 2;
		messageBuffer = new char[messageBufferSize];
		currentPosition = 0;
		bufferState = false;
	}
	Logger::~Logger() {
		delete[] messageBuffer;
	}

	void Logger::log(const char* message, const char* verbosity, ui32 verbosityLength) {
		ui32 messageSize = (ui32)strlen(message);
		ui32 tempPosition = _InterlockedExchangeAdd(&currentPosition, messageSize + verbosityLength + 1);
		memcpy(messageBuffer + tempPosition, verbosity, verbosityLength * sizeof(char));
		memcpy(messageBuffer + tempPosition + verbosityLength, message, messageSize * sizeof(char));
		messageBuffer[tempPosition + verbosityLength + messageSize] = '\n';
	}
	void Logger::HandleMessages() {
		if (currentPosition == 0 || currentPosition == halfBufferSize || semaphore.counter) return;

		semaphore.increment();

		ui32 tempBufferPos;
		if (bufferState) {
			tempBufferPos = _InterlockedExchange(&currentPosition, 0);
		}
		else {
			tempBufferPos = _InterlockedExchange(&currentPosition, halfBufferSize);
		}

		messageBuffer[tempBufferPos - 1] = '\0';
		if (bufferState) {
			puts(messageBuffer + halfBufferSize);
		}
		else
		{
			puts(messageBuffer);
		}
		bufferState = !bufferState;

		semaphore.decrement();
	}
}