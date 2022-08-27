#ifndef __PREFIX_H
#define __PREFIX_H

#pragma pack(1)

enum {
    PROD_WORDPERFECT = 1,
    PROD_SHELL,
    PROD_NOTEBOOK,
    PROD_CALCULATOR,
    PROD_FILEMANAGER,
    PROD_CALENDAR,
    PROD_EDITOR,
    PROD_MACROEDITOR,
    PROD_PLANPERFECT,
    PROD_DATAPERFECT,
    PROD_MAIL,
    PROD_PRINTER,
    PROD_SCHEDULER,
    PROD_WPOFFICE,
    PROD_DRAWPERFECT,
    PROD_LETTERPERFECT,
};

enum {
    TYPE_MACRO = 1,
    TYPE_HELP,
    TYPE_KEYDEF,
    TYPE_VAXKEY,
};

typedef struct {
    uint8_t     magic[4];
    uint32_t    prefixsz;
    uint8_t     product;
    uint8_t     type;
    struct {
        uint8_t major;
        uint8_t minor;
    } ver;
    uint16_t    key;
    uint16_t    reserved;
} prefix_t;

typedef struct {
    uint16_t    type;
    uint16_t    count;
    uint16_t    blksize;
    uint32_t    offset;
} index_t;

typedef struct {
    uint16_t    type;
    uint32_t    length;
    uint32_t    offset;
} packet_t;

#endif
