#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <strings.h>

#include "parseafm.h"
#include "prefix.h"
#include "util.h"

// Def flags
#define TF_SMALLCAPS 0x01
#define TF_SHADOW 0x02
#define TF_OUTLINE 0x04
#define TF_ITALIC 0x08
#define TF_OBLIQUE TF_ITALIC
#define TF_POSTSCRIPT 0x10

// Weight Flags
#define TF_EXTRALIGHT 0
#define TF_LIGHT 0x20
#define TF_NORMAL 0x40
#define TF_DEMIBOLD 0x60
#define TF_BOLD 0x80
#define TF_HEAVY 0xa0
#define TF_BLACK 0xc0
#define TF_ULTRABLACK 0xe0

// Low word, Attributes / Style
#define TFD_CASUAL (1 << 0)
#define TFD_CONNECTED (1 << 1)
#define TFD_DECORATIVE (1 << 2)
#define TFD_FORMAL (1 << 3)
#define TFD_FUTURE (1 << 4)
#define TFD_OLD (1 << 5)
#define TFD_SCRIPT (1 << 6)
#define TFD_CUPPED (1 << 7)
#define TFD_EXAGGERATEDLO (1 << 8)
#define TFD_HAIRLINE (1 << 9)
#define TFD_SLAB (1 << 10)
#define TFD_SLANTED (1 << 11)
#define TFD_TRANSITIONAL (1 << 12)
#define TFD_TRIANGULAR (1 << 13)
#define TFD_CURVED (1 << 14)
#define TFD_NONCON (1 << 15)

// Hi byte, Serif Type?
#define TFD_ROUND (1 << 0)
#define TFD_SQUARE (1 << 1)
#define TFD_ANGULAR (1 << 2)
#define TFD_EXAGGERATEDHI (1 << 3)
#define TFD_UNIFORM (1 << 4)
#define TFD_BALLS (1 << 5)
#define TFD_TAILS (1 << 6)
#define TFD_RESERVED (1 << 7)

typedef struct {
    uint16_t    tfcaps;     // cap height (psu)
    uint16_t    tfx;        // x height (psu)
    uint16_t    tft;        // lowercase t height (psu)
    uint16_t    tfasc;      // maximum ascender height (psu)
    uint16_t    tfdesc;     // maximum descender height (psu)
    uint16_t    tfitadj;    // italic adjust (caps) (psu, +/-)
    uint16_t    tfdsclw;    // low word of descriptor
    uint8_t     tfdschb;    // high byte of descriptor
    uint8_t     tfflgs1;    // tf def flags
} typeface_t;

static uint32_t weight_to_desc(FontInfo *fi)
{
    if (fi->gfi->weight == NULL) {
        fprintf(stderr, "typeface: no weight specified, set TF_NORMAL\n");
        return TF_NORMAL;
    }

    fprintf(stderr, "typeface: decoding weight string '%s' => ", fi->gfi->weight);

    // Look, I dunno wtf any of these mean.
    if (strcasecmp(fi->gfi->weight, "Book") == 0) {
        fprintf(stderr, "normal\n");
        return TF_NORMAL;
    }
    if (strcasecmp(fi->gfi->weight, "Bold") == 0) {
        fprintf(stderr, "bold\n");
        return TF_BOLD;
    }
    if (strcasecmp(fi->gfi->weight, "Demi") == 0) {
        fprintf(stderr, "demi\n");
        return TF_DEMIBOLD;
    }
    if (strcasecmp(fi->gfi->weight, "Roman") == 0) {
        fprintf(stderr, "normal\n");
        return TF_NORMAL;
    }
    if (strcasecmp(fi->gfi->weight, "Regular") == 0) {
        fprintf(stderr, "normal\n");
        return TF_NORMAL;
    }

    fprintf(stderr, "unknown, assuming normal.\n");
    return TF_NORMAL;
}

bool generate_typeface_table(FontInfo *fi,
                             FILE *stream,
                             const char *table,
                             bool replace)
{
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .prefixsz   = sizeof(prefix_t),
        .product    = PROD_PRINTER,
        .type       = 12,
        .ver        = { 3, 0 },
    };

    // WordPerfect wants these values in % of point size, so we try to simplify
    // the conversion by stating point size in such a way that we can just multiply
    // by 10.
    typeface_t typeface = {
        // The distance between a font's baseline and the top of any capital
        // (uppercase) letter.
        .tfcaps     = fi->gfi->capHeight * 10,
        // The distance between a font's baseline and the top of the lowercase
        // letter x.
        .tfx        = fi->gfi->xHeight * 10,
        // The distance between a font's baseline and the top of the lowercase
        // letter t.
        // The afm ascender field is the top of the letter d, close enough?
        .tft        = fi->gfi->ascender * 10,
        // The maximum height of any descender or ascender.
        .tfasc      = fi->gfi->ascender * 10,
        .tfdesc     = -fi->gfi->descender * 10,
        // Italic adjustment.
        .tfitadj    = -fi->gfi->italicAngle * 100,
        // Descriptor Flags
        .tfdsclw    = 0,    // FIXME
        .tfdschb    = 0,    // FIXME
        .tfflgs1    = TF_POSTSCRIPT,
    };

    fprintf(stderr, "typeface: begin %s, capheight is %u\n",
                    fi->gfi->fontName,
                    fi->gfi->capHeight);

    // Decode the weight string.
    typeface.tfflgs1 |= weight_to_desc(fi);

    // Is this right?
    if (fi->gfi->italicAngle) {
        typeface.tfflgs1 |= TF_ITALIC;
    }

    fwrite(&prefix, sizeof prefix, 1, stream);
    putstring(stream, "%s", table);
    fwrite(&replace, sizeof replace, 1, stream);
    fwrite(&typeface, sizeof typeface, 1, stream);

    fprintf(stderr, "typeface: complete\n");
    return true;
}
