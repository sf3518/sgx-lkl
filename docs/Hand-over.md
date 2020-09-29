Hand-over Document
===========================
`samples/ml/tensorflow/app/randomize_tensorflow.sh` and `run_tensorflow.sh`:

provides the randomization scripts for Tensorflow initial disk randomization

`samples/ml/tensorflow/app/shuffle_test_file.c`:

provides the main function of shuffling a list of files

`src/include/enclave/lthread.h`:

added int ongoing_shuffle and int is_shuffle_thread in lthread struct

`src/include/host/virtio_blkdev.h`:

added extern variables as the buffer to store virtio requests that will be executed in batches

`src/include/oblivious/frag.h`:

added globals and function declarations for hiding disk I/O calls and shuffling disk blocks

`src/lkl/setup.c`:
`lkl_setup_dummy_file_blocks():` set up dummy file blocks for dummy calls

`src/sched/lthread.c`: 
Defined `struct virtio_req_queue_slot ` as the data structure for one slot in the buffer

`block_bitmap_init(), block_bitmap_set() and offset_seen_since_last_shuffle():`
functions to initialize and set the block bitmap, and record the offsets since the last shuffle to make them oblivious

Added globals for hiding disk I/O calls and shuffling disk blocks, and the buffer for storing virtio requests

`get_and_fill_virtio_requests()`:  fetch the virtio requests and put them into the buffer queue

`get_next_fake_DR_block()`: 
get the mapping from file block to filesystem block, which avoids the need of an ioctl call each time we need a dummy block

`issue_virtio_reqs():` issue virtio requests in batches

`create_current_batch():`creates the current batch of virtio requests

`acquire_shuffle_lock():`acquire the shuffle lock

`release_shuffle_lock():`release the shuffle lock

`check_and_issue_virtio_reqs():`
wait for lkl startup to complete and issue virtio request batches if time elapsed exceeds batch intereval

`perform_shuffle():`perform the shuffle of files

`perform_shuffle_if_needed():`
perform the shuffle if the current thread is the shuffle thread and does not have an ongoing shuffle

`src/sched/frag.c:`
`count_digits():`count the number of digits

`alloc_one_donor():`allocate one donor file

`init_one_donor():`initialize a donor file

`init_donors():`initialize multiple donor files

`cleanup_donors():`close and unlink the donor files

`pick_donor():`pick a random donor file

TODO:
`shuffle_the_blocks():`the main function which implements shuffle

`pick_donor():`the function which handles the whole shuffle process by calling `shuffle_the_blocks():`
