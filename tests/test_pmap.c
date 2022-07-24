    assert(pmap_enter(&pm, 0x401000, pa, PTE_R | PTE_W | PTE_X) == -1);   /* W^X */
    assert(pmap_enter(&pm, 0x0000004000000000UL, pa, PTE_R) == -1);        /* bit 38 set but 63:39 clear */
    assert(pmap_enter(&pm, 0xffffffc000000000UL, pa, PTE_R) == 0);         /* canonical high half */
