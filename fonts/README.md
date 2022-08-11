# afm2add

I was once like you, thinking to myself "how hard can it possibly be to add a
font to a word processor?".

The problem is WordPerfect is not a *Text Editor*, it's a *Word Processor*. It
needs to know the physical page dimensions and proportional font geometry in
order to construct it's internal model of the page.

To teach WordPerfect to use a new font, you need a *PRS* (Printer Resource)
File. A PRS file describes what fonts are available, how to select a font,
how the spacing, scaling, kerning works and what glyphs are available.

A PRS file can also be part of a bundle of resource files called an *ALL* file,
which is just a collection of multiple PRS files in one file. This is useful
because one ALL file can describe all the available capabilities of one
printer, or share definitions between similar printers.

In order to generate a PRS file, you need a few other files.

    - A *ADD* (Resource Operations) file.
    - A *SPC* (Character Spacing) file.
    - A *TFC* (Typeface Definition) file.
    - A *MAP* (Character Map) file.
    - A *KRN* (Kerning Pairs Table) file.

You can then then give these files to `ADDALL.EXE`, to insert into any Printer
Resource File you want.

This utility (`afm2add`) will take an *AFM* (Adobe Font Metrics) file and
generate them for you.
