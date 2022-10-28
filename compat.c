#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/sysmacros.h>
#include <sys/times.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

#include "typecompat.h"
#include "compat32.h"

#define _STAT_VER_LINUX_OLD 1

static void translate_stat64_struct(struct oldstat *dst, struct linux_stat64 *src)
{
    dst->st_dev     = src->st_dev;
    dst->st_ino     = src->st_ino;
    dst->st_mode    = src->st_mode;
    dst->st_nlink   = src->st_nlink;
    dst->st_uid     = src->st_uid;
    dst->st_gid     = src->st_gid;
    dst->st_rdev    = src->st_rdev;
    dst->st_size    = src->st_size;
    dst->st_blksize = src->st_blksize;
    dst->st_blocks  = src->st_blocks;
    dst->st_atime_  = src->st_atime_;
    dst->st_mtime_  = src->st_mtime_;
    dst->st_ctime_  = src->st_ctime_;
}

static size_t translate_getdents(struct olddirent *dst, struct linux_dirent64 *src, size_t srcsz)
{
    size_t dstsz = 0;

    // I know for sure a dirent64 is smaller than a dirent, so no need to check
    // if it fits.
    while (srcsz) {
        strncpy(dst->d_name, src->d_name, sizeof(dst->d_name));

        dst->d_ino      = src->d_ino;
        dst->d_reclen   = sizeof(*dst) - sizeof(dst->d_name) + strlen(dst->d_name) + 1 + 1;
        dst->d_off      = src->d_off;

        // I don't know what to do here, well this is better than just
        // aborting.
        if (dst->d_off != src->d_off) {
            dst->d_off = -1;
        }

        srcsz          -= src->d_reclen;
        dstsz          += dst->d_reclen;
        dst             = (void *)((char *) dst + dst->d_reclen);
        src             = (void *)((char *) src + src->d_reclen);
    }

    return dstsz;
}

int _fxstat(int ver, int fd, void *statbuf)
{
    int result;
    struct linux_stat64 stat64buf;

    if (ver != _STAT_VER_LINUX_OLD) {
        return -1;
    }

    // We will just truncate long values.
    result = syscall(__NR_fstat64, fd, &stat64buf);

    translate_stat64_struct(statbuf, &stat64buf);

    return result;
}

int _xstat(int ver, const char *pathname, void *statbuf)
{
    int result;
    struct linux_stat64 stat64buf;

    if (ver != _STAT_VER_LINUX_OLD) {
        return -1;
    }

    // We will just truncate long values.
    result = syscall(__NR_stat64, pathname, &stat64buf);

    translate_stat64_struct(statbuf, &stat64buf);

    return result;
}

int _lxstat(int ver, const char *pathname, void *statbuf)
{
    int result;
    struct linux_stat64 stat64buf;

    if (ver != _STAT_VER_LINUX_OLD) {
        return -1;
    }

    // We will just truncate long values.
    result = syscall(__NR_lstat64, pathname, &stat64buf);

    translate_stat64_struct(statbuf, &stat64buf);

    return result;
}

struct dirent * readdir (DIR *dirp)
{
    struct dirstream *dir = (struct dirstream *) dirp;
    struct olddirent *de;
    int result;

    // Avoid bugs with large directory trees.
    dir->dd_max = dir->dd_max > sizeof(*de)
                ? sizeof(*de)
                : dir->dd_max;

    if (dir->dd_size <= dir->dd_nextloc) {
        void *buf = alloca(dir->dd_max);

        // read dir->dd_max bytes of directory entries.
        result = syscall(__NR_getdents64, dir->dd_fd, buf, dir->dd_max);

        if (result <= 0) {
            return NULL;
        }

        dir->dd_size = translate_getdents(dir->dd_buf, buf, result);
        dir->dd_nextloc = 0;
    }

    de = (void *) (((char *)dir->dd_buf) + dir->dd_nextloc);
    dir->dd_nextloc += de->d_reclen;
    dir->dd_nextoff = de->d_off;
    return (void *) de;
}

// WordPerfect is so old it uses geteuid16, lol.
uid_t geteuid(void)
{
    return syscall(__NR_geteuid32);
}

uid_t getuid(void)
{
    return syscall(__NR_getuid32);
}

clock_t times(struct tms *buf)
{
    // Adjust the ticks origin, to avoid early-boot compatability issues. Note
    // that wp does not use the parameter.
    return (time(0) - (50 * 365 * 24 * 60 * 60)) * sysconf(_SC_CLK_TCK);
}
