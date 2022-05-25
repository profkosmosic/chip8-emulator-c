# chip8-emulator-c
CHIP-8 emulator written in C and using the SDL2 library.

# INSTALLATION INSTRUCTIONS
- You'll need to download and set up **MinGW**: https://sourceforge.net/projects/mingw-w64/
- Download the **SDL2 Development Library(MinGW32)**: https://www.libsdl.org/download-2.0.php and unzip it.
- Copy the **/bin/** and **/lib/** folders from **/x86_64-w64-mingw32/** into the project root directory.
- Copy the **/SDL2/** folder that is inside the **/include/** directory into the project's **/include/** directory.
- Make a folder called **/build/** inside the project root.
- Compile the project from the command line using the line **mingw32-make**.
- All done! You can run the emulator by dragging and dropping a ROM file onto main.exe which is now inside the /bin/ folder or you can run it from the command line and include the ROM file as a 2nd argument on launch.

# KEYBOARD LAYOUT

| 1  | 2 | 3  | C |
| ------------- | ------------- | ------------- | ------------- |
| 4  | 5  | 6  | D  |
| 7  | 8  | 9  | E  |
| A  | 0  | B  | F  |

# SCREENSHOTS
