/*
#
#  This file is provided under a dual BSD/GPLv2 license.  When using or
#  redistributing this file, you may do so under either license.
#
#  GPL LICENSE SUMMARY
#
#  Copyright(c) 2010-2012 Intel Corporation. All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of version 2 of the GNU General Public License as
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
#  The full GNU General Public License is included in this distribution
#  in the file called LICENSE.GPL.
#
#  Contact Information:
#  intel.com
#  Intel Corporation
#  2200 Mission College Blvd.
#  Santa Clara, CA  95052
#  USA
#  (408) 765-8080
#
#
#  BSD LICENSE
#
#  Copyright(c) 2010-2012 Intel Corporation. All rights reserved.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in
#      the documentation and/or other materials provided with the
#      distribution.
#    * Neither the name of Intel Corporation nor the names of its
#      contributors may be used to endorse or promote products derived
#      from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#*/

/*------------------------------------------------------------------------------
 * File Name: iosf_wrapper.c
 *------------------------------------------------------------------------------
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/err.h>


#include "iosf.h"
#include "iosf_core.h"

iosf_result_t iosf_open(uint32_t bus_id, iosf_handle *handle)
{
	struct iosf_host *host;
	iosf_internal *iosf;

	if (!handle) return IOSF_INVALID_PARAM;
	
	host = iosf_request(bus_id);
	if (NULL == host) {
		return IOSF_NO_DEV;
	}	
	
	iosf = kzalloc(sizeof(*iosf), GFP_KERNEL);
	if (NULL == iosf) {
		return IOSF_NO_MEM;
	}
	iosf->magic = IOSF_MAGIC;
	iosf->data = host;
	iosf->bus_id = bus_id;
	*handle = (iosf_handle)iosf;
	return IOSF_OK;
}
iosf_result_t iosf_close(iosf_handle handle)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}
	
	host = (struct iosf_host *)iosf->data;
	iosf_release(host);
	memset(iosf, 0, sizeof (*iosf));
	kfree(iosf);
	return ret;
}

iosf_result_t  iosf_read32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t *value)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	int tmp;	
	
	iosf = (iosf_internal *)handle;

	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}

	host = (struct iosf_host *)iosf->data;
	tmp = kiosf_reg_read32(host, dest_port, offset, value);
	if (tmp)
	{
		if (-EPERM == tmp)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}
	return ret;

}

iosf_result_t iosf_write32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t value)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	int tmp;
	
	iosf = (iosf_internal *)handle;

	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}

	host = (struct iosf_host *)iosf->data;
	tmp = kiosf_reg_write32(host, dest_port, offset, value);
	if (tmp)
	{
		if (-EPERM == tmp)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}

	return ret;

}

iosf_result_t iosf_modify(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t mask, uint32_t value)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	int tmp;
	
	iosf = (iosf_internal *)handle;

	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}

	host = (struct iosf_host *)iosf->data;
	tmp = kiosf_reg_modify(host, dest_port, offset, mask, value);
	if (tmp)
	{
		if (-EPERM == tmp)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}
	return ret;
}

iosf_result_t iosf_msg(iosf_handle handle, uint32_t dest_port, uint8_t opcode)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	int tmp;
	
	iosf = (iosf_internal *)handle;

	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}

	host = (struct iosf_host *)iosf->data;
	tmp = kiosf_msg(host, dest_port, opcode);
	if (tmp)
	{
		if (-EPERM == tmp)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}
	return ret;
}

iosf_result_t iosf_msg_data(iosf_handle handle, uint32_t dest_port, uint8_t opcode, uint32_t data)
{
	iosf_internal *iosf;
	struct iosf_host *host;
	iosf_result_t ret = IOSF_OK;
	int tmp;
	
	iosf = (iosf_internal *)handle;

	if (!iosf || (IOSF_MAGIC != iosf->magic) || !iosf->data) {
		return  IOSF_INVALID_PARAM;
	}

	host = (struct iosf_host *)iosf->data;
	tmp = kiosf_msg_data(host, dest_port, opcode, data);
	if (tmp)
	{
		if (-EPERM == tmp)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}
	return ret;
}
EXPORT_SYMBOL(iosf_open);
EXPORT_SYMBOL(iosf_close);
EXPORT_SYMBOL(iosf_write32);
EXPORT_SYMBOL(iosf_read32);
EXPORT_SYMBOL(iosf_modify);
EXPORT_SYMBOL(iosf_msg);
EXPORT_SYMBOL(iosf_msg_data);
