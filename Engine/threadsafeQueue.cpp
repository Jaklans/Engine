#pragma once
#include "threadsafeQueue.h"
#include <thread>
#include <stdexcept>


/*namespace threading {
	// Task Queue ----------------------------------
	tsQueue::tsQueue(ui32 size)
	{
		//Increace the size to the next power of 2
		SIZE = 1;
		while (SIZE < size) {
			SIZE *= 2;
		}
		SIZE_MASK = SIZE - 1;

		m_tail = 0;
		m_producerHead = 0;
		m_consumerHead = 0;

		m_buffer = new void* [size] {0};
	}

	tsQueue::~tsQueue()
	{
		delete m_buffer;
	}

	void tsQueue::Enqueue(void* object) {
		ui32 temporaryIndex = _InterlockedIncrement(&m_producerHead) - 1;
		while (temporaryIndex >= m_tail + SIZE) {
			//Queue has run out of space, start a fire
			throw std::runtime_error("Queue has run out of space!");
		}

		m_buffer[temporaryIndex & SIZE_MASK] = object;
		_InterlockedIncrement(&m_consumerHead);
	}

	//Takes pointer for an array of objects
	void tsQueue::Enqueue(void** objects, ui32 count)
	{
		ui32 temporaryIndex = InterlockedAdd(reinterpret_cast<volatile long*>(&m_producerHead), count) - count;
		if (temporaryIndex >= m_tail + SIZE) {
			throw std::runtime_error("Queue has run out of space!");
		}
		temporaryIndex &= SIZE_MASK;
		if (count + temporaryIndex < SIZE) {
			memcpy(m_buffer + temporaryIndex, objects, sizeof(void*) * count);
		}
		else {
			ui32 topCount = SIZE - temporaryIndex;
			memcpy(m_buffer + temporaryIndex, objects, sizeof(void*) * topCount);
			memcpy(m_buffer, objects + topCount, sizeof(void*) * (count - topCount));
		}
		InterlockedAdd(reinterpret_cast<volatile long*>(&m_consumerHead), count);
	}

	//Trys to pop, and set ref to point at the poped object
	//If no item is in the queue, returns false
	bool tsQueue::Dequeue(void*& ref) {
		ui32 temporaryIndex = _InterlockedIncrement(&m_tail) - 1;
		if (temporaryIndex >= m_consumerHead) {
			_InterlockedDecrement(&m_tail);
			return false;
		}
		ref = m_buffer[temporaryIndex & SIZE_MASK];
		return true;
	}
} */

namespace threadsafe {
	template <class T>
	Queue<T>::Queue(ui32 size)
	{
		//Increace the size to the next power of 2
		SIZE = 1;
		while (SIZE < size) {
			SIZE *= 2;
		}
		SIZE_MASK = SIZE - 1;

		m_tail = 0;
		m_producerHead = 0;
		m_consumerHead = 0;

		m_buffer = new T[size];
	}

	template <class T>
	Queue<T>::~Queue()
	{
		delete m_buffer;
	}

	template <class T>
	void Queue<T>::Enqueue(T& object) {
		ui32 temporaryIndex = _InterlockedIncrement(&m_producerHead) - 1;
		while (temporaryIndex >= m_tail + SIZE) {
			//Queue has run out of space, start a fire
			throw std::runtime_error("Queue has run out of space!");
		}

		m_buffer[temporaryIndex & SIZE_MASK] = object;
		_InterlockedIncrement(&m_consumerHead);
	}

	//Takes pointer for an array of objects
	template <class T>
	void Queue<T>::Enqueue(T* objects, ui32 count)
	{
		ui32 temporaryIndex = InterlockedAdd(reinterpret_cast<volatile long*>(&m_producerHead), count) - count;
		if (temporaryIndex >= m_tail + SIZE) {
			throw std::runtime_error("Queue has run out of space!");
		}
		temporaryIndex &= SIZE_MASK;
		if (count + temporaryIndex < SIZE) {
			memcpy(m_buffer + temporaryIndex, objects, sizeof(T) * count);
		}
		else {
			ui32 topCount = SIZE - temporaryIndex;
			memcpy(m_buffer + temporaryIndex, objects, sizeof(T) * topCount);
			memcpy(m_buffer, objects + topCount, sizeof(T) * (count - topCount));
		}
		InterlockedAdd(reinterpret_cast<volatile long*>(&m_consumerHead), count);
	}

	//Trys to pop. Returns NULL if it fails
	template <class T>
	bool Queue<T>::Dequeue(T& ref) {
		ui32 temporaryIndex = _InterlockedIncrement(&m_tail) - 1;
		if (temporaryIndex >= m_consumerHead) {
			_InterlockedDecrement(&m_tail);
			return false;
		}
		 ref = m_buffer[temporaryIndex & SIZE_MASK]; 
		 return true;
	}

	template class Queue<job>;
}