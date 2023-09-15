#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include "beacon.h"
#define MAX_PATH_LENGTH 260

// https://github.com/nccgroup/Change-Lockscreen/blob/master/Change-Lockscreen/Change-Lockscreen/Program.cs

WINUSERAPI WINBOOL WINAPI USER32$SystemParametersInfoW(UINT uiAction,UINT uiParam,PVOID pvParam,UINT fWinIni);
WINBASEAPI DWORD   WINAPI KERNEL32$GetLastError (VOID);
WINBASEAPI VOID    WINAPI KERNEL32$Sleep (DWORD dwMilliseconds);
WINBASEAPI size_t    __cdecl MSVCRT$mbstowcs(wchar_t * _Dest,const char * _Source,size_t _MaxCount);

void go(char *buf, int len) {
    WCHAR oldWallPaper[MAX_PATH_LENGTH];
    WCHAR imagePath[MAX_PATH];

    int revertTime = 0;

    datap parser;
    
    BeaconDataParse(&parser, buf, len);

    const char *input = BeaconDataExtract(&parser, NULL);
    //BeaconPrintf(CALLBACK_OUTPUT, "Input: %s\n", input);
    revertTime = BeaconDataInt(&parser);
    //BeaconPrintf(CALLBACK_OUTPUT, "Revert Time: %d\n", revertTime);

    //set new wallpaper
    MSVCRT$mbstowcs(imagePath, input, MAX_PATH);


    //commented out for BOF, arguements are parsed by CNA script
    //parse command line arguements
    // for (int i = 1; i < argc; ++i)
    // {
    //     if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
    //         BeaconPrintf("Usage: %s <image path> [--revert] \n", argv[0]);
    //         BeaconPrintf("ChangeWallpaper Example:\n");
    //         BeaconPrintf("  ChangeWallpaper.exe \"\\\\win10@80\\image.jpg\" --revert 2\n");
    //         BeaconPrintf("ChangeWallpaper Options:\n");
    //         BeaconPrintf("  -h, --help\t\t\tShow this help message and exit\n");
    //         BeaconPrintf("  --revert\t\t\tRevert to original wallpaper after specified time in seconds\n");
    //         return 0;
    //     }
    //     if(strcmp(argv[i], "--revert")){
    //         revertTime = atoi(argv[i]);
    //         BeaconPrintf("Reverting to original wallpaper in %d seconds\n", revertTime);
    //     }
    // }

    //get original wallpaper path
    if(USER32$SystemParametersInfoW(SPI_GETDESKWALLPAPER, MAX_PATH_LENGTH, oldWallPaper, 0)) {
        BeaconPrintf(CALLBACK_OUTPUT, "Original wallpaper path: %ls\n", oldWallPaper);
    } else {
        BeaconPrintf(CALLBACK_ERROR, "Error: %d\n", KERNEL32$GetLastError());
    }
    
    if(USER32$SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, imagePath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE )) {
        BeaconPrintf(CALLBACK_OUTPUT, "Wallpaper changed to: %ls \n", imagePath);
        //revert to original wallpaper if specified
        if (revertTime != 0) {
            BeaconPrintf(CALLBACK_OUTPUT, "Taking a nap for %d seconds.\n", revertTime);
            KERNEL32$Sleep(revertTime*1000);
            BeaconPrintf(CALLBACK_OUTPUT, "Setting back to original wallpaper.\n");
            if(USER32$SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, oldWallPaper, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE )) {
                BeaconPrintf(CALLBACK_OUTPUT, "Wallpaper changed to: %ls \n",oldWallPaper);
            } 
            else {
                BeaconPrintf(CALLBACK_ERROR, "Error: %d\n", KERNEL32$GetLastError());
            }
        }
    } else {
        BeaconPrintf(CALLBACK_ERROR, "Error: %d\n", KERNEL32$GetLastError());
    }
    return;
}