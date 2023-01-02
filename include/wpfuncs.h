#ifndef __WPFUNCS_H
#define __WPFUNCS_H

static int (*GkWarning)(char *message) = (void *) 0x823b7d0;
static int (*GkYesNo)(char *message) = (void *) 0x0823b7f0;
static int (*GkOkCancel)(char *message) = (void *) 0x823b810;
static void (*xy_mouse)(void) = (void *) 0x8204060;
static int (*msqot)(void) = (void *) 0x81FA1B0;
static void (*dodelay)(int delay) = (void *) 0x8212430;
static void (*win_resize)(int cols, int rows) = (void *) 0x8203330;
static void (*frewrt)(void) = (void *) 0x8086530;
static bool (*findwp)(uint8_t keycode, uint16_t *wpc) = (void *) 0x81CE560;
static int (*lkspc)(uint16_t *spc, uint8_t **table) = (void *) 0x81F9E50;
static int (*lukupes)(uint8_t code, struct CHTAB *table, uint16_t *wpc) = (void *)0x81F9970;
static void (*resetkb)() = (void *)0x81F9FF0;
static uint16_t (*mapkey)(uint16_t code) = (void *)0x81f9780;

#endif
