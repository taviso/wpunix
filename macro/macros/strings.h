#ifndef __STRINGS_H
#define __STRINGS_H

function(_strchr,
    $pop(_c)
    $pop(_str)
    for (_i, 1, $len($(_str)), 1)
        if ($(_c) = mid($(_str), $(_i), 1))
            return($(_i))
        endif
    endfor
    return(-1)
)

#endif
