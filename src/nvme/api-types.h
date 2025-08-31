// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * Types used as part of the libnvme/libnvme-mi API, rather than specified
 * by the NVM Express specification.
 *
 * These are shared across both libnvme and libnvme-mi interfaces.
 *
 * This file is part of libnvme.
 * Copyright (c) 2022 Code Construct
 *
 * Authors: Jeremy Kerr <jk@codeconstruct.com.au>
 */
#ifndef _LIBNVME_API_TYPES_H
#define _LIBNVME_API_TYPES_H

#include <stdio.h>
#include <stdbool.h>

#include <nvme/types.h>

struct nvme_root;
typedef struct nvme_root *nvme_root_t;
struct nvme_link;
typedef struct nvme_link *nvme_link_t;

/**
 * nvme_create_root() - Initialize root object
 * @fp:		File descriptor for logging messages
 * @log_level:	Logging level to use
 *
 * Return: Initialized &nvme_root_t object
 */
nvme_root_t nvme_create_root(FILE *fp, int log_level);

/**
 * nvme_free_root() - Free root object
 * @r:	&nvme_root_t object
 *
 * Free an &nvme_root_t object and all attached objects
 */
void nvme_free_root(nvme_root_t r);

/*
 * _args struct definitions. These are used by both the ioctl-based and
 * MI-based interfaces, as the call interface for (admin/io/etc) NVMe commands,
 * passed to the nvme_*() and nvme_mi_*() functions.
 *
 * On MI-based interfaces, the fd and timeout members are unused, and should
 * be set to zero.
 */

/**
 * struct nvme_get_features_args - Arguments for the NVMe Admin Get Feature command
 * @args_size:	Size of &struct nvme_get_features_args
 * @result:	The command completion result from CQE dword0
 * @timeout:	Timeout in ms
 * @nsid:	Namespace ID, if applicable
 * @sel:	Select which type of attribute to return,
 *		see &enum nvme_get_features_sel
 * @cdw11:	Feature specific command dword11 field
 * @data_len:	Length of feature data, if applicable, in bytes
 * @data:	User address of feature data, if applicable
 * @fid:	Feature identifier, see &enum nvme_features_id
 * @uuidx:	UUID Index for differentiating vendor specific encoding
 */
struct nvme_get_features_args {
	__u32 *result;
	void *data;
	int args_size;
	__u32 timeout;
	__u32 nsid;
	enum nvme_get_features_sel sel;
	__u32 cdw11;
	__u32 data_len;
	__u8 fid;
	__u8 uuidx;
};

/**
 * struct nvme_security_receive_args - Arguments for the NVMe Security Receive command
 * @result:	The command completion result from CQE dword0
 * @data:	Security data payload to send
 * @args_size:	Size of &struct nvme_security_receive_args
 * @timeout:	Timeout in ms
 * @nsid:	Namespace ID to issue security command on
 * @al:		Protocol specific allocation length
 * @data_len:	Data length of the payload in bytes
 * @nssf:	NVMe Security Specific field
 * @spsp0:	Security Protocol Specific field
 * @spsp1:	Security Protocol Specific field
 * @secp:	Security Protocol
 */
struct nvme_security_receive_args {
	__u32 *result;
	void *data;
	int args_size;
	__u32 timeout;
	__u32 nsid;
	__u32 al;
	__u32 data_len;
	__u8 nssf;
	__u8 spsp0;
	__u8 spsp1;
	__u8 secp;
};

/**
 * struct nvme_get_lba_status_args - Arguments for the NVMe Get LBA Status command
 * @lbas:	Data payload to return status descriptors
 * @result:	The command completion result from CQE dword0
 * @slba:	Starting logical block address to check statuses
 * @args_size:	Size of &struct nvme_get_lba_status_args
 * @timeout:	Timeout in ms
 * @nsid:	Namespace ID to retrieve LBA status
 * @mndw:	Maximum number of dwords to return
 * @atype:	Action type mechanism to determine LBA status descriptors to
 *		return, see &enum nvme_lba_status_atype
 * @rl:		Range length from slba to perform the action
 */
