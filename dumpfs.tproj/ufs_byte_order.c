/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/* Copyright 1998 Apple Computer, Inc.
 *
 * UFS byte swapping routines to make a big endian file system useful on a
 * little endian machine.
 *
 * HISTORY
 *
 * 16 Feb 1998 A. Ramesh at Apple
 *      Rhapsody version created.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <ufs/ufs/dinode.h>
#include <ufs/ufs/dir.h>
#include <ufs/ffs/fs.h>
#include "ufs_byte_order.h"
#include <libkern/OSByteOrder.h>



#if 0
#define	byte_swap_longlong(thing) ((thing) = OSSwapBigToHostInt64(thing))
#define	byte_swap_int(thing) ((thing) = OSSwapBigToHostInt32(thing))
#define	byte_swap_short(thing) ((thing) = OSSwapBigToHostInt16(thing))
#else
#define	byte_swap_longlong(thing) ((thing) = OSSwapInt64(thing))
#define	byte_swap_int(thing) ((thing) = OSSwapInt32(thing))
#define	byte_swap_short(thing) ((thing) = OSSwapInt16(thing))
#endif
void
byte_swap_longlongs(unsigned long long *array, int count)
{
	register unsigned long long	i;

	for (i = 0;  i < count;  i++)
		byte_swap_longlong(array[i]);
}

void
byte_swap_ints(int *array, int count)
{
	register int	i;

	for (i = 0;  i < count;  i++)
		byte_swap_int(array[i]);
}


void
byte_swap_shorts(short *array, int count)
{
	register int	i;

	for (i = 0;  i < count;  i++)
		byte_swap_short(array[i]);
}


void
byte_swap_sbin(struct fs *sb)
{
	u_int16_t *usptr;
	unsigned long size;

	byte_swap_ints(((int32_t *)&sb->fs_firstfield), 52);
	byte_swap_int(sb->fs_cgrotor);
	byte_swap_int(sb->fs_cpc);
        byte_swap_shorts((int16_t *)sb->fs_opostbl,
                         sizeof(sb->fs_opostbl) / sizeof(int16_t));
        byte_swap_int(sb->fs_avgfilesize);
        byte_swap_int(sb->fs_avgfpdir);
        byte_swap_ints((int32_t *)sb->fs_sparecon,
                       sizeof(sb->fs_sparecon) / sizeof(int32_t));
	byte_swap_ints((int32_t *)&sb->fs_contigsumsize, 3);
	byte_swap_longlongs((u_int64_t *)&sb->fs_maxfilesize,3);
	byte_swap_ints((int32_t *)&sb->fs_state, 6);

	/* Got these magic numbers from mkfs.c in newfs */
	if (sb->fs_nrpos != 8 || sb->fs_cpc > 16) {
		usptr = (u_int16_t *)((u_int8_t *)(sb) + (sb)->fs_postbloff);
		size = sb->fs_cpc * sb->fs_nrpos;
		byte_swap_shorts((short *)usptr,size);	/* fs_postbloff */
	}
}

void
byte_swap_sbout(struct fs *sb)
{
	u_int16_t *usptr;
	unsigned long size;
	/* Got these magic numbers from mkfs.c in newfs */
	if (sb->fs_nrpos != 8 || sb->fs_cpc > 16) {
		usptr = (u_int16_t *)((u_int8_t *)(sb) + (sb)->fs_postbloff);
		size = sb->fs_cpc * sb->fs_nrpos;
		byte_swap_shorts((short *)usptr,size);	/* fs_postbloff */
	}

	byte_swap_ints(((int32_t *)&sb->fs_firstfield), 52);
	byte_swap_int(sb->fs_cgrotor);
	byte_swap_int(sb->fs_cpc);
        byte_swap_shorts((int16_t *)sb->fs_opostbl,
                         sizeof(sb->fs_opostbl) / sizeof(int16_t));
        byte_swap_int(sb->fs_avgfilesize);
        byte_swap_int(sb->fs_avgfpdir);
        byte_swap_ints((int32_t *)sb->fs_sparecon,
                       sizeof(sb->fs_sparecon) / sizeof(int32_t));
	byte_swap_ints((int32_t *)&sb->fs_contigsumsize, 3);
	byte_swap_longlongs((u_int64_t *)&sb->fs_maxfilesize,3);
	byte_swap_ints((int32_t *)&sb->fs_state, 6);
}
void
byte_swap_csum(struct csum *cs)
{
	byte_swap_ints((int *) cs, sizeof(struct csum) / sizeof(int32_t));
}

