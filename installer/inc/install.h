#ifndef __DOCKER_CLI_INSTALL_INCLUDED_H__
#define __DOCKER_CLI_INSTALL_INCLUDED_H__

void show_banner(void);
int check_previous_install(void);
int install(const char *base_path);
int add_to_path(void);
int start_on_boot(void);
int install_docker_service(void);

#endif // __DOCKER_CLI_INSTALL_INCLUDED_H__
