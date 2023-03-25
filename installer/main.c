#include <stdio.h>
#include <stdlib.h>
#include "installer.h"
#include "../constants/inc/error_codes.h"

int main(int argc, char *argv[])
{
    show_banner();
    int opt = 'n';

    printf("This will install docker engine in your system\nAre you sure you want to continue [Y/n] ");
    scanf("%c", &opt);

#ifdef __DEBUG
    printf("%c", opt);
#endif
    if (opt != 'Y')
        return ECANCELEDINSTALL;

    int status = EOK;

    if (status = check_previous_install())
        return status;

    printf("Getting file system...\n");
    get_fs();

    const char *user_path = getenv("USERPROFILE");

    printf("Installing docker...\n");
    if (status = install(user_path))
        return status;
    
    printf("Copying docker...\n");
    if (status = copy_docker(user_path))
        return status;

    printf("Copying daemon...\n");
    if (status = copy_daemon(user_path))
        return status;
    
    printf("Copying bin...\n");
    if (status = copy_bin_cli(user_path))
        return status;
    
    printf("Copying assets...\n");
    if (status = copy_assets(user_path))
        return status;

    printf("Adding docker to path...\n");
    if (status = add_to_path())
        return status;
/*
    printf("Creating docker service...\n");
    if (status = create_docker_service())
        return status;
*/

    printf("Adding docker to run on boot...\n");
    if (status = start_on_boot())
        return status;

#ifdef __DEBUG
    printf("%d\n", status);
#endif

    return status;
}
