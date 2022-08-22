#ifndef __UTILS_H
#define __UTILS_H

/* Specify the only editmode you support */
#define passthru_key_unless(mode)       \
    if (!edittype(mode))                \
        {original key}                  \
        {quit}                          \
    fi

/* Get the key code to the left of the cursor, or { } if nothing */
#define leftchar() kton(system(Left){ })

#endif
