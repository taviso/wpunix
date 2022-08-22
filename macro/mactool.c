#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <ctype.h>
#include <getopt.h>
#include <search.h>
#include <limits.h>
#include <errno.h>
#include <err.h>

#include "prefix.h"
#include "charset.h"
#include "commands.h"

static const char *titlestr = "Untitled Macro";

int decompile_macro(FILE *in, FILE *out);
bool finducs(uint32_t ucs, wpc_t *result);

static void print_usage(char *progname, int exitcode)
{
    fprintf(stderr, "WordPerfect macro compiler/decompiler, part of wpunix\n");
    fprintf(stderr, "Usage: %s [-d] [-c] [-s] [-t title] [-o outfile] [-i infile]\n", progname);
    fprintf(stderr, "   -t: Optional macro title.\n");
    fprintf(stderr, "   -d: Decompile macro to text.\n");
    fprintf(stderr, "   -c: Compile macro to WPM.\n");
    fprintf(stderr, "   -o: Output file name.\n");
    fprintf(stderr, "   -i: Input file name.\n");
    fprintf(stderr, "   -s: Strip all spaces (use { } for a literal space).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, " - If no files are specified, stdin and stdout are assumed.\n");
    fprintf(stderr, " - Lines beginning with # are discarded to allow use of cpp.\n");
    fprintf(stderr, " - Spaces are *not* ignored by wp, use tabs for indenting or use -s.\n");
    fprintf(stderr, " - Enter a literal '{' with {{}.\n");
    exit(exitcode);
}

static int namecmp(const void *a, const void *b)
{
    char * const *name = b;

    if (a == *name)
        return 0;

    if (!a || !*name)
        return 1;

    return strcasecmp(a, *name);
}

static int freadmacro(FILE *stream, wpc_t *result)
{
    char macro[256] = {0};
    const char **match;
    size_t count;

    result->set = 0;
    result->c   = 0;

    for (int n = 0; n < sizeof macro; n++) {
        macro[n] = fgetc(stream);

        // Macro truncated.
        if (macro[n] == EOF)
            err(EXIT_FAILURE, "macro name was truncated, read {%s", macro);

        // End of macro.
        if (macro[n] == '}')  {
            macro[n] = '\0';
            break;
        }

        if (!isascii(macro[n])) {
            errx(EXIT_FAILURE, "non-ascii character in macro name");
        }

        if (macro[n] == 0) {
            errx(EXIT_FAILURE, "stray ascii nul in macro");
        }

        if (n == sizeof(macro) - 1) {
            errx(EXIT_FAILURE, "macro is too long, maybe missing }?");
        }
    }

    // Is it a special macro alias?
    if (strcasecmp(macro, "Enter") == 0)
        strcpy(macro, "^J");
    if (strcasecmp(macro, "Tab") == 0)
        strcpy(macro, "^I");
    if (strcasecmp(macro, "Compose") == 0)
        strcpy(macro, "^@");

    // No luck, we have to search for it.

    // Number of macros we know about.
    count = UINT8_MAX;

    // Now we lookup that string in the tables.
    match = lfind(macro, kMacroNames, &count, sizeof(char *), namecmp);

    if (match) {
        result->set = WP_MACRO;
        result->c   = match - &kMacroNames[0];

        fprintf(stderr, "debug: %s => WP_MACRO, %u\n", macro, result->c);

        return 0;
    }

    // No match, maybe it's a control.
    match = lfind(macro, kFunctionKeys, &count, sizeof(char *), namecmp);

    if (match) {
        result->set = WP_CTRL;
        result->c   = match - &kFunctionKeys[0];

        fprintf(stderr, "debug: %s => WP_CTRL, %u\n", macro, result->c);

        return 0;
    }

    // No luck, maybe it's an Alt-code?
    if (strncasecmp(macro, "Alt-", 4) == 0) {
        // Could be valid!
        char *op = macro + 4;

        // Why do these have seperate sets? 
        if (isdigit(op[0]) && op[1] == 0) {
            result->set = WP_ALTNUM;
            result->c   = op[0] - '0';
            return 0;
        }

        if (isalpha(op[0]) && op[1] == 0) {
            result->set = WP_ALTALPHA;
            result->c   = toupper(op[0]) - 'A';
            return 0;
        }
    }

    // Is it a special literal?
    if (strlen(macro) == 1 && isascii(macro[0])) {
        switch (macro[0]) {
            // These are permitted because they're hard to enter.
            case '{':
            case ' ':
                result->set = WP_ASCII;
                result->c   = macro[0];
                return 0;
        }
    }

    errx(EXIT_FAILURE, "The macro name %s is not recognized.", macro);
}

int main(int argc, char **argv)
{
    prefix_t prefix = {
        .magic      = { -1, 'W', 'P', 'C' },
        .product    = PROD_WORDPERFECT,
        .type       = TYPE_MACRO,
        .ver        = { 1, 1 },
        .key        = 0,
        .reserved   = 0,
    };
    index_t index   = {
        .type       = 65531,
        .count      = 4,
        .blksize    = 40,
        .offset     = 0,
    };
    packet_t packet = {
        .type       = 1,
    };
    bool compile;
    bool decompile;
    bool stripspace;
    iconv_t cd;
    FILE *infile, *outfile;
    int opt;
    int lastc;
    wpc_t wc;
    int lines;

    // Assume stdin if no files specified.
    infile = stdin;
    outfile = stdout;

    // Make sure we can detect no mode specified.
    decompile = compile = false;

    // Default settings.
    stripspace = false;

    while ((opt = getopt(argc, argv, "so:i:dct:h")) != -1) {
        switch (opt) {
            case 't': titlestr = optarg;
                      break;
            case 'h': print_usage(*argv, EXIT_SUCCESS);
                      break;
            case 'i': infile = fopen(optarg, "r");
                      break;
            case 'o': outfile = fopen(optarg, "w");
                      break;
            case 'd': decompile = true;
                      break;
            case 'c': compile = true;
                      break;
            case 's': stripspace = true;
                      break;
            default:  print_usage(*argv, EXIT_FAILURE);
                      break;
        }
    }

    if (!infile || !outfile) {
        err(EXIT_FAILURE, "The files specified were invalid.");
    }

    if ((decompile ^ compile) == false) {
        warnx("Must specify exactly one of either decompile (-d) or compile (-c).");
        print_usage(*argv, EXIT_FAILURE);
    }

    if (decompile == true) {
        fprintf(stderr, "debug: decompile mode\n");
        return decompile_macro(infile, outfile);
    }

    // We read UTF-8, but can only lookup UCS-4LE.
    cd = iconv_open("UCS-4LE", "UTF-8");

    packet.length = strlen(titlestr) + 1;
    packet.offset = sizeof(prefix) + sizeof(index) + sizeof(packet);
    prefix.prefixsz =  packet.offset + packet.length;

    fwrite(&prefix, sizeof prefix, 1, outfile);
    fwrite(&index, sizeof index, 1, outfile);
    fwrite(&packet, sizeof packet, 1, outfile);

    fprintf(outfile, "%s", titlestr);
    fputc(0, outfile);

    // Keep track of the last character seen for state.
    lastc = 0;
    lines = 1;

    while (true) {
        int c = fgetc(infile);

        if (c == EOF)
            break;

        // Keep track of line numbers for errors.
        if (lastc == '\n')
            lines++;

        // Strip spaces if requested.
        if (c == ' ' && stripspace)
            continue;

        // Everything until the closing '}' is a special name.
        if (c == '{') {
            if (freadmacro(infile, &wc) != 0) {
                err(EXIT_FAILURE, "Failed to parse a macro name on line %d", lines);
            }
            goto writechar;
        }

        // A plain ascii char.
        if (isascii(c)) {
            wc.set = WP_ASCII;
            wc.c   = c;

            // Check for comments / cpp directives.
            if (c == '#' && (lastc == '\n' || lastc == '\0')) {
                // We skip everything until the next line.
                do {
                    if ((lastc = fgetc(infile)) == EOF)
                        break;
                } while (lastc != '\n');

                // If there was an error or EOF, handle it on next iteration of loop.
                continue;
            }

            // Record last character.
            lastc = c;

            goto writechar;
        } else {
            char ucsbuf[MB_LEN_MAX] = { c };
            uint32_t ucschar = 0;
            size_t insize = 1;
            size_t outsize = sizeof(ucschar);
            char *outptr   = (char *) &ucschar;
            char *inptr    = (char *) ucsbuf;

            do {
                // Attempt the conversion.
                if (iconv(cd, &inptr, &insize, &outptr, &outsize) != (size_t) -1) {
                    // Success, got a character!
                    break;
                }

                // If it failed with anything other than EINVAL, error.
                if (errno != EINVAL) {
                    err(EXIT_FAILURE, "iconv() rejected a sequence from the input file, junk on line %d?", lines);
                }

                // Conversion incomplete, read another character.
                ucsbuf[insize] = fgetc(infile);

                if (ucsbuf[insize++] == EOF) {
                    err(EXIT_FAILURE, "truncated multibyte sequence? line is %d", lines);
                }
            } while (insize < MB_LEN_MAX);

            fprintf(stderr, "debug: discovered UCS-4LE %08x\n", ucschar);

            // Now I need to translate that into a wpc_t.
            if (finducs(ucschar, &wc) != true) {
                err(EXIT_FAILURE, "unable to translate char into wordperfect character, line %u", lines);
            }
        }

      writechar:
        // At this point it seems okay.
        if (fwrite(&wc, sizeof wc, 1, outfile) != 1)
            err(EXIT_FAILURE, "error writing out wp character");
    }

    // Write a terminating nul char.
    wc.set = WP_ASCII;
    wc.c   = 0;

    if (fwrite(&wc, sizeof wc, 1, outfile) != 1)
        err(EXIT_FAILURE, "error writing out terminating character");

    iconv_close(cd);
    return 0;
}
