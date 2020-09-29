#ifndef FRAG_H
#define FRAG_H

#define MAX_FILE_NAME_LENGTH 200 
#define MAXBLOCKS (100 * 1024 * 256) // 100GB
#define MAX_NUM_OF_FILES 1024

struct donor_info {
    int fd;
    char file_name[30];
};

extern struct donor_info donors[10000]; 
extern int lkl_exit_started;

// Globals for hiding disk I/O calls
extern long long batch_interval;
extern uint64_t read_write_per_batch;
extern int sgxlkl_issue_fake_syscalls;
extern long fake_reads_count;
extern long fake_writes_count;
extern long real_reads_count;
extern long real_writes_count;
extern long batch_count;

// Globals for shuffling disk blocks
extern uint64_t shuffle_interval;
extern int execute_shuffle;
extern int sgxlkl_shuffle_all_files;
extern char *file_names[MAX_NUM_OF_FILES];
extern char *shuffle_file_names[MAX_NUM_OF_FILES];
extern char *fd_used_since_last_shuffle[MAX_NUM_OF_FILES];
extern int *open_files_bitmap;
extern int num_fd_to_close_after_shuffle;
extern int is_shuffle_needed;

extern int dummy_file_for_fake_reads;  
extern char * dummy_file_name; 
extern long long * fileblk_to_fsblk_map; 

void lkl_setup_dummy_file_blocks();

int perform_shuffle_if_needed();
void perform_frag_on();
int do_frag();


void block_bitmap_init(); 
void block_bitmap_set(long long offset, long count); 
int offset_seen_since_last_shuffle(long long offset, long count); 

#endif /* FRAG_H */
