#ifndef __CHARMAP_H
#define __CHARMAP_H

bool initialize_charmap(const char *inifile);
void destroy_charmap();

const char *wp_chartoglyph(uint8_t set, uint8_t c);
wpc_t wp_glyphtochar(const char *glyph);

uint32_t wp_setsize(uint32_t set);

#endif
