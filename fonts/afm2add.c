#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <err.h>

#include "parseafm.h"
#include "fonts.h"
#include "ini.h"
#include "charset.h"
#include "charmap.h"

static void freestorage(FontInfo *fi);

int main(int argc, char **argv)
{
    FontInfo *fi;
    char filename[8+1+3+1];
    FILE *afm, *tbl;

    if (argc != 2) {
        errx(EXIT_FAILURE, "No .afm file specified");
    }

    if ((afm = fopen(argv[1], "r")) == NULL) {
        err(EXIT_FAILURE, "Failed to open specified file");
    }

    // This describes the font file we're translating.
    if (parseFile(afm, &fi, P_GMP) != ok) {
        warnx("The afm file was not *fully* parsed (probably still okay).");
    }

    // This contains the pslgyph <=> wordperfect mappings.
    if (initialize_charmap("charmap.ini") != true) {
        err(EXIT_FAILURE, "Failed to parse the charmap ini file");
    }

    // Make sure this makes sense.
    if (fi->gfi->fontName == NULL) {
        err(EXIT_FAILURE, "The font does not have a name");
    }

    // Only proportional fonts use kerning and spacing.
    if (fi->gfi->isFixedPitch == false)  {
        sprintf(filename, "%.8s.KRN", fi->gfi->fontName);

        if ((tbl = fopen(filename, "w")) == NULL) {
            err(EXIT_FAILURE, "failed to create kerning table %s", filename);
        }

        if (generate_kerning_table(fi,
                                   tbl,
                                   fi->gfi->fontName,
                                   true,
                                   12) != true) {
            err(EXIT_FAILURE, "failed to generate kerning table");
        }

        fclose(tbl);
        sprintf(filename, "%.8s.SPC", fi->gfi->fontName);

        if ((tbl = fopen(filename, "w")) == NULL) {
            err(EXIT_FAILURE, "failed to create spacing table %s", filename);
        }

        if (generate_spacing_table(fi,
                                   tbl,
                                   fi->gfi->fontName,
                                   NULL,
                                   true,
                                   12) != true) {
            err(EXIT_FAILURE, "failed to generate spacing table");
        }

        fclose(tbl);
    }

    sprintf(filename, "%.8s.TYP", fi->gfi->fontName);

    if ((tbl = fopen(filename, "w")) == NULL) {
        err(EXIT_FAILURE, "failed to create typeface descriptor %s", filename);
    }

    if (generate_typeface_table(fi, tbl, fi->gfi->fontName, true) != true) {
        err(EXIT_FAILURE, "failed to generate typeface descriptor");
    }

    fclose(tbl);
    sprintf(filename, "%.8s.MAP", fi->gfi->fontName);

    if ((tbl = fopen(filename, "w")) == NULL) {
        err(EXIT_FAILURE, "failed to create character map %s", filename);
    }

    if (generate_charmap_table(fi, tbl, fi->gfi->fontName, true) != true) {
        err(EXIT_FAILURE, "failed to create character map");
    }

    fclose(tbl);

    sprintf(filename, "%.8s.ADD", fi->gfi->fontName);

    if ((tbl = fopen(filename, "w")) == NULL) {
        err(EXIT_FAILURE, "failed to create operations file %s", filename);
    }

    if (generate_operations_file("GhostScript",
                                 fi,
                                 NULL,
                                 NULL,
                                 NULL,
                                 tbl,
                                 true,
                                 12) != true) {
        err(EXIT_FAILURE, "failed to create operations file");
    }

    fprintf(stderr, "Translation seems to have worked.\n");

    destroy_charmap();
    freestorage(fi);
    fclose(afm);
    fclose(tbl);
    return 0;
}


static void freestorage(FontInfo *fi)
{
    if (fi != NULL) {
        if (fi->gfi != NULL) {
            free(fi->gfi->afmVersion);
            free(fi->gfi->fontName);
            free(fi->gfi->fullName);
            free(fi->gfi->familyName);
            free(fi->gfi->weight);
            free(fi->gfi->version);
            free(fi->gfi->notice);
            free(fi->gfi->encodingScheme);
            free(fi->gfi);
        }
        if (fi->cmi != NULL) {
            CharMetricInfo *temp = fi->cmi;
            Ligature *node = temp->ligs;

            for (int i = 0; i < fi->numOfChars; ++i) {
                Ligature *prev = NULL;

                for (node = temp->ligs; node != NULL; node = node->next) {
                    free(prev);
                    free(node->succ);
                    free(node->lig);
                    prev = node;
                }

                free(prev);
                free(temp->name);
                temp++;
            }

            free(fi->cmi);
        }

        free(fi->tkd);
        free(fi->cwi);

       for (int i = 0; i < fi->numOfPairs; ++i) {
           free(fi->pkd[i].name1);
           free(fi->pkd[i].name2);
       }

       free(fi->pkd);

        if (fi->ccd != NULL) {
            int i = 0, j = 0;
            CompCharData *ccd = fi->ccd;

            for (i = 0; i < fi->numOfComps; ++i) {
                for (j = 0; j < ccd[i].numOfPieces; ++j) {
                    free(ccd[i].pieces[j].pccName); 
                }

                free(ccd[i].ccName);
            }

            free(fi->ccd);
        }

        free(fi);
    }
}
