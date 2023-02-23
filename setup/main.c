#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "../constants/inc/error_msgs.h"
#include "../common/common.h"

int main(int argc, char *argv[])
{
    int status = 0;
    if ((status = exec("..\\..\\installer\\bin\\installer")) > 0) {
        fprintf(stderr, "%s\n", error_msg[status]);
        return status;
    }

    if ((status = exec("..\\..\\daemon\\bin\\dockerd")) > 0) {
        fprintf(stderr, "%s\n", error_msg[status]);
        return status;
    }

    return status;
}
