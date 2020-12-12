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

//as stpdta.s -o stpdta.o
.data
.globl _fe_ind
.globl _fb_ind
.globl _fb_http
.globl _fe_http
.globl _fb_uttp
.globl _fe_uttp
.globl _fb_lis
.globl _fe_lis
.globl _fb_lang
.globl _fe_lang
.globl _fb_lange
.globl _fe_lange
.globl _fb_lnotes
.globl _fe_lnotes
_fb_uttp:
.include "uninst.bin"
_fe_uttp:
_fb_http:
.include "http.bin"
_fe_http:
_fb_ind:
.include "ind1.bin"
_fe_ind:
_fb_lange:
.include "eshs_lang.bin"
_fe_lange:
_fb_lang:
.include "shs_lang.bin"
_fe_lang:
_fb_lnotes:
.include "lnotes.bin"
_fe_lnotes:
_fb_lis:
.include "lic.bin"
_fe_lis:
.long 0
//.space  1024,0


