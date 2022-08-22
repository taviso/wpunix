#ifndef __SYSTEM_H
#define __SYSTEM_H

/* System variables */
#define SYS_ATTRIB       1
#define SYS_CELL         2
#define SYS_CELLATTR    23
#define SYS_CELLSTATE   24
#define SYS_COLUMN       3
#define SYS_DOCUMENT     4
#define SYS_ENDNOTE      5
#define SYS_ENTRY       29
#define SYS_EQUATION     6
#define SYS_FIGURE       7
#define SYS_FOOTNOTE     8
#define SYS_LEFT         9
#define SYS_LINE        10
#define SYS_LIST        11
#define SYS_MENU        13
#define SYS_NAME        12
#define SYS_NUMBER      26
#define SYS_PAGE        14
#define SYS_PATH        15
#define SYS_POS         16
#define SYS_PRINT       17
#define SYS_RIGHT       18
#define SYS_ROW         22
#define SYS_ROWSTATE    27
#define SYS_SHELLVER    25
#define SYS_TABLEBOX    19
#define SYS_TEXTBOX     20
#define SYS_USERBOX     21

/* No longer documented in 8c, but still works */
#define SYS_EDITTYPE    33

/* SYS_ATTRIB Results */
#define ATTR_NORMAL            0
#define ATTR_EXTRALARGE        1
#define ATTR_VERYLARGE         2
#define ATTR_LARGE             4
#define ATTR_SMALL             8
#define ATTR_FINE             16
#define ATTR_SUPERSCRIPT      32
#define ATTR_SUBSCRIPT        64
#define ATTR_OUTLINE         128
#define ATTR_ITALICS         256
#define ATTR_SHADOW          512
#define ATTR_REDLINE        1024
#define ATTR_DBLUNDERLINE   2048
#define ATTR_BOLD           4096
#define ATTR_STRIKEOUT      8192
#define ATTR_UNDERLINE     16384
#define ATTR_SMALLCAPS     32768

/* The EditType values */
#define EDIT_MENU   0
#define EDIT_DOC    1
#define EDIT_HEADER 2
#define EDIT_FOOTER 2
#define EDIT_LINE   4
#define EDIT_WINDOW 8

/* The State flags */
#define STATE_DOC       3
#define STATE_NORMAL    4
#define STATE_EDITING   8
#define STATE_MACRODEF  16
#define STATE_MACROEXEC 32
#define STATE_MERGE     64
#define STATE_BLOCK     128
#define STATE_TYPEOVER  256
#define STATE_REVEAL    512
#define STATE_QUESTION  1024
#define STATE_LIST      2048
#define STATE_NOSHELL   32768

/* Test if EditType matches type */
#define edittype(n) {SYSTEM}EditType~==n

/* Check state flag */
#define stateset(n) {STATE}&n

/* Get the char to the left of the cursor */
#define leftchar() system(SYS_LEFT)

#endif
