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

#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_CURRENT_DIR

#define CONFIG_BASE   ""
#define CONFIG_APPDIR ""
#define CONFIG_LANG   ""
#define CONFIG_DATA   ""
#define CONFIG_CONFIG ""
#define CONFIG_LOG    ""
#define CONFIG_TMP    ""

#elif ! defined(SYSUNIX)
// Windows definetion

#define CONFIG_BASE   "c:\\shttps\\"
#define CONFIG_APPDIR CONFIG_BASE
#define CONFIG_SHARE  CONFIG_BASE
#define CONFIG_DATA   CONFIG_BASE
#define CONFIG_CONFIG CONFIG_BASE
#define CONFIG_LOG    CONFIG_BASE
#define CONFIG_TMP    CONFIG_BASE

#else

#define CONFIG_BASE   "/usr/local/"
#define CONFIG_APPDIR CONFIG_BASE "lib/smallsrv/"
#define CONFIG_SHARE  CONFIG_BASE "share/smallsrv/"
#define CONFIG_DATA   "/var/"
#define CONFIG_CONFIG "/etc/smallsrv/"
#define CONFIG_LOG    "/var/log/smallsrv/"
#define CONFIG_TMP    "/dev/shm"

#endif

#endif
