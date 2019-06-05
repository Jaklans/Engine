#pragma once
/*
//TODO: Add support to push multiple at one time
namespace threading {
	//Threadsafe Lockless Queue to hold ui64 (pointers)
	class tsQueue {
	public:
		tsQueue(ui32 size);
		~tsQueue();
		void Enqueue(void* object);
		void Enqueue(void** objects, ui32 count);
		bool Dequeue(void*& ref);
	private:
		volatile ui32 m_producerHead;
		volatile ui32 m_consumerHead;
		volatile ui32 m_tail;
		ui32 SIZE;
		ui32 SIZE_MASK;
		void** m_buffer;
	};
}*/
namespace threadsafe {
	template <class T>
	class Queue {
	public:
		Queue(ui32 size);
		~Queue();
		void Enqueue(T& object);
		void Enqueue(T* objects, ui32 count);
		bool Dequeue(T& ref);
	private:
		volatile ui32 m_producerHead;
		volatile ui32 m_consumerHead;
		volatile ui32 m_tail;
		ui32 SIZE;
		ui32 SIZE_MASK;
		T* m_buffer;
	};
}