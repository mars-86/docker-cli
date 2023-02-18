#ifndef __DOCKER_CLI_CMD_LINE_INCLUDED_H__
#define __DOCKER_CLI_CMD_LINE_INCLUDED_H__

int exec(const char *cmd);
const char *parse_cmdl(const char *base, const char **argv);
void free_cmdl(void *cmd);

#endif // __DOCKER_CLI_CMD_LINE_INCLUDED_H__