/* This is for the new 4.4 cylinder group block */
void
byte_swap_cgin(struct cg *cg, struct fs * fs)
{
	int32_t * ulptr;
	int16_t * usptr;
	int size;

	byte_swap_int(cg->cg_firstfield);
	byte_swap_int(cg->cg_magic);
	byte_swap_int(cg->cg_time);
	byte_swap_int(cg->cg_cgx);
	byte_swap_short(cg->cg_ncyl);
	byte_swap_short(cg->cg_niblk);
	byte_swap_int(cg->cg_ndblk);
	byte_swap_csum(&cg->cg_cs);
	byte_swap_int(cg->cg_rotor);
	byte_swap_int(cg->cg_frotor);
	byte_swap_int(cg->cg_irotor);
	byte_swap_ints(cg->cg_frsum, MAXFRAG);
	byte_swap_int(cg->cg_iusedoff);
	byte_swap_int(cg->cg_freeoff);
	byte_swap_int(cg->cg_nextfreeoff);
	byte_swap_int(cg->cg_clusteroff);
	byte_swap_int(cg->cg_nclusterblks);
	byte_swap_ints((int *)&cg->cg_sparecon, 13);

	byte_swap_int(cg->cg_btotoff);
	ulptr = ((int32_t *)((u_int8_t *)(cg) + (cg)->cg_btotoff));
	size = fs->fs_cpg;
	byte_swap_ints(ulptr, size);	/*cg_btotoff*/

	byte_swap_int(cg->cg_boff);
	usptr = ((int16_t *)((u_int8_t *)(cg) + (cg)->cg_boff));
	size = fs->fs_cpg * fs->fs_nrpos;
	byte_swap_shorts(usptr,size);	/*cg_boff*/

	byte_swap_int(cg->cg_clustersumoff);

	if ((unsigned int)fs->fs_contigsumsize > 0) {

	ulptr = ((int32_t *)((u_int8_t *)(cg) + (cg)->cg_clustersumoff));
		size = (fs->fs_contigsumsize + 1);
		byte_swap_ints(ulptr, size);	/*cg_clustersumoff*/
	}

}

// This is for the new 4.4 cylinder group block
void
byte_swap_cgout(struct cg *cg, struct fs * fs)
{
	int32_t * ulptr;
	int16_t * usptr;
	int size;

	byte_swap_int(cg->cg_firstfield);
	byte_swap_int(cg->cg_magic);
	byte_swap_int(cg->cg_time);
	byte_swap_int(cg->cg_cgx);
	byte_swap_short(cg->cg_ncyl);
	byte_swap_short(cg->cg_niblk);
	byte_swap_int(cg->cg_ndblk);
	byte_swap_csum(&cg->cg_cs);
	byte_swap_int(cg->cg_rotor);
	byte_swap_int(cg->cg_frotor);
	byte_swap_int(cg->cg_irotor);
	byte_swap_ints(cg->cg_frsum, MAXFRAG);
	byte_swap_int(cg->cg_freeoff);
	byte_swap_int(cg->cg_nextfreeoff);
	byte_swap_int(cg->cg_nclusterblks);
	byte_swap_ints((int *)&cg->cg_sparecon, 13);

	byte_swap_int(cg->cg_iusedoff);
	byte_swap_int(cg->cg_clusteroff);
	ulptr = ((int32_t *)((u_int8_t *)(cg) + (cg)->cg_btotoff));
	size = fs->fs_cpg;
	byte_swap_ints(ulptr, size);	/*cg_btotoff*/
	byte_swap_int(cg->cg_btotoff);

	usptr = ((int16_t *)((u_int8_t *)(cg) + (cg)->cg_boff));
	size = fs->fs_cpg * fs->fs_nrpos;
	byte_swap_shorts(usptr,size);	/*cg_boff*/
	byte_swap_int(cg->cg_boff);

	if ((unsigned int)fs->fs_contigsumsize > 0) {
	ulptr = ((int32_t *)((u_int8_t *)(cg) + (cg)->cg_clustersumoff));
		size = (fs->fs_contigsumsize + 1);
		byte_swap_ints(ulptr, size);	/*cg_clustersumoff*/

	}
	byte_swap_int(cg->cg_clustersumoff);

}

/* This value should correspond to the value set in the ffs_mounts */

#define RESYMLNKLEN 60

void
byte_swap_dinodecount(struct dinode *ep, int count, int writeout)
{
	int i,numinodes;
	struct dinode *dp;

	dp = ep;
	numinodes = count/sizeof(struct dinode);
	for(i=0;i<numinodes;i++) {
		if(writeout)
			byte_swap_dinode_out(dp);
		else
			byte_swap_dinode_in(dp);
		dp++;
	}
	
}

