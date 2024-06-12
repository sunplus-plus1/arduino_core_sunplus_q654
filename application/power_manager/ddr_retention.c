#include "ddr_retention.h"
// Address of ddr phy register to be save and restore
#ifdef DRAM_TYPE_LPDDR4
#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_retention_lpddr4_SP7350.txt>
#endif
#ifdef DRAM_TYPE_DDR4
#include <../../../../../boot/draminit/dwc/include/SP7350/DDR4/dwc_retention_ddr4_SP7350.txt>
#endif
#ifdef DRAM_TYPE_DDR3
#include <../../../../../boot/draminit/dwc/include/SP7350/DDR3/dwc_retention_ddr3_SP7350.txt>
#endif

void wait_loop(unsigned int wait_counter)
{
	unsigned int i;

	for (i = 0; i < wait_counter; i++)
	{
		__asm__("nop");
	}
}

void apb_ctl_before_retention(void)
{
	volatile uint16_t rd_data;

	wait_loop(10000);
	ctl_apb_wr(0x490,0x00000000);   //blocks AXI ports from taking anymore transactions

	while (1)
	{
		rd_data = ctl_apb_rd(0x3fc);
		if (rd_data == 0)
		{
			break;
		}
	}

	ctl_apb_wr(0x30,0x00000020);    //Causes system to move to self refresh state

	rd_data = ctl_apb_rd(0x4);
	while (((rd_data&0x30)!= 0x20) | ((rd_data&0x300) != 0x200))
	{
		rd_data = ctl_apb_rd(0x4);
	}

	ctl_apb_wr(0x320,0x00000000);   //SWCTL
	ctl_apb_wr(0x1b0,0x00001f70);   //Set dfi_init_start to 1

	rd_data = ctl_apb_rd(0x1bc);
	while((rd_data & 0x01)!= 0)
	{
		rd_data = ctl_apb_rd(0x1bc);    //Wait PHY de-assert dfi_init_complete
	}

	ctl_apb_wr(0x1b0,0x00001f50);   //Set dfi_init_start to 0

	rd_data = ctl_apb_rd(0x1bc);
	while((rd_data & 0x01)!= 1)
	{
		rd_data = ctl_apb_rd(0x1bc);  //Wait PHY assert dfi_init_complete
	}

	MOON0_REG->sft_cfg[14] = 0x00200000;  // 1->0: PwrOKIn MO_DDRPHY_PWROKIN ddrphy pwrokin
	wait_loop(10000);                        // When PWROKIN is 0, PHY input signals need be valid at least 10ns
	MOON0_REG->sft_cfg[14] = 0x00010000;  // 1->0: assert DDRCTL APB RST_N
	MOON0_REG->sft_cfg[14] = 0x00080000;  // 1->0: assert DDRPHY APB RST_N
	MOON0_REG->sft_cfg[14] = 0x00020000;  // 1->0: assert DDRCTL AXI RST_N
	MOON0_REG->sft_cfg[14] = 0x00040000;  // 1->0: assert DDRCTL AHB RST_N
	MOON0_REG->sft_cfg[14] = 0x00100000;  // 1->0: assert DDRDFI RST_N

	MOON0_REG->sft_cfg[2]  = 0x00040004;  //RF_MASK_V_SET(1 << 2);
	MOON0_REG->sft_cfg[2]  = 0x00080008;  //RF_MASK_V_SET(1 << 3);
	MOON0_REG->sft_cfg[2]  = 0x00100010;  //RF_MASK_V_SET(1 << 4);
	MOON0_REG->sft_cfg[2]  = 0x00200020;  //RF_MASK_V_SET(1 << 5);

}

int dram_test(void)
{
	uint8_t   *wr_src_8b  ;
	uint16_t  *wr_src_16b ;
	uint32_t  *wr_src_32b ;
	uint8_t   value_8b    ;
	uint16_t  value_16b   ;
	uint32_t  value_32b   ;
	uint32_t  *rd_src_32b ;
	uint8_t   *rd_src_8b  ;
	uint8_t   count, flip;

	unsigned char *des_adr = (unsigned char *)0x50000000;

	wr_src_16b   = (uint16_t *)(des_adr);
	value_16b    = 0;
	do {
		*wr_src_16b = value_16b;
		wr_src_16b  = wr_src_16b + 1;
		value_16b   = value_16b  + 1;
	} while ((ulong)wr_src_16b < (ulong)(des_adr + 0x1000));

	rd_src_32b   = (uint32_t *)(des_adr);
	value_16b    = 0;
	do {
		value_32b = ((value_16b + 1) << 16) | ((value_16b + 0) <<  0) ;
		if (value_32b != *rd_src_32b)
		{
			return -1;
		}
		rd_src_32b = rd_src_32b + 1;
		value_16b  = value_16b  + 2;
	} while ((ulong)rd_src_32b < (ulong)(des_adr + 0x1000));

	return 0;
}

