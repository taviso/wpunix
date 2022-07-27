#ifndef __TYPECOMPAT_H
#define __TYPECOMPAT_H

// These are the native kernel structure definitions.
struct linux_dirent64 {
    uint64_t d_ino;
    int64_t d_off;
    uint16_t d_reclen;
    uint8_t d_type;
    char d_name[0];
};

struct linux_stat64 {
    uint64_t st_dev;
    uint32_t __pad0;
    uint32_t __st_ino;
    uint32_t st_mode;
    uint32_t st_nlink;
    uint32_t st_uid;
    uint32_t st_gid;
    uint64_t st_rdev;
    uint32_t __pad3;
    int64_t  st_size;
    uint32_t st_blksize;
    uint64_t st_blocks;
    uint32_t st_atime_;
    uint32_t st_atime_nsec;
    uint32_t st_mtime_;
    uint32_t st_mtime_nsec;
    uint32_t st_ctime_;
    uint32_t st_ctime_nsec;
    uint64_t st_ino;
};

// The dirent structure expected by libc5 applications.
struct olddirent {
    uint32_t d_ino;
    uint32_t d_off;
    uint16_t d_reclen;
    char d_name[256];
    int8_t _pad0;
    int8_t _pad1;
};

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

struct oldstat {
    uint32_t st_dev;
    uint32_t st_ino;
    uint16_t st_mode;
    uint16_t st_nlink;
    uint16_t st_uid;
    uint16_t st_gid;
    uint32_t st_rdev;
    uint32_t st_size;
    uint32_t st_blksize;
    uint32_t st_blocks;
    uint32_t st_atime_;
    uint32_t st_atime_nsec;
    uint32_t st_mtime_;
    uint32_t st_mtime_nsec;
    uint32_t st_ctime_;
    uint32_t st_ctime_nsec;
    uint32_t __unused4;
    uint32_t __unused5;
};

#endif
