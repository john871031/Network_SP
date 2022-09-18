#include <stdio.h>
#include <fcntl.h>
#include "alt_functions.h"

/* A very minimal implementation of strsignal()... */

#define BUF_SIZE 100

char *
ALT_strsignal(int sig)
{
    static char buf[BUF_SIZE];          /* Not thread-safe */

    snprintf(buf, BUF_SIZE, "SIG-%d", sig);
    return buf;
}

/* A very minimal implementation of hstrerror()... */

char *
ALT_hstrerror(int err)
{
    static char buf[BUF_SIZE];          /* Not thread-safe */

    snprintf(buf, BUF_SIZE, "hstrerror-%d", err);
    return buf;
}

/* posix_openpt() is simple to implement */

int
ALT_posix_openpt(int flags)
{
    return open("/dev/ptmx", flags);
}
