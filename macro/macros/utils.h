#ifndef __UTILS_H
#define __UTILS_H

/* Specify the only state flags you support */
#define require(mode, ...)              \
    if (!stateset(mode))                \
        __VA_ARGS__                     \
        {quit}                          \
    fi

/* Get the key code to the left or right of the cursor */
#define leftcharnum() kton(leftchar())
#define rightcharnum() kton(rightchar())

/* Append to a variable */
#define append(var, str) assign(var, $(var)str)

/* Check if our position is sane */
#define leftedge() quote(leftchar()) = ""
#define rightedge() quote(rightchar()) = ""

/* Stringification */
#define _str(arg) #arg
#define str(arg) _str(arg)
#define quote(arg) _str(arg)

#endif
