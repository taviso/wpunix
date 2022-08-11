#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <search.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "prefix.h"
#include "charset.h"
#include "util.h"
#include "ini.h"

static char *glyph_names_table[WP_NUMSETS][UINT8_MAX];

static int comparename(const void *key, const void *val)
{
    if (key == val)
        return 0;
    if (!key || !val)
        return 1;
    return strcmp(key, val);
}

static int charmapline(void *user, const char *section, const char *name, const char *value)
{
   unsigned set, c;
   char *valptr;

    if (strcmp(section, "WPCharMap") != 0)
        return 1;

    // Parse the set number.
    set = strtoul(value, &valptr, 10);

    if (*valptr != ',' || set >= WP_NUMSETS)
        goto error;

    // Skip past the comma.
    valptr++;

    // Parse the character.
    c = strtoul(valptr, &valptr, 10);

    if (*valptr != '\0' || c >= UINT8_MAX)
        goto error;

    // Check for existing entry.
    if (glyph_names_table[set][c] != NULL) {
        // Harmless
        //warnx("There was a duplicate charmap entry for %u,%u", set, c);

        // I guess we will just use the last entry.
        free(glyph_names_table[set][c]);
    }

    // Make a copy of it the value.
    glyph_names_table[set][c] = strdup(name);

    return 1;

error:
    errx(EXIT_FAILURE, "The charmap entry for %s in %s was not valid.", name, section);
    return 0;
}

bool initialize_charmap()
{
    return ini_parse("charmap.ini", charmapline, NULL) == 0;
}

void destroy_charmap()
{
    for (int set = 0; set < WP_NUMSETS; set++) {
        for (int c = 0; c < UINT8_MAX; c++) {
            free(glyph_names_table[set][c]);
        }
    }
}

const char *wp_chartoglyph(uint8_t set, uint8_t c)
{
    if (set >= WP_NUMSETS)
        return NULL;

    return glyph_names_table[set][c];
}


wpc_t wp_glyphtochar(const char *glyph)
{
    unsigned set, c;
    wpc_t result = {0};

    for (set = 0; set < WP_NUMSETS; set++) {
        for (c = 0; c < UINT8_MAX; c++) {
            if (comparename(glyph, glyph_names_table[set][c]) == 0) {
                result.set = set;
                result.c = c;
                goto found;
            }
        }
    }

found:
    return result;
}

uint32_t wp_setsize(uint32_t set)
{
    if (set == WP_ASCII)
        return 127;

    for (int size = countof(glyph_names_table[set]); size > 0; size--) {
        if (glyph_names_table[set][size - 1])
            return size;
    }

    return 0;
}
