/* $Id$ */
#ifndef __RONET_MUTEX_H
#define __RONET_MUTEX_H

#ifdef WIN32
#  include <winsock.h>
#  include <windows.h>
#  define _mutex_t CRITICAL_SECTION
#else
#  include <pthread.h>
#  define _mutex_t pthread_mutex_t
#endif

namespace ronet {
	/**
	 * Manage locks on a thread.
	 */
	class RONET_DLLAPI Mutex {
		public:
			Mutex();
			virtual ~Mutex();
			
			/** Locks the mutex object */
			void lock();
			
			/** Unlocks the mutex object */
			void unlock();
			
			/**
			 * Tries to lock the mutex object and returns true if successful.
			 */
			bool trylock();
		protected:
		private:
			_mutex_t m_mutex;
	};

}

#endif /* __RONET_MUTEX_H */