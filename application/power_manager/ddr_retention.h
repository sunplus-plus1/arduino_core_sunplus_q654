#ifndef __PM_DDR_RET_H__
#define __PM_DDR_RET_H__

#include "pm_common.h"

#define UMCTL2_REG_BASE              ((volatile struct umctl2_regs *)UMCTL2_REG_Base)
#define DWC_PHY_REG_BASE             ((volatile struct dwc_phy_regs *)DWC_PHY_REG_Base)

#define UMCTL2_REG(OFFSET)		     (UMCTL2_REG_BASE->umctl2_reg[OFFSET >> 2])
#define DWC_PHY_REG(OFFSET)		     (DWC_PHY_REG_BASE->dwc_phy_reg[OFFSET ])

#define RET_CSRS_CNT                 1054

struct dwc_phy_regs {
	uint16_t dwc_phy_reg[0xA0000];	/* change the size here, (area >> 2) */
};

struct umctl2_regs {
	uint32_t umctl2_reg[1024];	/* change the size here, (area >> 2) */
};

inline void ctl_apb_wr(uint32_t adr, uint32_t dat)
{
	UMCTL2_REG(adr) = dat;
	//printf("UMCTL2_REG adr 0x%03x dat 0x%08x\n", adr, dat);
}

inline uint32_t ctl_apb_rd(uint32_t adr)
{
	return UMCTL2_REG(adr);
}

inline void dwc_ddrphy_apb_wr(uint32_t adr, uint32_t dat)
{
	DWC_PHY_REG(adr)=dat;
}

inline uint16_t dwc_ddrphy_apb_rd(uint32_t adr)
{
	return DWC_PHY_REG(adr);
}


#endif

