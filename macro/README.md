# mactool

This is a small utility to let you edit word perfect macros in a real editor.
It can decompile existing macros to plain text, then recompile them after
you've edited them.

# Usage

```
Usage: ./mactool [-d] [-c] [-t title] [-o outfile] [-i infile]
   -t: Optional macro title.
   -d: Decompile macro to text.
   -c: Compile macro to WPM.
   -o: Output file name.
   -i: Input file name.

If no files are specified, stdin and stdout are assumed.

```

## Decompiling

If you have a .wpm file you want to edit in vim, first do this:

```
$ ./mactool -d -o macro.txt -i macro.wpm
```

Now you can edit the macro however you like, vim, emacs, whatever.

> Note: Any non-ASCII characters in macro.txt must be UTF-8.

## Compiling

When you have finished editing, recompile the text into a .wpm file:

```
$ ./mactool -t "My Cool Macro" -c -o macro.wpm -i macro.txt
```

If that worked, you can now load it into WordPerfect and verify it looks correct.

## Example

```
$ ./mactool -d < tdquotes.wpm  > tdquotes.txt
debug: decompile mode
magic:     FF WPC
prefixsz:  82
product:   1
type:      1
version:   1.1
key:       0000
type:      65531
count:     4
blksize:   40
offset:    0
type:      1
length:    26
offset:    56
title: Typographic Double Quotes
```

```
$ ./mactool -c < tdquotes.txt  > output.wpm
debug: IF => WP_MACRO, 21
debug: SYSTEM => WP_MACRO, 53
debug: ORIGINAL KEY => WP_MACRO, 46
debug: QUIT => WP_MACRO, 32
debug: END IF => WP_MACRO, 16
debug: CASE => WP_MACRO, 7
debug: KTON => WP_MACRO, 56
debug: SYSTEM => WP_MACRO, 53
debug: ; => WP_MACRO, 11
debug: discovered UCS-4LE 0000201d
debug: QUIT => WP_MACRO, 32
debug: ; => WP_MACRO, 11
debug: LABEL => WP_MACRO, 22
debug: Backspace => WP_CTRL, 80
debug: discovered UCS-4LE 0000201c
debug: QUIT => WP_MACRO, 32
debug: LABEL => WP_MACRO, 22
debug: Backspace => WP_CTRL, 80
debug: discovered UCS-4LE 0000201d
debug: QUIT => WP_MACRO, 32
debug: ; => WP_MACRO, 11
debug: LABEL => WP_MACRO, 22
debug: discovered UCS-4LE 0000201c
debug: QUIT => WP_MACRO, 32
```

## Tips

If you have a really complicated macro, you can use cpp to preprocess your macro.

That means you can use `#include` to split your macro up into multiple files,
`#ifdef` to enable or disable features, and other preprocess features.

