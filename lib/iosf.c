/*
This file is provided under a dual BSD/LGPLv2.1 license.  When using 
or redistributing this file, you may do so under either license.

LGPL LICENSE SUMMARY

Copyright(c) <2010>. Intel Corporation. All rights reserved.

This program is free software; you can redistribute it and/or modify 
it under the terms of version 2.1 of the GNU Lesser General Public 
License as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public 
License along with this library; if not, write to the Free Software 
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 
USA. The full GNU Lesser General Public License is included in this 
distribution in the file called LICENSE.LGPL.

Contact Information:
	Intel Corporation
	2200 Mission College Blvd.
	Santa Clara, CA  97052

BSD LICENSE

Copyright (c) <2010>. Intel Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

  - Redistributions of source code must retain the above copyright 
	notice, this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright 
	notice, this list of conditions and the following disclaimer in 
	the documentation and/or other materials provided with the 
	distribution.
  - Neither the name of Intel Corporation nor the names of its 
	contributors may be used to endorse or promote products derived 
	from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*------------------------------------------------------------------------------
 * File Name: iosf.c
 *------------------------------------------------------------------------------
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "iosf.h"



iosf_result_t iosf_open(uint32_t bus_id, iosf_handle *handle)
{
	iosf_internal *iosf;
	char devname[24];
	int fd;
	iosf_result_t ret = IOSF_OK;

	if (!handle) return IOSF_INVALID_PARAM;

	iosf = malloc(sizeof(*iosf));
	if (NULL == iosf) {
		return IOSF_NO_MEM;
	}

	sprintf(devname, "/dev/iosf%d", bus_id);
	fd = open(devname,  O_RDWR | O_SYNC);
	if (fd < 0) {
	    free(iosf);
		return IOSF_NO_DEV;
	}

	memset(iosf, 0, sizeof(*iosf));
	iosf->magic = IOSF_MAGIC;
	iosf->data = (void *)fd;
	iosf->bus_id = bus_id;
	*handle = (iosf_handle)iosf;
	return IOSF_OK;



}

iosf_result_t iosf_close(iosf_handle handle)
{
    iosf_internal *iosf;
	int fd;
	iosf_result_t ret = IOSF_OK;
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}

	fd = (int)iosf->data;
	close(fd);
	memset(iosf, 0, sizeof(*iosf));
	free(iosf);
	return ret;



}


iosf_result_t iosf_read32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t *value)
{
    iosf_internal *iosf;
	struct iosf_info_user iosf_info;
	int fd;
	iosf_result_t ret = IOSF_OK;
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}
	
	iosf_info.dest_port = dest_port;
	iosf_info.offset = offset;
	iosf_info.flag = IOSF_32BITS_FLAG;
	fd = (int)iosf->data;
	if (ioctl(fd, IOSF_IOC_RD, (u_long)&iosf_info))
	{
		if (EPERM == errno)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}
	else 
		*value = iosf_info.value;
	return ret;


}

iosf_result_t iosf_write32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t value)
{
	iosf_internal *iosf;
	struct iosf_info_user iosf_info;
	int fd;
	iosf_result_t ret = IOSF_OK;	
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}
	
	iosf_info.dest_port = dest_port;
	iosf_info.offset = offset;
	iosf_info.value = value;
	iosf_info.flag = IOSF_32BITS_FLAG;
	fd = (int)iosf->data;	
	if (ioctl(fd, IOSF_IOC_WR, (u_long)&iosf_info))
	{
		if (EPERM == errno)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}

	return ret;
}


iosf_result_t iosf_modify(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t mask, uint32_t value)
{
	iosf_internal *iosf;
	struct iosf_info_user iosf_info;
	int fd;
	iosf_result_t ret = IOSF_OK;	
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}
	
	iosf_info.dest_port = dest_port;
	iosf_info.offset = offset;
	iosf_info.mask = mask;
	iosf_info.value = value;
	iosf_info.flag = IOSF_32BITS_FLAG;
	fd = (int)iosf->data;	
	if (ioctl(fd, IOSF_IOC_MODIFY, (u_long)&iosf_info))
	{
		if (EPERM == errno)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}

	return ret;
}
iosf_result_t iosf_msg(iosf_handle handle, uint32_t dest_port, uint8_t opcode)
{
	iosf_internal *iosf;
	struct iosf_info_user iosf_info;
	int fd;
	iosf_result_t ret = IOSF_OK;	
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}
	iosf_info.dest_port = dest_port;
	iosf_info.opcode = opcode;
	fd = (int)iosf->data;	
	if (ioctl(fd, IOSF_IOC_MSG, (u_long)&iosf_info))
	{
		if (EPERM == errno)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}

	return ret;
}

iosf_result_t iosf_msg_data(iosf_handle handle, uint32_t dest_port, uint8_t opcode, uint32_t data)
{
	iosf_internal *iosf;
	struct iosf_info_user iosf_info;
	int fd;
	iosf_result_t ret = IOSF_OK;	
	
	iosf = (iosf_internal *)handle;
	
	if (!iosf || (IOSF_MAGIC != iosf->magic) || (((int)iosf->data) < 0)) {
		return IOSF_INVALID_PARAM;
	}

	iosf_info.dest_port = dest_port;
	iosf_info.opcode = opcode;
	iosf_info.value = data;
	fd = (int)iosf->data;	
	if (ioctl(fd, IOSF_IOC_MSG_DATA, (u_long)&iosf_info))
	{
		if (EPERM == errno)
			ret = IOSF_NO_PERM;
		else
			ret = IOSF_INVALID_PARAM;
	}

	return ret;
}
#ifdef __cplusplus
}
#endif
