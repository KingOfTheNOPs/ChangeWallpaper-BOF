all:
	x86_64-w64-mingw32-gcc -c changewallpaper.c -masm=intel -Wall -DBOF -o ChangeWallpaper.o
	x86_64-w64-mingw32-strip --strip-unneeded ChangeWallpaper.o
clean:
	rm ChangeWallpaper.o    