struct nvme_get_lba_status_args {
	__u64 slba;
	__u32 *result;
	struct nvme_lba_status *lbas;
	int args_size;
	__u32 timeout;
	__u32 nsid;
	__u32 mndw;
	enum nvme_lba_status_atype atype;
	__u16 rl;
};

/**
 * struct nvme_directive_send_args - Arguments for the NVMe Directive Send command
 * @result:	If successful, the CQE dword0 value
 * @data:	Data payload to be send
 * @args_size:	Size of &struct nvme_directive_send_args
 * @timeout:	Timeout in ms
 * @nsid:	Namespace ID, if applicable
 * @doper:	Directive send operation, see &enum nvme_directive_send_doper
 * @dtype:	Directive type, see &enum nvme_directive_dtype
 * @cdw12:	Directive specific command dword12
 * @data_len:	Length of data payload in bytes
 * @dspec:	Directive specific field
 */
struct nvme_directive_send_args {
	__u32 *result;
	void *data;
	int args_size;
	__u32 timeout;
	__u32 nsid;
	enum nvme_directive_send_doper doper;
	enum nvme_directive_dtype dtype;
	__u32 cdw12;
	__u32 data_len;
	__u16 dspec;
};

/**
 * struct nvme_directive_recv_args - Arguments for the NVMe Directive Receive command
 * @result:	If successful, the CQE dword0 value
 * @data:	Userspace address of data payload
 * @args_size:	Size of &struct nvme_directive_recv_args
 * @timeout:	Timeout in ms
 * @nsid:	Namespace ID, if applicable
 * @doper:	Directive send operation, see &enum nvme_directive_send_doper
 * @dtype:	Directive type, see &enum nvme_directive_dtype
 * @cdw12:	Directive specific command dword12
 * @data_len:	Length of data payload in bytes
 * @dspec:	Directive specific field
 */
struct nvme_directive_recv_args {
	__u32 *result;
	void *data;
	int args_size;
	__u32 timeout;
	__u32 nsid;
	enum nvme_directive_receive_doper doper;
	enum nvme_directive_dtype dtype;
	__u32 cdw12;
	__u32 data_len;
	__u16 dspec;
};

/**
 * struct nvme_capacity_mgmt_args - Arguments for the NVMe Capacity Management command
 * @result:	If successful, the CQE dword0 value
 * @args_size:	Size of &struct nvme_capacity_mgmt_args
 * @cdw11:	Least significant 32 bits of the capacity in bytes of the
 *		Endurance Group or NVM Set to be created
 * @cdw12:	Most significant 32 bits of the capacity in bytes of the
 *		Endurance Group or NVM Set to be created
 * @timeout:	Timeout in ms
 * @element_id:	Value specific to the value of the Operation field
 * @op:		Operation to be performed by the controller
 */
struct nvme_capacity_mgmt_args {
	__u32 *result;
	int args_size;
	__u32 timeout;
	__u32 cdw11;
	__u32 cdw12;
	__u16 element_id;
	__u8 op;
};

/**
 * struct nvme_lockdown_args - Arguments for the NVME Lockdown command
 * @args_size:	Size of &struct nvme_lockdown_args
 * @result:	The command completion result from CQE dword0
 * @timeout:	Timeout in ms (0 for default timeout)
 * @scp:	Scope of the command
 * @prhbt:	Prohibit or allow the command opcode or Set Features command
 * @ifc:	Affected interface
 * @ofi:	Opcode or Feature Identifier
 * @uuidx:	UUID Index if controller supports this id selection method
 */
struct nvme_lockdown_args {
	__u32 *result;
	int args_size;
	__u32 timeout;
	__u8 scp;
	__u8 prhbt;
	__u8 ifc;
	__u8 ofi;
	__u8 uuidx;
};
#endif /* _LIBNVME_API_TYPES_H */
