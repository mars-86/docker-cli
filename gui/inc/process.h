#ifndef __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__
#define __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__

#include <windows.h>
#include <pthread.h>

int init_daemon(const char *path, char *args, pthread_t *tid);
int check_daemon_status(void);

#endif // __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__
