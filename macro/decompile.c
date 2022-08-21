#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <err.h>

#include "prefix.h"
#include "charset.h"
#include "commands.h"

int decompile_macro(FILE *infile, FILE *outfile)
{
    prefix_t prefix;
    index_t index;
    packet_t packet;
    char *title;
    wpc_t c;

    fread(&prefix, sizeof prefix, 1, infile);

    fprintf(stderr, "magic:     %02X %c%c%c\n", prefix.magic[0],
                                                prefix.magic[1],
                                                prefix.magic[2],
                                                prefix.magic[3]);
    fprintf(stderr, "prefixsz:  %u\n", prefix.prefixsz);
    fprintf(stderr, "product:   %u\n", prefix.product);
    fprintf(stderr, "type:      %u\n", prefix.type);
    fprintf(stderr, "version:   %u.%u\n", prefix.ver.major, prefix.ver.minor);
    fprintf(stderr, "key:       %04x\n", prefix.key);

    fread(&index, sizeof index, 1, infile);

    fprintf(stderr, "type:      %u\n", index.type);
    fprintf(stderr, "count:     %u\n", index.count);
    fprintf(stderr, "blksize:   %u\n", index.blksize);
    fprintf(stderr, "offset:    %u\n", index.offset);

    fread(&packet, sizeof packet, 1, infile);

    fprintf(stderr, "type:      %u\n", packet.type);
    fprintf(stderr, "length:    %u\n", packet.length);
    fprintf(stderr, "offset:    %u\n", packet.offset);

    fseek(infile, packet.offset, SEEK_SET);

    title = malloc(packet.length);

    fread(title, 1, packet.length, infile);

    fprintf(stderr, "title: %s\n", title);

    free(title);

    while (fread(&c, sizeof c, 1, infile) == 1) {
        if (c.word == 0)
            break;

        switch (c.set) {
            case WP_ASCII ... WP_NUMSETS - 1:
                    fputwpc(c, outfile);
                    break;
            case WP_MACRO:
                    fprintf(outfile, "{%s}", kMacroNames[c.c]);
                    break;
            case WP_CTRL:
                    fprintf(outfile, "{%s}", kFunctionKeys[c.c]);
                    break;
            case WP_ALTNUM:
                    fprintf(outfile, "{Alt-%c}", c.c + '0');
                    break;
            case WP_ALTALPHA:
                    fprintf(outfile, "{Alt-%c}", c.c + 'A');
                    break;
            default:
                    errx(EXIT_FAILURE, "Failed to translate charset %u", c.set);
        }
    }

    return 0;
}
