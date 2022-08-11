#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <err.h>

#include "parseafm.h"
#include "prefix.h"
#include "charset.h"
#include "charmap.h"
#include "util.h"
#include "gseval.h"
#include "fonts.h"

enum { PASS_COUNTPAIRS, PASS_ASCIIPAIRS, PASS_GLYPHPAIRS, PASS_ASCIIVALS, PASS_GLYPHVALS, PASS_TOTAL };

int compare_kerning_pair(const void *a, const void *b)
{
    const PairKernData *x = a;
    const PairKernData *y = b;
    wpc_t xleft, xright;
    wpc_t yleft, yright;

    xleft  = wp_glyphtochar(x->name1);
    xright = wp_glyphtochar(x->name2);
    yleft  = wp_glyphtochar(y->name1);
    yright = wp_glyphtochar(y->name2);

    // Quote from ADDALL.DOC:
    //  kern pairs must be *sorted* in ascending order, sorted by right char
    //  first and left char last.
    if (xright.word != yright.word)
        return xright.word - yright.word;

    return xleft.word - yleft.word;
}

static inline bool wpisascii(wpc_t c) {
    return c.set == WP_ASCII;
}

static inline bool wpasciipair(wpc_t left, wpc_t right) {
    return wpisascii(left) && wpisascii(right);
}

static inline bool wpcequal(wpc_t a, wpc_t b)
{
    return a.word == b.word;
}

bool generate_kerning_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            bool replace,
                            uint16_t ptsize)
{
    uint16_t asciipair = 0;
    uint16_t glyphpair = 0;
    uint16_t valsize = sizeof(int8_t);
    int32_t xamt = 0;
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .prefixsz   = sizeof(prefix_t),
        .product    = PROD_PRINTER,
        .type       = 12,
        .ver        = { 3, 0 },
    };

    fwrite(&prefix, sizeof prefix, 1, stream);
    putstring(stream, "%s", table);
    putbyte(stream, replace);
    putword(stream, ptsize * (3600 / PTS_PER_INCH));

    fprintf(stderr, "kerning: begin, size@%upt\n", ptsize);
    fprintf(stderr, "kerning: %u kerning pairs in %s.\n",
                    fi->numOfPairs,
                    fi->gfi->fontName);

    fprintf(stderr, "kerning: attempting to sort pairs (slow)\n");

    // Pairs must be sorted, this is probably wrong?
    qsort(fi->pkd,
          fi->numOfPairs,
          sizeof (PairKernData),
          compare_kerning_pair);

    fprintf(stderr, "kerning: sorting complete, now counting pairs\n");

    for (int pass = PASS_COUNTPAIRS; pass < PASS_TOTAL; pass++) {

        // Each pass we have to cycle through all the glyphs.
        for (int i = 0; i < fi->numOfPairs; i++) {
            wpc_t left  = wp_glyphtochar(fi->pkd[i].name1);
            wpc_t right = wp_glyphtochar(fi->pkd[i].name2);

            // WordPerfect doesn't know one of the pairs.
            if (!left.word || !right.word)
                continue;

            // Scale the vector to current pointsize.
            xamt = fi->pkd[i].xamt * (ptsize / 1000.0) * 10;

            // If this is exceeded I need a word instead of a byte.
            if (xamt > INT8_MAX || xamt < INT8_MIN) {
                if (valsize != sizeof(int16_t)) {
                    fprintf(stderr, "kerning: increasing width type to int16_t.\n");
                }
                valsize = sizeof(int16_t);
            }

            switch (pass) {
                // First pass, count kerning pairs.
                case PASS_COUNTPAIRS:
                    if (wpasciipair(left, right)) {
                        asciipair++;
                    } else {
                        glyphpair++;
                    }
                    break;
                case PASS_ASCIIPAIRS:
                    // Output ascii pairs
                    if (wpasciipair(left, right)) {
                        putbyte(stream, left.c);
                        putbyte(stream, right.c);
                    }
                    break;
                case PASS_GLYPHPAIRS:
                    // Output glyph pairs
                    if (wpasciipair(left, right)) {
                        continue;
                    }

                    putbyte(stream, left.c);
                    putbyte(stream, left.set);
                    putbyte(stream, right.c);
                    putbyte(stream, right.set);
                    break;
                case PASS_ASCIIVALS:
                    // Output ascii values.
                    if (wpasciipair(left, right)) {
                        if (valsize == sizeof(int8_t)) {
                            putbyte(stream, xamt);
                        } else {
                            putword(stream, xamt);
                        }
                    }
                    break;
                case PASS_GLYPHVALS:
                    // Output glyph values.
                    if (wpasciipair(left, right)) {
                        continue;
                    }

                    if (valsize == sizeof(int8_t)) {
                        putbyte(stream, xamt);
                    } else {
                        putword(stream, xamt);
                    }
                    break;
            }
        }

        if (pass == PASS_COUNTPAIRS) {
                fprintf(stderr, "kerning: %u/%u pairs recognized\n",
                                asciipair + glyphpair,
                                fi->numOfPairs);
                fprintf(stderr, "kerning: %u ascii pairs, %u glyph/mixed pairs\n",
                                asciipair,
                                glyphpair);

                if (asciipair + glyphpair == 0) {
                    errx(EXIT_FAILURE, "No glyphs in this font that I recognize, is it a graphic font???");
                }

                putword(stream, asciipair);
                putword(stream, glyphpair);
                putbyte(stream, valsize);

        }
    }

    fprintf(stderr, "kerning: table %s generated\n", table);
    return true;
}
