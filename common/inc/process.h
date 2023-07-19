#ifndef __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__
#define __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__

#include <pthread.h>
#include "error.h"

enum _DOCKER_CLI_MODULES {
    DOCKER_CLI_DAEMON
};

typedef enum _DOCKER_CLI_MODULES DOCKER_CLI_MODULE;

void initialize_modules_ipc(void);
void destroy_modules_ipc(void);
int initialize_shared(void);
void destroy_shared(void);
int set_condition_var(DOCKER_CLI_MODULE mod);
int get_condition_var(DOCKER_CLI_MODULE mod);
pthread_cond_t *get_thread_condition(DOCKER_CLI_MODULE mod);
pthread_mutex_t *get_thread_mutex(DOCKER_CLI_MODULE mod);
int exec(const char *cmd);

#endif // __DOCKER_CLI_COMMON_PROCESS_INCLUDED_H__
