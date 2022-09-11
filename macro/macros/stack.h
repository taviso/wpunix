#ifndef __STACK_H
#define __STACK_H

/*
 * This code emulates a simple stack so that functions can use parameters.
 */

assign(_retval, 0)
assign(_stack_ptr, 0)

#define push(...)                               \
    foreach(_p, __VA_ARGS__)                    \
        assign(`_stack_`$(_stack_ptr), $(_p))   \
        $inc(_stack_ptr)                        \
    endfor

#define pop(n)                              \
    if ($(_stack_ptr) < n)                  \
        status(`*** stack underflow ***`)   \
        quit()                              \
    endif                                   \
    $sub(_stack_ptr, n)

#define $pop(var) pop(1) assign(_(var), $(`_stack_`$(_stack_ptr)))

#define return(...) __VA_OPT__(assign(_retval, __VA_ARGS__)) {return}

#endif
