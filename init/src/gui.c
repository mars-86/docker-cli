#include "../inc/gui.h"

HICON load_icon(HINSTANCE hinst, const char *path)
{
    return (HICON)LoadImageA(
        hinst,
        path,
        IMAGE_ICON,
        32,
        32,
        LR_LOADFROMFILE
    );
}

HWND create_main_window(LPCSTR lpClassName, LPCSTR lpWindowName, HINSTANCE hInstance, LPVOID lpParam)
{
    return CreateWindowExA(
        0,
        lpClassName,
        lpWindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,    
        NULL,
        hInstance,
        lpParam
    );
}
