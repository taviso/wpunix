#
# vim: ft=asm
#

.include "stabs.s"

.include "basemap.s"

# WARNING: These definitions should be sorted by address.
resetorder

function main, 0x8128000, %int
    param argc, %int
    param argv, ** %char
    param envp, ** %char

function pcline, 0x8129200, %int
    param argc, %int
    param argv, ** %char

function skipw, 0x8129250, %int
    param argc, %int
    param argv, ** %char

function writesyntax, 0x8129f30, %void

function seterminal, 0x812A0D0, %int
    param str, * %char

function wopen51, 0x81cb030, %int
    param fcb, * %void

function rddec, 0x81dac90, %bool
    param numstr, ** %char
    param num, * %uint16_t

function set_tname, 0x82013C0, %void
    param name, * %char

function xy_mouse, 0x8204060, %void

function MessageBox, 0x823b710, %int
    param type, %unsigned
    param message, * %char

function GkWarning, 0x823b7d0, %int
    param message, * %char

function GkYesNo, 0x0823b7f0, %int
    param message, * %char

function GkOkCancel, 0x823b810, %int
    param message, * %char

function wstrcat, 0x82bc600, * %char
    param s1, * %char
    param s2, * %char

function wstrcpy, 0x82bc6b0, * %char
    param s1, * %char
    param s2, * %char

function wstrlen, 0x82bc760, %size_t
    param str, * %char

function wstrncmp, 0x82BC830, %size_t
    param s1, * %char
    param s2, * %char
    param n, %size_t

function initthird, 0x82bcb50, %int

function thrSavePipes, 0x82bce30, %int
    param pipestr, * %char
