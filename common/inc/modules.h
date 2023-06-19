#ifndef __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__
#define __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__

#include "error.h"

#define DAEMON_RES_PLAIN_OUTPUT 0x0000
#define DAEMON_RES_FORMAT_OUTPUT 0x0001
#define DAEMON_RES_STATUS 0x0002
#define DAEMON_RES_TEXT 0x0004
#define DAEMON_RES_DATA 0x0008
#define DAEMON_RES_COMPLETE DAEMON_RES_STATUS | DAEMON_RES_TEXT | DAEMON_RES_DATA

#define PROTO_LEN 16
#define CODE_LEN 4
#define CODE_TEXT_LEN 64
#define RES_TEXT_LEN 512

struct _DAEMON_STATUS {
    char proto[PROTO_LEN];
    union {
        unsigned short n_code;
        char t_code[CODE_LEN];
    };
    char code_text[CODE_TEXT_LEN];
};

typedef struct _DAEMON_STATUS daemon_status_t;

struct _DAEMON_RESPONSE {
    daemon_status_t *status;
    char *res_text;
    char *data;
};

typedef struct _DAEMON_RESPONSE daemon_res_t;

DOCKERCLI_CODE check_daemon_status(void);
DOCKERCLI_CODE parse_daemon_response(daemon_res_t *res, int flags);
void free_daemon_response(daemon_res_t *res);

#endif // __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__
