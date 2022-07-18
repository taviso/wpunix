# WordPerfect for UNIX (TM) Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX
work on Linux.

You need the following file to build this, available
[here](https://winworldpc.com/product/wordperfect/7x-unix).

```
$ md5sum COREL_WPUNIX.iso
c777f6024a95d68a1fc85b42c8e2212b  COREL_WPUNIX.iso
```

# Screenshots

The classic "Reveal Codes" view.

![Screenshot](/doc/wpscreenshot-layout.png?raw=true "Layout")

It uses [Sixels](https://en.wikipedia.org/wiki/Sixel) to generate print previews in the terminal!

![Screenshot](/doc/wpscreenshot-sixels.png?raw=true "Print Preview")

# Getting Started

The default keybinding to open the menu is <kbd>Esc</kbd>,<kbd>=</kbd>. That means press the <kbd>Esc</kbd> key and then the <kbd>=</kbd> key.

If you want the classic "blue" look...

- Go to `File`>`Setup`>`Display`.
- Select `Colors/Fonts/Attributes`.
- Set the `BG` for all Attributes to `f`.

![image](https://user-images.githubusercontent.com/123814/179447833-cb688b39-2aad-4816-a031-974df80e4663.png)

## Tips 

- The default keybinding for the famous "Reveal Codes" window is <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>F3</kbd>.
- There is online help available, press <kbd>F3</kbd> at any time.
- Set your macro files directory to a somewhere you can write (like your `$HOME`) in `Location of Files` settings.


