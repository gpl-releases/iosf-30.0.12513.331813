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
 * File Name: iosf.h
 *------------------------------------------------------------------------------
 */

	
/*------------------------------------------------------------------------------
* File Name: iosf.h
*------------------------------------------------------------------------------
*/

#ifndef __LINUX_IOSF_H
#define __LINUX_IOSF_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "iosf_abi.h"

/** @defgroup iosf_ iosf api */
/** @ingroup iosf_ */
/*@{*/


typedef void * iosf_handle;



typedef enum {
	IOSF_OK	              = 0x0, /**< 0x0 */ 
	IOSF_INVALID_PARAM        , /**< 0x1 */
	IOSF_NOT_INITIALIZED      , /**< 0x2 */
	IOSF_ALREADY_INITIALIZED  , /**< 0x3 */
	IOSF_NO_DEV       		 , /**< 0x4 */
	IOSF_NOT_IMPLEMENTED 	 , /**< 0x5 */
	IOSF_DEVICE_BUSY  		 , /**< 0x6 */
	IOSF_NO_MEM 			 , /**< 0x7 */
	IOSF_NO_PERM			 , /**< 0x8 */	
	IOSF_TIMED_OUT   		 , /**< 0x7 */
	IOSF_FIRST_ERROR          = IOSF_INVALID_PARAM, 
	IOSF_LAST_ERROR           = IOSF_TIMED_OUT
} iosf_result_t;

/**
* This function is used to open IOSF host.
* @param[in] bus_id - IOSF bus ID. (CE5300 supports: 0)
* @param[out] handle - pointer to store the returned IOSF handle. 
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_DEV invalid parameter[error occurs].
* @retval IOSF_NO_MEM fail to allocate memory.[error occurs].
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*
*/
iosf_result_t iosf_open(uint32_t bus_id, iosf_handle *handle);

/**
* This function is used to close IOSF host.
* @param[in] handle - IOSF handle.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*
*/
iosf_result_t iosf_close(iosf_handle handle);

/**
* This function is used to read configure register of dest port.
* @param[in] handle - IOSF handle.
* @param[in] dest_port - destination component port.
* @param[in] offset - register offset. (32 bit addressing).
* @param[in] value - pointer to store the returned value.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_PERM  no privilege to perform the action[error occurs]
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*/
iosf_result_t iosf_read32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t *value);

/**
* This function is used to write configure register of dest port.
* @param[in] handle - IOSF handle.
* @param[in] dest_port - destination component port.
* @param[in] offset - register offset. (32 bit addressing).
* @param[in] value - value to write.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_PERM  no privilege to perform the action[error occurs]
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*/
iosf_result_t iosf_write32(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t value);

/**
* This function is used to modify configure register of dest port.
* @param[in] handle - IOSF handle.
* @param[in] dest_port - destination component port.
* @param[in] offset - register offset. (32 bit addressing).
* @param[in] mask - 32 bits mask, bit of mask that is 1¡¯b will be written, bit of mask that is 0¡¯b willn¡¯t change.
* @param[in] value - value to write.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_PERM no privilege to perform the action[error occurs]
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*/
iosf_result_t iosf_modify(iosf_handle handle, uint32_t dest_port, uint32_t offset, uint32_t mask, uint32_t value);

/**
* This function is used to send a message to dest port.
* @param[in] handle - IOSF handle.
* @param[in] dest_port - destination component port.
* @param[in] opcode - IOSF message opcode.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_PERM  no privilege to perform the action[error occurs]
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*/

iosf_result_t iosf_msg(iosf_handle handle, uint32_t dest_port, uint8_t opcode);
/**
* This function is used to send a message with data to dest port.
* @param[in] handle - IOSF handle.
* @param[in] dest_port - destination component port.
* @param[in] opcode - IOSF message opcode.
* @param[in] data - IOSF message data.
* 
* @retval IOSF_OK [Successfully].
* @retval IOSF_NO_PERM  no privilege to perform the action[error occurs]
* @retval IOSF_INVALID_PARAM invalid parameter[error occurs].
*/
iosf_result_t iosf_msg_data(iosf_handle handle, uint32_t dest_port, uint8_t opcode, uint32_t data);

/*@}*/
static inline int is_iosf_err(iosf_result_t ret)
{
	if ((ret >= IOSF_FIRST_ERROR) && (ret <= IOSF_LAST_ERROR))
		return 1;
	else
		return 0;
}	

#ifdef __cplusplus
}
#endif

#endif
