#include "utils.h"

// detect ARM64 platforms
#ifdef __aarch64__
/**
 * Initialize cycle counter and performance counters for ARM64 platforms
 */
void ccnt_init(void)
{
  // 0: Enable all counters in the PMNC control-register
  // 2: Reset CCNT to zero
  // 3: if 1, divide by 64
  unsigned int cr = 0;
  asm volatile("mrs %0, pmcr_el0\t\n" : "=r"(cr));
  cr |= (1U << 0);  // set bit 0 - enable ccnt
  cr |= (1U << 2);  // set bit 2 - clear ccnt
  cr &= ~(1U << 3); // clear bit 3
  asm volatile("msr pmcr_el0, %0\t\n" ::"r"(cr));
  // Enable cycle counter specifically
  // bit 31: enable cycle counter
  // bits 0-3: enable performance counters 0-3
  asm volatile("mrs %0, pmcntenset_el0\t\n" : "=r"(cr));
  cr |= 0x80000000;
  asm volatile("msr pmcntenset_el0, %0\t\n" ::"r"(cr));
}

unsigned int timestamp(void)
{
  unsigned int cc = 0;
  __asm__ volatile("mrs %0, pmccntr_el0\t\n" : "=r"(cc));
  return cc;
}
#else
/*
   The `timestamp()` function retrieves a 32-bit timestamp representing
   the current CPU cycle count using the Time Stamp Counter on
   x86-based architectures.
*/
unsigned int timestamp(void)
{
  unsigned int bottom, top;
  asm volatile("rdtsc"
               : "=a"(bottom), "=d"(top));
  return bottom;
}
#endif

/**
 * Generate and store random bytes in the provided buffer.
 *
 * @param data Pointer to the buffer for storing random bytes.
 * @param size Number of random bytes to generate and store.
 */
void random_bytes(uint8_t *data, size_t size)
{
  ssize_t i;
  static int fd = -1;

  ssize_t xlen = (ssize_t)size;
  assert(xlen >= 0);
  if (fd == -1)
  {
    for (;;)
    {
      fd = open("/dev/urandom", O_RDONLY);
      if (fd != -1)
        break;
      sleep(1);
    }
  }

  while (xlen > 0)
  {
    if (xlen < 1048576)
      i = xlen;
    else
      i = 1048576;

    i = read(fd, data, (size_t)i);
    if (i < 1)
    {
      sleep(1);
      continue;
    }

    data += i;
    xlen -= i;
  }
}

/**
 * This function monitors the progress of a benchmark by periodically updating
 * and displaying information about the number of completed iterations.
 *
 * @param arg A pointer to a Progress structure containing benchmark progress data.
 */
void *progress_function(void *arg)
{
  double elapsed_time;
  size_t iterations_completed;
  size_t iterations_total;
  const char *lib_name;
  uint8_t percent_completed;

  Progress *progress = arg;

  iterations_total = progress->iterations_total;
  lib_name = progress->lib_name;
  do
  {
    sleep(1);
    iterations_completed = progress->iterations_completed;
    elapsed_time = seconds() - progress->start_time;

    if ((uint64_t)elapsed_time % 10 == 0)
    {
      percent_completed = 100 * iterations_completed / iterations_total;
      printf("[%s] Iteration %zu/%zu (%hhu%%), elapsed time: %f\n", lib_name,
             iterations_completed, iterations_total, percent_completed, elapsed_time);
    }
  } while (iterations_completed < iterations_total - 1);
  return 0;
}

/**
 * Retrieve the current time in seconds.
 *
 * @return current time in seconds
 */
double seconds(void)
{
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) == -1)
  {
    printf("seconds(): clock_gettime() failed!\n");
    return 0.0;
  }

  return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}
