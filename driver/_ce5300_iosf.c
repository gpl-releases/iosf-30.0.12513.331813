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
 * File Name:_ce5300_iosf.c 
 * Driver for  IOSF(Intel On chip System Fabric)
 *------------------------------------------------------------------------------
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/io.h>

#include "iosf_core.h"
#include "iosf_abi.h"

#include "_ce5300_iosf.h"
#include "_iosf.h"

int  _ce5300_iosf_port_is_valid(struct iosf_host * host, uint8_t dest_port)
{
	int ret = 0;
	
	switch (dest_port)
	{
		case IOSF_PORT_VT:
		case IOSF_PORT_MCU:
		case IOSF_PORT_PUNIT:	
		case IOSF_PORT_CPUNIT:
		case IOSF_PORT_ITUNIT1:	
		case IOSF_PORT_SAPms_BRIDGE:
		case IOSF_PORT_ITUNIT2:				
		case IOSF_PORT_USB_AFE:	
		case IOSF_PORT_ADAC:
		case IOSF_PORT_HDMI_TX_AFE:
		case IOSF_PORT_HDMI_RX_AFE:
		case IOSF_PORT_FPLL:
		case IOSF_PORT_HPLL:
		case IOSF_PORT_DPLL:
		case IOSF_PORT_APLL:
		case IOSF_PORT_PSF_0S:
		case IOSF_PORT_PSF_0N:	
		case IOSF_PORT_PSF1:
		case IOSF_PORT_PSF3:
			
		case IOSF_PORT_HUNIT:
		case IOSF_PORT_BUNIT:
		case IOSF_PORT_DDR_IO:
		case IOSF_PORT_REUT0:
		case IOSF_PORT_REUT1:

		case IOSF_PORT_DFX_LAKEMORE:
		case IOSF_PORT_DFX_OMAR:
		case IOSF_PORT_SATA_AFE:
		case IOSF_PORT_PCIE_AFE:
			break;
			
		default:
				ret = -EINVAL;
	}

	return !ret;
}

int _ce5300_iosf_msg_opcode_is_valid(struct iosf_host * host, uint8_t opcode)
{
	int ret = -EINVAL;
	
	if ( ((opcode >= 0x80) && (opcode <= 0x9F)) || 
		 ((opcode >= 0xA0) && (opcode <= 0xFF) ) )
		ret = 0;
	return ret;
}

int _ce5300_iosf_msg_data_opcode_is_valid(struct iosf_host * host, uint8_t opcode)
{
	int ret = -EINVAL;
	
	if ( ((opcode >= 0x40) && (opcode <= 0x5F)) || 
		 ((opcode >= 0x60) && (opcode <= 0x7F) ) )
		ret = 0;
	return ret;
}	

int _ce5300_iosf_reg_read32(struct iosf_host * host, uint8_t dest_port, uint32_t offset, uint32_t *value)
{
	uint8_t rd_opcode = 0;
	int ret = 0;


	switch (dest_port)
	{
		case IOSF_PORT_VT:
		case IOSF_PORT_MCU:
		case IOSF_PORT_PUNIT:	
		case IOSF_PORT_CPUNIT:
		case IOSF_PORT_ITUNIT1:	
		case IOSF_PORT_SAPms_BRIDGE:
		case IOSF_PORT_ITUNIT2:				
		case IOSF_PORT_USB_AFE:	
		case IOSF_PORT_ADAC:
		case IOSF_PORT_HDMI_TX_AFE:
		case IOSF_PORT_HDMI_RX_AFE:
		case IOSF_PORT_FPLL:
		case IOSF_PORT_HPLL:
		case IOSF_PORT_DPLL:
		case IOSF_PORT_APLL:
		case IOSF_PORT_PSF_0S:
		case IOSF_PORT_PSF_0N:	
		case IOSF_PORT_PSF1:
		case IOSF_PORT_PSF3:
			rd_opcode = IOSF_OPCODE_CRRd;
			break;
			
		case IOSF_PORT_HUNIT:
		case IOSF_PORT_BUNIT:
		case IOSF_PORT_DDR_IO:
		case IOSF_PORT_REUT0:
		case IOSF_PORT_REUT1:
			rd_opcode = IOSF_OPCODE_RegRd;
			break;

		case IOSF_PORT_DFX_LAKEMORE:
		case IOSF_PORT_DFX_OMAR:
		case IOSF_PORT_SATA_AFE:
		case IOSF_PORT_PCIE_AFE:
			rd_opcode = IOSF_OPCODE_0_RegRd;
			break;
			
		default:
				ret = -EINVAL;


	}
	if (ret)
		return ret;

	return _common_iosf_reg_read32(host, dest_port, rd_opcode, offset, value);

}	

