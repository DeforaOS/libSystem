/* $Id$ */
/* Copyright (c) 2014 Pierre Pronchery <khorben@defora.org> */
/* This file is part of DeforaOS System libSystem */
/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */



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
