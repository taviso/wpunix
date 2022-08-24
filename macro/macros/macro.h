#ifndef __MACROS_H
#define __MACROS_H

//#pragma GCC poison foobar

/*
 *
 *  The idea here is that all macros should use _(arg), which strips any optional parens.
 *
 *  https://www.mikeash.com/pyblog/friday-qa-2015-03-20-preprocessor-abuse-and-optional-parentheses.html
 *
 */
#define _EXTRACT(...) _EXTRACT __VA_ARGS__
#define _NOTHING_EXTRACT
#define _PASTE(x, ...) x ## __VA_ARGS__
#define _EVALUATING_PASTE(x, ...) _PASTE(x, __VA_ARGS__)
#define __(q) _EVALUATING_PASTE(_NOTHING, _EXTRACT q)
/* Yeesh, I need a second step to avoid spaces which matter in `foo: $(bar)` */
#define _(q) _EVALUATING_PASTE(,__(q))

/* Wrappers to handle tildes automatically for you */
#define assign(var,expr) {ASSIGN}_(var)~_(expr)~
#define call(label) {CALL}_(label)~
#define case(expr) {CASE}_(expr)~
#define casecall(expr) {CASE CALL}_(expr)~
#define chain(macroname) {CHAIN}_(macroname)~
#define char(var,message) {CHAR}_(var)~_(message)~
#define for(var,start,stop,step) {FOR}_(var)~_(start)~_(stop)~_(step)~
#define go(label) {GO}_(label)~
#define goto(label) {GO}_(label)~
#define if(expr) {IF}_(expr)~
#define ifexists(var) {IF EXISTS}_(var)~
#define input(message) {INPUT}_(message)~
#define kton(key) {KTON}_(key)~
#define label(l) {LABEL}_(l)~
#define look(var) {LOOK}_(var)~
#define mid(var,offset,count) {MID}_(var)~_(offset)~_(count)~
#define ntok(number) {NTOK}_(number)~
#define oncancel(action) {ON CANCEL}_(action)~
#define onerror(action) {ON ERROR}_(action)~
#define onnotfound(action) {ON NOT FOUND}_(action)~
#define pause(key) {PAUSE KEY}_(key)~
#define prompt(message) {PROMPT}_(message)~
#define shellassign(shellvar,expr) {SHELL ASSIGN}_(shellvar)~_(expr)~
#define shellmacro(macroname) {SHELL MACRO}_(macroname)~
#define shellvariable(shellvar) {SHELL VARIABLE}_(shellvar)~
#define speed(val) {SPEED}_(val)~
#define status(message) {STATUS PROMPT}_(message)~
#define statusprompt(message) status(message)
#define system(sysvar) {SYSTEM}_(sysvar)~
#define variable(var) {VARIABLE}_(var)~
#define wait(val) {WAIT}_(val)~
#define while(expr) {WHILE}_(expr)~

#define quit() {QUIT}
#define return {RETURN}

#define bell() {BELL}
#define break {BREAK}

/* Some syntactic sugar */
#define fi {END IF}
#define endif {END IF}
#define else {ELSE}
#define esac ~
#define end ~
#define endfor {END FOR}
#define endwhile {END WHILE}
#define endcase ~
#define match(c,label) _(c)~_(label)~
#define default {OTHERWISE}
#define declare(var) {ASSIGN}_(var)~~
#define continue {NEXT}

#define $len(var) {LEN}_(var)~
#define len(var) {LEN}_(var)~
#define $(var) {VARIABLE}_(var)~
#define $$(var) $(var)

/* Make it clear explicit characters are intentional */
#define literal(str...) str

/* {FOREACH} is hard to handle because it's variadic */
#define __COUNT(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, ...) z
#define _COUNT(args...) __COUNT(args, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define  _T0()
#define  _T1(a) _(a)
#define  _T2(a, b) _T1(a)~_(b)
#define  _T3(a, b, c) _T2(a, b)~_(c)
#define  _T4(a, b, c, d) _T3(a, b, c)~_(d)
#define  _T5(a, b, c, d, e) _T4(a, b, c, d)~_(e)
#define  _T6(a, b, c, d, e, f) _T5(a, b, c, d, e)~_(f)
#define  _T7(a, b, c, d, e, f, g) _T6(a, b, c, d, e, f)~_(g)
#define  _T8(a, b, c, d, e, f, g, h) _T7(a, b, c, d, e, f, g)~_(h)
#define  _T9(a, b, c, d, e, f, g, h, i) _T8(a, b, c, d, e, f, g, h)~_(i)
#define _T10(a, b, c, d, e, f, g, h, i, j) _T9(a, b, c, d, e, f, g, h, i)~_(j)
// ...and so on.
#define _foreach(x, y) __foreach(x, y)
#define __foreach(x, y) x ## y
#define foreach(args...) {FOR EACH}_foreach(_T, _COUNT(args)(args))~~

#endif
