#ifndef __DOCKER_CLI_CONSTANTS_ERROR_CODES_INCLUDED_H__
#define __DOCKER_CLI_CONSTANTS_ERROR_CODES_INCLUDED_H__

enum _ERROR_CODES {
    EOK = 0,
    ECANCELEDINSTALL = 1,
    EDOCKINSTALLED = 2,
    ECANNOTCPIPE = 3,
    ECANNOTIFS = 4,
    ECANNOTIDOCK = 5,
    ECANNOTINITDAE = 6,
    DOCKERCLIE_CNOTCHECKDAESTAT = 7,
    ESYSTEM = 99
};

typedef enum _ERROR_CODES ECODE;

#endif // __DOCKER_CLI_CONSTANTS_ERROR_CODES_INCLUDED_H__
