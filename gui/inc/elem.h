#ifndef __DOCKER_CLI_GUI_ELEM_INCLUDED_H__
#define __DOCKER_CLI_GUI_ELEM_INCLUDED_H__

#include <windows.h>
#include "../../constants/inc/error_codes.h"

#define WM_NOTIFYCALLBACK 0x8001

HICON load_icon(HINSTANCE hinst, const char *path);
HWND create_main_window(LPCSTR lpClassName, LPCSTR lpWindowName, HINSTANCE hInstance, LPVOID lpParam);
int create_tray_icon(HWND hwnd);

#endif // __DOCKER_CLI_GUI_ELEM_INCLUDED_H__