int _ce5300_iosf_reg_write32(struct iosf_host * host, uint8_t dest_port, uint32_t offset, uint32_t value)
{
	uint8_t wr_opcode = 0;
	int ret = 0;

	switch (dest_port)
	{
		case IOSF_PORT_VT:
		case IOSF_PORT_MCU:
		case IOSF_PORT_PUNIT:	
		case IOSF_PORT_CPUNIT:
		case IOSF_PORT_ITUNIT1:	
		case IOSF_PORT_SAPms_BRIDGE:
		case IOSF_PORT_ITUNIT2:				
		case IOSF_PORT_USB_AFE:	
		case IOSF_PORT_ADAC:
		case IOSF_PORT_HDMI_TX_AFE:
		case IOSF_PORT_HDMI_RX_AFE:
		case IOSF_PORT_FPLL:
		case IOSF_PORT_HPLL:
		case IOSF_PORT_DPLL:
		case IOSF_PORT_APLL:
		case IOSF_PORT_PSF_0S:
		case IOSF_PORT_PSF_0N:	
		case IOSF_PORT_PSF1:
		case IOSF_PORT_PSF3:
			wr_opcode = IOSF_OPCODE_CRWr;
			break;
			
		case IOSF_PORT_HUNIT:
		case IOSF_PORT_BUNIT:
		case IOSF_PORT_DDR_IO:
		case IOSF_PORT_REUT0:
		case IOSF_PORT_REUT1:
			wr_opcode = IOSF_OPCODE_RegWr;
			break;

		case IOSF_PORT_DFX_LAKEMORE:
		case IOSF_PORT_DFX_OMAR:
		case IOSF_PORT_SATA_AFE:
		case IOSF_PORT_PCIE_AFE:
			wr_opcode = IOSF_OPCODE_1_RegWr;
			break;
			
		default:
				ret = -EINVAL;

	}

	if (ret)
		return ret;
	
	return _common_iosf_reg_write32(host, dest_port,wr_opcode,  offset, value);

}

int _ce5300_iosf_reg_modify(struct iosf_host * host, uint8_t dest_port, uint32_t offset, uint32_t mask,  uint32_t value)
{
	uint8_t rd_opcode = 0;
	uint8_t wr_opcode = 0;
	int ret = 0;

	switch (dest_port)
	{
		case IOSF_PORT_VT:
		case IOSF_PORT_MCU:
		case IOSF_PORT_PUNIT:	
		case IOSF_PORT_CPUNIT:
		case IOSF_PORT_ITUNIT1:	
		case IOSF_PORT_SAPms_BRIDGE:
		case IOSF_PORT_ITUNIT2:				
		case IOSF_PORT_USB_AFE:	
		case IOSF_PORT_ADAC:
		case IOSF_PORT_HDMI_TX_AFE:
		case IOSF_PORT_HDMI_RX_AFE:
		case IOSF_PORT_FPLL:
		case IOSF_PORT_HPLL:
		case IOSF_PORT_DPLL:
		case IOSF_PORT_APLL:
		case IOSF_PORT_PSF_0S:
		case IOSF_PORT_PSF_0N:	
		case IOSF_PORT_PSF1:
		case IOSF_PORT_PSF3:
			rd_opcode = IOSF_OPCODE_CRRd;
			wr_opcode = IOSF_OPCODE_CRWr;
			break;
			
		case IOSF_PORT_HUNIT:
		case IOSF_PORT_BUNIT:
		case IOSF_PORT_DDR_IO:
		case IOSF_PORT_REUT0:
		case IOSF_PORT_REUT1:
			rd_opcode = IOSF_OPCODE_RegRd;
			wr_opcode = IOSF_OPCODE_RegWr;
			break;

		case IOSF_PORT_DFX_LAKEMORE:
		case IOSF_PORT_DFX_OMAR:
		case IOSF_PORT_SATA_AFE:
		case IOSF_PORT_PCIE_AFE:
			rd_opcode = IOSF_OPCODE_0_RegRd;
			wr_opcode = IOSF_OPCODE_1_RegWr;
			break;
			
		default:
				ret = -EINVAL;
	}

	if (ret)
		return ret;
	return _common_iosf_reg_modify(host, dest_port, rd_opcode, wr_opcode, offset, mask, value);

}
