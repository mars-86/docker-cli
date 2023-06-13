#ifndef __DOCKER_CLI_GUI_THREAD_INCLUDED_H__
#define __DOCKER_CLI_GUI_THREAD_INCLUDED_H__

#include <pthread.h>
#include "../../common/common.h"

DOCKERCLI_CODE init_daemon(const char *path, char *args, pthread_t *tid);
DOCKERCLI_CODE check_daemon_status(void);

#endif // __DOCKER_CLI_GUI_THREAD_INCLUDED_H__
