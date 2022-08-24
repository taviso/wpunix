#define _GNU_SOURCE
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "wptypes.h"
#include "wpfuncs.h"
#include "wpvars.h"
#include "hook.h"

// WordPerfect expects 4 decimal characters on mouse press, XXYY. I don't know
// what terminal generates this sequence, certainly no mouse tracking
// mode that XTerm supports looks like that.
//
// https://www.xfree86.org/current/ctlseqs.html#Mouse%20Tracking
//
// The closest is DEC Locator mode, which sends CSI M C b C x C y, but those are
// not decimal coordinates -- they're binary. It's also 3 bytes of parameters, not
// 4. The solution is to patch the callback table and decode the parameters
// ourselves.
static void xy_xterm_mouse()
{
    // The button is currently ignored, but we could examine it.
    uint16_t button = msqot() - 0x20;
    *mouse_x = msqot() - 0x21;
    *mouse_y = msqot() - 0x21;
}

void _init()
{
    // This code is called on startup, we can hook or replace wp internals.

    // Patch the mouse event callback to use an XTerm compatible version.
    ms_tbl[0].nbytes     = 3;
    ms_tbl[0].mscallback = xy_xterm_mouse;
}
