#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "parseafm.h"
#include "prefix.h"
#include "charset.h"
#include "util.h"
#include "charmap.h"

bool generate_charmap_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            bool replace)
{
    uint16_t glyphcnt = 0;
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .prefixsz   = sizeof(prefix_t),
        .product    = PROD_PRINTER,
        .type       = 12,
        .ver        = { 3, 0 },
    };


    fprintf(stderr, "charmap: begin %s.\n", fi->gfi->fontName);

    fwrite(&prefix, sizeof prefix, 1, stream);
    putstring(stream, "%s", table);
    putbyte(stream, replace);

    for (int set = 0; set < WP_NUMSETS; set++) {
        if (set == WP_ASCII) {
            // The first 32 ASCII entries are implicit, yuck.
            putbyte(stream, wp_setsize(set) - 32);
            continue;
        }

        putbyte(stream, wp_setsize(set));
    }

    // First Pass, generate the chars and count glyphs.
    for (int set = 0; set < WP_NUMSETS; set++) {
        for (int c = 0; c < wp_setsize(set); c++) {
            const char *glyph = wp_chartoglyph(set, c);

            // The first 32 ascii entries are implicit, yuck.
            if (set == WP_ASCII && c < 32) {
                continue;
            }

            // No character mapped.
            if (glyph == NULL) {
                putbyte(stream, -1);
                continue;
            }

            // Is this a normal ASCII character?
            if (set == WP_ASCII) {
                putbyte(stream, c);
                continue;
            }

            // We do have a glyph for it.
            glyphcnt++;

            // Leave undefined for now.
            putbyte(stream, -1);
        }
    }

    fprintf(stderr, "charmap: glyph count %u\n", glyphcnt);

    // Report number of glyphs.
    putword(stream, glyphcnt);

    // Second Pass, generate glyphs.
    for (int set = 0; set < WP_NUMSETS; set++) {
        for (int c = 0; c < wp_setsize(set); c++) {
            const char *glyph = wp_chartoglyph(set, c);

            // Is this a normal ASCII character?
            if (set == WP_ASCII)
                continue;

            // Not ASCII, no glyph.
            if (glyph == NULL)
                continue;

            // We do have a glyph for it.
            putbyte(stream, c);
            putbyte(stream, set);
            putstring(stream, "\"/%s glyphshow \"", glyph);
            glyphcnt--;
        }
    }

    assert(glyphcnt == 0);

    return true;
}
