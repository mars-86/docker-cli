#include <stdio.h>
#include <stdlib.h>
#include "installer.h"
#include "../constants/inc/error_codes.h"

int main(int argc, char *argv[])
{
    show_banner();
    int opt = 'n';

    puts("This will install docker engine in your system\nAre you sure you want to continue [Y/n] ");
    scanf("%c", &opt);

#ifdef __DEBUG
    printf("%c", opt);
#endif

    if (opt != 'Y')
        return ECANCELEDINSTALL;

    int status = EOK;

    if (status = check_previous_install())
        return status;

    puts("Getting file system... ");
    get_fs();
    puts("OK\n");

    const char *user_path = getenv("USERPROFILE");

    puts("Installing docker...\n");
    if (status = install(user_path))
        return status;

    puts("Adding docker to path... ");
    if (status = add_to_path())
        return status;
    puts("OK\n");
/*
    printf("Creating docker service...\n");
    if (status = create_docker_service())
        return status;
*/

    puts("Adding docker to run on boot... ");
    if (status = start_on_boot())
        return status;
    puts("OK\n");

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    return status;
}
