#ifndef __STACK_H
#define __STACK_H

/*
 * This code emulates a simple stack so that functions can use parameters.
 */

assign(_retval, 0)
assign(_stackptr, 0)

#define push(...)                               \
    foreach(_p, __VA_ARGS__)                    \
        assign(`_stack`$(_stackptr), $(_p))     \
        $inc(_stackptr)                         \
    endfor

#define pop(n)                              \
    if ($(_stackptr) < n)                   \
        status(`*** stack underflow ***`)   \
        quit()                              \
    endif                                   \
    $sub(_stackptr, n)

#define $pop(var) pop(1) assign(_(var), $(`_stack`$(_stackptr)))

#define return(...) __VA_OPT__(assign(_retval, __VA_ARGS__)) {return}

#endif
