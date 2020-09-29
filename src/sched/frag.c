#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <linux/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "atomic.h"
#include "oblivious/frag.h"
#include "host/sgxlkl_util.h"
#include <sys/stat.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <enclave/enclave_util.h>
#include "syscall.h"


#ifndef EXT4_IOC_GETFSBLK_ID
#define EXT4_IOC_GETFSBLK_ID 		_IOR('f', 33,long) 
#endif

#ifndef EXT4_IOC_MOVE_EXT
struct move_extent {
	__s32 reserved;	/* original file descriptor */
	__u32 donor_fd;	/* donor file descriptor */
	__u64 orig_start;	/* logical start offset in block for orig */
	__u64 donor_start;	/* logical start offset in block for donor */
	__u64 len;	/* block length to be moved */
	__u64 moved_len;	/* moved block length */
};

#define EXT4_IOC_MOVE_EXT      _IOWR('f', 15, struct move_extent)
#endif
#define BLKSIZE 4096 
#define MAX_DONORS 300

struct donor_info donors[10000]; 
int init_donor(int);
int num_donors; 
int num_open_files = 0;

int count_digits(int num) {
	return num < 10 ? 1 : (1 + count_digits(num / 10));
}

int alloc_one_donor(int i, int fd,  int num_blocks) {
	int ret;
	ret = __filter_syscall4(SYS_fallocate, fd, 0, 0, num_blocks * BLKSIZE);
	if(ret != 0) {
		sgxlkl_error("Unable to allocate space for donor file %s - Err:: \n", donors[i].file_name, strerror(ret));
	}
	return 0; 
}

int init_one_donor(int i, int num_blocks) {
	int count = (11 + (2 * count_digits(i)));
	snprintf(donors[i].file_name, count, "donor_%d.out", i);
	int fd = __filter_syscall4(SYS_openat, AT_FDCWD, donors[i].file_name, O_CREAT | O_RDWR, 0);

    int ret = alloc_one_donor(i,fd,num_blocks);
    if (fd <= 0 || ret != 0) {
        sgxlkl_error("Unable to alloc space donor file %s: Err=%d, %s\n", donors[i].file_name, errno, strerror(errno));
    } else {
        num_open_files++;
		donors[i].fd = fd;
    }
    return 1;
}

void init_donors(int num_max_file_blocks) {
	int count_donors = 0;
	for (int i = 0; i < num_donors; i++) {
		if (init_one_donor(i, num_max_file_blocks) > 0)
			count_donors++;
		else
			break;
	}
	num_donors = count_donors;
	return;
}

int cleanup_donors() {
	int ret, i;
	for(i = 0; i < num_donors; i++) {
		/* Close the donor files. */
        ret = __filter_syscall1(SYS_close, donors[i].fd);
		if(ret < 0) {
			sgxlkl_error("%s: Error unable to close fd=%d, err=%s\n", "cleanup_donors", donors[i].fd, strerror(errno));
		}

        /* Unlink the donor files. */
		ret = __filter_syscall2(SYS_unlinkat, AT_FDCWD, donors[i].file_name);
		if(ret < 0) {
			sgxlkl_error("%s: Error unable to unlink file=%s, fd=%d, donor_idx=%d, err=%s\n", "cleanup_donors", donors[i].file_name, donors[i].fd, i, strerror(errno));
		}
		num_open_files--;
	}
	return 0;
}

int pick_donor() {
	return rand() % num_donors;
}

// TODO: Implement this function
/* This is the main shuffle function. All the file blocks that need to be shuffled are ordered. The algorithm
 * first calls FisherYates, to randomize the order in which the blocks will be shuffled. Then, for each block,
 * pick a donor file at random, and swap the current block with the corresponding donor block. */
int shuffle_the_blocks() {
    return 0;
}

// TODO: Implement this function
/* This is the function which handles the whole shuffle process. */
int do_frag() {
    return 0;
}
