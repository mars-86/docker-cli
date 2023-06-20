#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../inc/process.h"

static HANDLE sharedh, sharedm;
static FILE *shared;
static char shared_path[MAX_PATH];
static int daemon_terminate = 0;
pthread_cond_t daemon_thread_cond;
pthread_mutex_t daemon_thread_mutex;
pthread_condattr_t daemon_cond_attr;
pthread_mutexattr_t daemon_mutex_attr;

void initialize_modules_ipc(void)
{
    int status;
    if (pthread_condattr_init(&daemon_cond_attr))
        puts("pthread_condattr_init error");

    if (status = pthread_condattr_setpshared(&daemon_cond_attr, PTHREAD_PROCESS_SHARED)) {
        puts("pthread_condattr_setpshared error");
    }
    
    if (pthread_cond_init(&daemon_thread_cond, &daemon_cond_attr))
        puts("pthread_cond_init error");

    if (pthread_mutexattr_init(&daemon_mutex_attr))
        puts("pthread_mutexattr_init error");

    if (status = pthread_mutexattr_setpshared(&daemon_mutex_attr, PTHREAD_PROCESS_SHARED)) {
        puts("pthread_mutexattr_setpshared error");
    }

    if (pthread_mutex_init(&daemon_thread_mutex, &daemon_mutex_attr))
        puts("pthread_mutex_init error");
}

void destroy_modules_ipc(void)
{
    pthread_mutex_destroy(&daemon_thread_mutex);
    pthread_cond_destroy(&daemon_thread_cond);

    pthread_condattr_destroy(&daemon_cond_attr);
    pthread_mutexattr_destroy(&daemon_mutex_attr);
}

int initialize_shared(void)
{
    const char *home = getenv("DOCKER_CLI_HOME");
    sprintf(shared_path, "%s\\tmp\\shared", home);

    if (!(shared = fopen(shared_path, "w")))
        return -1;

    fputc('0', shared);

    fclose(shared);

    return 0;
}

int initialize_shared_win(void)
{
    const char *home = getenv("DOCKER_CLI_HOME");
    sprintf(shared_path, "%s\\tmp\\shared", home);

    if ((sharedh = CreateFileA(shared_path, GENERIC_READ | GENERIC_WRITE, 0, NULL, NULL, 0, NULL) == INVALID_HANDLE_VALUE))
        return DOCKERCLIE_SYSTEM;

    if ((sharedm = CreateFileMappingA(sharedh, NULL, PAGE_READWRITE, 0, 0, "shared_memory"))) {
        CloseHandle(sharedh);
        return DOCKERCLIE_SYSTEM;
    }

    return DOCKERCLIE_OK;
}

void destroy_shared_win(void)
{
    CloseHandle(sharedm);
    CloseHandle(sharedh);
}

/* windows doesn't support PTHREAD_PROCESS_SHARED attribute */
/* it's OK by now to save status in a file so the processes can read from it, */
/* in tne future it would be better to implement a mechanism with windows's */
/* shared memory API */
int set_condition_var(DOCKER_CLI_MODULE mod)
{
    const char *home = getenv("DOCKER_CLI_HOME");
    sprintf(shared_path, "%s\\tmp\\shared", home);

    if (!(shared = fopen(shared_path, "w")))
        return -1;

    if (mod == DOCKER_CLI_DAEMON)
        fputc('1', shared);
    fclose(shared);

    return 0;
}

int get_condition_var(DOCKER_CLI_MODULE mod)
{
    const char *home = getenv("DOCKER_CLI_HOME");
    sprintf(shared_path, "%s\\tmp\\shared", home);

    int check;
    if (!(shared = fopen(shared_path, "r")))
        return -1;

    if (mod == DOCKER_CLI_DAEMON)
        check = fgetc(shared);
    fclose(shared);

    return (check - '0');
}

pthread_cond_t *get_thread_condition(DOCKER_CLI_MODULE mod)
{
    if (mod == DOCKER_CLI_DAEMON)
        return &daemon_thread_cond;

    return NULL;
}

pthread_mutex_t *get_thread_mutex(DOCKER_CLI_MODULE mod)
{
    if (mod == DOCKER_CLI_DAEMON)
        return &daemon_thread_mutex;

    return NULL;
}

int exec(const char *cmd)
{
#ifdef __DEBUG
    printf("%s\n", cmd);
#endif
    fflush(stdout);
    return system(cmd);
}
