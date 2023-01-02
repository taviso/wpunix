#ifndef __WPTYPES_H
#define __WPTYPES_H
#pragma pack(push, 1)

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
    WP_NUMSETS,
};

struct MSTBL {
    uint16_t    field_0;
    uint16_t    field_2;
    uint32_t    nbytes;
    void      (*mscallback)();
};

struct WPCHAR {
    uint8_t c;
    uint8_t set;
};

struct CHTAB {
    uint8_t start;
    uint8_t end;
    uint16_t _pad;
    uint32_t data;
};

#pragma pack(pop)
#endif
