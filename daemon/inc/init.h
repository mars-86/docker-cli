#ifndef __DOCKER_DAEMON_INIT_INCLUDED_H__
#define __DOCKER_DAEMON_INIT_INCLUDED_H__

#include <windows.h>

void show_banner(void);
int init_daemon(const char *path, char *args, PROCESS_INFORMATION *proc);

#endif // __DOCKER_DAEMON_INIT_INCLUDED_H__
