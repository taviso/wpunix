/*
 * (C) 1988, 1989 by Adobe Systems Incorporated. All rights reserved.
 *
 * This file may be freely copied and redistributed as long as:
 *   1) This entire notice continues to be included in the file, 
 *   2) If the file has been modified in any way, a notice of such
 *      modification is conspicuously indicated.
 *
 * PostScript, Display PostScript, and Adobe are registered trademarks of
 * Adobe Systems Incorporated.
 * 
 * ************************************************************************
 * THE INFORMATION BELOW IS FURNISHED AS IS, IS SUBJECT TO CHANGE WITHOUT
 * NOTICE, AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY ADOBE SYSTEMS
 * INCORPORATED. ADOBE SYSTEMS INCORPORATED ASSUMES NO RESPONSIBILITY OR 
 * LIABILITY FOR ANY ERRORS OR INACCURACIES, MAKES NO WARRANTY OF ANY 
 * KIND (EXPRESS, IMPLIED OR STATUTORY) WITH RESPECT TO THIS INFORMATION, 
 * AND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR PARTICULAR PURPOSES AND NONINFRINGEMENT OF THIRD PARTY RIGHTS.
 * ************************************************************************
 */

/*  parseAFMclient.c
 *
 *  This file is an example of how an application might use the provided
 *  AFM Parser (parseAFM.c).
 *
 *  In a nutshell, the client of the parser (like this file) chooses
 *  the AFM File that it wishes to have parsed, opens the file for 
 *  reading (and does any/all error handling associated with that task),
 *  and passes the resulting file pointer to the procedure "parseFile"
 *  (in parseAFM.c). In addition to the file pointer, the client also
 *  needs to pass a pointer to a FontInfo record type (for which storage
 *  will be allocated and data filled in), and a mask representing which
 *  sections of the AFM should be saved in the FontInfo record.
 *
 *  In the procedure "main", the mask is built from command line switches, 
 *  but your application, of course, can set that mask any way you prefer.
 *  "main" then calls the "parseFile" procedure (of parseAFM.c) to do the
 *  grunt work, and checks the error codes upon "parseFile"'s return.
 *
 *  The rest of this sample application is a collection of print routines 
 *  that show how to reference each of the sections of data and a 
 *  "freeStorage" routine (that many unix programmers may feel they 
 *  don't need but is included for portability to other systems that do 
 *  need to manage storage). The print procedures are there just to
 *  give meaning to this application, and hopefully your application
 *  will use the data collected from the AFM file in a more useful way.
 *  
 * History:
 *	original: DSM  Thu Oct 20 17:39:59 PDT 1988
 *  modified: DSM  Mon Jul  3 14:17:50 PDT 1989
 *    - added 'storageProblem' check in main
 */

#include <stdio.h>
#include "parseAFM.h"


/*************************** GLOBALS ***************************/
FontInfo *fi;
FLAGS myflags = 0;
  

/*************************** printGlobals **********************/

printGlobals()
{
    printf("StartFontMetrics %s\n", fi->gfi->afmVersion);
    printf("FontName %s\n", fi->gfi->fontName);
    printf("FullName %s\n", fi->gfi->fullName);
    printf("FamilyName %s\n", fi->gfi->familyName);
    printf("Weight %s\n", fi->gfi->weight);
    printf("ItalicAngle %3.1f\n", fi->gfi->italicAngle);
    if (fi->gfi->isFixedPitch)
        printf("IsFixedPitch true\n");
    else
        printf("IsFixedPitch false\n");
    printf("UnderlinePosition %d\n", fi->gfi->underlinePosition);
    printf("UnderlineThickness %d\n", fi->gfi->underlineThickness);
    printf("Version %s\n", fi->gfi->version);
    printf("FontBBox %d %d %d %d\n", 
        fi->gfi->fontBBox.llx, fi->gfi->fontBBox.lly, 
        fi->gfi->fontBBox.urx, fi->gfi->fontBBox.ury);
    printf("Notice %s\n", fi->gfi->notice);
    printf("EncodingScheme %s\n", fi->gfi->encodingScheme);
    printf("CapHeight %d\n", fi->gfi->capHeight);
    printf("XHeight %d\n", fi->gfi->xHeight);
    printf("Descender %d\n", fi->gfi->descender);
    printf("Ascender %d\n", fi->gfi->ascender);
    
} /* printGlobals */


