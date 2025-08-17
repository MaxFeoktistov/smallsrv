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

.globl _fe_ind_ru
.globl _fb_ind_ru

.globl _fb_uttp
.globl _fe_uttp
.globl _fb_lis
.globl _fe_lis
/*
.globl _fb_lang
.globl _fe_lang
.globl _fb_lange
.globl _fe_lange
*/
.globl _fb_s1
.globl _fb_s2
.globl _fb_s3
.globl _fe_s1
.globl _fe_s2
.globl _fe_s3
.globl _fb_vpnc
.globl _fe_vpnc

.globl _am_lang
.globl _am_lange
.globl _ar_lang
.globl _ar_lange
.globl _bg_lang
.globl _bg_lange
.globl _ch_lang
.globl _ch_lange
.globl _da_lang
.globl _da_lange
.globl _de_lang
.globl _de_lange
.globl _en_lang
.globl _en_lange
.globl _es_lang
.globl _es_lange
.globl _et_lang
.globl _et_lange
.globl _fi_lang
.globl _fi_lange
.globl _fr_lang
.globl _fr_lange
.globl _ge_lang
.globl _ge_lange
.globl _hg_lang
.globl _hg_lange
.globl _it_lang
.globl _it_lange
.globl _jp_lang
.globl _jp_lange
.globl _jw_lang
.globl _jw_lange
.globl _kr_lang
.globl _kr_lange
.globl _mn_lang
.globl _mn_lange
.globl _pe_lang
.globl _pe_lange
.globl _pg_lang
.globl _pg_lange
.globl _pl_lang
.globl _pl_lange
.globl _rm_lang
.globl _rm_lange
.globl _rs_lang
.globl _rs_lange
.globl _ru_lang
.globl _ru_lange
.globl _sw_lang
.globl _sw_lange
.globl _tu_lang
.globl _tu_lange
.globl _ua_lang
.globl _ua_lange


/*.globl _fb_ipbase
.globl _fe_ipbase*/
.globl _fb_lnotes
.globl _fe_lnotes
.align 0
_fb_uttp:
.include "uninst.bin"
_fe_uttp:
_fb_ind:
.include "ind1.bin"
_fe_ind:

_fb_ind_ru:
.include "ind1r.bin"
_fe_ind_ru:

_fb_ipbase:
/* .include "ipbase.s" */
_fe_ipbase:


_am_lang:
.include "am.shs_lang.bin"
_am_lange:

_ar_lang:
.include "ar.shs_lang.bin"
_ar_lange:

_bg_lang:
.include "bg.shs_lang.bin"
_bg_lange:

_ch_lang:
.include "ch.shs_lang.bin"
_ch_lange:

_da_lang:
.include "da.shs_lang.bin"
_da_lange:

_de_lang:
.include "de.shs_lang.bin"
_de_lange:

_en_lang:
.include "en.shs_lang.bin"
_en_lange:

_es_lang:
.include "es.shs_lang.bin"
_es_lange:

_et_lang:
.include "et.shs_lang.bin"
_et_lange:

_fi_lang:
.include "fi.shs_lang.bin"
_fi_lange:

_fr_lang:
.include "fr.shs_lang.bin"
_fr_lange:

_ge_lang:
.include "ge.shs_lang.bin"
_ge_lange:

_hg_lang:
.include "hg.shs_lang.bin"
_hg_lange:

_it_lang:
.include "it.shs_lang.bin"
_it_lange:

_jp_lang:
.include "jp.shs_lang.bin"
_jp_lange:

_jw_lang:
.include "jw.shs_lang.bin"
_jw_lange:

_kr_lang:
.include "kr.shs_lang.bin"
_kr_lange:

_mn_lang:
.include "mn.shs_lang.bin"
_mn_lange:

_pe_lang:
.include "pe.shs_lang.bin"
_pe_lange:

_pg_lang:
.include "pg.shs_lang.bin"
_pg_lange:

_pl_lang:
.include "pl.shs_lang.bin"
_pl_lange:

_rm_lang:
.include "rm.shs_lang.bin"
_rm_lange:

_rs_lang:
.include "rs.shs_lang.bin"
_rs_lange:

_ru_lang:
.include "ru.shs_lang.bin"
_ru_lange:

_sw_lang:
.include "sw.shs_lang.bin"
_sw_lange:

_tu_lang:
.include "tu.shs_lang.bin"
_tu_lange:

_ua_lang:
.include "ua.shs_lang.bin"
_ua_lange:



/*
_fb_lange:
.include "eshs_lang.bin"
_fe_lange:
_fb_lang:
.include "shs_lang.bin"
_fe_lang:

*/

_fb_lnotes:
.include "lnotes.bin"
_fe_lnotes:
_fb_lis:
.include "lic.bin"
_fe_lis:
_fb_s2:
.include "vpn_if_client_up.bin"
_fe_s2:
_fb_s3:
.include "vpn_if_client_down.bin"
_fe_s3:

_fb_vpnc:
.include "vpnclient.bin"
_fe_vpnc:

.globl _fb_sert
.globl _fe_sert

_fb_sert:
.include "temp_sert.bin"
_fe_sert:

.globl _fb_s4
.globl _fe_s4
_fb_s4:
.include "manifest.bin"
_fe_s4:

.long 0
//.space  1024,0


