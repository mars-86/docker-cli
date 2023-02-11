#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../inc/install.h"
#include "../../constants/inc/error_codes.h"

static const LPCSTR env_skey = "Environment";
static const LPCSTR run_skey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

static int env_exist(const char *name, const char *skey)
{
    int status = RegGetValueA(HKEY_CURRENT_USER, skey, name, RRF_RT_ANY, NULL, NULL, NULL);
    return !status ? 1 : 0;
}

static int edit_dns(void)
{
    int status = 0;
    status = system("wsl -d docker-cli rm /etc/resolv.conf");
    status = system("wsl -d docker-cli -- cp ../../conf/wsl.conf /etc");
    status = system("wsl -d docker-cli chmod 644 /etc/wsl.conf");

    return status;
}

void show_banner(void)
{
    printf(
        "==========================================================================================\n"
        "   _____         _____         _____            _____   _____            _____            \n"
        " ||     \\\\     //     \\\\     //      ||    // ||      ||     \\\\        //      ||      || \n"
        " ||      \\\\   //       \\\\   //       ||   //  ||      ||      ||      //       ||      || \n"
        " ||       || ||         || ||        ||  //   ||      ||     //      ||        ||      || \n"
        " ||       || ||         || ||        ||//     ||===== ||=====        ||        ||      || \n"
        " ||       || ||         || ||        ||\\\\     ||      ||\\\\           ||        ||      || \n"
        " ||      //   \\\\       //   \\\\       ||  \\\\   ||      ||  \\\\          \\\\       ||      || \n"
        " ||_____//     \\\\_____//     \\\\_____ ||   \\\\  ||_____ ||    \\\\         \\\\_____ ||_____ || \n"
        "                                                                                          \n"
        "==========================================================================================\n"
    );
}

int check_previous_install(void)
{
    /* for some weird reason wsl returns null characters after the letters */
    FILE *pipe;
    if ((pipe = _popen("wsl -l", "rt")) == NULL)
        return ECANNOTCPIPE;

    char name[] = "docker-cli";
    int i = 0, c, len = strlen(name), installed = 0;
    while ((c = fgetc(pipe)) != EOF) {
        if (c == '\0') continue;
#ifdef __DEBUG
        printf("%c - %c\n", name[i], c);
#endif
        i = (name[i] == c) ? (i + 1) : 0;
        if (i == len) {
            installed = EDOCKINSTALLED;
            break;
        }
    }
    int ret = _pclose(pipe);

    return installed;
}

int install(const char *base_path)
{
    char fs_path[512], install_path[512], ifs_cmd[512];
    const char *file_name = "alpine-minirootfs-3.17.1-x86_64.tar.gz";
    sprintf(fs_path, "%s\\%s", getenv("TMP"), file_name);
    sprintf(install_path, "%s\\docker-cli", base_path);
    sprintf(ifs_cmd, "wsl --import docker-cli %s %s", install_path, fs_path);

#ifdef __DEBUG
    printf("%s\n", ifs_cmd);
#endif
    if (system(ifs_cmd) < 0)
        return ECANNOTIFS;
    
    edit_dns();
    /* terminate vm to set changes */
    system("wsl -t docker-cli");

    const char *idocker_cmd = "wsl -d docker-cli -- apk add --update docker docker-cli-compose openrc";
    if (system(idocker_cmd) < 0)
        return ECANNOTIDOCK;

    return 0;
}

int add_to_path(void)
{
    HKEY hkey;
    DWORD len;
    char docker_path[512], *newpathval, dockbpath[512];
    const char *dockhvname = "DOCKER_CLI_HOME";
    if (!env_exist(dockhvname, env_skey)) {
        sprintf(docker_path, "%s%s\0", getenv("USERPROFILE"), "\\docker-cli");
        RegOpenKeyExA(HKEY_CURRENT_USER, env_skey, 0, KEY_SET_VALUE , &hkey);
        RegSetValueExA(hkey, dockhvname, 0, REG_EXPAND_SZ, docker_path, strlen(docker_path));
        RegGetValueA(HKEY_CURRENT_USER, env_skey, "Path", RRF_RT_ANY, NULL, NULL , &len);
        newpathval = (char *)malloc(len * sizeof(char));
        RegGetValueA(HKEY_CURRENT_USER, env_skey, "Path", RRF_RT_ANY | RRF_NOEXPAND, NULL, newpathval, &len);
        sprintf(dockbpath, "%%%s%%\\bin;\0", dockhvname);
        newpathval = (char *)realloc(newpathval, (len + strlen(dockbpath)) * sizeof(char));
        memcpy(&newpathval[len - 1], dockbpath, strlen(dockbpath) + 1);
        RegSetValueExA(hkey, "Path", 0, REG_EXPAND_SZ, newpathval, strlen(newpathval));
        free(newpathval);
    }
    return 0;
}

int cp_bin_cli(const char *base_path)
{
    char cp_cmd[512];
    sprintf(cp_cmd, "%s%s%s", "cp -r ..\\..\\cli\\bin ", base_path, "\\docker-cli");
    system(cp_cmd);
}

int cp_daemon(const char *base_path)
{
    char mkdir_cmd[512], cp_cmd[512];
    sprintf(mkdir_cmd, "%s%s%s", "mkdir ", base_path, "\\docker-cli\\daemon");
    sprintf(cp_cmd, "%s%s%s", "cp ..\\..\\daemon\\bin\\dockerd ", base_path, "\\docker-cli\\daemon\\");

    int status = 0;
    status = system(mkdir_cmd);
    status = system(cp_cmd);

    return status;
}

int start_on_boot(void)
{
    HKEY hkey;
    DWORD len;
    char dockerd_path[512];
    const char *dockdvname = "Dockerd";
    if (!env_exist(dockdvname, run_skey)) {
        sprintf(dockerd_path, "%s%s\0", getenv("USERPROFILE"), "\\docker-cli\\daemon\\dockerd");
        RegOpenKeyExA(HKEY_CURRENT_USER, run_skey, 0, KEY_SET_VALUE , &hkey);
        RegSetValueExA(hkey, dockdvname, 0, REG_SZ, dockerd_path, strlen(dockerd_path));
    }
    return 0;
}
