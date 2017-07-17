#include <mm/mm.h>
void memory_init(void)
{
    pmm_init();
    vmm_init();
    slob_init();
}