void
byte_swap_dinode_in(struct dinode *di)
{
	int i;

	di->di_mode = OSSwapInt16(di->di_mode);
	di->di_nlink = OSSwapInt16(di->di_nlink);
#ifdef LFS
	di->di_u.inumber = OSSwapInt32(di->di_u.inumber);
#else
	di->di_u.oldids[0] = OSSwapInt16(di->di_u.oldids[0]);
	di->di_u.oldids[1] = OSSwapInt16(di->di_u.oldids[1]);
#endif
	di->di_size = OSSwapInt64(di->di_size);
	di->di_atime = OSSwapInt32(di->di_atime);
	di->di_atimensec = OSSwapInt32(di->di_atimensec);
	di->di_mtime = OSSwapInt32(di->di_mtime);
	di->di_mtimensec = OSSwapInt32(di->di_mtimensec);
	di->di_ctime = OSSwapInt32(di->di_ctime);
	di->di_ctimensec = OSSwapInt32(di->di_ctimensec);
	if (((di->di_mode & IFMT) != IFLNK ) || (di->di_size > RESYMLNKLEN)) {
		for (i=0; i < NDADDR; i++)	/* direct blocks */
			di->di_db[i] = OSSwapInt32(di->di_db[i]);
		for (i=0; i < NIADDR; i++)	/* indirect blocks */
			di->di_ib[i] = OSSwapInt32(di->di_ib[i]);
	}
	di->di_flags = OSSwapInt32(di->di_flags);
	di->di_blocks = OSSwapInt32(di->di_blocks);
	di->di_gen = OSSwapInt32(di->di_gen);
	di->di_uid = OSSwapInt32(di->di_uid);
	di->di_gid = OSSwapInt32(di->di_gid);
	di->di_spare[0] = OSSwapInt32(di->di_spare[0]);
	di->di_spare[1] = OSSwapInt32(di->di_spare[1]);
}


void
byte_swap_dinode_out(struct dinode *di)
{
	int i;
	int mode;
	int64_t inosize;

	mode = (di->di_mode & IFMT);
	inosize = di->di_size;
 
	di->di_mode = OSSwapInt16(di->di_mode);
	di->di_nlink = OSSwapInt16(di->di_nlink);
#ifdef LFS
	di->di_u.inumber = OSSwapInt32(di->di_u.inumber);
#else
	di->di_u.oldids[0] = OSSwapInt16(di->di_u.oldids[0]);
	di->di_u.oldids[1] = OSSwapInt16(di->di_u.oldids[1]);
#endif
	di->di_size = OSSwapInt64(di->di_size);
	di->di_atime = OSSwapInt32(di->di_atime);
	di->di_atimensec = OSSwapInt32(di->di_atimensec);
	di->di_mtime = OSSwapInt32(di->di_mtime);
	di->di_mtimensec = OSSwapInt32(di->di_mtimensec);
	di->di_ctime = OSSwapInt32(di->di_ctime);
	di->di_ctimensec = OSSwapInt32(di->di_ctimensec);
	if ((mode != IFLNK) || (inosize > RESYMLNKLEN)) {
		for (i=0; i < NDADDR; i++)	/* direct blocks */
			di->di_db[i] = OSSwapInt32(di->di_db[i]);
		for (i=0; i < NIADDR; i++)	/* indirect blocks */
			di->di_ib[i] = OSSwapInt32(di->di_ib[i]);
	}
	di->di_flags = OSSwapInt32(di->di_flags);
	di->di_blocks = OSSwapInt32(di->di_blocks);
	di->di_gen = OSSwapInt32(di->di_gen);
	di->di_uid = OSSwapInt32(di->di_uid);
	di->di_gid = OSSwapInt32(di->di_gid);
	di->di_spare[0] = OSSwapInt32(di->di_spare[0]);
	di->di_spare[1] = OSSwapInt32(di->di_spare[1]);
}

#if notdef /* [ */
void
byte_swap_direct(struct direct *dirp)
{
	byte_swap_int(dirp->d_ino);
	byte_swap_short(dirp->d_reclen);
}

void
byte_swap_dirtemplate_in(struct dirtemplate *dirt)
{
	byte_swap_int(dirt->dot_ino);
	byte_swap_short(dirt->dot_reclen);
	byte_swap_int(dirt->dotdot_ino);
	byte_swap_short(dirt->dotdot_reclen);
}
void
byte_swap_minidir_in(struct direct *dirp)
{
	byte_swap_int(dirp->d_ino);
	byte_swap_short(dirp->d_reclen);
}
#endif /* notdef ] */
