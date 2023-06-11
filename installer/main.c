#include <stdio.h>
#include <stdlib.h>
#include "installer.h"
#include "../constants/inc/error_codes.h"

int main(int argc, char *argv[])
{
    show_banner();
    int opt = 'n';

    fputs("This will install docker engine in your system\nAre you sure you want to continue [Y/n] ", stdout);
    scanf("%c", &opt);

#ifdef __DEBUG
    printf("%c", opt);
#endif

    if (opt != 'Y')
        return ECANCELEDINSTALL;

    int status = EOK;

    if (status = check_previous_install())
        return status;

    fputs("Getting file system... ", stdout);
    get_fs();
    puts("OK");

    const char *user_path = getenv("USERPROFILE");

    puts("Installing docker...");
    if (status = install(user_path))
        return status;

    fputs("Adding docker to path... ", stdout);
    if (status = add_to_path())
        return status;
    puts("OK");
/*
    printf("Creating docker service...\n");
    if (status = create_docker_service())
        return status;
*/

    fputs("Adding docker to run on boot... ", stdout);
    if (status = start_on_boot())
        return status;
    puts("OK");

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    return status;
}
