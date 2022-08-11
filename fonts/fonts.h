#ifndef __FONTS_H
#define __FONTS_H

#define PTS_PER_INCH 72

bool generate_kerning_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            bool replace,
                            uint16_t ptsize);
bool generate_spacing_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            const char *krnfile,
                            bool replace,
                            uint16_t ptsize);
bool generate_typeface_table(FontInfo *fi,
                             FILE *stream,
                             const char *table,
                             bool replace);
bool generate_charmap_table(FontInfo *fi,
                            FILE *stream,
                            const char *table,
                            bool replace);
bool generate_operations_file(const char *ptrname,
                              FontInfo *fi,
                              const char *spcfile,
                              const char *typfile,
                              const char *mapfile,
                              FILE *stream,
                              bool replace,
                              uint16_t ptsize);

#endif
