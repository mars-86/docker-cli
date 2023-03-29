#ifndef __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__
#define __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__

#include <windows.h>

int init_daemon(const char *path, char *args, PROCESS_INFORMATION *proc);

#endif // __DOCKER_CLI_INIT_PROCESS_INCLUDED_H__
