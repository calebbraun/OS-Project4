#include <stdlib.h>
#include <stdio.h>
#include "tlb.h"
#include "pagetable.h"
#include "global.h" /* for tlb_size */
#include "statistics.h"

int start_pointer = 0;

/*******************************************************************************
 * Looks up an address in the TLB. If no entry is found, calls
 * pagetable_lookup() to get the entry from the page table instead
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t tlb_lookup(vpn_t vpn, int write) {
  pfn_t pfn;

  /*
   * FIX ME : Step 5
   * Note that tlb is an array with memory already allocated and initialized to
   * 0/null meaning that you don't need special cases for a not-full tlb, the
   * valid field will be 0 for both invalid and empty tlb entries, so you can
   * just check that!
   */

  /*
   * Search the TLB - hit if find valid entry with given VPN
   * Increment count_tlbhits on hit.
   */
  int tlbEntryIndex = -1;

  for (int i = 0; i < tlb_size; i++) {
    if (tlb[i].vpn == vpn && tlb[i].valid != 0) {
      count_tlbhits++;
      pfn = tlb[i].pfn;
      tlbEntryIndex = i;
      break;
    }
  }

  /*
   * If it was a miss, call the page table lookup to get the pfn
   * Add current page as TLB entry. Replace any invalid entry first,
   * then do a clock-sweep to find a victim (entry to be replaced).
   */

  if (tlbEntryIndex == -1) {
    pfn = pagetable_lookup(vpn, write);
    for (int i = 0; i < tlb_size; i++) {
      if (tlb[i].valid == 0) {
        tlb[i].vpn = vpn;
        tlb[i].pfn = pfn;
        tlb[i].valid = 1;
        tlbEntryIndex = i;
      }
    }
  }

  /* If the pagetable didn't have any invalid entries */
  if (tlbEntryIndex == -1) {
    pfn = pagetable_lookup(vpn, write);
    /* Clock sweep until we find unused entry */
    while (tlb[start_pointer].used != 0) {
      tlb[start_pointer].used = 0;
      start_pointer = (start_pointer + 1) % tlb_size;
    }
    tlb[start_pointer].vpn = vpn;
    tlb[start_pointer].pfn = pfn;
    tlbEntryIndex = start_pointer;
  }

  /*
   * In all cases perform TLB house keeping. This means marking the found
   * TLB entry as used and if we had a write, dirty. We also need to update the
   * page table entry in memory with the same data.
   */

  tlb[tlbEntryIndex].used = 1;
  current_pagetable[vpn].used = 1;
  current_pagetable[vpn].valid = 1;
  if (write == 1) {
    tlb[tlbEntryIndex].dirty = 1;
    current_pagetable[vpn].dirty = 1;
  }

  return pfn;
}
