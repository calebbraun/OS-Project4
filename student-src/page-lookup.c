#include "swapfile.h"
#include "statistics.h"
#include "pagetable.h"

/*******************************************************************************
 * Looks up an address in the current page table. If the entry for the given
 * page is not valid, traps to the OS.
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t pagetable_lookup(vpn_t vpn, int write) {
  pfn_t pfn;

  if (current_pagetable[vpn].valid == 0) {
    count_pagefaults++;
    pagefault_handler(vpn, write);
    current_pagetable[vpn].valid = 1;
  }

  pfn = current_pagetable[vpn].pfn;

  /* FIX ME - Part 2
   * Determine the PFN corresponding to the passed in VPN.
   * current_pagetable variable accesses the process page table.
   * if the pagetable entry is not valid, increment count_pagefaults, call
   * pagefault_handler (traps to OS, returns frame number), and change pagetable
   * entry to now be valid
   */

  return pfn;
}
