A Snake (1998) / Blockade (1976) clone.
Written in C. Only standard libraries.

# Controls
- Use **arrow keys** to move around
- You can speed up by **holding** the key
- Press `Q` (without shift) to quit while playing
- Another way to quit the game is to **lose**
- The only way to lose when you **eat yourself**

You can move outside the screen; you will find yourself on the opposite side, like in PacMan.
You can also resize the terminal window; this will help you make the game area smaller, so you won't need to move across great distances to "fill your stomach".

To provide some original experience, I've also added a "level up" feature that makes the game more enjoyable: with every 10th unit of your snake's length the amount of apples on the screen is increased by 1. This way, only the early game is painful; the longer you get, the faster and easier it is for you to get even longer than that. 

My personal record up to this moment is somewhat around 100 snekimeters. I wonder if the game may break you PC if your snake is too long. Probably you will lose to a hardlock long time before reaching this moment.

# Requirements
The versions below is just what I use;
other versions may work too.

- Linux (tested), MacOS (should work natively), Windows 10+ w/ WSL (should work)
- GCC v14.2.0 (or equivalent) compiler
- GNU Make v4.4.1

# Commands
Run one of the following commands from the root of the project
(either Linux/MacOS or WSL):
- `make` or `make all` will build the project and run
- `make build` will only build, `make run` will only run

When built, run the game from the terminal like this: `./Snek`.

# How to do the same
The following standard libraries were used:
- `termios.h` is used to configure terminal emulator (disable canonical mode*, disable echo**, configure keyboard input)
- `stdio.h` performs basic `printf`
- `stduni.h` reads raw input (`read`)
- `stdlib.h` is simply needed for `size_t` (IIRC), because it suits better for `for` loops (spoiler: it isn't better actually, don't mind it)
- `ioctl.h` reads current terminal window size; `signal.h` is used to catch the moment when this window is resized; the `200809L` (i.e. September 2008) C standard is needed to intercept signals properly.
- To manipulate other terminal aspects (clear terminal, change color or put a character at some position) I simply use [ASNI](https://en.wikipedia.org/wiki/ANSI_escape_code) escape codes

(*) The canonical mode is the default one; it only reads input when you press the "enter" key. The opposite is the "raw" mode, which reads input immediately (either on "timeout" or "minimal byte received" basis).

(**) When "echo" is enabled, you will see the key codes of the keys you press in the terminal.

Other important information that may be useful:
- The `struct termios` itself is just a helper data type, it does nothing by itself; you need to get the terminal attributes (`tcgetattr`) to fill it with the actual information, and then you use `tcsetattr` to override these settings. In the source code, I read the terminal configuration twice and save it into two different structures, because **it is important (!)** to reset the terminal to its default state (otherwise it may be corrupted). It is also the reason why `Ctrl+C` keybind is disabled (the program really needs to quit properly). The corrupted terminal *may be* reset to its default state; but it is your duty once you fail these steps.
- The `VTIME` property is actually just the delay (in 1/10 of seconds) between consequent inputs; if you use it along with `VMIN` (minimal amount of bytes received), the game won't start unless your press any key at least once.

For more information, read the source code (it is commented and documented).

# Known issues

- Terminal does not clear itself when the game starts (it is not a bug, it is a feature). But it is being cleared when the screen is resized (to avoid artifacts). 
- RNG (for random apple position) is determined and will always be the same (a "C feature") (it is actually funny and feels like some "hand crafted level design")
