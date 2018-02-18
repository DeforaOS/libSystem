/* $Id$ */
/* Copyright (c) 2014-2018 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */



#ifndef LIBSYSTEM_SYSTEM_LICENSE_H
# define LIBSYSTEM_SYSTEM_LICENSE_H


/* types */
typedef unsigned int LicenseFlags;
# define LF_CAN_JAIL			0x00000001
# define LF_CAN_MODIFY			0x00000002
# define LF_CAN_PATENT			0x00000004
# define LF_CAN_REDISTRIBUTE		0x00000008
# define LF_CAN_REVERSE_ENGINEER	0x00000010
# define LF_KEEP_ACKNOWLEDGEMENT	0x00000100
# define LF_KEEP_COPYRIGHT		0x00000200
# define LF_KEEP_DISCLAIMER		0x00000400
# define LF_KEEP_SOURCE_CODE		0x00000800
# define LF_VIRAL			0x00001000
# define LF_VIRAL_LIBRARIES		0x00002000

# define LICENSE_BSD2_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_PATENT \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER)
# define LICENSE_BSD2_STRING		"BSD (2 clause)"
# define LICENSE_BSD3_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_PATENT \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER)
# define LICENSE_BSD3_STRING		"BSD (3 clause)"
# define LICENSE_BSD4_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_PATENT \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_ACKNOWLEDGEMENT \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER)
# define LICENSE_BSD4_STRING		"BSD (4 clause)"
# define LICENSE_GNU_GPL2_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_PATENT \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER \
	 | LF_KEEP_SOURCE_CODE \
	 | LF_VIRAL \
	 | LF_VIRAL_LIBRARIES)
# define LICENSE_GNU_GPL2_STRING	"GNU GPL 2"
# define LICENSE_GNU_LGPL21_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_PATENT \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER \
	 | LF_KEEP_SOURCE_CODE \
	 | LF_VIRAL)
# define LICENSE_GNU_LGPL21_STRING	"GNU LGPL 2.1"
# define LICENSE_GNU_GPL3_FLAGS \
	(LF_CAN_MODIFY \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER \
	 | LF_KEEP_SOURCE_CODE \
	 | LF_VIRAL \
	 | LF_VIRAL_LIBRARIES)
# define LICENSE_GNU_GPL3_STRING	"GNU GPL 3"
# define LICENSE_GNU_LGPL3_FLAGS \
	(LF_CAN_MODIFY \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_KEEP_COPYRIGHT \
	 | LF_KEEP_DISCLAIMER \
	 | LF_KEEP_SOURCE_CODE \
	 | LF_VIRAL)
# define LICENSE_GNU_LGPL3_STRING	"GNU LGPL 3"
# define LICENSE_PROPRIETARY_FLAGS	0x0000000
# define LICENSE_PROPRIETARY_STRING	"Proprietary"
# define LICENSE_PUBLIC_DOMAIN_FLAGS \
	(LF_CAN_JAIL \
	 | LF_CAN_MODIFY \
	 | LF_CAN_REDISTRIBUTE \
	 | LF_CAN_REVERSE_ENGINEER)
# define LICENSE_PUBLIC_DOMAIN_STRING	"Public domain"

#endif /* !LIBSYSTEM_SYSTEM_LICENSE_H */
