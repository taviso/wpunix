# mactool

This is a utility to allow you to edit WordPerfect for UNIX macros in a real
editor.

It can decompile existing macros to plain text, and then recompile them after
making changes.

This allows you to use standard programming tools to help build and manage your
macros, such as revision control, preprocessing, diff, patch, grep, and so on.

# Usage

```
WordPerfect macro compiler/decompiler, part of wpunix
Usage: ./mactool [-d] [-c] [-s] [-t title] [-o outfile] [-i infile]
   -t: Optional macro title.
   -d: Decompile macro to text.
   -c: Compile macro to WPM.
   -o: Output file name.
   -i: Input file name.
   -s: Strip all spaces (use { } for a literal space).

 - If no files are specified, stdin and stdout are assumed.
 - Lines beginning with # are discarded to allow use of cpp.
 - Spaces are *not* ignored by wp, use tabs for indenting or use -s.
 - Enter a literal '{' with {{}.

```

## Decompiling

If you have a `.wpm` file you want to edit, first do this:

```
$ ./mactool -d -o macro.mac -i macro.wpm
magic:     FF WPC
prefixsz:  71
product:   1
type:      1
version:   1.1
key:       0000
type:      65531
count:     4
blksize:   40
offset:    0
type:      1
length:    15
offset:    56
title: Test Macro
```

The `.mac` file is plain text, it can be edited with any tool you like (vim,
emacs, notepad, anything).

> Note: The file should be encoded as UTF-8.

You can even decompile and edit recorded macros.

## Compiling

When you have finished editing, recompile the text into a .wpm file:

```
$ ./mactool -t "My Cool Macro" -c -o macro.wpm -i macro.mac
```

An optional title can be specified that is displayed by WordPerfect in the editor.

If that worked, you can now load it into WordPerfect and verify it looks correct.

## Preprocessing

If you have a really complicated macro, you can use `cpp` to preprocess your
macro.

This allows you to use `#include`, `#ifdef` and `#define` to build complex and
powerful macros that would have been very difficult with the macro editor alone.

### Example

If you've spent any time writing macros, you know it can be difficult to get
the tildes balanced. There are some routines defined in `macro.h` that take
care of applying all the tildes for you.

Let's take a look.

```cpp
#include "system.h"
#include "macro.h"
#include "utils.h"
#include "keycodes.h"

#pragma title Typographic Single Quotes

/* If we're not editing a document, do nothing */
passthru_key_unless(EDIT_DOC)

/* Lookup what character is left of the cursor */
case (leftchar())
    match(KEY_SPACE, NewQuote)
    match(KEY_ENTER, NewQuote)
    match(KEY_TAB, NewQuote)
    match(KEY_HPAGE, NewQuote)
    /* 0x041b ’ */
    match(1051, CloseQuote)
    /* 0x0213 ‛ */
    match(531, OpenQuote)
endcase

/* Not whitespace; assume we're closing an open quote */
literal(’)
quit()

/* Adjacent to an existing quote; toggle it */
label(OpenQuote)
    {Backspace}
    literal(‛)
    quit()

label(CloseQuote)
    {Backspace}
    literal(’)
    quit()

/* Whitespace; assume we're opening a new quote */
label(NewQuote)
    literal(‛)
    quit()
```

As you can see, there is not a single tilde in this routine, the cpp macros
take care of everything.

To compile this, you would use a commandline like this:

```
$ cpp tdquotes.mac | ./mactool -c -s -t "Typographic Single Quotes" -o tdquotes.wpm
```

See the [macros
directory](https://github.com/taviso/wpunix/tree/main/macro/macros) for more
examples.

# Notes

Here are a few things to be aware of when using mactool.

## Whitespace

Tabs are ignored by WordPerfect, use them wherever you like. If you want a
literal tab in your macro, use `{^I}` or `{Tab}`.

Spaces are *not* ignored. That means if you add a space, then WordPerfect will
literally send a space when executing your macro. This is why the macro editor
hilights them as `˚`.

This is *probably* not what you intended.

The best solution is to use `cpp` and the `-s` option, which takes care of all
the whitespace for you. If you ever really do need a literal space, use `{ }`.

If you don't want to use `cpp`, just be careful to use tabs when indenting your
code. vim users might find the `listchars` feature useful.
