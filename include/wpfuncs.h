#ifndef __WPFUNCS_H
#define __WPFUNCS_H

static int (*GkWarning)(char *message) = (void *) 0x8234E80;
static int (*GkYesNo)(char *message) = (void *) 0x8234EA0;
static int (*GkOkCancel)(char *message) = (void *) 0x8234EC0;

#endif
