// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
//

/* Brain-dead simple portability wrapper over thread APIs for C */
#pragma once

#ifdef _WIN32

#include <windows.h>

#define CTHR_RWLOCK_TYPE	SRWLOCK
#define CTHR_RWLOCK_INIT	SRWLOCK_INIT
#define CTHR_RWLOCK_LOCK_WRITE(x)	AcquireSRWLockExclusive(&x)
#define CTHR_RWLOCK_UNLOCK_WRITE(x)	ReleaseSRWLockExclusive(&x)
#define CTHR_RWLOCK_LOCK_READ(x)	AcquireSRWLockShared(&x)
#define CTHR_RWLOCK_UNLOCK_READ(x)	ReleaseSRWLockShared(&x)
#define CTHR_RWLOCK_TRYLOCK_READ(x)	TryAcquireSRWLockShared(&x)

#define CTHR_THREAD_TYPE	HANDLE
#define CTHR_THREAD_RTYPE	unsigned __stdcall
#define CTHR_THREAD_RETURN	_endthreadex(0); return 0;
#define CTHR_THREAD_CREATE(thr, func, arg)	((thr = (HANDLE)_beginthreadex(0, 0, func, arg, 0, 0)) != 0L)
#define CTHR_THREAD_JOIN(thr)			do { WaitForSingleObject(thr, INFINITE); CloseHandle(thr); } while(0)
#define CTHR_THREAD_CLOSE(thr)			CloseHandle((HANDLE)thr);

#else

#include <pthread.h>

#define CTHR_RWLOCK_TYPE	pthread_rwlock_t
#define CTHR_RWLOCK_INIT	PTHREAD_RWLOCK_INITIALIZER
#define CTHR_RWLOCK_LOCK_WRITE(x)	pthread_rwlock_wrlock(&x)
#define CTHR_RWLOCK_UNLOCK_WRITE(x)	pthread_rwlock_unlock(&x)
#define CTHR_RWLOCK_LOCK_READ(x)	pthread_rwlock_rdlock(&x)
#define CTHR_RWLOCK_UNLOCK_READ(x)	pthread_rwlock_unlock(&x)
#define CTHR_RWLOCK_TRYLOCK_READ(x)	(pthread_rwlock_tryrdlock(&x) == 0)

#define CTHR_THREAD_TYPE pthread_t
#define CTHR_THREAD_RTYPE	void *
#define CTHR_THREAD_RETURN	return NULL
#define CTHR_THREAD_CREATE(thr, func, arg)	(pthread_create(&thr, NULL, func, arg) == 0)
#define CTHR_THREAD_JOIN(thr)			pthread_join(thr, NULL)
#define CTHR_THREAD_CLOSE(thr)

#endif
