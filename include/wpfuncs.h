#ifndef __WPFUNCS_H
#define __WPFUNCS_H

static int (*GkWarning)(char *message) = (void *) 0x823b7d0;
static int (*GkYesNo)(char *message) = (void *) 0x0823b7f0;
static int (*GkOkCancel)(char *message) = (void *) 0x823b810;
static void (*xy_mouse)(void) = (void *) 0x8204060;
static int (*msqot)(void) = (void *) 0x81FA1B0;

#endif
