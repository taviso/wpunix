#ifndef __WPVARS_H
#define __WPVARS_H

static uint16_t *mouse_x = (void *)0x8356CD0;
static uint16_t *mouse_y = (void *)0x8356CD8;
static struct MSTBL *ms_tbl = (void *)0x8329CB0;
static uint8_t *intrptflg = (void *)0x8329D4D;
static uint16_t *lastkey = (void *)0x8329D4E;
static uint8_t **np_tab = (void *)0x83298b4;
static uint8_t **cc_tab = (void *)0x83298b0;
static uint8_t **e0_tab = (void *)0x83298ac;
static struct CHTAB *ch_tab = (void *)0x832989C;
static struct CHTAB *ex_tab = (void *)0x8329894;
static char (*kybdnam)[16] = (void *)0x8329D54;
static uint32_t *scnbuf = (void *)0x8329D40;

#endif
