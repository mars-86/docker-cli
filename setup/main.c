#include <stdio.h>
#include <stdlib.h>
#include "../constants/inc/error_msgs.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    int status = 0;
    if ((status = system("..\\..\\installer\\bin\\installer")) > 0) {
        fprintf(stderr, "%s\n", error_msg[status]);
        return status;
    }

    if ((status = system("..\\..\\daemon\\bin\\dockerd")) > 0) {
        fprintf(stderr, "%s\n", error_msg[status]);
        return status;
    }

    return status;
}
