#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "parseafm.h"
#include "prefix.h"
#include "util.h"
#include "fonts.h"

enum {
    OP_ADD,
    OP_DELETE,
    OP_REPLACE,
    OP_CHANGE,
    OP_VAR,
    OP_ADDGROUP,
    OP_DELGROUP,
};

enum {
    RSRC_BUILTIN,
    RSRC_CARTRIDGE,
    RSRC_MEMORYKB,
    RSRC_MEMORYSLOT,
    RSRC_WHEEL,
};

#define OR_PORTRAIT 0x10
#define OR_LANDSCAPE 0x20
#define OR_REVERSEPORTRAIT 0x40
#define OR_REVERSELANDSCAPE 0x80
#define OR_ALL (            \
      OR_PORTRAIT           \
    | OR_LANDSCAPE          \
    | OR_REVERSEPORTRAIT    \
    | OR_REVERSELANDSCAPE   \
)

#define FLAG_HIGHQUALITY 0x01
#define FLAG_MEDIUMQUALITY 0x02
#define FLAG_LOWQUALITY 0x04
#define FLAG_QUALITYALL (FLAG_HIGHQUALITY|FLAG_MEDIUMQUALITY|FLAG_LOWQUALITY)
#define FLAG_DIACUPPERCASE 0x08
#define FLAG_DIACADJUST 0x10
#define FLAG_HIDDEN 0x1
#define FLAG_SCALABLE 0x04
#define FLAG_PRSINCLUDE 0x80

typedef struct {
    uint8_t     restype;
    uint16_t    ptsize;
    uint16_t    cellheight;
    uint16_t    width;
    uint16_t    widthunits;
    uint16_t    addleading;
    uint16_t    basebias;
    uint16_t    hmud;
    uint16_t    hmun;
    uint16_t    vmud;
    uint16_t    vmun;
    uint16_t    resq;
    uint8_t     orients;
    uint8_t     flags0;
    uint8_t     flags1;
    uint8_t     flags2;
    uint16_t    celladjust;
} font_t;

const char libname[] = "";
const char ldstring[] = "";
const char unldstring[] = "";

// If you leave this blank, wp will append "R" for regular and so on.
const char encoded[] = "1";

bool generate_operations_file(const char *ptrname,
                              FontInfo *fi,
                              const char *spcfile,
                              const char *typfile,
                              const char *mapfile,
                              FILE *stream,
                              bool replace,
                              uint16_t ptsize)
{
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .prefixsz   = sizeof(prefix_t),
        .product    = PROD_PRINTER,
        .type       = 12,
        .ver        = { 3, 0 },

    };
    font_t font = {
        .restype    = RSRC_BUILTIN,
        // Because this is a scalable font, we just define the characteristics
        // for a "base" ptsize, and let WordPerfect figure it out for any other
        // size
        .ptsize     = ptsize * (3600 / PTS_PER_INCH), // 3600ths
        // I honestly have no idea how to calculate this.
        .cellheight = (fi->gfi->ascender - fi->gfi->descender) * (ptsize / 1000.0),
        .width      = 1000,
        .hmud       = 1200, // PS = 1200
        .hmun       = 1,    // PS = 1
        .vmud       = 0,    // Monospace Width?
        .vmun       = 0,    // Amount to Slant (Degrees)
        // I guess this is true for all postscript fonts?
        .orients    = OR_ALL,
        .flags0     = FLAG_HIGHQUALITY,
        .flags2     = FLAG_SCALABLE | FLAG_PRSINCLUDE,
    };

    fprintf(stderr, "operations: begin, size@%upt\n", ptsize);
    fprintf(stderr, "operations: Printer %s\n", ptrname);

    if (fi->gfi->isFixedPitch) {
        // This is a monospace font, let's just use the width of the first
        // character.
        assert(fi->numOfChars);
        assert(fi->cmi[0].wx);
        font.vmud = fi->cmi[0].wx;
        fprintf(stderr, "operations: Monospace font, width: %u\n", font.vmud);
    }

    fwrite(&prefix, sizeof prefix, 1, stream);
    putstring(stream, "%s", ptrname);
    putword(stream, 1); // opcnt
    putbyte(stream, OP_ADD);
    putstring(stream, "%s", fi->gfi->fullName);
    putbyte(stream, replace);
    fwrite(libname, sizeof libname, 1, stream);

    fwrite(&font, sizeof font, 1, stream);
    putstring(stream, "%s", ldstring);
    putstring(stream, "%s", unldstring);
    putstring(stream, "\"%s\"", fi->gfi->fontName);
    putstring(stream, "%s", encoded);

    if (typfile) {
        putstring(stream, "%s", typfile);
    } else {
        putstring(stream, ".%.8s.TYP", fi->gfi->fontName);
    }

    if (mapfile) {
        putstring(stream, "%s", mapfile);
    } else {
        putstring(stream, ".%.8s.MAP", fi->gfi->fontName);
    }

    if (fi->gfi->isFixedPitch == false) {
        if (spcfile) {
            putstring(stream, "%s", spcfile);
        } else {
            putstring(stream, ".%.8s.SPC", fi->gfi->fontName);
        }
    } else {
        // Monospace.
        putbyte(stream, 0);
    }

    return true;
}