/*************************** printCharWidths *********************/
printCharWidths()
{
    int i = 0;
    for (i = 0; i < 256; ++i)
        printf("  code: %3d   width: %4d\n", i, fi->cwi[i]);
    printf("\n");
 
} /* printCharWidths */


/*************************** printAllCharMetrics *****************/
printAllCharMetrics()
{
    int i = 0;
    CharMetricInfo *temp = fi->cmi;
    Ligature *node = temp->ligs;
   printf("StartCharMetrics %d\n", fi->numOfChars);
    for (i = 0; i < fi->numOfChars; ++i)
    {
        printf(
          "C %d ; WX %d ; N %s ; B %d %d %d %d ;",
            temp->code, temp->wx, temp->name, temp->charBBox.llx, 
            temp->charBBox.lly, temp->charBBox.urx, temp->charBBox.ury);
        for (node = temp->ligs; node != NULL; node = node->next)
        {
            printf(" L %s %s ;",  node->succ, node->lig);
        }
        printf("\n");
        temp++;
    } /* for */
    printf("EndCharMetrics\n");

} /* printAllCharMetrics */


/*************************** printKernPairData ******************/
printKernPairData()
{
    int i = 0;
    PairKernData *pkd = fi->pkd;
    
    if (fi->numOfPairs != 0)
    {
        printf("StartKernData\n");
        printf("StartKernPairs %d\n",fi->numOfPairs);
        for (i = 0; i < fi->numOfPairs; ++i)
        printf(
          "KPX %s %s %d\n", 
            pkd[i].name1, pkd[i].name2, pkd[i].xamt);
/*          "KPX %s %s %d %d\n", 
            pkd[i].name1, pkd[i].name2, pkd[i].xamt, pkd[i].yamt); */
        printf("EndKernPairs\n");
        printf("EndKernData\n");
    }
    
} /* printKernPairData */


/*************************** printKernTrackData *******************/
printKernTrackData()
{
    int i = 0;
    TrackKernData *tkd = fi->tkd;
    
    if (fi->numOfTracks != 0)
    {
        printf("StartTrackKern %d\n",fi->numOfTracks);
        for (i = 0; i < fi->numOfTracks; ++i)
            printf(
              "TrackKern %d %d %d %d %d\n", 
                tkd[i].degree, tkd[i].minPtSize, tkd[i].minKernAmt, 
                tkd[i].maxPtSize, tkd[i].maxKernAmt);
        printf("EndTrackKern\n");
    }
    
} /* printKernTrackData */


/*************************** printCompCharData ********************/
printCompCharData()
{
    int i = 0, j = 0;
    CompCharData *ccd = fi->ccd;
    
    if (fi->numOfComps != 0)
    {
        printf("StartComposites %d\n",fi->numOfComps);
        for (i = 0; i < fi->numOfComps; ++i)
        {
            printf("CC %s %d ;", 
                ccd[i].ccName, ccd[i].numOfPieces);
            for (j = 0; j < ccd[i].numOfPieces; ++j)
                printf(
                  " PCC %s %d %d ;", ccd[i].pieces[j].pccName, 
                    ccd[i].pieces[j].deltax, ccd[i].pieces[j].deltay);
            printf("\n");
        }
        printf("EndComposites\n");
    }
    
} /* printCompCharData */



/*************************** freeStorage ***********************/

