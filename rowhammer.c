#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 DRAM Disturbance Error userspace tester
 https://www.ece.cmu.edu/~safari/pubs/kim-isca14.pdf
*/

unsigned int mycrc32(char *buffer, unsigned length)
{
        unsigned i;
        unsigned int crc32 = ~0L;
        for (i = 0; i < length; i++)
            crc32 += (unsigned)((unsigned char) buffer[i]);
        return crc32;
}

int main(int argc, char *argv[])
{
    char *bigseg;
    unsigned long ROW_SIZE = 1 << 13;    
    unsigned long ADDR_OFFSET = 1 << 23; 
    char *addr_up;
    char *addr_down;
    size_t segsize = (1024 * 1204) * 64;
    int rnd;

    bigseg = NULL;
    char *start;
    char *end;
    int tryit;
    unsigned int crc_orig =0;// mycrc32(start, segsize);
    unsigned int crc= 0;
    int cnt = 0;
    int iter=0;

    while(1)
    {
//      if (bigseg) free(bigseg);
        bigseg =  malloc(segsize);
        rnd=open("/dev/urandom", O_RDONLY);
        read(rnd, bigseg, segsize/5);
        close(rnd);
        start = bigseg;
        end   = bigseg + segsize;
        crc_orig = mycrc32(start, segsize);
        printf("#%i ", iter);

        for (addr_up = start; ; addr_up += (ROW_SIZE*3))
        {
            addr_down = addr_up + ADDR_OFFSET;
            if (addr_down >= end)
                break;
            for (tryit = 0; tryit < 4000000; tryit++)
            {
                asm volatile("movl (%0), %%eax" : : "r" (addr_up) : "eax");
                asm volatile("movl (%0), %%ebx" : : "r" (addr_down) : "ebx");
                asm volatile("clflush (%0)" : : "r" (addr_up) : "memory");
                asm volatile("clflush (%0)" : : "r" (addr_down) : "memory");
                asm volatile("mfence");
            }
            printf(".");
            cnt++;
            fflush(0);
            if ((cnt%40) == 0)
            {
                if (crc_orig != mycrc32(start, segsize))
                {
                    printf(" DRAM CRC FAIL! ;-)\n");
                    exit(-1);
                } else
                {
                    iter++;
                    printf(" OK\n");
                    break;
                }
            }
            if (iter > 32)
            {
                printf("DRAM seems works fine...\n");
                exit(0);
            }
        }
    }
}
