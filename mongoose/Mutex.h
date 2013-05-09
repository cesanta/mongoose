#ifndef _MONGOOSE_MUTEX_H
#define _MONGOOSE_MUTEX_H

#include <pthread.h>

namespace Mongoose
{
    class Condition;

    class Mutex
    {
        public:
            Mutex();
            virtual ~Mutex();
            virtual void lock();
            virtual void unlock();

        protected:
            pthread_mutex_t _mutex;
    };
};

#endif

