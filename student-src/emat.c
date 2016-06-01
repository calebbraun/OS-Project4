#include "statistics.h"

#define MEMORY_ACCESS_TIME 100    /* 100 ns */
#define DISK_ACCESS_TIME 10000000 /* 10 million ns = 10 ms */

/*	Available variables from statistics that you may find useful
 *    count_pagefaults   - the number of page faults that occurred (includes
 * 		required)
 *    count_tlbhits      - the number of tlbhits that occurred
 *    count_writes       - the number of stores/writes that occurred
 *    count_reads        - the number of reads that occurred
 * 	  disk_accesses		 - the number of disk accesses NOT including
 * 		required faults
 * Any other values you might need are composites of the above values.
 */

double compute_emat_all() {
  /* FIX ME - Compute the average memory access time, including required page
   * faults
   * that occur when loading a new process.
   */

  int emat = 0;
  emat += MEMORY_ACCESS_TIME * 2 * (count_writes + count_reads);
  emat -= MEMORY_ACCESS_TIME * count_tlbhits;
  emat += MEMORY_ACCESS_TIME * 3 * count_pagefaults;
  emat += DISK_ACCESS_TIME * count_pagefaults;

  return emat;
}

double compute_emat_unforced() {
  /* FIX ME - Compute the average memory access time NOT including required
   * faults
   */

  int emat = 0;
  emat += MEMORY_ACCESS_TIME * 2 * (count_writes + count_reads);
  emat -= MEMORY_ACCESS_TIME * count_tlbhits;
  emat += MEMORY_ACCESS_TIME * 3 * count_diskaccesses;
  emat += DISK_ACCESS_TIME * count_diskaccesses;

  return emat;
}
