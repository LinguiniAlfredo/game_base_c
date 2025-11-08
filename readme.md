
### To compile for Linux, you need the following dependencies
```console
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### To compile for Windows, you need the following dependencies
```console
sudo pacman -S mingw-w64-gcc
yay -S mingw-w64-sdl2 mingw-w64-sdl2_image mingw-w64-sdl2_ttf mingw-w64-sdl2_mixer
```

Then copy SDL2.dll, SDL2_image.dll, SDL2_ttf.dll, SDL2_mixer.dll from /usr/x86_64-wg4-mingw32/bin into the windows release folder
