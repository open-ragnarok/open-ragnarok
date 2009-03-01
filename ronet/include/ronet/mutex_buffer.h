/* $Id$ */
#ifndef __RONET_MUTEX_BUFFER_H
#define __RONET_MUTEX_BUFFER_H

#include "buffer.h"
#include "mutex.h"

namespace ronet {
	template <typename T, int _StorSize = 2048>
	class MutexBuffer : public Buffer<T, _StorSize> {
	};
}

#endif /* __RONET_MUTEX_BUFFER_H */