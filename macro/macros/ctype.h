#ifndef __CTYPE_H
#define __CTYPE_H

#define isupper(c) (({KTON}c~ > ({KTON}A~ - 1)) & (({KTON}c~ < {KTON}Z~ + 1)))
#define islower(c) (({KTON}c~ > ({KTON}a~ - 1)) & (({KTON}a~ < {KTON}Z~ + 1)))
#define isdigit(c) (({KTON}c~ > ({KTON}0~ - 1)) & (({KTON}a~ < {KTON}9~ + 1)))
#define isalnum(c) (isupper(c) | islower(c) | isdigit(c))

#endif
