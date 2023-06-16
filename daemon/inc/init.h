#ifndef __DOCKER_DAEMON_INIT_INCLUDED_H__
#define __DOCKER_DAEMON_INIT_INCLUDED_H__

#include <pthread.h>
#include "../../common/common.h"

void show_banner(void);
int init_daemon(const char *path, char *args, pthread_t *tid);

#endif // __DOCKER_DAEMON_INIT_INCLUDED_H__
