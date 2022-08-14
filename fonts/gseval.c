#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include <ghostscript/ierrors.h>
#include <ghostscript/iapi.h>

#include "util.h"
#include "gseval.h"

#define MAX_CMD 1024

static char *outbuf;
static size_t outbufsz;

static int gsdll_stdin(void *instance, char *buf, int len)
{
    return 0;
}

static int gsdll_stdout(void *instance, const char *str, int len)
{
    // Make room for more data.
    outbuf = realloc(outbuf, outbufsz + len + 1);

    // Append this output.
    memcpy(outbuf + outbufsz, str, len);

    // Add a nul terminator.
    outbuf[outbufsz += len] = 0;
    return len;
}

static int gsdll_stderr(void *instance, const char *str, int len)
{
    return fwrite(str, 1, len, stderr);
}

static void *minst;

void gs_init_interpreter()
{
    char * gsargs[] = { "gseval", "-q" };

    if (gsapi_new_instance(&minst, NULL) < 0) {
        fprintf(stderr, "gsapi_new_instance() failed\n");
        exit(EXIT_FAILURE);
    }

    gsapi_set_arg_encoding(minst, GS_ARG_ENCODING_UTF8);

    gsapi_set_stdio(minst, gsdll_stdin, gsdll_stdout, gsdll_stderr);

    if (gsapi_init_with_args(minst, countof(gsargs), gsargs) != 0){
        fprintf(stderr, "gsapi_init_with_args() failed\n");
        exit(EXIT_FAILURE);
    }
    return;
}

static void gs_reset_output()
{
    free(outbuf);
    outbufsz = 0;
    outbuf   = NULL;
}

void gs_shutdown()
{
    gsapi_exit(minst);
    gsapi_delete_instance(minst);

    // Clear output buffer.
    gs_reset_output();

    // Reset pointers.
    minst = NULL;
}

int32_t gs_eval_int(const char *str, ...)
{
    va_list ap;
    int code;
    int result;
    char cmdbuf[MAX_CMD];

    // Reset output.
    gs_reset_output();

    va_start(ap, str);
        vsnprintf(cmdbuf, MAX_CMD, str, ap);
    va_end(ap);

    code = gsapi_run_string(minst, cmdbuf, 0, &result);

    if (code != 0) {
        fprintf(stderr,"gs error for `%s`\n", cmdbuf);
        exit(EXIT_FAILURE);
    }

    gsapi_run_string(minst, "flush", 0, &result);

    assert(outbufsz);

    return strtol(outbuf, NULL, 10);
}

char * gs_eval_str(const char *str, ...)
{
    va_list ap;
    int code;
    int result;
    char cmdbuf[MAX_CMD];

    // Reset output.
    gs_reset_output();

    va_start(ap, str);
        vsnprintf(cmdbuf, MAX_CMD, str, ap);
    va_end(ap);


    code = gsapi_run_string(minst, cmdbuf, 0, &result);

    if (code != 0) {
        fprintf(stderr, "gs error for `%s`\n", cmdbuf);
        exit(EXIT_FAILURE);
    }
    gsapi_run_string(minst, "flush", 0, &result);

    assert(outbufsz);

    // Truncate to the first newline.
    *strchrnul(outbuf, '\n') = '\0';

    return strdup(outbuf);
}

bool gs_eval(const char *str, ...)
{
    va_list ap;
    int result;
    char cmdbuf[MAX_CMD];

    va_start(ap, str);
        vsnprintf(cmdbuf, MAX_CMD, str, ap);
    va_end(ap);

    return gsapi_run_string(minst, cmdbuf, 0, &result) == 0;
}
