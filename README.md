# CHIP-8 EMULATOR
CHIP-8 emulator written in C and using the SDL2 library.<br/>
You can learn more about the CHIP-8 here: https://en.wikipedia.org/wiki/CHIP-8<br/>
The emulator has only been tested on Windows so far but I do plan on porting it over to macOS and Linux at some point as well.
## INSTALLATION INSTRUCTIONS
- You'll need to download and set up **MinGW** if you haven't already: https://sourceforge.net/projects/mingw-w64/files/
- Download the **SDL2 Development Library(MinGW32)**: https://www.libsdl.org/download-2.0.php and unzip it.
- Copy the **/bin/** and **/lib/** folders from **/x86_64-w64-mingw32/** into the project root directory.
- Copy the **/SDL2/** folder that is inside the **/include/** directory into the project's **/include/** directory.
- Make a folder called **/build/** inside the project root.
- Compile the project from the command line using the line **mingw32-make**.
- All done! You can run the emulator by dragging and dropping a ROM file onto main.exe which is now inside the /bin/ folder or you can run it from the command line and include the ROM file path as a 2nd argument on launch.
## KEYBOARD LAYOUT

| 1  | 2 | 3  | C |
| ------------- | ------------- | ------------- | ------------- |
| 4  | 5  | 6  | D  |
| 7  | 8  | 9  | E  |
| A  | 0  | B  | F  |

## SCREENSHOTS
### SPACE INVADERS
![invaders](https://user-images.githubusercontent.com/61395703/170269121-64a136e5-a479-4944-9857-acb08998b749.png)
### TETRIS
![tetris](https://user-images.githubusercontent.com/61395703/170268833-60167ff8-3d6b-4257-b36c-90c15267b9e4.png)
## SOURCES
- Cowgod's CHIP-8 technical reference: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- Daniel McCarthy's CHIP-8 in C course
