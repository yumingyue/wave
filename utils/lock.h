#ifndef LOCK_H
#define LOCK_H
#include<pthread.h>
#include<stddef.h>

/**
 *定义成这样，方便我们以后换所。
 */
typedef pthread_rwlock_t lock;
static int inline lock_init(lock* lock){
    return pthread_rwlock_init(lock,NULL);
}

static int inline lock_rdlock(lock* lock){
    return pthread_rwlock_rdlock(lock);
}

static int inline lock_tryrdlock(lock* lock){
    return pthread_rwlock_tryrdlock(lock);
}
static int inline lock_wrlock(lock* lock){
    return pthread_rwlock_wrlock(lock);
}
static int inline lock_trywrlock(lock* lock){
    return pthread_rwlock_trywrlock(lock);
}
static int inline lock_unlock(lock* lock){
    return pthread_rwlock_unlock(lock);
}
#endif
