#ifndef __PROMPT_H
#define __PROMPT_H

#include "charset.h"

#define curshome() {home}
#define cursnext() {^M}

#define clrline() {Del to EOL}
#define clrscreen() {Del to EOP}

#define MSG_BLINK      {^P}
#define MSG_BOLD       {^L}
#define MSG_DBLUND     {^K}
#define MSG_FINE       {^D}
#define MSG_ITALIC     {Home}
#define MSG_LARGE      {^B}
#define MSG_OUTLINE    {^G}
#define MSG_REDLINE    {Enter}
#define MSG_REV        {^Q}
#define MSG_SHADOW     {^I}
#define MSG_SMALL      {^C}
#define MSG_SMCAP      {^O}
#define MSG_STRKOUT    {^M}
#define MSG_SUBSCRPT   {^F}
#define MSG_SUPSCRPT   {^E}
#define MSG_UND        {^N}
#define MSG_VRYLRG     {^A}

#define attron(a)  {^N}_(a)
#define attroff(a) {^O}_(a)
#define attrclr()  {^Q}

// Useful for making menu boxes.
#define reverse(txt) attron(MSG_REV) _(txt) attroff(MSG_REV)
#define bold(txt) attron(MSG_BOLD) _(txt) attroff(MSG_BOLD)

// This is the table from Appendix A.
assign(_pos_table, (
    // 0        1        2        3        4        5        6        7        8        9
    {C 4,2}  {C 5,7}  {C 5,8}  {C 5,0}  {C 5,1}  {C 5,2}  {C 5,3}  {C 4,0}  {C 5,18} {C 4,1}  // 0
    {C 5,19} {C 5,4}  {C 5,5}  {C 5,9}  {C 5,10} {C 5,6}  {C 6,27} {C 6,28} {C 6,26} {C 5,13} // 10
    {C 4,5}  {C 4,6}  {C 5,11} {C 5,15} {C 6,23} {C 6,24} {C 6,21} {C 6,22} {C 5,17} {C 6,25} // 20
    {C 6,29} {C 6,30} {C 0,32} {C 0,33} {C 0,34} {C 0,35} {C 0,36} {C 0,37} {C 0,38} {C 0,39} // 30
    {C 0,40} {C 0,41} {C 0,42} {C 0,43} {C 0,44} {C 0,45} {C 0,46} {C 0,47} {C 0,48} {C 0,49} // 40
    {C 0,50} {C 0,51} {C 0,52} {C 0,53} {C 0,54} {C 0,55} {C 0,56} {C 0,57} {C 0,58} {C 0,59} // 50
    {C 0,60} {C 0,61} {C 0,62} {C 0,63} {C 0,64} {C 0,65} {C 0,66} {C 0,67} {C 0,68} {C 0,69} // 60
    {C 0,70} {C 0,71} {C 0,72} {C 0,73} {C 0,74} {C 0,75} {C 0,76} {C 0,77} {C 0,78} {C 0,79} // 70
    {C 0,80} {C 0,81} {C 0,82} {C 0,83} {C 0,84} {C 0,85} {C 0,86} {C 0,87} {C 0,88} {C 0,89} // 80
    {C 0,90} {C 0,91} {C 0,92} {C 0,93} {C 0,94} {C 0,95} {C 0,96} {C 0,97} {C 0,98} {C 0,99} // 90
    {C 0,100}
))

assign(_box_horiz, `════════════════════════════════════════════════════════════════════════════════`
)
assign(_box_space,
    { }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }
    { }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }
    { }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }{ }
)

#define msgpos(x,y) {^P} mid(_pos_table,_(x),1) \
                         mid(_pos_table,_(y),1)

#define boxtop(x, y, w) msgpos(_(x),_(y)) `╔` mid(_box_horiz, 0, _(w)) `╗`
#define boxbot(x, y, w) msgpos(_(x),_(y)) `╚` mid(_box_horiz, 0, _(w)) `╝`
#define boxtxt(x, y, w, txt) msgpos(_(x),_(y))                  \
                            `║` mid(_box_space, 0, _(w)) `║`    \
                            msgpos(_(x),_(y)) {right} _(txt)
#define boxsel(x, y, w, txt) msgpos(_(x),_(y))                  \
                            `║` attron(MSG_REV)                 \
                            mid(_box_space, 0, _(w))            \
                            attroff(MSG_REV) `║`                \
                            msgpos(_(x),_(y)) {right}           \
                            reverse(_(txt))

#endif
