# WordPerfect for UNIX Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX (Released 1997)
work on modern Linux.

You need the following file to build this, available
[here](https://winworldpc.com/product/wordperfect/7x-unix).

```
$ md5sum COREL_WPUNIX.iso
c777f6024a95d68a1fc85b42c8e2212b  COREL_WPUNIX.iso
```

> Note: If you just want a `.deb` to try out, check the [Releases](https://github.com/taviso/wpunix/releases) tab.

# Screenshots

The default look.

![image](https://user-images.githubusercontent.com/123814/179531850-7ed44cf6-2d7d-4d8c-960b-6086870bb4d4.png)

The classic "[Reveal Codes](https://wptoolbox.com/tips/RevealCodes.html)" view.

![Screenshot](/doc/wpscreenshot-layout.png?raw=true "Layout")

It uses [Sixels](https://en.wikipedia.org/wiki/Sixel) to generate print previews in the terminal!

![Screenshot](/doc/wpscreenshot-sixels.png?raw=true "Print Preview")

The built-in macro editor.

![image](https://user-images.githubusercontent.com/123814/179529235-98f10f47-ad77-443e-8c54-b1b5139b585e.png)

There is a interactive equation editor, it can use sixels or ascii art.

![image](https://user-images.githubusercontent.com/123814/180581956-271e6959-b9f4-4e8f-b7bd-b59c7bf486ff.png)


I found this fun print ad for this release [here](https://archive.org/details/word-perfect-5.1-for-unix-ad-1993/page/n1/mode/2up) with a screenshot of the text interface and the GUI.

# Licensing

In theory, this is an evaluation copy, but I haven't noticed any limitations.

# Getting Started

Run `wp` to start WordPerfect.

The default keybinding to open the menu is <kbd>Esc</kbd>,<kbd>=</kbd>. That means press the <kbd>Esc</kbd> key and then the <kbd>=</kbd> key.

You can exit with <kbd>F7</kbd>.

There is a full manual available online [here](https://lock.cmpxchg8b.com/files/wp8gui.pdf).

There is also a huge amount of useful information that is *mostly* relevant to wpunix on the [wpdos](http://wpdos.org) and [wptoolbox](https://wptoolbox.com) sites.

If you want the classic "white on blue" look...

- Go to `File`>`Setup`>`Display`.
- Select `Colors/Fonts/Attributes`.
- Set the `BG` for all Attributes to `f`.
- Press <kbd>F7</kbd> to close.

![image](https://user-images.githubusercontent.com/123814/179447833-cb688b39-2aad-4816-a031-974df80e4663.png)

## Tips 

- The default keybinding for the famous "Reveal Codes" window is <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>F3</kbd>.
- There is online help available, press <kbd>F3</kbd> at any time.
- Set your macro files directory to a somewhere you can write (like your `$HOME`) in `Location of Files` settings.
- If your terminal supports sixels, enable the `vt340hi_sixel` driver to get print previews.

# Terminals

WordPerfect comes with a utility called `wpterm` for adjusting the terminal definition.

The bundled terminal definitions were decades out of date, so I updated them to work with modern xterm-compatible terminals.



