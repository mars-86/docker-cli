#ifndef __DOCKER_DAEMON_INIT_INCLUDED_H__
#define __DOCKER_DAEMON_INIT_INCLUDED_H__

#include <windows.h>
#include <pthread.h>

void show_banner(void);
int init_daemon(const char *path, char *args);

#endif // __DOCKER_DAEMON_INIT_INCLUDED_H__
