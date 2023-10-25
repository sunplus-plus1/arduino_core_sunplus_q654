/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/cm4/sys.c
 * @brief	machine specific system primitives implementation.
 */


#include "metal/io.h"
#include "metal/sys.h"
#include <metal/irq_controller.h>
#include <metal/log.h>
#include <irq_ctrl.h>
#include "sp7350_cm4.h"

#define IRQ_GIC_LINE_COUNT 200

typedef struct {
	void *arg;
	metal_irq_handler handler;
} irq_metal_TypeDef;

static irq_metal_TypeDef IRQMetalTable[IRQ_GIC_LINE_COUNT] = { 0U };

static void metal_sp_irq_set_enable(struct metal_irq_controller *irq_cntr,
				      int irq, unsigned int state)
{
	if (irq < irq_cntr->irq_base ||
	    irq >= irq_cntr->irq_base + irq_cntr->irq_num) {
		metal_log(METAL_LOG_ERROR, "%s: invalid irq %d\n",
			  __func__, irq);
		return;
	} else if (state == METAL_IRQ_ENABLE) {
		IRQ_Enable(irq);
	} else {
		IRQ_Disable(irq);
	}
}

void METAL_IRQHandler(void)
{
	int irq = IRQ_GetCurrentIRQNum();

	if (IRQMetalTable[irq].handler != NULL )
	{
		IRQMetalTable[irq].handler(irq, IRQMetalTable[irq].arg);
	}
	else
	{
		printf("irq=%d\n", irq);
	}
}

static int metal_sp_irq_register(struct metal_irq_controller *irq_cntr,
					int irq, metal_irq_handler hd, void *arg)
{
	//printf(">>>>>> %s:irq=%d @ %p\n", __FUNCTION__, irq, hd);
	IRQMetalTable[irq].handler = hd;
	IRQMetalTable[irq].arg = arg;

	IRQ_SetHandler(irq, METAL_IRQHandler);

	return 0;
}

static METAL_IRQ_CONTROLLER_DECLARE(sp_irq_cntr,
				    0, 224,
				    NULL,
				    metal_sp_irq_set_enable,
				    metal_sp_irq_register,
				    NULL)

int metal_sp_irq_init(void)
{
	int ret;

	ret =  metal_irq_register_controller(&sp_irq_cntr);
	if (ret < 0) {
		metal_log(METAL_LOG_ERROR, "%s: register irq controller failed.\n",
			  __func__);
		return ret;
	}
	return 0;
}

void sys_irq_restore_enable(unsigned int flags)
{
	__enable_irq();
	/* FIXME: Assembler messages: writing to APSR
	 * without specifying a bitmask is deprecated.
	 */
	__set_xPSR(flags);
}

unsigned int sys_irq_save_disable(void)
{
	__disable_irq();
	return __get_xPSR();
}

void metal_machine_cache_flush(void *addr, unsigned int len)
{
	/* CM4 have no cache */
}

void metal_machine_cache_invalidate(void *addr, unsigned int len)
{
	/* CM4 have no cache */
}

/**
 * @brief poll function until some event happens
 */
void __attribute__((weak)) metal_generic_default_poll(void)
{
}

void *metal_machine_io_mem_map(void *va, metal_phys_addr_t pa,
			       size_t size, unsigned int flags)
{
	(void)size;
	(void)flags;

	return (va = (void *)pa);
}
