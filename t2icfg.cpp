
//  GENERATED FILE, DON'T  EDIT THIS!  !!!  Edit t2icfg.cpp instead...

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


#define MAINSTR 1

#ifndef STRING_CONST_H_t2
#include "g4s1.hh"
#endif


#ifndef STRING_CONST_H
#include "g4strc.h"
//#include "strc.h"
#endif



#ifndef SRV_H
#include "srv.h"
#endif

#ifdef  G4STRING_CONST_H

#define CS(a)  x_##a
#else
#define CS(a)  a

#endif

short MnuOffset[20];
int iofs;
#ifdef CD_VER
char *phtml_ini="";
#endif

#ifdef USE_IPV6

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, CS( t2T_4362878   )},\
{#n "_bind",MAX_ADAPT*15,0,(uint *)& (bind_a[f]), CS( t2T_4197922  )},\
{#n "ipv6",2,1<<f,(uint *)0, CS( t2T_1259448  )},

// #define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), t " E.g. 192.168.0.1-192.168.0.16,127.0.0.1" )},\
// {#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), td )},\
// {#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), CS("IPv6 " t " E.g. ::1,FE80::-FEFF::" )},\
// {#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]), CS("IPv6 "  td )},

#define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), "%s E.g. 192.168.0.1-192.168.0.16,127.0.0.1", x_##t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]),  x_##td },\
{#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), x_## t2T_5466093 ,x_##t },\
{#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]), "IPv6 %s" ,   x_##td },

#else
#define OIPV6(n,f)

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]),  CS(t )},\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]),  CS(td )},


#endif

// #define XLIMIT(a,b,c)  \
// {"no" #a "_ltime",0,0,(uint *)0, CS("No limitation for " b )},\
// { #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), CS("Time per that will calculating limits (in seconds)" )},\
// { #a "_ip_limit" ,0x400,0x40000000,(uint *) (ip_limit+c), CS("Limit per IP (Kb)" )},\
// { #a "_net_limit" ,0x400,0x40000000,(uint *) (net_limit+c), CS("Limit per network (Kb)" )}, \
// { #a "_limit" ,0x400,0x40000000,(uint *) (limit+c), CS("Total limit for server (Kb)" )}

