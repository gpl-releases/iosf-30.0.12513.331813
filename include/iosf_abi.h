/*
#
#  This file is provided under BSD license.  When using or
#  redistributing this file, you may do so under either license.
#
#
#
#  BSD LICENSE
#
#  Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
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
* File Name: iosf_abi.h
*------------------------------------------------------------------------------
*/
//! \file
#ifndef  __LINUX_IOSF_ABI_H
#define  __LINUX_IOSF_ABI_H 


#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define IOSF_MAGIC (('I' << 24) || ('O' << 16) | ('S' << 8) | ('F'))

typedef struct iosf_internal{
	uint32_t magic;
	void *data;
	uint32_t bus_id;
} iosf_internal;	

struct iosf_info_user
{
	uint32_t  dest_port;
	uint32_t  opcode;
	uint32_t  offset;
	uint32_t  mask;
	uint32_t  flag;
	uint32_t  value;

};




// IOSF PORT Definitions:
#define IOSF_PORT_VT              0x00
#define IOSF_PORT_MCU             0x01

#define IOSF_PORT_HUNIT           0x02
#define IOSF_PORT_BUNIT           0x03

#define IOSF_PORT_PUNIT           0x04
#define IOSF_PORT_CPUNIT          0x0A

#define IOSF_PORT_PCIE_AFE        0x11

#define IOSF_PORT_DFX_LAKEMORE    0x38
#define IOSF_PORT_DFX_OMAR        0x39
#define IOSF_PORT_DFX_TG          0x3A
#define IOSF_PORT_ITUNIT1         0x40
#define IOSF_PORT_SAPms_BRIDGE    0x41
#define IOSF_PORT_ITUNIT2         0x42

#define IOSF_PORT_DDR_IO          0x50
#define IOSF_PORT_REUT0           0x54
#define IOSF_PORT_REUT1           0x55
#define IOSF_PORT_SATA_AFE        0x59
#define IOSF_PORT_USB_AFE         0x60


#define IOSF_PORT_ADAC            0x81
#define IOSF_PORT_HDMI_TX_AFE     0x82
#define IOSF_PORT_HDMI_RX_AFE     0x83

#define IOSF_PORT_FPLL            0x88
#define IOSF_PORT_HPLL            0x89
#define IOSF_PORT_DPLL            0x8A
#define IOSF_PORT_APLL            0x8B

#define IOSF_PORT_MOCA_AFE        0x8E
#define IOSF_PORT_MOCA_MAC        0x8F


#define IOSF_PORT_PSF_0S          0x90  /* south */
#define IOSF_PORT_PSF_0N		  0x32  /*north */


#define IOSF_PORT_PSF1            0x91

#define IOSF_PORT_PSF3            0x93



#define  IOSF_8BITS_FLAG		1
#define  IOSF_16BITS_FLAG		(1 << 1)
#define  IOSF_32BITS_FLAG		(1 << 2)

#define IOSF_IOC_MAGIC  'I'

#define IOSF_IOC_RD		_IOW(IOSF_IOC_MAGIC, 1, struct iosf_info_user)
#define IOSF_IOC_WR		_IOW(IOSF_IOC_MAGIC, 2, struct iosf_info_user)
#define IOSF_IOC_MODIFY		_IOW(IOSF_IOC_MAGIC, 3, struct iosf_info_user)
#define IOSF_IOC_MSG		_IOW(IOSF_IOC_MAGIC, 4, struct iosf_info_user)
#define IOSF_IOC_MSG_DATA	_IOW(IOSF_IOC_MAGIC, 5, struct iosf_info_user)






#endif // __LINUX_IOSF_ABI_H

