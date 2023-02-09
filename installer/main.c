#include <stdio.h>
#include <stdlib.h>
#include "installer.h"

int main(int argc, char *argv[])
{
    int status = 0;
    if ((status = check_previous_install()) > 0)
        return status;

    printf("Getting file system...\n");
    get_fs();

    printf("Installing docker...\n");
    if ((status = install(getenv("USERPROFILE"))) > 0)
        return status;

    printf("Adding docker to path...\n");
    if ((status = add_to_path()) > 0)
        return status;
    
    printf("Copy bin...\n");
    if ((status = cp_bin_cli(getenv("USERPROFILE"))) > 0)
        return status;

    return status;
}
