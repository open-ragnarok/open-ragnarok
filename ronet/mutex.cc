/* $Id$ */
#include "stdafx.h"

#include "ronet/mutex.h"

ronet::Mutex::Mutex() {
#ifdef WIN32
	InitializeCriticalSection(&m_mutex);
#else
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_mutex_init(&m_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}

ronet::Mutex::~Mutex() {
#ifdef WIN32
	DeleteCriticalSection(&m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}

void ronet::Mutex::lock() {
#ifdef WIN32
	EnterCriticalSection(&m_mutex);
#else
	pthread_mutex_lock(&m_mutex);
#endif
}

void ronet::Mutex::unlock() {
#ifdef WIN32
	LeaveCriticalSection(&m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}

bool ronet::Mutex::trylock() {
#ifdef WIN32
	return(TryEnterCriticalSection(&m_mutex) == TRUE);
#else
	return(pthread_mutex_trylock(&m_mutex) == 0);
#endif
}
