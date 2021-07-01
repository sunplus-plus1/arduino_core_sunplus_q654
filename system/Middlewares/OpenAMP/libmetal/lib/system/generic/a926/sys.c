/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/a926/sys.c
 * @brief	machine specific system primitives implementation.
 */


#include "metal/io.h"
#include "metal/sys.h"

void sys_irq_restore_enable(unsigned int flags)
{
	(void)flags;
}

unsigned int sys_irq_save_disable(void)
{
  return 0;
}

void metal_machine_cache_flush(void *addr, unsigned int len)
{
	(void)addr;
	(void)len;
}

void metal_machine_cache_invalidate(void *addr, unsigned int len)
{
	(void)addr;
	(void)len;
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
	(void)va;
	(void)pa;
	(void)size;
	(void)flags;

	return va;
}
