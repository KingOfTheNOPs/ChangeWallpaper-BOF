# ChangeWallpaper-BOF
x64 BOF that changes the user's desktop wallpaper

## Usage
Import the changewallpaper.cna script on Cobalt Strike.

```
Usage: ChangeWallpaper <image path> [--revert XX] 
ChangeWallpaper Example: 
   ChangeWallpaper \\win10@80\image.jpg --revert 20 
   ChangeWallpaper C:\Windows\Temp\image.jpg 
ChangeWallpaper Options: 
    --revert XX, Revert to original wallpaper after specified time in seconds, replace the XX with time 
    BOF currently sleeps for the duration specified therefore the beacon will be unavailble until after the time specified is over
```

## Installation
Ensure Mingw-w64 is installed on the host prior to compiling.

```
make
```

### Notes
The BOF can be also used to cohorce the user to a webdav server allowing the NTLM request to be relayed to another service. 

Originally the goal was to build a BOF that can change the desktop **lockscreen** wallpaper, as it can enable the WebDAV client service as a low level user and sometimes cohorces the machine account instead of the user account as outlined in: https://github.com/nccgroup/Change-Lockscreen but unfortunately it does not appear that this is feasible without access to the Windows Runtime API StorageFile.GetFileFromPathAsync();
