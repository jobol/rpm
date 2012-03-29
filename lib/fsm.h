#ifndef H_FSM
#define H_FSM

/** \ingroup payload
 * \file lib/fsm.h
 * File state machine to handle a payload within an rpm package.
 */

#include <rpm/rpmfi.h>
#include "cpio.h"

extern int _fsm_debug;

/**
 */
#define	FSM_VERBOSE	0x8000
#define	FSM_INTERNAL	0x4000
#define	FSM_SYSCALL	0x2000
#define	FSM_DEAD	0x1000

#define	_fv(_a)		((_a) | FSM_VERBOSE)
#define	_fi(_a)		((_a) | FSM_INTERNAL)
#define	_fs(_a)		((_a) | (FSM_INTERNAL | FSM_SYSCALL))
#define	_fd(_a)		((_a) | (FSM_INTERNAL | FSM_DEAD))

typedef enum fileStage_e {
    FSM_UNKNOWN =   0,
    FSM_INIT	=  _fd(1),
    FSM_PRE	=  _fd(2),
    FSM_PROCESS	=  _fv(3),
    FSM_POST	=  _fd(4),
    FSM_UNDO	=  5,
    FSM_FINI	=  6,

    FSM_PKGINSTALL	= _fd(7),
    FSM_PKGERASE	= _fd(8),
    FSM_PKGBUILD	= _fd(9),
    FSM_PKGUNDO		= _fd(11),

    FSM_CREATE	=  _fd(17),
    FSM_MAP	=  _fd(18),
    FSM_MKDIRS	=  _fi(19),
    FSM_RMDIRS	=  _fi(20),
    FSM_MKLINKS	=  _fi(21),
    FSM_NOTIFY	=  _fd(22),
    FSM_DESTROY	=  _fd(23),
    FSM_VERIFY	=  _fd(24),
    FSM_COMMIT	=  _fd(25),

    FSM_UNLINK	=  _fs(33),
    FSM_RENAME	=  _fs(34),
    FSM_MKDIR	=  _fs(35),
    FSM_RMDIR	=  _fs(36),
    FSM_LSETFCON=  _fs(39),
    FSM_CHOWN	=  _fs(40),
    FSM_LCHOWN	=  _fs(41),
    FSM_CHMOD	=  _fs(42),
    FSM_UTIME	=  _fs(43),
    FSM_SYMLINK	=  _fs(44),
    FSM_LINK	=  _fs(45),
    FSM_MKFIFO	=  _fs(46),
    FSM_MKNOD	=  _fs(47),
    FSM_LSTAT	=  _fs(48),
    FSM_STAT	=  _fs(49),
    FSM_READLINK=  _fs(50),
    FSM_SETCAP	=  _fs(52),

    FSM_NEXT	=  _fd(65),
    FSM_EAT	=  _fd(66),
    FSM_POS	=  _fd(67),
    FSM_PAD	=  _fd(68),
    FSM_TRAILER	=  _fd(69),
    FSM_HREAD	=  _fd(70),
    FSM_HWRITE	=  _fd(71),
    FSM_DREAD	=  _fs(72),
    FSM_DWRITE	=  _fs(73),
} fileStage;
#undef	_fv
#undef	_fi
#undef	_fs
#undef	_fd

/** \ingroup payload
 */
enum cpioMapFlags_e {
    CPIO_MAP_PATH	= (1 << 0),
    CPIO_MAP_MODE	= (1 << 1),
    CPIO_MAP_UID	= (1 << 2),
    CPIO_MAP_GID	= (1 << 3),
    CPIO_FOLLOW_SYMLINKS= (1 << 4), /*!< only for building. */
    CPIO_MAP_ABSOLUTE	= (1 << 5),
    CPIO_MAP_ADDDOT	= (1 << 6),
    CPIO_MAP_TYPE	= (1 << 8),  /*!< only for building. */
    CPIO_SBIT_CHECK	= (1 << 9)
};
typedef rpmFlags cpioMapFlags;

typedef struct fsmIterator_s * FSMI_t;
typedef struct fsm_s * FSM_t;
typedef struct rpmpsm_s * rpmpsm;

typedef struct hardLink_s * hardLink_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Execute a file state machine goal
 * @param goal
 * @param ts		transaction set
 * @param fi		transaction element file info
 * @param cfd
 * @param psm		owner psm (or NULL)
 * @retval archiveSize	pointer to archive size
 * @retval failedFile	pointer to first file name that failed (malloced)
 * @return		0 on success
 */
int rpmfsmRun(fileStage goal, rpmts ts, rpmte te, rpmfi fi, FD_t cfd,
	      rpmpsm psm, rpm_loff_t * archiveSize, char ** failedFile);

int rpmPackageFilesInstall(rpmts ts, rpmte te, rpmfi fi, FD_t cfd,
              rpmpsm psm, char ** failedFile);

int rpmPackageFilesRemove(rpmts ts, rpmte te, rpmfi fi,
              rpmpsm psm, char ** failedFile);

RPM_GNUC_INTERNAL
void rpmpsmNotify(rpmpsm psm, int what, rpm_loff_t amount);
#ifdef __cplusplus
}
#endif

#endif	/* H_FSM */
