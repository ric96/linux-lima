/*
 * Copyright (C) 2018 Lima Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <linux/io.h>
#include <linux/device.h>

#include "lima_device.h"
#include "lima_dlbu.h"
#include "lima_vm.h"

#define LIMA_DLBU_MASTER_TLLIST_PHYS_ADDR  0x0000
#define	LIMA_DLBU_MASTER_TLLIST_VADDR      0x0004
#define	LIMA_DLBU_TLLIST_VBASEADDR         0x0008
#define	LIMA_DLBU_FB_DIM                   0x000C
#define	LIMA_DLBU_TLLIST_CONF              0x0010
#define	LIMA_DLBU_START_TILE_POS           0x0014
#define	LIMA_DLBU_PP_ENABLE_MASK           0x0018

#define dlbu_write(reg, data) writel(data, ip->iomem + LIMA_DLBU_##reg)
#define dlbu_read(reg) readl(ip->iomem + LIMA_DLBU_##reg)

void lima_dlbu_enable(struct lima_device *dev)
{
	struct lima_sched_pipe *pipe = dev->pipe + lima_pipe_pp;
	struct lima_ip *ip = dev->ip + lima_ip_dlbu;
	int i, mask = 0;

	for (i = 0; i < pipe->num_processor; i++) {
		struct lima_ip *pp = pipe->processor[i];
		mask |= 1 << (pp->id - lima_ip_pp0);
	}

	dlbu_write(PP_ENABLE_MASK, mask);
}

void lima_dlbu_disable(struct lima_device *dev)
{
	struct lima_ip *ip = dev->ip + lima_ip_dlbu;
	dlbu_write(PP_ENABLE_MASK, 0);
}

void lima_dlbu_set_reg(struct lima_ip *ip, u32 *reg)
{
	dlbu_write(TLLIST_VBASEADDR, reg[0]);
	dlbu_write(FB_DIM, reg[1]);
	dlbu_write(TLLIST_CONF, reg[2]);
	dlbu_write(START_TILE_POS, reg[3]);
}

int lima_dlbu_init(struct lima_ip *ip)
{
	struct lima_device *dev = ip->dev;

	dlbu_write(MASTER_TLLIST_PHYS_ADDR, dev->dlbu_dma | 1);
	dlbu_write(MASTER_TLLIST_VADDR, LIMA_VA_RESERVE_DLBU);

	return 0;
}

void lima_dlbu_fini(struct lima_ip *ip)
{
	
}
