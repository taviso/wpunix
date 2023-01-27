# WordPerfect for UNIX Character Terminals

This is a set of build scripts and patches to make WordPerfect for UNIX
work on modern Linux.

WordPerfect is a powerful *word processor* for the terminal. There is more
information and a quick introduction on the
[wiki](https://github.com/taviso/wpunix/wiki/Getting-Started), and a
[FAQ](https://github.com/taviso/wpunix/wiki/FAQ) covering common questions and
problems.

It features print previews, an equation editor, mouse support, drop down menus,
spelling and grammar checking, builtin macro editor and more.

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

| Ubuntu / Debian           | Fedora              |
| --------------------------|---------------------|
| build-essential           | glibc-devel.i686    |
| gcc-multilib              | libgcc.i686         |
| bsdtar / libarchive-tools | bsdtar              |
| patchelf                  | patchelf            |
| rpm2cpio                  | glibc-static        |
|                           | glibc-static.i686   |


## Getting Started

Run `wp` to start WordPerfect. The default keybinding to open the menu is
<kbd>Esc</kbd>+<kbd>=</kbd>.

There is a quick start guide on the [wiki](https://github.com/taviso/wpunix/wiki/Getting-Started).

The complete user manual is available online [here](https://lock.cmpxchg8b.com/files/wp8gui.pdf).

If you've never used WordPerfect before, you might prefer the `cua` Keyboard Layout.
This enables the familiar <kbd>Ctrl</kbd>+<kbd>C</kbd> for *Copy*, <kbd>Ctrl</kbd>+<kbd>V</kbd>
for *Paste*, and so on. Follow the instructions
[here](https://github.com/taviso/wpunix/wiki/Getting-Started#keyboard-layout) to enable it.

> Note: Print previews require a terminal with sixel support, such as [XTerm](https://invisible-island.net/xterm/).

## Screenshots

The default look.

![image](https://user-images.githubusercontent.com/123814/187082851-731af626-3cf1-44ac-874b-517c8ff545e6.png)

There are interactive equation and graphics editors, they can use [sixels](https://en.wikipedia.org/wiki/Sixel) or ascii art.

![equation](https://user-images.githubusercontent.com/123814/187083922-d1c38e75-734d-4161-853c-407ec38bc9d4.gif)

There are built-in spelling and grammar checking tools, and a comprehensive interactive thesaurus.

![thesaurus](https://user-images.githubusercontent.com/123814/188775253-78742e7d-269f-4916-b493-411c5c208b33.gif)

## Licensing

In theory, this is a demonstration copy, but I haven't noticed any limitations.

