#ifndef __PRINTER_H
#define __PRINTER_H

/* Insert some raw postscript, no newlines */
#define postscript(cmd) {format} 4 6 2 1 _(cmd) { } {enter}{exit}

#endif
