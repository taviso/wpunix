#define _GNU_SOURCE
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "wptypes.h"
#include "wpfuncs.h"
#include "wpvars.h"
#include "hook.h"

// There is a bug in mapkey() that prevent us entering the zeroth entry of
// extended character sets. This reimplementation avoids that error by
// checking the set and code, rather than just the code.
// https://github.com/taviso/wpunix/issues/29

#define KEY_SETNUM 0xf

#pragma pack(push, 1)
union KEYCODE {
    struct {
        unsigned c     : 8;
        unsigned set   : 4;
        unsigned type  : 4;
    };
    uint16_t val;
};
#pragma pack(pop)

// Mapkey returns a WPCHAR, {0,0} is error.
uint16_t zero_ext_mapkey(uint16_t code)
{
    union KEYCODE keycode = { .val = code };
    uint8_t *spctab;
    uint16_t wpc = 0;
    uint16_t spc = code;

    if (keycode.c || keycode.type == KEY_SETNUM) {
        if ((code & 0xff00) == 0) {
            *lastkey = code;
            if (findwp(code, &wpc))
                return 0;
            return wpc;
        }

        // E0 has it's own special character table, e0_tab.
        if (keycode.c != 0xE0) {
            // The first nibble appears to be flags or type.
            if (keycode.type == KEY_SETNUM)
                return keycode.val & 0xFFF;

            // Choose the correct special tab.
            // np = non printing?
            // cc = character code?
            spctab = keycode.c > 0x1f ? *np_tab : *cc_tab;

            if (keycode.c > 0x2F || keycode.c == 0x20 || lkspc(&spc, &spctab)) {
                // Lookup key extended set?
                if (lukupes(spc, ch_tab, &wpc)) {
                    // No idea what this keycode is.
                    if (spc == 0x71E) {
                        resetkb();
                        (*kybdnam)[0] = 0;
                        *scnbuf = 0;
                        return 0;
                    }

                    *lastkey = spc & 0xFF;

                    if (findwp(spc, &wpc)) {
                        *scnbuf = 0;
                        return 0;
                    }
                } else if (!wpc) {
                    *scnbuf = 0;
                }

                return wpc;
            }

            if (!spc) {
                *scnbuf = 0;
            }

            return spc;
        }

        // e0 has a special table, but I don't know what causes it.
        spctab = *e0_tab;

        // Lookup special?
        if (!lkspc(&spc, &spctab)) {
            if (!spc) {
                *scnbuf = 0;
            }
            return spc;
        }
    }

    // We reach here for function keys, for example.
    spc = __builtin_bswap16(spc);

    // Lookup key extended set?
    if (lukupes(spc, ex_tab, &spc)) {
        if (spc == 3) {
            return 3;
        }
    } else if (spc) {
        return spc;
    }

    // Function key not known?
    *scnbuf = 0;
    return 0;
}
