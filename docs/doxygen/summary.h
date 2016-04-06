/*

 This file is provided under a dual BSD/GPLv2 license.  When using or
 redistributing this file, you may do so under either license.

 GPL LICENSE SUMMARY

 Copyright(c) 2011-2012 Intel Corporation. All rights reserved.

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License as
 published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 The full GNU General Public License is included in this distribution
 in the file called LICENSE.GPL.

 Contact Information:

 Intel Corporation
 2200 Mission College Blvd.
 Santa Clara, CA  97052

 BSD LICENSE

 Copyright(c) 2011-2012 Intel Corporation. All rights reserved.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
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

//-----------------------------------------------------------------------------

/**
@weakgroup iosf_summary IOSF Summary

<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">1 Introduction</h2>	
<p>
This document is applicable to the Intel(R) Atom(TM) processor CE5300 and the Intel(R) Puma(TM) 6 SoC. 
<BR/>
The document describes the IOSF (Intel On-chip System Fabric) driver-programming interface for the Intel(R) CE Media Processors and explains how to access other components in the SOC (e.g. DPLL, APLL). It also describes the Application Programming Interface (API) and illustrates the use of the API with some samples. APIs include user mode API, and kernel mode API which is used by other drivers.
<BR/>
<hr>
\image html fig_2.png "Figure 2. IOSF Module Layout"
</p>

\anchor acronyms_and_terminology	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">1.1	Acronyms and Terminology</h2>	

<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">Term</td><td>Definition</td></tr>
<tr><td>API</td><td>Application Programming Interface</td></tr>
<tr><td>IOSF</td><td>Intel On-chip System Fabric</td></tr>
<tr><td>NB</td><td>Northbridge</td></tr>
</tr> 
</table>
\anchor index_1_2
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">1.2	IOSF module </h2>	
<p>
As you can see in Figure 1, the IOSF driver is used to access other components in the SOC through the CPU controlling CUnit. The IOSF user API is built on top of the driver to provide access to components of the SOC.  
</p>
<hr>
\image html fig_1.png "Figure 1. IOSF Arch Diagram"
<BR/>
In the following chapters, SPI Master is referred to by the acronym "SPI" unless otherwise noted. 

<BR/>
As illustrated in Figure 2, driver code is located in the directory driver, library code is located in the directory lib. The common including files which are used by the driver and lib are placed in the include directory. Unit test is placed in the unit_test directory. 

\anchor iosf_package_contents
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">2	IOSF Package Contents</h2>	
The following tables list the IOSF package contents:
\anchor table_2_1
@section Table_2_1  Table 2-1 Drivers
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr  style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" >
<td width="50%"><b>IOSF kernel driver</b></td><td><b>iosf.ko</b></td></tr>
<tr><td>Common head file</td><td>iosf_abi.h</td></tr>
</tr> 
</table>
<br/>
\anchor table_2_2
@section Table_2_2 Table 2-2 User Libs
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr  style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">IOSF User Libs</td><td>names</td></tr>
<tr><td>IOSF user lib</td><td>libiosf.a</td></tr>
</tr> 
</table>
	<br/>

\anchor table_2_3
@section Table_2_3 Table 2-3 User Head Files
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%">IOSF User Head Files</td><td>names</td></tr>
<tr><td>IOSF user head file</td><td>iosf.h</td></tr>
</tr> 
</table>

\anchor api_overview	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">3. API Overview</h2>	
In this section, the basic data types, data structures, and IOSF API taxonomy are described. The IOSF API list is also included.
\anchor index_3_1	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">3.1	Scalar Data Types</h2>	
The traditional (unsigned) char, (unsigned) short, (unsigned) int are defined by uint*_t types which are classified by bit width. For example, uint_32 is used to represent unsigned int. The following table describes all data types defined by the IOSF API. 
\anchor table_3_1
@section Table_3_1 Table 3-1. IOSF Basic Data Types
<table width="70%" style="border: 1px solid #555" border="1" cellspacing="0" cellpadding="3">
<tr style="color:#FFFFFF; background-color:#0860A8;font-weight:bold;" ><td width="50%"><b>C/C++ data type</b></td><td><b>IDL Data Types</b></td></tr>
<tr><td>unsigned char</td><td>Uint8_t</td></tr>
<tr><td>unsigned short</td><td>Uint16_t</td></tr>
<tr><td>unsigned int</td><td>Uint32_t</td></tr>
</tr> 
</table>
\anchor index_3_2
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">3.2	Compound Data Types</h2>	
Variables of these compound data types are used as inputs to and/or outputs from IOSF APIs. In this section, all compound types are described.
(1)	Port definition.
The macro is used to indicate the port index. When you use the general interface, you need one parameter as port.
Defined in include/iosf_abi.h
\code
// PORT Definitions:
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
#define IOSF_PORT_PSF_0S          0x90
#define IOSF_PORT_PSF_0N	      0x32
#define IOSF_PORT_PSF1            0x91
#define IOSF_PORT_PSF3            0x93
#define IOSF_PORT_MOCA_AFE        0x8E
\endcode
IOSF module return data type.
Defined in include/iosf.h
\code
typedef enum {
	IOSF_OK	              = 0 , // 0x0  
	IOSF_INVALID_PARAM        , /// 0x1 
	IOSF_NOT_INITIALIZED      , // 0x2 
	IOSF_ALREADY_INITIALIZED  , // 0x3 
	IOSF_NO_DEV       	    , // 0x5
	IOSF_NOT_IMPLEMENTED 	    , // 0x6 
	IOSF_DEVICE_BUSY  	    ,// 0x7 
	IOSF_NO_MEM 		    , // 0x8 
	IOSF_NO_PERM		    , // 0x9 
	IOSF_TIMED_OUT   	    , // 0xa 
	IOSF_FIRST_ERROR = IOSF_INVALID_PARAM, 
	IOSF_LAST_ERROR  = IOSF_TIMED_OUT
} iosf_result_t; 
\endcode
It is used to describe the result type of IOSF module.

(2)	IOSF host handle.
Defined in include/iosf.h
\code
typedef  void  *iosf_handle;
\endcode
It is a handle for user to control the IOSF host. 

\anchor iosf_api_functions	
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">4	IOSF API Functions</h2>
In this section, API functions are described.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">4.1	IOSF API</h2>
The IOSF API contains two parts, one is used in user mode and applications can use it. The other is used in kernel mode and other drivers can use it. The identical APIs are used to support the two modes. It is enough for the user to include the header file "iosf.h". The security issues will be addressed in the implementation to avoid security holes. One of the key security checks is that if a process wants to perform an action, it should own root privilege or be in the appointed group. (Group ID is given when a module is being insmoded.)
Internally, one table is used to map port to opcode (0x6, 0x7, or 0x10, 0x11), so opcode is not required in the parameter list.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">4.1.1	IOSF Functions</h2>
\ref iosf_ "Functions Link"

\anchor usage_model
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">5	IOSF API Usage Model</h2>
IOSF API usage mode is the same in the application layer and kernel driver layer.
5.1.1.1	IOSF API Usage Model
After the IOSF driver is inserted into kernel, the user application should perform the following to operate the IOSF host on the Intel(R) CE Media Processor:
- Open the IOSF host.
- Operate on IOSF host.
- Close the IOSF host.

<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.1.1	Open IOSF host</h2>
Call iosf_open to get the IOSF handle. A code example follows:
\code
	uint32_t bus_id = 0;
	iosf_result_t ret;
	iosf_handle iosf;
	ret = iosf_open(bus_id, &iosf); // get IOSF handle
	....
	....
\endcode
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.1.2	Operate On IOSF host </h2>
Use iosf_read32/ iosf_write32/iosf_modify to perform configuration register read/write operation. 
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:13pt">5.1.1.1.3	Close IOSF host</h2>
When the user doesn't want to use the IOSF host, the IOSF host needs closed. A code example follows:
\code
iosf_handle iosf;
uint32_t bus_id = 0;
iosf = iosf_open(bus_id, &iosf);
....
....
iosf_close(iosf); // close the IOSF host. After closing, users should not use the handle iosf.
\endcode
 
\anchor pseudo_code
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:18pt">6	Pseudo Code for IOSF</h2>
The following pseudo code will give some basic application examples of IOSF. If development is in kernel mode, symbols are imported from the IOSF drivers. Otherwise, symbols are imported from the IOSF library.
<h2 style="color:#FFFFFF; background-color:#0860A8;font-weight:bold; font-size:15pt">6.1	IOSF User Application Pseudo Code</h2>
The following pseudo code is used to read configuration registers of component DPLL, offset 0x0c.
\code
#include "iosf.h"
uint32_t bus_id = 0;
iosf_handle iosf;
uint8_t dest_port = IOSF_PORT_DPLL;
uint32_t offset = 0x0c;
uint32_t value;
iosf_result_t ret;
ret = iosf_open(bus_id, &iosf);
....
ret = iosf_read32(iosf, dest_port, offset, &value);
....
....
....
iosf_close(iosf);
\endcode

*/
