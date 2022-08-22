#ifndef __MACROS_H
#define __MACROS_H

/* Wrappers to handle tildes automatically for you */
#define assign(var,expr) {ASSIGN}var~expr~
#define call(label) {CALL}label~
#define case(expr) {CASE}expr~
#define casecall(expr) {CASE CALL}expr~
#define chain(macroname) {CHAIN}macroname~
#define char(var,message) {CHAR}var~message~
#define for(var,start,stop,step) {FOR}var~start~stop~step~
#define goto(label) {GO}label~
#define if(expr) {IF}expr~
#define ifexists(var) {IF EXISTS}var~
#define input(message) {INPUT}message~
#define kton(key) {KTON}key~
#define label(l) {LABEL}l~
#define len(var) {LEN}var~
#define look(var) {LOOK}var~
#define mid(var,offset,count) {MID}var~offset}count~
#define ntok(number) {NTOK}number~
#define oncancel(action) {ON CANCEL}action~
#define onerror(action) {ON ERROR}action~
#define onnotfound(action) {ON NOT FOUND}action~
#define pause(key) {PAUSE KEY}key~
#define prompt(message) {PROMPT}message~
#define shellassign(shellvar,expr) {SHELL ASSIGN}shellvar~expr~
#define shellmacro(macroname) {SHELL MACRO}macroname~
#define shellvariable(shellvar) {SHELL VARIABLE}shellvar~
#define speed(val) {SPEED}val~
#define status(message)~ {STATUS PROMPT}message~
#define system(sysvar) {SYSTEM}sysvar~
#define wait(val) {WAIT}val~
#define while(expr) {WHILE}expr~

#define quit() {QUIT}
#define return {RETURN}

#define bell() {BELL}
#define break {BREAK}

/* sh-like syntactic sugar */
#define fi {END IF}
#define endif {END IF}
#define else {ELSE}
#define esac ~
#define end ~
#define endfor {END FOR}
#define endwhile {END WHILE}
#define endcase ~
#define match(c,label) c~label~
#define go goto
#define default {OTHERWISE}

/* Make it clear explicit characters are intentional */
#define literal(c) c

#endif
