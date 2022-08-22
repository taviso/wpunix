#ifndef __MACROS_H
#define __MACROS_H

/* Wrappers to handle tildes automatically for you */
#define if(c) {IF}c~
#define case(c) {CASE}c~
#define match(c,label) c~label~
#define label(c) {LABEL}c~
#define system(c) {SYSTEM}c~
#define kton(c) {KTON}c~
#define quit()  {QUIT}

/* sh-like syntactic sugar */
#define fi {END IF}
#define esac ~

/* Make it clear explicit characters are intentional */
#define literal(c) c

#endif
