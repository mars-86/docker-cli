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

    int status = 0;
    if (status = check_previous_install())
        return status;

    printf("Getting file system...\n");
    get_fs();

    printf("Installing docker...\n");
    if (status = install(getenv("USERPROFILE")))
        return status;
    
    printf("Copying bin...\n");
    if (status = cp_bin_cli(getenv("USERPROFILE")))
        return status;

    printf("Copying daemon...\n");
    if (status = cp_daemon(getenv("USERPROFILE")))
        return status;
    
    printf("Adding docker to path...\n");
    if (status = add_to_path())
        return status;

    return status;
}