static uint32_t sum32(uint32_t sum, uint8_t *data, int len)
{
        int val = 0, pos =0;


        for (; pos + 4 <= len; pos += 4) {
                sum += *(int *)(data + pos);
        }

        // word0: 3 2 1 0
        // word1: _ 6 5 4
        for (; len - pos; len--)
                val = (val << 8) | data[len - 1];

        sum += val;

        return sum;
}

void ddr_retention_save_param(void)
{
	// Size of ddr phy register is 16 bit.
	volatile uint16_t * const ddr_ret = (uint16_t * const) CM4_SRAM_RET_ADDRESS;  // SRAM (CM4) CPU view

	// Enabling Ucclk (PMU)
	dwc_ddrphy_apb_wr(0xd0000, 0x0);
	dwc_ddrphy_apb_wr(0xc0080, 0x3);

	// Save ddr phy registers to SRAM.
	for (int i=0; i < RET_CSRS_CNT; i++) {
		ddr_ret[i] = DWC_PHY_REG(ret_ddrphy_addr[i]);
	}

	// Disabling Ucclk (PMU)
	dwc_ddrphy_apb_wr(0xc0080, 0x2);
	dwc_ddrphy_apb_wr(0xd0000, 0x1);
}

static int save_flag = 0;
void ddr_retention_save(void)
{
	if(save_flag == 0)
	{
	    /* Make only one save, otherwise there may be a memory data error after DDR retention (readable but not writable)*/
		//ddr_retention_save_param();
		save_flag = 1;
	}
#ifdef DRAM_TYPE_LPDDR4
	printf("lpddr4_retention_save \n");
#endif
#ifdef DRAM_TYPE_DDR4
	printf("ddr4_retention_save \n");
#endif
#ifdef DRAM_TYPE_DDR3
	printf("ddr3_retention_save \n");
#endif
	apb_ctl_before_retention();
}


int dwc_umctl2_init_before_ctl_rst(void)
{

	ctl_apb_wr(0x304, 0x00000001);
	ctl_apb_wr(0x030, 0x00000001);
	ctl_apb_rd(0x004);
	#ifdef DRAM_TYPE_LPDDR4
	printf("dwc_umctl2_lpddr4_1600_SP7350_for_realchip\n");
	#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_1600_SP7350_for_realchip.txt>
	//printf("dwc_umctl2_lpddr4_1333_SP7350\n");
	//#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_1333_SP7350.txt>
	//printf("dwc_umctl2_lpddr4_1200_SP7350\n");
	//#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_1200_SP7350.txt>
	//printf("dwc_umctl2_lpddr4_1066_SP7350\n");
	//#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_1066_SP7350.txt>
	//printf("dwc_umctl2_lpddr4_933_SP7350\n");
	//#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_933_SP7350.txt>
	//printf("dwc_umctl2_lpddr4_800_SP7350\n");
	//#include <../../../../../boot/draminit/dwc/include/SP7350/LPDDR4/dwc_umctl2_lpddr4_800_SP7350.txt>
	#endif
	#ifdef DRAM_TYPE_DDR4
	printf("Q654_DDR4_ASIC_SDRAM_DATARATE_2666\n");
	#include <../../../../../boot/draminit/dwc/include/SP7350/DDR4/Q654_DDR4_ASIC_SDRAM_DATARATE_2666.txt>
	#endif
	#ifdef DRAM_TYPE_DDR3
	printf("Q654_DDR3_ASIC_SDRAM_DATARATE_1866\n");
	#include <../../../../../boot/draminit/dwc/include/SP7350/DDR3/Q654_DDR3_ASIC_SDRAM_DATARATE_1866.txt>
	#endif

	#ifdef NT6AN1024F32AV
	printf("NT6AN1024F32AV\n");
	#elif defined(MT53D1024M32D4)
	printf("MT53D1024M32D4, 2rank, FBGA=D9WHT\n");
	#elif defined(MT53E1G32D2_A)
	printf("MT53E1G32D2_A, 1rank, FBGA=D9ZQX\n");
	#elif defined(MT53E1G32D2_B)
	printf("MT53E1G32D2_B, 2rank, FBGA=D8CJG\n");
	#elif defined(MT53E2G32D4_C)
	printf("MT53E2G32D4_C, 2rank, FBGA=D8CJN\n");
	#elif defined(RS1G32LO4D2BDS)
	printf("RS1G32LO4D2BDS, 2rank\n");
	#elif defined(HG4XD04G)
	printf("HG4XD04G, 2rank\n");
	#elif defined(MT40A512M16TB)
	printf("MT40A512M16TB, 1rank, FBGA=D8BPK\n");
	#elif defined(MT41K512M16VRP)
	printf("MT41K512M16VRP, 1rank, FBGA=D9ZWN\n");
	#elif defined(K4B8G1646D)
	printf("K4B8G1646D, SAMSUNG 512Mx16, 2rank\n");
	#endif
	return 0;
}

