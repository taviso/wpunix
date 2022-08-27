#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <err.h>

#include "prefix.h"

static void hexDump (const char * desc, const void * addr, const int len, int perLine);

typedef struct {
    uint16_t unknown1;
    uint16_t dataoff;
    uint16_t numfields;
    uint16_t unknown4;
} driver_t;

typedef struct {
    uint16_t datastart;
    uint16_t numdrivers;
    uint16_t unknown;
} grshdr_t;

static void print_usage(char *progname, int exitcode)
{
    fprintf(stderr, "WordPerfect UNIX Graphics Settings Tool\n");
    fprintf(stderr, "Usage: %s [-d driver] [-f field] [-s val] FILENAME.GRS\n", progname);
    fprintf(stderr, "   -d: Name of the driver you want to view/edit.\n");
    fprintf(stderr, "   -f: Number of data field you want to see/edit.\n");
    fprintf(stderr, "   -s: New value to set for field.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, " - With no commands, prints all available drivers\n");
    exit(exitcode);
}

int main(int argc, char **argv)
{
    char drivername[64];
    prefix_t prefix;
    grshdr_t header;
    driver_t driver;
    int16_t fieldnum;
    uint16_t datalen;
    char *driverstr = NULL;
    char *newval = NULL;
    int opt;
    FILE *grsfile = NULL;
    const char *mode = "r";

    // Print all fields by default.
    fieldnum = -1;

    while ((opt = getopt(argc, argv, "s:f:d:h")) != -1 ) {
        switch (opt) {
            case 'd': driverstr = optarg;
                      break;
            case 'h': print_usage(*argv, EXIT_SUCCESS);
                      break;
            case 'f': fieldnum = atoi(optarg);
                      break;
            case 's': newval = optarg;
                      mode = "r+";
                      break;
            default:  print_usage(*argv, EXIT_FAILURE);
                      break;
        }
    }

    if (optind < argc) {
        grsfile = fopen(argv[optind], mode);
    }

    if (grsfile == NULL) {
        err(EXIT_FAILURE, "Failed to open the GRS file specified");
    }

    if (fread(&prefix, sizeof prefix, 1, grsfile) != 1) {
        errx(EXIT_FAILURE, "This file does not contain a WordPerfect prefix");
    }

    fprintf(stderr, "magic:     %02X %c%c%c\n", prefix.magic[0],
                                                prefix.magic[1],
                                                prefix.magic[2],
                                                prefix.magic[3]);
    fprintf(stdout, "product:   %u %u\n", prefix.product, prefix.type);
    fprintf(stdout, "version:   %u.%u\n", prefix.ver.major, prefix.ver.minor);

    // Check that this looks correct.
    if (memcmp(prefix.magic, "\xffWPC", 4) != 0) {
        err(EXIT_FAILURE, "This does not appear to be a WordPerfect File");
    }

    if (prefix.product != 18 || prefix.type != 12) {
        err(EXIT_FAILURE, "This is a WordPerfect file, but not a GRS file");
    }

    if (prefix.ver.major != 3 || prefix.ver.minor != 1) {
        err(EXIT_FAILURE, "This is a GRS file, but not a supported version");
    }

    if (fread(&header, sizeof header, 1, grsfile) != 1) {
        err(EXIT_FAILURE, "Failed to read GRS header");
    }

    // Now print all of the drivers.
    for (int i = 0; i < header.numdrivers; i++) {
        uint16_t namelen;

        memset(drivername, 0, sizeof drivername);

        if (fread(&namelen, sizeof namelen, 1, grsfile) != 1) {
            err(EXIT_FAILURE, "Could not read length of driver name");
        }

        if (fread(drivername, 1, namelen, grsfile) != namelen) {
            err(EXIT_FAILURE, "Could not read driver name");
        }

        if (fread(&driver, sizeof driver, 1, grsfile) != 1) {
            err(EXIT_FAILURE, "failed to read driver header");
        }

        if (driverstr == NULL || strcmp(driverstr, drivername) == 0) {
            fprintf(stdout, "Driver %u, %s", i, drivername);

            fprintf(stdout, " @%04x, %u fields\n", header.datastart
                                         + prefix.prefixsz
                                         + driver.dataoff,
                                         driver.numfields);

            //fprintf(stdout, "\tunknown: %04x\n", driver.unknown1);
            //fprintf(stdout, "\tunknown: %04x\n", driver.unknown4);

            if (driverstr && strcmp(driverstr, drivername) == 0) {
                break;
            }
        }
    }

    // If no driver specified, we were just dumping drivers.
    if (driverstr == NULL) {
        fprintf(stderr, "Please speciy a drivername to examine/modify.\n");
        return 0;
    }

    if (strcmp(driverstr, drivername) != 0) {
        errx(EXIT_FAILURE, "driver %s was not found", driverstr);
    }

    // Okay, we have a valid driver, seek to it.
    if (fseek(grsfile, header.datastart + prefix.prefixsz + driver.dataoff, SEEK_SET) != 0) {
        err(EXIT_FAILURE, "failed to seek to %s driver data", driverstr);
    }

    if (fread(&datalen, sizeof datalen, 1, grsfile) != 1) {
        err(EXIT_FAILURE, "failed to query size of driver data");
    }

    fprintf(stdout, "Driver %s has %u bytes of data\n", drivername, datalen);

    // Subtract 2 bytes for the length we've already read.
    if (datalen < sizeof(uint16_t) * (driver.numfields + 1)) {
        err(EXIT_FAILURE, "The driver data appears to be corrupt");
    }

    // Subtract all the sizes.
    datalen -= 2;
    datalen -= 2 * driver.numfields;

    for (int field = 0; field < driver.numfields; field++) {
        uint8_t data[256];
        uint16_t length;

        if (fread(&length, sizeof length, 1, grsfile) != 1) {
            err(EXIT_FAILURE, "couldn't read packet length");
        }

        // Empty field.
        if (length == 0)
            continue;

        if (length > 256 || length > datalen) {
            err(EXIT_FAILURE, "length suspiciously big, giving up");
        }

        if (fread(data, 1, length, grsfile) != length) {
            err(EXIT_FAILURE, "failed to read in the field data");
        }

        // Keep track of how much data it has used.
        datalen -= length;

        if (fieldnum != -1 && fieldnum != field) {
            continue;
        }

        fprintf(stdout, "\tfield %u: %u bytes\n", field, length);

        // Dump the current data.
        hexDump(NULL, data, length, 16);

        if (newval != NULL) {
            uint16_t val;

            if (length != sizeof(uint16_t)) {
                err(EXIT_FAILURE, "Sorry, only WORD values can currently be edited");
            }

            val = strtol(newval, NULL, 0);

            // Seek back to the data location.
            if (fseek(grsfile, - sizeof(uint16_t), SEEK_CUR) != 0) {
                err(EXIT_FAILURE, "failed to seek on stream to write new data");
            }

            if (fwrite(&val, sizeof val, 1, grsfile) != 1) {
                err(EXIT_FAILURE, "failed to write new data");
            }

            hexDump("\tNew Field Value", &val, length, 16);
        }
    }

    assert(datalen == 0);
    fclose(grsfile);
    return 0;
}

// Usage:
//     hexDump(desc, addr, len, perLine);
//         desc:    if non-NULL, printed as a description before hex dump.
//         addr:    the address to start dumping from.
//         len:     the number of bytes to dump.
//         perLine: number of bytes on each output line.

static void hexDump (
    const char * desc,
    const void * addr,
    const int len,
    int perLine
) {
    // Silently ignore silly per-line values.

    if (perLine < 4 || perLine > 64) perLine = 16;

    int i;
    unsigned char buff[perLine+1];
    const unsigned char * pc = (const unsigned char *)addr;

    // Output description if given.

    if (desc != NULL) printf ("%s:\n", desc);

    // Length checks.

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    if (len < 0) {
        printf("  NEGATIVE LENGTH: %d\n", len);
        return;
    }

    // Process every byte in the data.

    for (i = 0; i < len; i++) {
        // Multiple of perLine means new or first line (with line offset).

        if ((i % perLine) == 0) {
            // Only print previous-line ASCII buffer for lines beyond first.

            if (i != 0) printf ("  %s\n", buff);

            // Output the offset of current line.

            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.

        printf (" %02x", pc[i]);

        // And buffer a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
            buff[i % perLine] = '.';
        else
            buff[i % perLine] = pc[i];
        buff[(i % perLine) + 1] = '\0';
    }

    // Pad out last line if not exactly perLine characters.

    while ((i % perLine) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII buffer.

    printf ("  %s\n", buff);
}

