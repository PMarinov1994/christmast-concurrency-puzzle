#ifndef __GLOBAL_H
#define __GLOBAL_H


#define U_TO_MS(us) (1000 * us)

#define MAX_DEERS 9
#define MAX_ELFS_TO_HANDLE 3

#define ELF_THREAD_COUNT 10
#define DEER_THREAD_COUNT MAX_DEERS

// Higher value, less change of getting home
#define DEER_COMMING_HOME_CHANCE 4000

// Higher value, less change of requesting help
#define ELF_NEEDS_HELP_CHANCE 5000

#endif // __GLOBAL_H
