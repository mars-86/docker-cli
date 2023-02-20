#ifndef __DOCKER_DAEMON_INIT_INCLUDED_H__
#define __DOCKER_DAEMON_INIT_INCLUDED_H__

#include <pthread.h>

void show_banner(void);
void init_dockerd(pthread_t *tid, void *cmd);

#endif // __DOCKER_DAEMON_INIT_INCLUDED_H__
