#ifndef __DOCKER_CLI_INIT_GUI_INCLUDED_H__
#define __DOCKER_CLI_INIT_GUI_INCLUDED_H__

#include <windows.h>

HICON load_icon(HINSTANCE hinst, const char *path);
HWND create_main_window(LPCSTR lpClassName, LPCSTR lpWindowName, HINSTANCE hInstance, LPVOID lpParam);

#endif // __DOCKER_CLI_INIT_GUI_INCLUDED_H__
