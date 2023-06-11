#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "../inc/install.h"
#include "../../constants/inc/error_codes.h"
#include "../../common/common.h"

#define NFOLDERS 4
#define MKDIR_CMD "mkdir"
#define MAX_CMD_LEN 512

void perror_win(const char *msg)
{
        WCHAR *buff;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buff, 0, NULL);
        fprintf(stderr, "%s: %S\n", msg, buff);
        LocalFree(buff);
}

static const LPCSTR env_skey = "Environment";
static const LPCSTR run_skey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

enum FOLDER_PATH_NAME {
    DOCKERCLI_BIN_PATH_IDX = 0,
    DOCKERCLI_CLI_PATH_IDX,
    DOCKERCLI_DAEMON_PATH_IDX,
    DOCKERCLI_TMP_PATH_IDX
};

static const char folders_path[NFOLDERS][MAX_PATH] = {
    "\\docker-cli\\bin",
    "\\docker-cli\\cli",
    "\\docker-cli\\daemon",
    "\\docker-cli\\tmp"
};

static int env_exist(const char *name, const char *skey)
{
    int status = RegGetValueA(HKEY_CURRENT_USER, skey, name, RRF_RT_ANY, NULL, NULL, NULL);
    return !status ? 1 : 0;
}

static int edit_dns(void)
{
    int status = 0;
    status = exec("wsl -d docker-cli rm /etc/resolv.conf");
    status = exec("wsl -d docker-cli -- cp ../../conf/wsl.conf /etc");
    status = exec("wsl -d docker-cli chmod 644 /etc/wsl.conf");

    return status;
}

static int copy_start_scripts(void)
{
    int status = 0;
    status = exec("wsl -d docker-cli -- cp ../../scripts/start-daemon /usr/bin");
    status = exec("wsl -d docker-cli chmod 754 /usr/bin/start-daemon");

    return status;
}

static int mk_folders(const char *base_path)
{
    char mkdir_cmd[MAX_CMD_LEN];
    int i, status;

    for (i = 0; i < NFOLDERS; ++i) {
        memset(mkdir_cmd, 0, MAX_CMD_LEN);
        sprintf(mkdir_cmd, "%s %s%s", MKDIR_CMD, base_path, folders_path[i]);
#ifdef __DEBUG
        printf("%s\n", mkdir_cmd);
#endif
        if (status = exec(mkdir_cmd) < 0)
            return DOCKERCLIE_CREATEFOLDER;
    }

    return EOK;
}

static int copy_docker(const char *base_path)
{
    char cp_cmd[MAX_CMD_LEN];
    sprintf(cp_cmd, "%s %s%s", "cp -r ..\\..\\init\\bin\\docker-cli", base_path, "\\docker-cli\\bin\\");

    int status = EOK;
    status = exec(cp_cmd);

    return status;
}

static int copy_daemon(const char *base_path)
{
    char cp_cmd[MAX_CMD_LEN];
    sprintf(cp_cmd, "%s %s%s", "cp ..\\..\\daemon\\bin\\dockerd", base_path, "\\docker-cli\\daemon\\");

    int status = EOK;
    status = exec(cp_cmd);

    return status;
}

static int copy_bin_cli(const char *base_path)
{
    char cp_cmd[MAX_CMD_LEN];
    sprintf(cp_cmd, "%s %s%s", "cp -r ..\\..\\cli\\bin\\docker", base_path, "\\docker-cli\\cli\\");
    
    int status = EOK;
    status = exec(cp_cmd);

    return status;
}

static int copy_assets(const char *base_path)
{
    char cp_cmd[MAX_CMD_LEN];
    sprintf(cp_cmd, "%s %s%s", "cp ..\\..\\assets\\settings.ico", base_path, "\\docker-cli\\");
    
    int status = EOK;
    status = exec(cp_cmd);

    return status;
}

