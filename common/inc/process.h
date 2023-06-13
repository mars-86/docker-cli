#ifndef __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__
#define __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__

#include <pthread.h>

int daemon_terminate = 0;
pthread_cond_t daemon_thread_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t daemon_thread_mutex = PTHREAD_MUTEX_INITIALIZER;

int exec(const char *cmd);

#endif // __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__
