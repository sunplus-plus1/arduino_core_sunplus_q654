#include "sp7021_hal_icm.h"

void HAL_ICM_IRQHandler(ICM_InitTypeDef *pICM_Init)
{
	uint32_t cnt;
	uint32_t fstate;
	uint32_t pwh;
	uint32_t pwl;

	/* patch:a solution of hareware BUG */
	ICM_Patch(pICM_Init);
	ICM_GetFifoState(pICM_Init, &fstate);
	while( !(fstate & ICM_FEMPTY) )
	{
		ICM_ReadIntrCnt(pICM_Init, &cnt);
		ICM_GetSigPulseWidth(pICM_Init, &pwh, &pwl);
		printf("pwh=%d,pwl=%d,cnt=%d\n", pwh, pwl, cnt);
	}
}

#if 0
void HAL_ICM_Init(ICM_InitTypeDef *pICM_Init)
{
	ICM_InputSigSrc(pICM_Init);
	ICM_ClkSel(pICM_Init);
	ICM_SetDetectMode(pICM_Init);
	ICM_TrigTimesEdgeEvent(pICM_Init);
	ICM_SetDebounceTimes(pICM_Init);
	ICM_Enable(pICM_Init);

	printf("(%d)SP_ICM_REG->cfg0:%p:0x%x\n", __LINE__, &(pICM_Init->instance->cfg0), pICM_Init->instance->cfg0);
	printf("(%d)SP_ICM_REG->cfg1:%p:0x%x\n", __LINE__, &(pICM_Init->instance->cfg1), pICM_Init->instance->cfg1);
	printf("(%d)pinmux_icm_data:%p:0x%x\n", __LINE__, &(SP_PIN_MUX_CTRL2->pinmux_icm_data[pICM_Init->index/2]),SP_PIN_MUX_CTRL2->pinmux_icm_data[pICM_Init->index/2]);
}
#else
void HAL_ICM_Init(ICM_InitTypeDef *pICM_Init)
{
	HAL_ICM_SetConfig(pICM_Init);
	ICM_Enable(pICM_Init);
}
#endif
void HAL_ICM_GetConfig(ICM_InitTypeDef *pICM_Init)
{
	pICM_Init->muxsel = ICM_GETCFG(0, MUXSEL);
	pICM_Init->clksel = ICM_GETCFG(0, CLKSEL);
	pICM_Init->eemode = ICM_GETCFG(1, EEMODE);
	pICM_Init->etimes = ICM_GETCFG(1, ETIMES);
	pICM_Init->dtimes = ICM_GETCFG(1, DTIMES);
	pICM_Init->cntscl = pICM_Init->instance->cntscl;
	pICM_Init->tstscl = pICM_Init->instance->tstscl;
}

void HAL_ICM_SetConfig(ICM_InitTypeDef *pICM_Init)
{

	ICM_SETCFG(0, MUXSEL, pICM_Init->muxsel);
	ICM_SETCFG(0, CLKSEL, pICM_Init->clksel);
	ICM_SETCFG(1, EEMODE, pICM_Init->eemode);
	ICM_SETCFG(1, ETIMES, pICM_Init->etimes);
	ICM_SETCFG(1, DTIMES, pICM_Init->dtimes);
	pICM_Init->instance->cntscl = pICM_Init->cntscl;
	pICM_Init->instance->tstscl = pICM_Init->tstscl;
}

void HAL_ICM_PINMUX(ICM_InitTypeDef *pICM, int data_pinmux, int clk_pinmux)
{
	ICM_DATA_SetPinMux(pICM, data_pinmux);
	if ( (pICM->clksel < 4)&&(clk_pinmux != -1) )
	{
		ICM_CLK_SetPinMux(pICM, clk_pinmux);
	}
}

/* 
  SP7021 have the function of Pin Multiplex. Here set pin for an external interrupt to use
  by configuring the Pinmux control register.
*/
void ICM_DATA_SetPinMux(ICM_InitTypeDef *pICM, int data_pinmux)
{   
	#if 0
	if((pICM->index + 1) % 2)
    {
        /* A valid write to the register, the corresponding mask bits must set 1 at the same time*/
        SP_PIN_MUX_CTRL2->pinmux_icm_data[pICM->index/2] = RF_MASK_V(0x7f, data_pinmux);
    }
    else
    {   
        SP_PIN_MUX_CTRL2->pinmux_icm_data[pICM->index/2] = RF_MASK_V((0x7f << 8), (data_pinmux << 8));
    }
    #endif
    HAL_PINMUX_Cfg(pICM->index + PINMUX_ICM0_D, pICM->Pin_data);
}

void ICM_CLK_SetPinMux(ICM_InitTypeDef *pICM, int clk_pinmux)
{   
	#if 0
	if((pICM->index + 1) % 2)
    {   
        SP_PIN_MUX_CTRL2->pinmux_icm_clk[pICM->index/2] = RF_MASK_V(0x7f, clk_pinmux);
    }
    else
    {   
        SP_PIN_MUX_CTRL2->pinmux_icm_clk[pICM->index/2] = RF_MASK_V((0x7f << 8), (clk_pinmux << 8));
    }
    #endif
    HAL_PINMUX_Cfg(pICM->index + PINMUX_ICM0_CLK, pICM->Pin_clk);
}