void show_banner(void)
{
    printf(
        "    ______                                       __  \n"
        "   / __   \\____  _____ __ __ ____ ______  _____ / /_ \n"
        "  / /  \\  / __ \\/ ___// /  /  __ /  __ / / ___// /(_)\n"
        " / /___/ / /_/ / /___/    / ____/  ___/ / /___/ // / \n"
        "/_______/\\____/\\____/_/\\__\\___//__/ _\\  \\____/_//_/  \n"
        "                                                     \n"
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
    int status = EOK;

    char fs_path[MAX_PATH], install_path[MAX_PATH], install_data_path[MAX_PATH], ifs_cmd[512], idt_cmd[512];
    const char *file_name = "alpine-minirootfs-3.17.1-x86_64.tar.gz";
    sprintf(fs_path, "%s\\%s", getenv("TMP"), file_name);
    sprintf(install_path, "%s\\docker-cli", base_path);
    sprintf(install_data_path, "%s\\docker-cli\\data", base_path);
    sprintf(ifs_cmd, "wsl --import docker-cli %s %s", install_path, fs_path);
    sprintf(idt_cmd, "wsl --import docker-cli-data %s %s", install_data_path, fs_path);

#ifdef __DEBUG
    printf("%s\n", ifs_cmd);
#endif
    if ((exec(ifs_cmd)) < 0)
        return ECANNOTIFS;

    if ((status = mk_folders(base_path)) != EOK)
        return status;

/*
    TODO: save docker data to another partition
    if (exec(idt_cmd) < 0)
        return ECANNOTIFS;
*/

    edit_dns();
    copy_start_scripts();

    /* terminate vm to set changes */
    exec("wsl -t docker-cli");

    const char *idocker_cmd = "wsl -d docker-cli -- apk add --update docker docker-cli-compose openrc curl";
    if ((exec(idocker_cmd)) < 0)
        return ECANNOTIDOCK;

    fputs("Copying docker... ", stdout);
    if (status = copy_docker(base_path))
        return status;
    puts("OK");

    fputs("Copying daemon... ", stdout);
    if (status = copy_daemon(base_path))
        return status;
    puts("OK");

    fputs("Copying bin... ", stdout);
    if (status = copy_bin_cli(base_path))
        return status;
    puts("OK");

    fputs("Copying assets... ", stdout);
    if (status = copy_assets(base_path))
        return status;
    puts("OK");

    return status;
}

int add_to_path(void)
{
    HKEY hkey;
    DWORD len;
    char docker_path[MAX_PATH], *newpathval, dockbpath[MAX_PATH];
    const char *dockhvname = "DOCKER_CLI_HOME";
    if (!env_exist(dockhvname, env_skey)) {
        sprintf(docker_path, "%s%s", getenv("USERPROFILE"), "\\docker-cli");
        RegOpenKeyExA(HKEY_CURRENT_USER, env_skey, 0, KEY_SET_VALUE , &hkey);
        RegSetValueExA(hkey, dockhvname, 0, REG_EXPAND_SZ, docker_path, strlen(docker_path));
        RegGetValueA(HKEY_CURRENT_USER, env_skey, "Path", RRF_RT_ANY, NULL, NULL , &len);
        newpathval = (char *)malloc(len * sizeof(char));
        RegGetValueA(HKEY_CURRENT_USER, env_skey, "Path", RRF_RT_ANY | RRF_NOEXPAND, NULL, newpathval, &len);
        /* fix: had to use full path because REG_EXPAND_SZ did not work with DOCKER_CLI_HOME */
        sprintf(dockbpath, "%s\\cli;", docker_path);
        newpathval = (char *)realloc(newpathval, (len + strlen(dockbpath)) * sizeof(char));
        memcpy(&newpathval[len - 1], dockbpath, strlen(dockbpath) + 1);
        RegSetValueExA(hkey, "Path", 0, REG_EXPAND_SZ, newpathval, strlen(newpathval));
        free(newpathval);
    }
    return EOK;
}

int start_on_boot(void)
{
    HKEY hkey;
    DWORD len;
    char docker_cli_path[MAX_PATH];
    const char *dockdvname = "Docker Cli";
    if (!env_exist(dockdvname, run_skey)) {
        sprintf(docker_cli_path, "%s%s", getenv("USERPROFILE"), "\\docker-cli\\bin\\docker-cli");
        RegOpenKeyExA(HKEY_CURRENT_USER, run_skey, 0, KEY_SET_VALUE , &hkey);
        RegSetValueExA(hkey, dockdvname, 0, REG_SZ, docker_cli_path, strlen(docker_cli_path));
    }
    return EOK;
}

int install_docker_service(void)
{
    SC_HANDLE mngrh;
    if (!(mngrh = OpenSCManagerA(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CREATE_SERVICE)))
        return ESYSTEM;

    SC_HANDLE srvh;
    /*
    srvh = OpenService(mngrh, "Docker cli", DELETE);
    DeleteService(srvh);
    return EOK;
    */

    char dockerd_path[MAX_PATH];
    sprintf(dockerd_path, "%s%s\0", getenv("USERPROFILE"), "\\docker-cli\\daemon\\dockerd");
    if (!(srvh = CreateServiceA(
        mngrh,
        "Docker cli",
        "Docker command line interface",
        SERVICE_START | SERVICE_STOP | SERVICE_PAUSE_CONTINUE | DELETE,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        dockerd_path,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    )))
        return ESYSTEM;

    /*
    srvh = OpenService(mngrh, "Docker cli", SERVICE_START);
    if (!StartServiceA(srvh, 0, NULL)) {
        perror_win("Start service");
        return ESYSTEM;
    }
    */

    if (!CloseServiceHandle(mngrh))
        return ESYSTEM;

    return EOK;
}
