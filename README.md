# WordPerfect for UNIX Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX
work on modern Linux.

WordPerfect is a powerful *word processor* for the terminal. There is more
information and a quick introduction on the
[wiki](https://github.com/taviso/wpunix/wiki/Getting-Started), and a
[FAQ](https://github.com/taviso/wpunix/wiki/FAQ) covering common questions and
problems.

It features print previews, an equation editor, mouse support, drop down menus
and more.

![Print Previews](https://user-images.githubusercontent.com/123814/186571054-af88f26c-850a-4d88-94b3-02482d15e039.gif)

## Building

> Note: If you just want a `deb` or `rpm` to try out, check the [Releases](https://github.com/taviso/wpunix/releases) tab.

You need the following file to build this, available
[here](https://archive.org/details/corel-wpunix-8/box_f.jpg).

```
$ md5sum COREL_WPUNIX_1.iso
036d10f279aecc61704b214e7bd58a6d COREL_WPUNIX_1.iso
```

### Packages

The following packages are required to build the WordPerfect patches.

| Ubuntu              | Fedora              |
| ------------------- | ------------------- |
| build-essential     | glibc-devel.i686    |
| gcc-multilib        | libgcc.i686         |
| bsdtar              | bsdtar              |
| patchelf            | patchelf            |

## Getting Started

Run `wp` to start WordPerfect. The default keybinding to open the menu is
<kbd>Esc</kbd>+<kbd>=</kbd>.

There is a full manual available online [here](https://lock.cmpxchg8b.com/files/wp8gui.pdf).

There is also a huge amount of useful information that is *mostly* relevant to
wpunix on the [wpdos](http://www.columbia.edu/~em36/wpdos/unix.html) site.

## Screenshots

The default look.

![image](https://user-images.githubusercontent.com/123814/179531850-7ed44cf6-2d7d-4d8c-960b-6086870bb4d4.png)

The classic "[Reveal Codes](https://wptoolbox.com/tips/RevealCodes.html)" view.

![Screenshot](/doc/wpscreenshot-layout.png?raw=true "Layout")

There is a interactive equation editor, it can use [Sixels](https://en.wikipedia.org/wiki/Sixel) or ascii art.

![image](https://user-images.githubusercontent.com/123814/184558891-d9503147-5b8d-49c8-930b-3c1ac010aecb.png)

## Licensing

In theory, this is a demonstration copy, but I haven't noticed any limitations.

