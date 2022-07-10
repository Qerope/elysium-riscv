    assert(pmap_enter(&pm, 0x0000004000000000UL, pa, PTE_R) == -1);        /* bit 38 set but 63:39 clear */
    assert(pmap_enter(&pm, 0xffffffc000000000UL, pa, PTE_R) == 0);         /* canonical high half */
