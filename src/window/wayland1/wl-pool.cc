
#include "wl-pool.hh"


#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>


#include "lib/print.hh"



/* Shared memory support code */
void WL_Pool::
randname(char *buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A'+(r&15)+(r&16)*2;
        r >>= 5;
    }
}

int WL_Pool::
create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        // Print::ln(name);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

int WL_Pool::
allocate_shm_file(int size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}


void WL_Pool::init(int _size)
{
    if(data != nullptr)
    {
        Print::ln("WARN: tried to initialize an already allocated WL_Pool in 'WL_Pool::init';");
        return;
    }

    size = _size;

    shm_fd = allocate_shm_file(size);
    if (shm_fd == -1)
    {
        Print::ln("Failed to allocate shm file in 'WL_Pool::init'");
        return;
    }

    data = (uint32_t*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED)
    {
        Print::ln("ERROR: mmap failed in WL_Pool::init.");
        close(shm_fd);
        return;
    }

    pool = wl_shm_create_pool(wl_shm, shm_fd, size);


    // Note: compositor allocates its own memory pool based on the fd. AFter the compoistor-allocation the currently mmaped file is no longer of any use?
    close(shm_fd);
}
