#ifndef __GSEVAL_H
#define __GSEVAL_H

void gs_init_interpreter();
void gs_shutdown();

int32_t gs_eval_int(const char *str, ...);
char * gs_eval_str(const char *str, ...);
bool gs_eval(const char *str, ...);

#endif