freeStorage()
{
    if (fi != NULL)
    {
        if (fi->gfi != NULL)
        { 
            free(fi->gfi->afmVersion); fi->gfi->afmVersion = NULL;
            free(fi->gfi->fontName); fi->gfi->fontName = NULL;
            free(fi->gfi->fullName); fi->gfi->fullName = NULL;
            free(fi->gfi->familyName); fi->gfi->familyName = NULL;
            free(fi->gfi->weight); fi->gfi->weight = NULL;
            free(fi->gfi->version); fi->gfi->version = NULL;
            free(fi->gfi->notice); fi->gfi->notice = NULL;
            free(fi->gfi->encodingScheme); fi->gfi->encodingScheme = NULL;
            free(fi->gfi); fi->gfi = NULL;
        }
  
        if (fi->cwi != NULL)
        { free(fi->cwi); fi->cwi = NULL; }

        if (fi->cmi != NULL)
        { 
            int i = 0;
            CharMetricInfo *temp = fi->cmi;
            Ligature *node = temp->ligs;
            
            for (i = 0; i < fi->numOfChars; ++i)
            {
                for (node = temp->ligs; node != NULL; node = node->next)
                {
                    free(node->succ); node->succ = NULL;
                    free(node->lig); node->lig = NULL;
                }
                
                free(temp->name); temp->name = NULL;
                temp++;
            }
            
            free(fi->cmi); fi->cmi = NULL;
        }

        if (fi->tkd != NULL)
        { free(fi->tkd); fi->tkd = NULL; }
  
        if (fi->pkd != NULL)
        { 
            free(fi->pkd->name1); fi->pkd->name1 = NULL;
            free(fi->pkd->name2); fi->pkd->name2 = NULL;
            free(fi->pkd); fi->pkd = NULL;
        }

        if (fi->ccd != NULL)
        { 
            int i = 0, j = 0;
    	    CompCharData *ccd = fi->ccd;
    	    
    	    for (i = 0; i < fi->numOfComps; ++i)
    	    {
    	        for (j = 0; j < ccd[i].numOfPieces; ++j)
    	        {
    	            free(ccd[i].pieces[j].pccName); 
    	            ccd[i].pieces[j].pccName = NULL;
    	        }
    	        
    	        free(ccd[i].ccName); ccd[i].ccName = NULL;
    	    }
    
            free(fi->ccd); fi->ccd = NULL;
        }
        
        free(fi);

    } /* if */ 
  
} /* freeStorage */


/*************************** printValues **************************/

printValues()
{
    if (fi->gfi != NULL) printGlobals();
    if (fi->cmi != NULL) printAllCharMetrics();
    if (fi->pkd != NULL) printKernPairData();
    if (fi->tkd != NULL) printKernTrackData(); 
    if (fi->ccd != NULL) printCompCharData();
    printf("EndFontMetrics\n");

} /* printValues */



/*************************** main *********************************/

main (argc, argv) 
  int argc; 
  char **argv;
{
    char *filename, *prog = *argv;
    FILE *fp;
    
    ++argv; --argc; myflags |= P_ALL; 

    if (!argc)
    {
                printf("    usage: parseAFM [AFM File]\n");
                exit(0);
    }
    else 
        filename = *argv;

    if (!filename[0]) 
    {
	printf ("*** ERROR: can't open. filename is missing.\n",
		filename );
	return 0;
    }
       
    fp = fopen(filename, "r" );
    if (fp == NULL) 
    {
	printf ("*** ERROR: can't find: %s\n", filename );
        exit(1);
    }
    
    switch (parseFile(fp, &fi, myflags))
    {
        case parseError:
            printf("*** ERROR: problem in parsing the AFM File.\n");
        case ok:
            fclose(fp);
            printValues();
            freeStorage();
            break;
        case earlyEOF:
            printf("The AFM File ended prematurely.\n");
            break;
        case storageProblem:
            printf("*** ERROR: problem allocating storage.\n");
            break;
        default:
            break;
    }
    
} /* main */
