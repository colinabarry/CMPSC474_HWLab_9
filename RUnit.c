#include "Queue.h"
#include "Util.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>


int main()
{
    const int SHM_SIZE = sizeof(Qnode) * 50;
    const char *SHM_NAME = "calculated_data";
    int shm_id;
    int offset = 0;
    void *ptr;

    shm_id = shm_open(SHM_NAME, O_RDONLY, 0666);
    // ftruncate(shm_id, SHM_SIZE);

    ptr = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_id, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap RUnit");
        exit(EXIT_FAILURE);
    }

    int data = ((Qnode *)ptr)->data;
    offset += sizeof(Qnode);
    while (offset <= SHM_SIZE - sizeof(Qnode)) {
        printf("Data: %d\n", data);
        ptr += offset;
        data = ((Qnode *)ptr)->data;
        offset += sizeof(Qnode);
    }


    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}