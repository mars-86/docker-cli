#ifndef __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__
#define __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__

#include "error.h"

#define PROTO_LEN 16
#define RES_TEXT 512

struct _DAEMON_RESPONSE {
    char proto[PROTO_LEN];
    int code;
    char res_text[RES_TEXT];
    char *data;
};

typedef struct _DAEMON_RESPONSE daemon_res_t;

DOCKERCLI_CODE check_daemon_status(void);
DOCKERCLI_CODE parse_daemon_response(daemon_res_t *res);

#endif // __DOCKER_CLI_COMMON_MODULES_INCLUDED_H__
