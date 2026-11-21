A Snake (1998) / Blockade (1976) clone.
Written in C. Only standard libraries.

# Controls
- Use **arrow keys** to move around
- You can speed up by **holding** the key
- The only way to quit the game is to **lose**
- You lose when you **leave the screen boundaries**
(that includes cases when you shrink the window)
- You lose when you **eat yourself**
- The easiest way to eat yourself is to **move backwards**
(the game won't prevent you from doing that)

# Requirements
- Linux (tested), MacOS or Windows 10+ w/ WSL (should work)
- GCC v14.2.0 (or equivalent) compiler
- GNU Make v4.4.1

# Commands
Run one of the following commands from the root of the project
(either Linux/MacOS or WSL):
- `make` or `make all` will build the project and run
- `make build` will only build, `make run` will only run

# How to do the same
The following standard libraries were used:
- `termios.h` is used to configure terminal emulator (disable canonical mode*, disable echo**, configure keyboard input)
- `stdio.h` performs basic `printf`
- `stduni.h` reads raw input (`read`)
- `stdlib.h` is simply needed for `size_t` (IIRC), because it suits better for `for` loops
- `ioctl.h` reads current terminal window size; `signal.h` is used to catch the moment when this window is resized; the `200809L` (i.e. September 2008) C standard is needed to intercept signals properly (but it is still bugged and sometimes leads to undefined behaviour when the terminal is resized)
- To manipulate other terminal aspects (clear terminal, change color or put a character at some position) I simply use [ASNI](https://en.wikipedia.org/wiki/ANSI_escape_code) escape codes

(*) The canonical mode is the default one; it only reads input when you press the "enter" key. The opposite is the "raw" mode, which reads input immediately (either on "timeout" or "minimal byte received" basis).

(**) When "echo" is enabled, you will see the key codes of the keys you press in the terminal.

Other important information that may be useful:
- The `struct termios` itself is just a helper data type, it does nothing by itself; you need to get the terminal attributes (`tcgetattr`) to fill it with the actual information, and then you use `tcsetattr` to override these settings. In the source code, I read the terminal configuration twice and save it into two different structures, because **it is important (!)** to reset the terminal to its default state (otherwise it may be corrupted). It is also the reason why `Ctrl+C` keybind is disabled (the program really needs to quit properly). The corrupted terminal *may be* reset to its default state; but it is your duty once you fail these steps.
- The `VTIME` property is actually just the delay (in 1/10 of seconds) between consequent inputs; if you use it along with `VMIN` (minimal amount of bytes received), the game won't start unless your press any key at least once.

For more information, read the source code (it is commented and documented).

# Known issues

- Terminal does not clear itself when the game starts (it is not a bug, it is a feature) 
- RNG (for random apple position) is determined and will always be the same (a "C feature")