#define XLIMIT(a,b,c)  \
{"no" #a "_ltime",0,0,(uint *)0, CS( t2T_1727805 ),b },\
{ #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), CS( t2T_3322121  )},\
{ #a "_ip_limit" ,0x400,0x40000000,(uint *) (ip_limit+c), CS( t2T_5543034  )},\
{ #a "_net_limit" ,0x400,0x40000000,(uint *) (net_limit+c), CS( t2T_6151093  )}, \
{ #a "_limit" ,0x400,0x40000000,(uint *) (limit+c), CS( t2T_3979193  )}


CfgParam ConfigParams[]={

{0,0,0,0, CS(sGENERAL_S )},
#ifdef SYSUNIX
{"noicon",0,FL_NOICON,(uint *)0,CS( sDON_T_OUT )},
{"hide"  ,0,FL_HIDE,(uint *)0, CS(sSTART_AS_0 )},
#else
{"noicon",0,FL_NOICON,(uint *)0, CS(sDISABLE_I )},
{"hide"  ,0,FL_HIDE,(uint *)0, CS(sMINIMIZE_ )},
#endif
#ifndef CD_VER
{"detail",0,FL_FULLLOG,(uint *)0, CS(sDETAIL_LO )},
{"nolog",1,0,(uint *)0, CS(sDISABLE_S )},
{"log",256,0,(uint *)&flog, CS(sSTORE_LOG )},
{"logday",0,FL_LOGDAY,(uint *)0, CS(sNEW_LOG_F )},
{"nolimitlog",1,0,(uint *)0, CS( t2T_1872208 )},
{"limitlog",0,4096,(uint *)&trim_log_lines, CS( t2T_4078374   )},


#endif

{"nofrom_same_host",0,0,(uint *)0, CS(sDISABLE_T )},
{"from_same_host",0,256,(uint *)&max_cln_host, CS(sUSERS_FRO  )},

#define SPD(a,b) {"no" #b  "_speed",0,0,(uint *)0, CS( t2T_4199171  )},\
{ #b "_speed",0, 0x200000,(uint *) & (ipspeed[a]), CS( t2T_3759240 )},\
{ #b "_spdusr",0,0x200000,(uint *) & (ipspdusr[a]),CS(  t2T_4406032  )},

RANGES(ip,sIP_RANGES,sIP_RANGESD)

{"cryptpwd",1,FL1_CRYPTPWD,(uint *)0, CS( t2T_3105507  )},
{"delpwd",1,FL1_DELPAS,(uint *)0, CS( t2T_2188250  )},
#ifdef WITHMD5
{"md5pwd",2,FL2_MD5PASS,(uint *)0, CS( t2T_2190235  )},
{"md5realm",255,0,(uint *)&realm, CS( t2T_2154049  )},
{"md5digest",2,FL2_USEMD5D,(uint *)0, CS( t2T_2847174  )},
{"md5paranoidal",2,FL2_PARANOIDMD5,(uint *)0, CS( t2T_4537181  )},

#endif

#ifndef CD_VER
{"radmin",0,FL_RADMIN,(uint *)0, CS(sENABLE_TO )},
RANGES(adm,sIP_RANGES1,sIP_RANGESD1)
{"twopoint",1,FL1_2P,(uint *)0, CS( t2T_2186080  )},

#ifndef SYSUNIX
//{"utf8",2,FL2_UTF,(uint *)0, CS("Convert filenames from UTF-8" )},
{"utf8",2,FL2_UTF,(uint *)0, CS( t2T_1406971  )},
#endif

#endif

{0,0,0,0, CS(sHTTP_SERV )},
{"nomax",0,0,(uint *)0, CS(sDISABLE_H )},
{"max"   ,0,1024   ,(uint *)&max_cln, CS(sHOW_MANY_ )},
{"port"  ,1,0xFFFF,(uint *)&http_port,    CS(sTCP_IP_PO )},

RANGES(http,sIP_RANGES,sIP_RANGESD)

OIPV6(http,0)
SPD(0,http)
{"dir",255,0,(uint *)&def_dir, CS(sDEFAULT_W )},
{"def",128,0,(uint *)&def_name, CS(sDEFAULT_F )},
{"error",256,0,(uint *)&error_file, CS(sERROR_FIL )},
#if defined(CD_VER) || !defined(SYSUNIX)
{"cgi_ident",254, 0,(uint *)&cgi_detect, CS(sCGI_IDENT )},
{"perl",256,0,(uint *)&perl, CS(sPERL__IF_ )},
#endif
{"php",256,0,(uint *)&phtml_dir, CS(sPHP__IF_Y )},
#ifdef CD_VER
{"phpini",256,0,(uint *)&phtml_ini, CS(sPHP_INI_D )},
#endif
#if defined(CD_VER) || !defined(SYSUNIX)
{"nowintypes",0,3,(uint *)0, CS(sDISABLE_T0 )},
#endif
{"ssihtm",0,FL_SSIHTM, (uint *)0, CS(sENABLE_TO0 )},
{"noshare",1,0,(uint *)0, CS(sDISABLE_S0 )},
{"share",255,0,(uint *)&doc_dir, CS(sSHARE_DIR )},
{"post_limit",0x800,0x2000000,(uint *)&post_limit, CS(sTHE_LIMIT )},
{"nooutdir"  ,0,FL_NODIR,(uint *)0, CS(sDON_T_SHO )},
#if  defined(CD_VER) || !defined(SYSUNIX)
{"norunhtm"  ,0,FL_NORUNHTM,(uint *)0, CS(sNEVER_RUN )},

#endif
{"cgi_timeout",30,0x100000,(uint *)&cgi_timeout, CS(sLIMIT_OF_ )},
{"nbrkcgi"  ,1,FL1_NBRK,(uint *)0, CS( t2T_2132379   )},

{"header",2048,0,(uint *)&header, CS(sADVANCED_ )},
#ifndef SYSUNIX
{"dntshowhiden",1,FL1_DNTSHOWHIDEN,(uint *)0, CS( t2T_4811739   )},
{"runsystem"  ,1,FL1_RUNSYSTEM,(uint *)0, CS( t2T_3611249   )},
#endif

{"ssi_chunk",2,FL2_CHUNKED,(uint *)0, CS( t2T_3048175  )},

{"nomsd" ,1,FL1_NOM, (uint *)0, CS( t2T_1674985 )},
{"http_gzip" ,1,FL1_GZ, (uint *)0, CS( t2T_2371793 )},
{"gz_lib",256,0,(uint *)& zlib,   CS( t2T_1758218 )},
{"gz_low"  ,0,0x1000000,(uint *)&GZSizeLow, CS( t2T_3165815 )},
{"nogz_ext"  ,1024,0,(uint *)&NoGZSuf, CS( t2T_2726483 )},

{"ip_base",256,0,(uint *)& ipbsfile,  CS(  t2T_2488633 )},
{"ip_cntr"  ,1,FL1_IPCNTR,(uint *)0, CS( t2T_2443331 )},
{"ip2cntr_srv"  ,2,FL2_IPCONTRY,(uint *)0, CS( t2T_3645830 )},
{"ip2cntr_aut"  ,2,FL2_IPCNTRAUT,(uint *)0, CS( t2T_3765166 )},
{"http_doh"  ,2,FL2_DOH,(uint *)0, CS( t2T_2243001 )},


XLIMIT(http,"HTTP",7),

#if V_FULL
{0,0,0,0, CS(sDNS_SERVE )},
{"nohosts",2,0,(uint *)0, CS(sDISABLE_D )},
{"hosts",256,0,(uint *)&dns_file, CS(sHOSTS_FIL )},
{"noreqursion",2,0,(uint *)0, CS(sDISABLE_R )},
{"dnscache",0,0x4000,(uint *)&dns_cach_size, CS( t2T_4067961  )},
{"dnstimeout",100,2000,(uint *)&DnsTms, CS( t2T_3305957  )},
OIPV6(dns,SDNS_IND)

{"dnsupl",0,FL_DNSUPLVL,(uint *)0, CS(sRECURSION )},
{"dnstcp",0,FL_DNSTCP ,(uint *)0, CS(sENABLE_DN )},
                        //{"dnsaptr",1,FL1_AUTOPTR ,(uint *)0, CS("Auto make PTR from A record"  )},

RANGES(dns,sIP_RANGES,sIP_RANGESD)

{"nodnscachefile",2,0,(uint *)0, CS(sDON_T_SAV )},
{"dnscachefile",256,0,(uint *)&dnscachefile, CS(sDNS_CACHE )},
{"dnsno6",2,FL2_DNSNO6 ,(uint *)0, CS( t2T_2110311  )},

{"nodns_bld",1,0,(uint *)0, CS( t2T_1641062  )},
{"dns_bld",63,0,(uint *)&dnsblname, CS( t2T_2559746  )},
{"dns_detect_dos"  ,0,0x1000000,(uint *)&dns_dos_limit, CS( t2T_6448516 )},

/*
#ifndef SYSUNIX
{0,0,0,0, CS(sTO_REMOTE )},
{"noddns_time",0,0,(uint *)0, CS(sDISABLE_C )},
{"ddns_time"  ,0,0x1fFFFF,(uint *)&time_update, CS(sTIME_TO_R )},
{"ddns",258,0,(uint *)&dns_user, CS(sURL_ON_RE )},
#endif
*/

{0,0,0,0, CS(sPROXY_SER )},
{"noproxy_max",0,0,(uint *)0, CS(sDISABLE_P )},
{"proxy_max",0,1024,(uint *)&max_prx, CS(sHOW_MANY_0 )},
{"proxy" ,1,0xFFFF,(uint *)&proxy_port, CS(sTCP_IP_PO0 )},

OIPV6(proxy,1)

#ifdef MEMPRX
{"cache" ,0,0x3FFF, (uint *)&cash_max_kb, CS(sMEMORY_CA )},
#endif
{"noproxy_dir",0,0,(uint *)0, CS(sDISABLE_T1 )},
{"proxy_dir",255,0,(uint *)&proxy_dir, CS(sCACHE_PRO )},
{"proxy_time",0,0x6000,(uint *)&(proxy_chk.time), CS(sFOR_HOW_M )},
{"proxy_fsize",0,0x7FFFFFFF,(uint *)&max_pfile, CS( t2T_5252181  )},
{"proxy_laccess" ,1,FL1_LACPROXY, (uint *)0, CS( t2T_4346165 )},

{"ignocache"  ,0,FL_IGNNOCH,(uint *)0, CS(sIGNORE_NO )},
{"proxy_hrd"  ,1,FL1_PRXHRD,(uint *)0, CS( t2T_3002398  )},
{"proxyusers" ,0,FL_PRXUSER,(uint *)0, CS(sPROXY_FOR )},

RANGES(proxy,sIP_RANGES,sIP_RANGESD)

 SPD(1,proxy)
{"proxy_big" ,1,FL1_BIGPROXY, (uint *)0, CS( t2T_3282821 )},
{"proxy_sbig" ,1,FL1_BBPROXY, (uint *)0, CS( t2T_3391538 )},
{"proxy_tryes",0,0x2000,(uint *)&max_cont_st, CS( t2T_4869017  )},
{"proxy_same",0,1024,(uint *)&cnt_same, CS( t2T_3460001  )},

{"noupproxy",3,0,(uint *)0, CS(sDON_T_USE )},
{"upproxy" ,164,0,(uint *)&up_proxy, CS(sUP_LEVEL_ )},
{"upproxy_port" ,1,0xFFFF,(uint *)&up_proxy_port, CS(sTCP_IP_PO1 )},
{"noup_user",3,0,(uint *)0, CS(sUP_LEVEL_0 )},
{"up_user" ,128,0,(uint *)&up_user, CS(sUP_LEVEL_1 )},
{"ever_upproxy" ,1,FL1_UPPRX, (uint *)0, CS( t2T_3630380 )},
{"nouphosts" ,4096,0, (uint *)&nohosts, CS( t2T_3118030 )},

{"bad_hosts",4096,0,(uint *)&bad_hosts, CS(sDISABLED_ )},
{"proxy_timeout",30,0xFFFFF,(uint *)&PRXTimeout, CS( t2T_5697828  )},
{"proxy_gzip" ,1,FL1_GZPRX, (uint *)0, CS( t2T_3133276 )},

{"noproxy_antivirus",3,0,(uint *)0, CS( t2T_3436009 )},
{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, CS( t2T_6602174 
//"Antivirus command for check active content (Checked filename will be added to end)"
 )},
{"proxy_avport" ,1,0xFFFF,(uint *)&proxy_antivirus_port, CS( t2T_7871815  )},
{"proxy_avhtml" ,2,FL2_PAVHTML, (uint *)0, CS( t2T_3955615 )},
{"proxy_avall" ,2,FL2_PAVALL, (uint *)0, CS( t2T_3456905 )},


 XLIMIT(proxy, "Proxy",2),

{0,0,0,0, CS(sFTP_SERVE )},
{"noftp_max",0,0,(uint *)0, CS(sDISABLE_F )},
{"ftp_max",0,1024,(uint *)&max_ftp, CS(sHOW_MANY_1 )},
{"ftp_port",1,0xFFFF,(uint *)&ftp_port, CS(sTCP_IP_PO2 )},
{"ftp_timeout",30,0xFFFFF,(uint *)&FTPTimeout, CS(sLIMIT_OF_0 )},

OIPV6(ftp,2)
RANGES(ftp,sIP_RANGES,sIP_RANGESD)
SPD(2,ftp)

{"noftp_pasvp",0,0,(uint *)0, CS( t2T_2064283  )},
{"ftp_pasvp",1,0xFFFF,(uint *)&first_pass_port, CS( t2T_5234218 )},

{"ftp_oone",2,FL2_NOMFTP, (uint *)0, CS( t2T_2686706  )},


{"ftp_wospace",0,FL_FTWOSPACE, (uint *)0, CS(sCONVERT_N )},
{"noftp_upload",3,0,(uint *)0, CS(sDON_T_USE0 )},
{"ftp_upload",128,0,(uint *)&ftp_upload, CS(sNAME_OF_U )},
{"ftp_vdirs",0,FL_FTPVDIR, (uint *)0, CS(sENABLE_VI )},

{"ftp_same" ,1,FL1_FTPSAME, (uint *)0, CS( t2T_2789225 )},
{"ftp_proxy" ,1,FL1_FTPROXY, (uint *)0, CS( t2T_3196657 )},

XLIMIT(ftpi, CS( t2T_297279 ),3),
XLIMIT(ftpo, CS( t2T_297855 ),4),

{0,0,0,0, CS(sPOP__SERV )},
{"nopop3_max",0,0,(uint *)0, CS(sDISABLE_P0 )},
{"pop3_max"  ,0,1024   ,(uint *)&max_pop, CS(sHOW_MANY_1 )},
{"pop_port" ,1,0xFFFF,(uint *)&pop_port,     CS(sTCP_IP_PO3 )},
{"pop_timeout"  ,30,0xFFFFF,(uint *)&POPTimeout, CS(sPOP__SMTP )},

RANGES(pop,sIP_RANGES,sIP_RANGESD)
OIPV6(pop,4)
SPD(4,pop)

{"pop3_proxy",1,FL1_POPROXY,(uint *)0, CS( t2T_3304985  )},

{"wmail",2,FL2_WMAIL,(uint *)0, CS( t2T_1893843  )},
{"nowmailsent",1,0,(uint *)0, CS( t2T_2071505  )},
{"wmailsent" ,64,0,(uint *)&loc_sent, CS( t2T_2964100  )},
{"nowmailtrash",1,0,(uint *)0, CS( t2T_2262526  )},
{"wmailtrash" ,64,0,(uint *)&loc_trash, CS( t2T_3387087  )},
{"wmail_utf",2,FL2_WMUTF, (uint *)0, CS( t2T_2791726  )},


{0,0,0,0, CS(sSMTP_SERV )},
{"nosmtp_max",0,0,(uint *)0, CS(sDISABLE_S1 )},
{"smtp_max"  ,1,1024   ,(uint *)&max_smtp, CS(sHOW_MANY_1 )},
{"smtp_name" ,128,0,(uint *)&smtp_name, CS(sSMTP_SERV0 )},
OIPV6(smtp,3)
{"vhalias",0,FL_VHALIAS,(uint *)0, CS(sUSE_ALL_V )},
{"smtp_dns" ,16,0,(uint *)&dns_server_for_mail, CS(sDNS_SERVE0 )},
{"smtp_nomx",1,FL1_MHST,(uint *)0, CS( t2T_2692044  )},
{"nosmtpproxy",1,0,(uint *)0, CS( t2T_2158005  )},
{"smtpproxy" ,128,0,(uint *)&smtproxy , CS( t2T_3232509  )},

{"smtp_port" ,1,0xFFFF,(uint *)&smtp_port,   CS( sTCP_IP_PO4 )},
{"smtp_out",255,0,(uint *)&  out_path, CS(sOUTPUT_PA )},
{"nosmtp_sent",6,0,(uint *)0, CS(sDISABLE_T2 )},
{"smtp_sent",255,0,(uint *)&sent_path, CS(sSENT_PATH )},
{"sent_time",0,0x6000,(uint *)&(smtp_chk.time), CS(sFOR_HOW_M0 )},
{"smtp_err",255,0,(uint *)&  err_path, CS(sERROR_PAT )},
{"smtp_any",0,FL_SMTPANY,(uint *)0, CS(sRECEIVE_T )},
RANGES(smtp,  t2T_281408  /* sIP_RANGES */,sIP_RANGESD)
{"smtp_pop_ip",1,FL1_SMTP_AVT,(uint *)0, CS( t2T_3845776  )},
{"smtp_msg_limit" ,0x1000,0x1C00000,(uint *)&max_msg_size, CS(sLIMIT_OF_1 )},
{"smtp_nobreak",0,FL_NOBRKSMTP,(uint *)0, CS(sDONT_BREAK )},
{"blacklist" ,0x8000,0,(uint *)&blacklist, CS(sDISABLED_0 )},
{"smtp_conform",1,FL1_CONFIRM,(uint *)0, CS( t2T_3864377  )},
{"forward",0,FL_FORWARD,(uint *)0, CS(sENABLE_TO1 )},
{"fwdrun", 0,FL_FWDRUN ,(uint *)0, CS(sENABLE_TO2 )},

{"goodlist", 256,0 ,(uint *)&glst, CS( t2T_2205754 )},
{"badlist", 256,0 ,(uint *)&blst, CS( t2T_1940982 )},
{"graylist", 256,0 ,(uint *)&graylst, CS( t2T_2688977 )},
{"chklists", 1,FL1_CHKUSR ,(uint *)0, CS( t2T_2692031  )},
{"msgspam", 200,0 ,(uint *)&msg_spam, CS( t2T_2571888  )},

{"noantivirus",1,0,(uint *)0, CS( t2T_2094607  )},
{"antivirus" ,256,0,(uint *)&antiv , CS( t2T_2616985  )},
{"run_timeout",30,0x100000,(uint *)&ttl_avr, CS(sLIMIT_OF_ )},
{"antispam" ,2048,0,(uint *)&antispam , CS( t2T_2908859  )},
{"spamfltr" ,2048,0,(uint *)&spamfltr , CS( t2T_2945553  )},
{"nocheckback",1,FL1_NOCHKBCK,(uint *)0, CS( t2T_3570899  )},
{"fake" ,0x102,0,(uint *)&fake, CS( t2T_1590071  )},
{"dnsbl" ,320,0,(uint *)&dnsbl, CS( t2T_1681448  )},
{"checkmx",2,FL2_CHKMX,(uint *)0, CS( t2T_2264384  )},
{"mxignbl",2,FL2_MX_GLIST,(uint *)0, CS( t2T_2795522  )},


{"spam_time" ,0,0x7FFFFFFF,(uint *)(ltime+1), CS( t2T_4238571  )},

XLIMIT(smtp, "SMTP",0),

{"nolimitus",1,FL1_NOLUS,(uint *)0, CS( t2T_2847040  )},
{"uncheckip",2,FL2_MLNOIP,(uint *)0, CS( t2T_2886929  )},
{"time_btw"  ,2,1024   ,(uint *)&time_btw, CS( t2T_2888615  )},



{0,0,0,0, CS( t2T_103934  )},
{"nodhcp_max",0,0,(uint *)0, CS( t2T_1814608  )},
{"dhcp_max",0,0xFEFEFE ,(uint *)&total_dhcp_ip, CS( t2T_4657330  )},
{"dhcp_ip",16,0,(uint *)&dhcp_addr, CS( t2T_2534733  )},
{"dhcp_bcast",16,0,(uint *)&dhcp_bcast, CS( t2T_3395438  )},
{"dhcp_first",16,0,(uint *)&first_dhcpc, CS( t2T_3689298  )},
{"dhcp_mask",16,0,(uint *)&netmaskc, "Netmask"},
{"dhcp_gate",16,0,(uint *)&gatewayc, "Gateway"},
{"dhcp_dns",64,0,(uint *)&dhcp_dnsc, CS( t2T_2779449  )},
{"dhcp_name",64,0,(uint *)&DhcpServerName, CS( t2T_3881618  )},
{"dhcp_file",256,0,(uint *)&dhcp_fname, CS( t2T_3263802  )},
{"dhcp_rdns",1,FL1_DHCPDNS,(uint *)0, CS( t2T_2981372  )},
{"dhcp_lo",1,FL1_DHCPLO,(uint *)0, CS( t2T_2361397  )},

#ifdef TELNET


{0,0,0,0, CS( t2T_1039340  )},
{"notel_max",0,0,(uint *)0, CS( t2T_1671671  )},
{"tel_max"  ,0,1024,(uint *)&max_tel, CS(sHOW_MANY_1 )},
{"tel_port" ,1,0xFFFF,(uint *)&tel_port, CS( t2T_3432320  )},

RANGES(tel,sIP_RANGES,sIP_RANGESD)
OIPV6(tel,6)
SPD(6,tel)
{"tel_cmd",256,0,(uint *)&tel_cmd, CS( t2T_2348618  )},

#endif


{0,0,0,0, CS(sTLS__SERV )},
{"notls_max",0,0,(uint *)0, CS(sDISABLE_TLS )},
{"tls_max"  ,0,1024,(uint *)&max_ssl, CS(sHOW_MANY_1 )},
{"tls_port" ,1,0xFFFF,(uint *)&ssl_port,     CS(sTCP_IP_TLS )},

RANGES(ssl,sIP_RANGES,sIP_RANGESD)
OIPV6(tls,5)
SPD(5,tls)
{"smtp_tls",1,FL1_SMTPTLS,(uint *)0, CS( t2T_2938073  )},
{"ftp_tls",2,FL2_FTPTLS,(uint *)0, CS( t2T_2499029  )},

{"tls_lib"  ,256,0,(uint *)&TLSLibrary, CS(sTLS__LIB )},

{"tls_cert_file" ,256,0,(uint *)&s_cert_file, CS(sTLS__SERT )},
{"tls_key_file" ,256,0,(uint *)&s_key_file, CS(sTLS__KEYF )},
{"tls_capath" ,255,0,(uint *)&CApath, CS(sTLS__CAPATH )},
{"tls_cafile" ,256,0,(uint *)&CAfile, CS(sTLS__CAFILE )},
{"tls_priority" ,2047,0,(uint *)&tls_priority, CS( t2T_4948901 )},

{"admtls",1,FL1_ATLS,(uint *)0, CS( t2T_1962848  )},
{"tls_wmail",2,FL2_WMTLS,(uint *)0, CS( t2T_2809214  )},


#endif
#ifndef FREEVER

{"registr_user",128,0,(uint *)&user_name,0},
{"registr_code",0,0xFFFFffff,(uint *)&chk_code3a,0},
{"registr_code1",0,0xFFFFffff,(uint *)&chk_code3b,0},
#endif

{0,0,0,(uint *)0,0}
};
