#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "parseafm.h"
#include "prefix.h"
#include "charset.h"
#include "util.h"
#include "fonts.h"
#include "charmap.h"

bool generate_spacing_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            const char *krnfile,
                            bool replace,
                            uint16_t ptsize)
{
    uint32_t count  = 0;
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .prefixsz   = sizeof(prefix_t),
        .product    = PROD_PRINTER,
        .type       = 12,
        .ver        = { 3, 0 },
    };

    fwrite(&prefix, sizeof prefix, 1, stream);
    putstring(stream, "%s", table);

    fprintf(stderr, "spacing: begin, size@%upt\n", ptsize);
    fprintf(stderr, "spacing: %u characters in %s.\n",
                    fi->numOfChars,
                    fi->gfi->fontName);

    putbyte(stream, replace);
    putword(stream, ptsize * (3600 / PTS_PER_INCH));
    putword(stream, -1); // Units

    // Generate number of entries.
    for (int set = 0; set < WP_NUMSETS; set++) {
        fprintf(stderr, "spacing: set %u, size: %u\n", set, wp_setsize(set));

        // The first 32 ASCII entries are implicit, yuck.
        if (set == WP_ASCII) {
            putbyte(stream, wp_setsize(set) - 32);
        } else {
            putbyte(stream, wp_setsize(set));
        }
    }

    // Width size flags.
    // 01h - widths are bytes
    // 02h - widths are words
    putbyte(stream, sizeof(uint16_t));

    // For each character of each character set, output width.
    for (int set = 0; set < WP_NUMSETS; set++) {
        for (int c = 0; c < wp_setsize(set); c++) {
            const char *glyph = wp_chartoglyph(set, c);
            int16_t width = -1;

            // The first 32 ascii entries are implicit, yuck.
            if (set == WP_ASCII && c < 32) {
                continue;
            }

            if (glyph == NULL) {
                // We don't have a glyph, just output -1 to skip it.
                putword(stream, width);
                continue;
            }

            // See if we have this char.
            for (int i = 0; i < fi->numOfChars; ++i) {
                if (fi->cmi[i].name != NULL && strcmp(fi->cmi[i].name, glyph) == 0) {
                    width = fi->cmi[i].wx;
                    count++;
                    break;
                }
            }

            if (width == -1) {
                //fprintf(stderr, "spacing: font does not know glyph %s\n", glyph);
            }

            putword(stream, width);
        }
    }

    fprintf(stderr, "spacing: recognized %u chars\n", count);

    if (krnfile) {
        putstring(stream, "%s", krnfile);
    } else {
        putstring(stream, ".%.8s.KRN", fi->gfi->fontName);
    }

    return true;
}
