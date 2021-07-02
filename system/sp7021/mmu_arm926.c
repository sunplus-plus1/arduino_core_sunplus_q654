//#include "sp70xx.h"
#include "sp7021_hal_conf.h"


/* Memory map description from: SP7021  Series memory map

                                                     Memory Type
0xffffffff |--------------------------|             ------------
           |       FLAG SYNC          |             Device Memory
0xfffff000 |--------------------------|             ------------
           |         Fault            |                Fault
0xfff00000 |--------------------------|             ------------
           |                          |                Normal
           |                          |
           |      Daughterboard       |
           |         memory           |
           |                          |
0x80505000 |--------------------------|             ------------
           |TTB (L2 Sync Flags   ) 4k |                Normal
0x80504C00 |--------------------------|             ------------
           |TTB (L2 Peripherals-B) 16k|                Normal
0x80504800 |--------------------------|             ------------
           |TTB (L2 Peripherals-A) 16k|                Normal
0x80504400 |--------------------------|             ------------
           |TTB (L2 Priv Periphs)  4k |                Normal
0x80504000 |--------------------------|             ------------
           |    TTB (L1 Descriptors)  |                Normal
0x80500000 |--------------------------|             ------------
           |          Stack           |                Normal
           |--------------------------|             ------------
           |          Heap            |                Normal
0x80400000 |--------------------------|             ------------
           |         ZI Data          |                Normal
0x80300000 |--------------------------|             ------------
           |         RW Data          |                Normal
0x80200000 |--------------------------|             ------------
           |         RO Data          |                Normal
           |--------------------------|             ------------
           |         RO Code          |              USH Normal
0x80000000 |--------------------------|             ------------
           |      Daughterboard       |                Fault
           |      HSB AXI buses       |
0x40000000 |--------------------------|             ------------
           |      Daughterboard       |                Fault
           |  test chips peripherals  |
0x2c002000 |--------------------------|             ------------
           |     Private Address      |            Device Memory
0x2c000000 |--------------------------|             ------------
           |      Daughterboard       |                Fault
           |  test chips peripherals  |
0x20000000 |--------------------------|             ------------
           |       Peripherals        |           Device Memory RW/RO
           |                          |              & Fault
0x00000000 |--------------------------|
*/
#include "mmu.inc"
#include "mem_sp7021.h"





									

static uint32_t Sect_Normal;	 
static uint32_t Sect_Normal_Cod; 
static uint32_t Sect_Device_RW; //outer & inner wb/wa,	ro, domain 0, base addr 0

extern void* Image$$TTB$$ZI$$Base;
extern void* __etext;
extern void* Image$$TTB$$ZI$$Limit;


void MMU_CreateTranslationTable(void)
{
	mmu_region_attributes_Type region;
	uint32_t domain_access_mode = 0xFFFFFFFF;
	int i  = 0; 
	uint32_t* pdata = NULL;

	//printf("%s, __etext = 0x%p, __etext = 0x%08x\n", __FUNCTION__, __etext, &__etext);
	//printf("%s, Image$$TTB$$ZI$$Base = 0x%p, Image$$TTB$$ZI$$Base = 0x%08x\n", __FUNCTION__, Image$$TTB$$ZI$$Base, &Image$$TTB$$ZI$$Base);
	//printf("%s, Image$$TTB$$ZI$$Limit = 0x%p, Image$$TTB$$ZI$$Limit = 0x%08x\n", __FUNCTION__, Image$$TTB$$ZI$$Limit, &Image$$TTB$$ZI$$Limit);
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, 0, 4096, DESCRIPTOR_FAULT);
	section_normal(Sect_Normal, region);
	section_device_rw(Sect_Device_RW, region);
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __ROM_BASE, __ROM_SIZE/0x100000, Sect_Normal); // multiple of 1MB sections
	MMU_TTSection ((uint32_t*)&Image$$TTB$$ZI$$Base, __DEVICE_BASE, __DEVICE_SIZE/0x100000, Sect_Device_RW);

#if 0
	pdata =(uint32_t*)&Image$$TTB$$ZI$$Base;
	for (i = 0; i < 4096;i++)
	{
		printf("0x%08x ", *(pdata+i));

		if ((i+1)%16 == 0)
			printf("\n");
	}
  #endif
	  __set_TTBR0((uint32_t*)&Image$$TTB$$ZI$$Base);
    __ISB();

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(0xFFFFFFFF);
    __ISB();
}


