#include <string>
#include <iostream>

#include "Mutex.h"

using namespace std;

namespace Mongoose
{
	#ifdef _MSC_VER
	static int pthread_mutex_init(pthread_mutex_t *mutex, void *unused) {
	  (void) unused;
	  *mutex = CreateMutex(NULL, FALSE, NULL);
	  return *mutex == NULL ? -1 : 0;
	}

	static int pthread_mutex_destroy(pthread_mutex_t *mutex) {
	  return CloseHandle(*mutex) == 0 ? -1 : 0;
	}

	static int pthread_mutex_lock(pthread_mutex_t *mutex) {
	  return WaitForSingleObject(*mutex, INFINITE) == WAIT_OBJECT_0? 0 : -1;
	}

	static int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	  return ReleaseMutex(*mutex) == 0 ? -1 : 0;
	}
	#endif

    Mutex::Mutex()
    {
        pthread_mutex_init(&_mutex, NULL);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void Mutex::lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void Mutex::unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
}
