#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <direntry.h>
#include <string.h>
#include <err.h>

#define __NR_readlink 85
#define __NR_stat 106
#define __NR_lstat 107
#define __NR_fstat 108
#define __NR_stat64 195
#define __NR_lstat64 196
#define __NR_fstat64 197
#define __NR_geteuid32 201
#define __NR_getdents 141
#define __NR_getdents64 220

struct dirent64 {
    unsigned long long d_ino;
    unsigned long long d_off;
    unsigned short d_reclen;
    unsigned char  d_type;
    char           d_name[256];
};

// The dirent structure expected by libc5 applications.
struct olddirent {
    long d_ino;
    long d_off;
    unsigned short d_reclen;
    char d_name[256];
    char _pad0;
    char _pad1;
};

struct oldstat {
    unsigned long  st_dev;
    unsigned long  st_ino;
    unsigned short st_mode;
    unsigned short st_nlink;
    unsigned short st_uid;
    unsigned short st_gid;
    unsigned long  st_rdev;
    unsigned long  st_size;
    unsigned long  st_blksize;
    unsigned long  st_blocks;
    unsigned long  st_atime;
    unsigned long  st_atime_nsec;
    unsigned long  st_mtime;
    unsigned long  st_mtime_nsec;
    unsigned long  st_ctime;
    unsigned long  st_ctime_nsec;
    unsigned long  __unused4;
    unsigned long  __unused5;
};

struct stat64 {
    unsigned long long  st_dev;
    unsigned char       __pad0[4];
    unsigned long       __st_ino;
    unsigned int        st_mode;
    unsigned int        st_nlink;
    unsigned long       st_uid;
    unsigned long       st_gid;
    unsigned long long  st_rdev;
    unsigned char       __pad3[4];
    long long           st_size;
    unsigned long       st_blksize;
    unsigned long long  st_blocks;
    unsigned long       st_atime;
    unsigned long       st_atime_nsec;
    unsigned long       st_mtime;
    unsigned int        st_mtime_nsec;
    unsigned long       st_ctime;
    unsigned long       st_ctime_nsec;
    unsigned long long  st_ino;
};

static void translate_stat64_struct(struct oldstat *dst, struct stat64 *src)
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
    dst->st_atime   = src->st_atime;
    dst->st_mtime   = src->st_mtime;
    dst->st_ctime   = src->st_ctime;
}

static size_t translate_getdents(struct olddirent *dst, struct dirent64 *src, size_t srcsz)
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
    struct stat64 stat64buf;

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
    struct stat64 stat64buf;

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
    struct stat64 stat64buf;

    if (ver != _STAT_VER_LINUX_OLD) {
        return -1;
    }

    // We will just truncate long values.
    result = syscall(__NR_lstat64, pathname, &stat64buf);

    translate_stat64_struct(statbuf, &stat64buf);

    return result;
}

struct dirstream {
    int dd_fd;
    off_t dd_nextloc;
    size_t dd_size;
    void *dd_buf;
    off_t dd_nextoff;
    size_t dd_max;
    int dd_getdents;
    void *dd_lock;
};

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
