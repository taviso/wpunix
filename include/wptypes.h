#ifndef __WPTYPES_H
#define __WPTYPES_H
#pragma pack(push, 1)

struct MSTBL {
    uint16_t    field_0;
    uint16_t    field_2;
    uint32_t    nbytes;
    void      (*mscallback)();
};

#pragma pack(pop)
#endif
