#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdatomic.h>
#include <threads.h>

atomic_int atomic_counter;
float max_delay; // max delay between packets for emulation (ms)

// time interval passed by value
int timer_tick(void* arg) {
    struct timespec ts = {(int)arg, 0};
    while(1) {
        nanosleep(&ts, NULL);
        atomic_store(&atomic_counter, 0);
        printf("%s: %ld seconds passed\n", __func__, ts.tv_sec);        
    }
    return 0;
}

static inline void skip(void) {
    puts("false");
}

static inline void pass(void) {
    puts("true");
}

// simulating incoming packet.
// Actually just wait for random delay in interval [0,..,max_delay] milliseconds
static inline void wait_packet(void) {
    // toy random interval generation
    struct timespec ts = {0, (long)(max_delay * rand())};
    nanosleep(&ts, NULL);
}

/* Usage: ratelimit <rate limit> <interval in seconds> <max delay between packets, ms, float> */
int main(int ac, char **av) {
    int LIMIT;
    long interval;
    thrd_t timer_thread;
        
// Sorry, no check for argument correctness, acceptable for test
    
    sscanf(av[3], "%f", &max_delay);
    sscanf(av[2], "%ld", &interval);
    sscanf(av[1], "%d", &LIMIT);

    // normalizing delay onto [0...max_delay] milliseconds
    max_delay *= 1.e+6 / (float)RAND_MAX; 
    srand(time(0));
    atomic_init(&atomic_counter, 0);
    
    wait_packet();
    atomic_fetch_add(&atomic_counter, 1);
    
    thrd_create(&timer_thread, timer_tick, (void*)interval);
    thrd_detach(timer_thread);
    
    while(1) {
        if(atomic_load(&atomic_counter) <= LIMIT)
            pass();
        else
            skip();
        wait_packet();
        atomic_fetch_add(&atomic_counter, 1);
    }

    return 0;
}