#ifdef DRAM_TYPE_LPDDR4
int dwc_umctl2_init_after_ctl_rst(void)
{

	ctl_apb_wr(0x304, 0x00000000);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000122);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000120);
	//ctl_apb_rd(0x1c4, 0xcf000000);
	ctl_apb_rd(0x1c4);
	ctl_apb_wr(0x1c4, 0xc8000000);
	ctl_apb_wr(0x320, 0x00000000);
	ctl_apb_wr(0x1b0, 0x00000054);
	ctl_apb_wr(0x1b0, 0x00000054);
	ctl_apb_wr(0x304, 0x00000002);
	return 0;
}
#endif
#ifdef DRAM_TYPE_DDR4
int dwc_umctl2_init_after_ctl_rst(void)
{

	ctl_apb_wr(0x304, 0x00000000);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000122);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000120);
	//ctl_apb_rd(0x1c4, 0xcf000000);
	ctl_apb_rd(0x1c4);
	ctl_apb_wr(0x1c4, 0xc8000000);
	ctl_apb_wr(0x320, 0x00000000);
	ctl_apb_wr(0x1b0, 0x00000054);
	ctl_apb_wr(0x1b0, 0x00000054);
	ctl_apb_wr(0x304, 0x00000002);
	return 0;
}
#endif

#ifdef DRAM_TYPE_DDR3
int dwc_umctl2_init_after_ctl_rst(void)
{
	ctl_apb_wr(0x304, 0x00000000);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000020);
	//ctl_apb_rd(0x030, 0x00000020);
	ctl_apb_rd(0x030);
	ctl_apb_wr(0x030, 0x00000020);
	//ctl_apb_rd(0x1c4, 0xcf000000);
	ctl_apb_rd(0x1c4);
	ctl_apb_wr(0x1c4, 0xcf000000);
	ctl_apb_wr(0x320, 0x00000000);
	ctl_apb_wr(0x1b0, 0x00000040);
	ctl_apb_wr(0x1b0, 0x00000040);
	ctl_apb_wr(0x304, 0x00000002);
	return 0;
}
#endif

