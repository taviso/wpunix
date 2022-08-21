#ifndef __CHARSET_H
#define __CHARSET_H

enum {
    WP_ASCII,
    WP_MULTI1,
    WP_MULTI2,
    WP_BOXDRAW,
    WP_TYPOGRAPHIC,
    WP_ICONIC,
    WP_MATH,
    WP_MATHEXT,
    WP_GREEK,
    WP_HEBREW,
    WP_CYRILLIC,
    WP_JAPANESE,
    WP_USERDEF,
    WP_NUMSETS
};

enum {
    WP_ALTNUM = 0xFF,
    WP_ALTALPHA = 0xFE,
    WP_MACRO = 0xFC,
    WP_CTRL = 0x80,
};

typedef union {
    struct {
        uint8_t c;
        uint8_t set;
    };
    uint16_t word;
} wpc_t;

const char *ucs_to_psglyph(uint32_t ucs);
uint32_t wp_to_ucs(uint32_t set, uint32_t character);
uint32_t wp_setsize(uint32_t set);
uint32_t psglyph_to_ucs(const char *psglyph);
bool ucs_to_wp(uint32_t u, uint8_t *set, uint8_t *c);

int fputwpc(wpc_t wpc, FILE *stream);

#endif
