#ifndef __SYSTEM_H
#define __SYSTEM_H

/* The EditType values */
#define EDIT_MENU   0
#define EDIT_DOC    1
#define EDIT_HEADER 2
#define EDIT_FOOTER 2
#define EDIT_LINE   4
#define EDIT_WINDOW 8

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
#define stateset(n) {STATE}&n==n

#endif
