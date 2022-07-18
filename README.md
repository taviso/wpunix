# WordPerfect for UNIX (TM) Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX
work on modern Linux.

You need the following file to build this, available
[here](https://winworldpc.com/product/wordperfect/7x-unix).

```
$ md5sum COREL_WPUNIX.iso
c777f6024a95d68a1fc85b42c8e2212b  COREL_WPUNIX.iso
```

# Screenshots

The default look.

![image](https://user-images.githubusercontent.com/123814/179528294-14f8416d-16cc-41c0-898e-6656fd3cc5a6.png)

The classic "Reveal Codes" view.

![Screenshot](/doc/wpscreenshot-layout.png?raw=true "Layout")

It uses [Sixels](https://en.wikipedia.org/wiki/Sixel) to generate print previews in the terminal!

![Screenshot](/doc/wpscreenshot-sixels.png?raw=true "Print Preview")

The built-in macro editor.

![image](https://user-images.githubusercontent.com/123814/179529235-98f10f47-ad77-443e-8c54-b1b5139b585e.png)

# Licensing

In theory, this is an evaluation copy, but I haven't noticed any limitations.

# Getting Started

Run `wp` to start WordPerfect.

The default keybinding to open the menu is <kbd>Esc</kbd>,<kbd>=</kbd>. That means press the <kbd>Esc</kbd> key and then the <kbd>=</kbd> key.

You can exit with <kbd>F7</kbd>.

There is a full scanned manual available online [here](http://www.columbia.edu/~em36/wpdos/WPDOS51Reference.pdf), WordPerfect DOS 5.1 and WordPerfect UNIX 7.0 are very similar.

There is a huge amount of useful information that is *mostly* relevant to wpunix on the [wpdos](http://wpdos.org) and [wptoolbox](https://wptoolbox.com) sites.

If you want the classic "blue" look...

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



