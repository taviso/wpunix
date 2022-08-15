# WordPerfect for UNIX Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX work on modern Linux.

WordPerfect is a powerful *word processor* for the terminal. There is more information and a quick introduction on the [wiki](https://github.com/taviso/wpunix/wiki/Getting-Started), and a [FAQ](https://github.com/taviso/wpunix/wiki/FAQ) covering common questions and problems.

## Building

> Note: If you just want a `deb` or `rpm` to try out, check the [Releases](https://github.com/taviso/wpunix/releases) tab.

You need the following file to build this, available from [archive.org](https://archive.org/details/corel-word-perfect-7-for-unix-linux-7.0-1998-06-english-cd).

```
$ md5sum COREL_WPUNIX.iso
c777f6024a95d68a1fc85b42c8e2212b  COREL_WPUNIX.iso
```

### Packages

The following packages are required

| Ubuntu              | Fedora              |
| ------------------- | ------------------- |
| build-essential     | glibc-devel.i686    |
| gcc-multilib        | libgcc.i686         |
| bsdtar              | bsdtar              |
| patchelf            | patchelf            |

## Getting Started

Run `wp` to start WordPerfect. The default keybinding to open the menu is <kbd>Esc</kbd>+<kbd>=</kbd>.

There is a full manual available online [here](https://lock.cmpxchg8b.com/files/wp8gui.pdf).

There is also a huge amount of useful information that is *mostly* relevant to wpunix on the [wpdos](http://www.columbia.edu/~em36/wpdos/unix.html) site.

## Screenshots

The default look.

![image](https://user-images.githubusercontent.com/123814/179531850-7ed44cf6-2d7d-4d8c-960b-6086870bb4d4.png)

The classic "[Reveal Codes](https://wptoolbox.com/tips/RevealCodes.html)" view.

![Screenshot](/doc/wpscreenshot-layout.png?raw=true "Layout")

It uses [Sixels](https://en.wikipedia.org/wiki/Sixel) to generate print previews in the terminal!

![Screenshot](/doc/wpscreenshot-sixels.png?raw=true "Print Preview")

The built-in macro editor.

![image](https://user-images.githubusercontent.com/123814/179529235-98f10f47-ad77-443e-8c54-b1b5139b585e.png)

There is a interactive equation editor, it can use sixels or ascii art.

![image](https://user-images.githubusercontent.com/123814/184558891-d9503147-5b8d-49c8-930b-3c1ac010aecb.png)

## Licensing

In theory, this is an evaluation copy, but I haven't noticed any limitations.

