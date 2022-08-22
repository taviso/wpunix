#ifndef __SYSTEM_H
#define __SYSTEM_H

/* The EditType values */
#define EDIT_MENU   0
#define EDIT_DOC    1
#define EDIT_HEADER 2
#define EDIT_FOOTER 2
#define EDIT_LINE   4
#define EDIT_WINDOW 8

/* Test if EditType matches type */
#define edittype(n) {SYSTEM}EditType~==n

#endif