void apb_ctl_after_retention(void)
{
	volatile uint32_t rd_data;

#ifdef DRAM_TYPE_LPDDR4
	//PLLD_800MHz
	MOON3_REG->sft_cfg[11] = 0x00180008;  //PLLD PSTDIV
	MOON3_REG->sft_cfg[11] = 0x7f801000;  //PLLD FBKDIV 32
	//PLLD_666MHz
	//MOON3_REG->sft_cfg[11] = 0xffff0808;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[12] = 0xffffc0be;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
	//PLLD_600MHz
	//MOON3_REG->sft_cfg[11] = 0xffff0408;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[12] = 0xffffc0bd;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
	//PLLD_533MHz
	//MOON3_REG->sft_cfg[11] = 0xffff800A;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[12] = 0xffffc0bd;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
	//PLLD_466MHz
	//MOON3_REG->sft_cfg[11] = 0xffff9812;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[12] = 0xffffc0bf;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
	//PLLD_400MHz
	//MOON3_REG->sft_cfg[11] = 0xffff100a;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[12] = 0xffffc0bc;  //PLLD PSTDIV
	//MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
#endif
#ifdef DRAM_TYPE_DDR4
	MOON3_REG->sft_cfg[11] = 0xffff0808;  //PLLD PSTDIV
	MOON3_REG->sft_cfg[12] = 0xffffc0be;  //PLLD PSTDIV
	MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
#endif
#ifdef DRAM_TYPE_DDR3
	MOON3_REG->sft_cfg[11] = 0xffff9812;  //PLLD PSTDIV
	MOON3_REG->sft_cfg[12] = 0xffffc0bf;  //PLLD PSTDIV
	MOON3_REG->sft_cfg[13] = 0xffff0107;  //PLLD PSTDIV
#endif
	//while(((MOON3_REG->sft_cfg[13]) & (1<<8)) != (1<<8));  //wait pll lock

	MOON0_REG->sft_cfg[2]  = 0x00040000;  //RF_MASK_V_CLR(1 << 2);
	MOON0_REG->sft_cfg[2]  = 0x00080000;  //RF_MASK_V_CLR(1 << 3);
	MOON0_REG->sft_cfg[2]  = 0x00100000;  //RF_MASK_V_CLR(1 << 4);
	MOON0_REG->sft_cfg[2]  = 0x00200000;  //RF_MASK_V_CLR(1 << 5);
	wait_loop(1000);
	MOON0_REG->sft_cfg[14] = 0x00200020; // 0->1: PwrOKIn MO_DDRPHY_PWROKIN ddrphy pwrokin
	wait_loop(1000);
	wait_loop(1000);
	MOON0_REG->sft_cfg[14] = 0x00010001;  // 0->1: de-assert DDRCTL APB RST_N
	MOON0_REG->sft_cfg[14] = 0x00080008;  // 0->1: de-assert DDRPHY APB RST_N

	wait_loop(1000);

	// -------------------------------------------------------
	// 2.
	// -------------------------------------------------------
	// DRAM MRS SETTING

	dwc_umctl2_init_before_ctl_rst();

	ctl_apb_wr(0x0d0,0xc0030003);   //skips the DRAM init routine and starts up in self_refresh mode
	ctl_apb_wr(0x30,0x00000020);    //Keep system on self refresh state
	ctl_apb_wr(0x1b0,0x00000050);   //indicates to controller that PHY has completed re-training/initialization

	MOON0_REG->sft_cfg[14] = 0x00020002;  // 0->1: de-assert DDRCTL AXI RST_N
	MOON0_REG->sft_cfg[14] = 0x00040004;  // 0->1: de-assert DDRCTL AHB RST_N
	MOON0_REG->sft_cfg[14] = 0x00100010;  // 0->1: de-assert DDRDFI RST_N
	wait_loop(1000);
	wait_loop(1000);

	dwc_umctl2_init_after_ctl_rst();

	wait_loop(1000);

	// -------------------------------------------------------
	// 3.
	// -------------------------------------------------------

	volatile uint16_t * const ddr_ret = (uint16_t *) CM4_SRAM_RET_ADDRESS;	// SRAM (CM4) CPU view

	dwc_ddrphy_apb_wr(0xd0000,0x0); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel
	dwc_ddrphy_apb_wr(0xc0080,0x3); // DWC_DDRPHYA_DRTUB0_UcclkHclkEnables

	// Restore ddr phy registers from SRAM.
	for (int i=0; i < RET_CSRS_CNT; i++) {
		DWC_PHY_REG(ret_ddrphy_addr[i]) = ddr_ret[i];
	}

	dwc_ddrphy_apb_wr(0xc0080,0x2); // DWC_DDRPHYA_DRTUB0_UcclkHclkEnables
	dwc_ddrphy_apb_wr(0xd0000,0x1); // DWC_DDRPHYA_APBONLY0_MicroContMuxSel

	ctl_apb_wr(0x1b0,0x00000070);   //trigger dfi_init_start

	while(1)
	{
		rd_data = ctl_apb_rd(0x1bc);
		if(rd_data == 1)
		{
		 break;
		}
	}

	ctl_apb_wr(0x1b0,0x00000050);
	#ifdef DRAM_TYPE_LPDDR4
	ctl_apb_wr(0x1b0,0x00000055); //trigger SDRAM initilaztion.
	#else
	ctl_apb_wr(0x1b0,0x00000051); //trigger SDRAM initilaztion.
	#endif
	ctl_apb_wr(0x030,0x00000000); //PWRCTL
	ctl_apb_wr(0x1c4,0x00000001);   //enable the PHY master interface.
	ctl_apb_wr(0x304,0x00000000);   //enable dq and hif.
	rd_data = ctl_apb_rd(0x4);
	while(((rd_data & 0x30)!=0x00) || ((rd_data&0x07) != 0x01))
	{
		rd_data = ctl_apb_rd(0x4);      //Wait exit self_refresh and entry normal mode
	}

	ctl_apb_wr(0x490,0x00000001);   //no longer blocks AXI ports from taking anymore transactions
}

void ddr_retention_load(void)
{
	//while(((MOON3_REG->sft_cfg[13]) & (1<<8)) != (1<<8));  //wait pll lock
	wait_loop(1000);

	apb_ctl_after_retention();

	printf("ddr_retention_load \n");
}
