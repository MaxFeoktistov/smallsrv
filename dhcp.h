/*
 * Copyright (C) 1999-2020 Maksim Feoktistov.
 *
 * This file is part of Small HTTP server project.
 * Author: Maksim Feoktistov 
 *
 *
 * Small HTTP server is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * Small HTTP server is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/ 
 *
 * Contact addresses for Email:  support@smallsrv.com
 *
 * 
 */

/*
RFC 1532        Clarifications and Extensions for BOOTP     October 1993


   0                   1                   2                   3
   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     op (1)    |   htype (1)   |   hlen (1)    |   hops (1)    |
   +---------------+---------------+---------------+---------------+
   |                            xid (4)                            |
   +-------------------------------+-------------------------------+
   |           secs (2)            |           flags (2)           |
   +-------------------------------+-------------------------------+
   |                           ciaddr (4)                          |
   +---------------------------------------------------------------+
   |                           yiaddr (4)                          |
   +---------------------------------------------------------------+
   |                           siaddr (4)                          |
   +---------------------------------------------------------------+
   |                           giaddr (4)                          |
   +---------------------------------------------------------------+
   |                                                               |
   |                           chaddr (16)                         |
   |                                                               |
   |                                                               |
   +---------------------------------------------------------------+
   |                                                               |
   |                           sname  (64)                         |
   +---------------------------------------------------------------+
   |                                                               |
   |                           file   (128)                        |
   +---------------------------------------------------------------+
   |                                                               |
   |                           vend   (64)                         |
   +---------------------------------------------------------------+
 
 
 */

struct dhcpPacket{ 
 uchar op;   
 uchar htype;
 uchar hlen; 
 uchar hops; 
 ulong xid;   
 ushort secs; 
 ushort flags;
 ulong ciaddr;
 ulong yiaddr;
 ulong siaddr;
 ulong giaddr;
 uchar chaddr[16];   
 char sname[64]; 
 uchar file[128];   
 uchar vend[1222];
} __attribute__ ((packed));

