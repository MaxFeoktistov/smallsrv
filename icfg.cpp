/*
 * Copyright (C) 1999-2023 Maksim Feoktistov.
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

#ifdef TLSVPN
#include "vpn.h"
#endif


// #ifdef  G4STRING_CONST_H
//
// #define CS(a)  x_##a
// #else
// #define CS(a)  a
//
// #endif

short MnuOffset[20];
int iofs;
#ifdef CD_VER
char *phtml_ini="";
#endif

#ifdef USE_IPV6

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, CXS(S1t2T_4362878, "Bind to all addapters"  )},\
{#n "_bind",MAX_ADAPT*15,0,(uint *)& (bind_a[f]), CXS(S1t2T_4197922, "IPs and IPv6 to bind, through coma. (0.0.0.0 - bind to all IP; ::0 bind to all IPv6)" )},\
{#n "ipv6",2,1<<f,(uint *)0, CXS(S1t2T_1259448, "Also work through IPv6" )},

// #define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), t " E.g. 192.168.0.1-192.168.0.16,127.0.0.1" )},\
// {#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), td )},\
// {#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), CS("IPv6 " t " E.g. ::1,FE80::-FEFF::" )},\
// {#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]), CS("IPv6 "  td )},

#define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), "%s E.g. 192.168.0.1-192.168.0.16,127.0.0.1", t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]),  td },\
{#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), "IPv6 %s E.g. ::1,FE80::-FEFF::", t },\
{#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]), "IPv6 %s" , td },

#else
#define OIPV6(n,f)

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]),  t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]),  td },


#endif

// #define XLIMIT(a,b,c)  \
// {"no" #a "_ltime",0,0,(uint *)0, CS("No limitation for " b )},\
// { #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), CS("Time per that will calculating limits (in seconds)" )},\
// { #a "_ip_limit" ,0x400,0x40000000,(uint *) (ip_limit+c), CS("Limit per IP (Kb)" )},\
// { #a "_net_limit" ,0x400,0x40000000,(uint *) (net_limit+c), CS("Limit per network (Kb)" )}, \
// { #a "_limit" ,0x400,0x40000000,(uint *) (limit+c), CS("Total limit for server (Kb)" )}

#define XLIMIT(a,b,c)  \
{"no" #a "_ltime",0,0,(uint *)0, CXS(S1t2T_1727805, "No limitation for %s"),b },\
{ #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), CXS(S1t2T_3322121, "Time per that will calculating limits (in seconds)" )},\
{ #a "_ip_limit" ,0x400,0x40000000,(uint *) (ip_limit+c), CXS(S1t2T_5543034, "Limit per IP (Kb)" )},\
{ #a "_net_limit" ,0x400,0x40000000,(uint *) (net_limit+c), CXS(S1t2T_6151093, "Limit per network (Kb)" )}, \
{ #a "_limit" ,0x400,0x40000000,(uint *) (limit+c), CXS(S1t2T_3979193, "Total limit for server (Kb)" )}


CfgParam ConfigParams[]={

{0,0,0,0, CXS(S2sGENERAL_S, "General setting")},
#ifdef SYSUNIX
{"noicon",0,FL_NOICON,(uint *)0,CXS(S2sDON_T_OUT, "Don't out log to terminal.")},
{"hide"  ,0,FL_HIDE,(uint *)0, CXS(S2sSTART_AS_0, "Start as daimon.")},
#else
{"noicon",0,FL_NOICON,(uint *)0, CXS(S2sDISABLE_I, "Disable icon in notification area.")},
{"hide"  ,0,FL_HIDE,(uint *)0, CXS(S2sMINIMIZE_, "Minimize on startup.")},
#endif
#ifndef CD_VER
{"detail",0,FL_FULLLOG,(uint *)0, CXS(S2sDETAIL_LO, "Create detailed log for POP/SMTP/FTP. (By default only"
" basic events are added to the log)")},
{"nolog",1,0,(uint *)0, CXS(S2sDISABLE_S, "Disable saving log.")},
{"log",256,0,(uint *)&flog, CXS(S2sSTORE_LOG, "Select filename and location of log.")},
{"logday",0,FL_LOGDAY,(uint *)0, CXS(S2sNEW_LOG_F, "Create a daily log (new log created after each day). It's necessary"
" to get statistics for a day.")},
{"nolimitlog",1,0,(uint *)0, CXS(S1t2T_1872208, "Don't trim log lines")},
{"limitlog",0,4096,(uint *)&trim_log_lines, CXS(S1t2T_4078374, "Limit the length of the log lines. The length of each line should not exceed this value"  )},
#ifdef SEPLOG
{"seplog",2,FL2_SEPARATELOG,(uint *)0, CXS(S1t2T_2946294, "Separate log for each server")},
#endif

{"dbgle",2,FL2_DEBUG_RE,(uint *)0, CXS(S1t2T_2189963, "Add to log debug info from logical expresion in SSI and Antivirus/Forward files" )},
#endif

{"nofrom_same_host",0,0,(uint *)0, CXS(S2sDISABLE_T, "No restrict the number of simultaneous connections from"
" each host.")},
{"from_same_host",0,256,(uint *)&max_cln_host, CXS(S2sUSERS_FRO, "Number of simultaneous requests from each host. "
"(Note: Restriction includes all TCP connections (HTTP,FTP,POP,SMTP,Proxy)")},

#define SPD(a,b) {"no" #b  "_speed",0,0,(uint *)0, CXS(S1t2T_4199171, "Don't restrict speed of outgoing transfer" )},\
{ #b "_speed",0, 0x200000,(uint *) & (ipspeed[a]), CXS(S1t2T_3759240, "Limit for summary speed of outgoing transfer for all connections from the same IP (KBytes/minute)")},\
{ #b "_spdusr",0,0x200000,(uint *) & (ipspdusr[a]),CXS(S1t2T_4406032, "How many another connections must have activity, to check on speed limitation" )},

RANGES(ip,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))

{"cryptpwd",1,FL1_CRYPTPWD,(uint *)0, CXS(S1t2T_3105507, "Don't save uncrypted passwords in config file" )},
{"delpwd",1,FL1_DELPAS,(uint *)0, CXS(S1t2T_2188250, "Remove passwords from the log" )},
#ifdef WITHMD5
{"md5pwd",2,FL2_MD5PASS,(uint *)0, CXS(S1t2T_2190235, "Save passwords as MD5 Digest (RFC2069/RFC2617)" )},
{"md5realm",255,0,(uint *)&realm, CXS(S1t2T_2154049, "Realm - string for MD5 Digest (RFC2069/RFC2617)" )},
{"md5digest",2,FL2_USEMD5D,(uint *)0, CXS(S1t2T_2847174, "Using MD5 Digest for authorization if posible (RFC2069/RFC2617)" )},
{"md5paranoidal",2,FL2_PARANOIDMD5,(uint *)0, CXS(S1t2T_4537181, "Using paranoidal variant of MD5 Digest for authorization if posible (RFC2617 qop=auth)" )},

#endif

#ifndef CD_VER
{"radmin",0,FL_RADMIN,(uint *)0, CXS(S2sENABLE_TO, "Enable web administration.")},
RANGES(adm,sIP_RANGES1,sIP_RANGESD1)
{"twopoint",1,FL1_2P,(uint *)0, CXS(S1t2T_2186080, "Enable 2 point in filenames (may be dangerous)" )},

#ifndef SYSUNIX
//{"utf8",2,FL2_UTF,(uint *)0, CS("Convert filenames from UTF-8" )},
{"utf8",2,FL2_UTF,(uint *)0, CXS(S1t2T_1406971, "For the log window: Check the log for UTF-8 characters and convert the log to Unicode, if any. (take a little longer)" )},
#else
#ifdef USE_SYSPASS

{"sysuser",3,FL3_SYS_USERS,(uint *)0, CXS(S1T_39580082, "Use system users/passwords databases. Warning: Digest and APOP authorization methods will not work for system users")},
{"grp_pop",64,0,(uint *)&(access_groups[0]), CXS(S1T_48821451, "The name of the system group whose members are allowed to connect to the POP3 server (if system user/password databases are used)")},
{"grp_smtp",64,0,(uint *)&(access_groups[1]), CXS(S1T_44994085, "The name of the system group whose members are allowed to send mail via SMTP server (if system user/password databases are used)")},
{"grp_ftpr",64,0,(uint *)&(access_groups[2]), CXS(S1T_43526262, "The name of the system group whose members are allowed to receive files via FTP (if system user/password databases are used)")},
{"grp_ftpw",64,0,(uint *)&(access_groups[3]), CXS(S1T_40165245, "The name of the system group whose members are allowed to upload files via FTP (if system user/password databases are used)")},
{"grp_ftpe",64,0,(uint *)&(access_groups[6]), CXS(S1T_45385361, "The name of the system group whose members are allowed to set executable file mode via FTP (if system user/password databases are used)")},
{"grp_proxy",64,0,(uint *)&(access_groups[4]), CXS(S1T_39605550, "The name of the system group whose members are allowed to use Proxy and VPN (if system user/password databases are used)")},
{"grp_admin",64,0,(uint *)&(access_groups[5]), CXS(S1T_48452563, "The name of the system group whose members are allowed to administrate the server (if system user/password databases are used)")},

#endif
#endif

#endif

{0,0,0,0, CXS(S2sHTTP_SERV, "HTTP server setting")},
{"nomax",0,0,(uint *)0, CXS(S2sDISABLE_H, "Disable HTTP server.")},
{"max"   ,0,1024   ,(uint *)&max_cln, CXS(S2sHOW_MANY_, "Number of HTTP requests working simultaneous."
" Approcsimately 20Kb of memory is reserved for each"
" thread. Usually 12 connections are enought for 3-8"
" visitors per minute.")},
{"port"  ,1,0xFFFF,(uint *)&http_port,    CXS(S2sTCP_IP_PO, "TCP/IP port for HTTP server. Usualy it's 80")},

RANGES(http,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))

OIPV6(http,0)
SPD(0,http)
{"dir",255,0,(uint *)&def_dir, CXS(S2sDEFAULT_W, "Default web folder.")},
{"def",128,0,(uint *)&def_name, CXS(S2sDEFAULT_F, "Default file name. (Wildcards are accepted, such as"
" index.* to allow any index file in folder)")},
{"error",256,0,(uint *)&error_file, CXS(S2sERROR_FIL, "Error file. Full path to file or script that"
" will be returned if requested file is not found")},

{"keep_alive_max",0,0x2000,(uint *)&maxKeepAlive, CXS(S1t2T_5660775, "Limit on the number of idle keep-alive connections waiting")},
{"keep_alive_timeout",10,3600*8,(uint *)&TimeoutKeepAlive, CXS(S1t2T_8226615, "Timeout in seconds for idle keep-alive connection")},
{"keep_alive_idle",0,3600*2,(uint *)&keepalive_idle, CXS(S1t2T_6286042, "Check live in seconds for idle keep-alive connection. 0 - use system default."
  " (Supported from Linux 2.4, from Windows 10 v1709)")},


#if defined(CD_VER) || !defined(SYSUNIX)
{"cgi_ident",254, 0,(uint *)&cgi_detect, CXS(S2sCGI_IDENT, "CGI ident. The part of a URL that indicates a CGI script"
" Default is \"\\cgi-bin\\\", but you could use \"\\cgi-\","
" \"\\local-bin\\\", \".cgi\", etc.")},
{"perl",256,0,(uint *)&perl, CXS(S2sPERL__IF_, "Perl. Specify location of \"perl.exe\" or \"perlis.dll\"")},
#endif
{"fcgi_ident",254, 0,(uint *)&fcgi_detect, CXS(S1t2T_3708837, "FastCGI ident. The part of a URL that indicates a FastCGI script. Default is \".fcgi\"")},
#ifdef SYSUNIX
{"fcgi_gid",0,0x100000,(uint *)&fcgi_group, CXS(S1t2T_3647783, "Use this group id, to detect FastCGI. Direct 0 to disable using group id.")},
{"fcgi_unix",3 , FL3_FCGI_SI,(uint *)0, CXS(S1t2T_3024149, "Use UNIX socket for FastCGI. Otherwise used localhost TCP socket" )},
{"fcgi_upath",255,0,(uint *)&fcgi_upath, CXS(S1t2T_3572758, "Directory to create FastCGI UNIX sockets. May be /tmp, /var/tmp, /dev/shm, ...")},
#endif

{"php",256,0,(uint *)&phtml_dir, CXS(S2sPHP__IF_Y, "PHP. Specify location of \"php-cgi.exe\" or \"phpisapi.dll\"")},
{"fcgi_php",3, FL3_FCGI_PHP,(uint *)0, CXS(S1t2T_2893951, "Run PHP as FastCGI." )},

#ifdef CD_VER
{"phpini",256,0,(uint *)&phtml_ini, CXS(S2sPHP_INI_D, "php.ini directory.")},
#endif
#if defined(CD_VER) || !defined(SYSUNIX)
{"nowintypes",0,3,(uint *)0, CXS(S2sDISABLE_T0, "Do not detect CGI application with Windows types.")},
#endif
{"ssihtm",0,FL_SSIHTM, (uint *)0, CXS(S2sENABLE_TO0, "Enable Server Side Includes (SSI) checking"
" in HTML files. By default SSI checking in .sht*,.sml*,.asp*"
" files only.  Warning: SSI processing uses more memory,"
" and and creates a small delay")},
{"noshare",1,0,(uint *)0, CXS(S2sDISABLE_S0, "Disable share dir.")},
{"share",255,0,(uint *)&doc_dir, CXS(S2sSHARE_DIR, "Share dir. Specify location for CGI"
" current dir. By default CGI current dir will be the CGI script dir.")},
{"post_limit",0x800,0x2000000,(uint *)&post_limit, CXS(S2sTHE_LIMIT, "Limit bytes received by POST method to."
" Note: Large value may use excessive PC and network resources.")},
{"nooutdir"  ,0,FL_NODIR,(uint *)0, CXS(S2sDON_T_SHO, "Do not show directory listing")},
#if  defined(CD_VER) || !defined(SYSUNIX)
{"norunhtm"  ,0,FL_NORUNHTM,(uint *)0, CXS(S2sNEVER_RUN, "Never execute .htm,.gif,.jpg files.")},

#endif
{"cgi_timeout",30,0x100000,(uint *)&cgi_timeout, CXS(S2sLIMIT_OF_, "Limit of time for script execution. (in seconds)")},
{"nbrkcgi"  ,1,FL1_NBRK,(uint *)0, CXS(S1t2T_2132379, "Don't break CGI, when connection closed"  )},

{"header",2048,0,(uint *)&header, CXS(S2sADVANCED_, "Advanced code for control header.")},
#ifndef SYSUNIX
{"dntshowhiden",1,FL1_DNTSHOWHIDEN,(uint *)0, CXS(S1t2T_4811739, "Don't retry hidden files"  )},
{"runsystem"  ,1,FL1_RUNSYSTEM,(uint *)0, CXS(S1t2T_3611249, "Run 'system' files"  )},
#endif

{"ssi_chunk",2,FL2_CHUNKED,(uint *)0, CXS(S1t2T_3048175, "Use 'chunked' transfer for SSI and CGI." )},

{"nomsd" ,1,FL1_NOM, (uint *)0, CXS(S1t2T_1674985, "Disable multi stream download for one file.")},
{"http_gzip" ,1,FL1_GZ, (uint *)0, CXS(S1t2T_2371793, "Use gzip packing, if posible.")},
{"gz_lib",256,0,(uint *)& zlib,   CXS(S1t2T_1758218, "DLL library ZLib.")},
{"gz_low"  ,0,0x1000000,(uint *)&GZSizeLow, CXS(S1t2T_3165815, "Pack if size of file great then")},
{"nogz_ext"  ,1024,0,(uint *)&NoGZSuf, CXS(S1t2T_2726483, "Don't pack files with next sufixes")},

{"ip_base",256,0,(uint *)& ipbsfile,  CXS(S1t2T_2488633, "IP database file for countries features.")},
{"ip_cntr"  ,1,FL1_IPCNTR,(uint *)0, CXS(S1t2T_2443331, "Add REMOTE_COUNTRY variable to CGI/SSI enviroment.")},
{"ip2cntr_srv"  ,2,FL2_IPCONTRY,(uint *)0, CXS(S1t2T_3645830, "Enable return country info for '/$_ip2country_$?ip=x.x.x.x' request")},
{"ip2cntr_aut"  ,2,FL2_IPCNTRAUT,(uint *)0, CXS(S1t2T_3765166, "$_ip2country_$ service for authorized users only")},
{"http_doh"  ,2,FL2_DOH,(uint *)0, CXS(S1t2T_2243001, "Enable DNS over HTTP(S).")},
{"noerrout"  ,2,FL2_NOERROUT,(uint *)0, CXS(S1t2T_3104751, "Don't out error stream (STDERR) from CGI scripts to remote users")},
#ifdef SEPLOG
{"dupstderr",2,FL2_DUBSTDERR,(uint *)0, CXS(S1t2T_3457564, "Dublicate CGI stderr to http.err log")},
#endif


XLIMIT(http,"HTTP",7),

#if V_FULL
{0,0,0,0, CXS(S2sDNS_SERVE, "DNS server setting")},
{"nohosts",2,0,(uint *)0, CXS(S2sDISABLE_D, "Disable DNS server.")},
{"hosts",256,0,(uint *)&dns_file, CXS(S2sHOSTS_FIL, "Hosts file. File with hosts names and IP"
" addresses for DNS server.")},
{"noreqursion",2,0,(uint *)0, CXS(S2sDISABLE_R, "Disable recursion.")},
{"dnscache",0,0x4000,(uint *)&dns_cach_size, CXS(S1t2T_4067961, "Size of DNS cache (in records)." )},
{"dnstimeout",100,2000,(uint *)&DnsTms, CXS(S1t2T_3305957, "Timeout, before resend request again. In milliseconds" )},
OIPV6(dns,SDNS_IND)

{"dnsupl",0,FL_DNSUPLVL,(uint *)0, CXS(S2sRECURSION, "Recursion calls allways begin from two first defined root servers. (If you use DNS servers of your provider instead real root DNS)")},
{"dnstcp",0,FL_DNSTCP ,(uint *)0, CXS(S2sENABLE_DN, "Enable DNS over TCP.")},
                        //{"dnsaptr",1,FL1_AUTOPTR ,(uint *)0, CS("Auto make PTR from A record"  )},

RANGES(dns,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))

{"nodnscachefile",2,0,(uint *)0, CXS(S2sDON_T_SAV, "Don't save DNS cache on exit.")},
{"dnscachefile",256,0,(uint *)&dnscachefile, CXS(S2sDNS_CACHE, "DNS cache file name.")},
{"dnsno6",2,FL2_DNSNO6 ,(uint *)0, CXS(S1t2T_2110311, "Don't try to recursive find AAAA records. (for networks that don't use Internet through IPv6)" )},

{"nodns_bld",1,0,(uint *)0, CXS(S1t2T_1641062, "Disable build in DNSBL server" )},
{"dns_bld",63,0,(uint *)&dnsblname, CXS(S1t2T_2559746, "Host name of build in DNSBL server" )},
{"dns_detect_dos"  ,0,0x1000000,(uint *)&dns_dos_limit, CXS(S1t2T_6448516, "Detect DoS request. Number of DoS-like requests to block IP (0 - disable)")},
{"dns_dos_hosts",299,0,(uint *)&DNS_DoS_hosts, CXS(S1t2T_5042572, "A space-separated list of bad hostnames. DoS detection names" )},

/*
#ifndef SYSUNIX
{0,0,0,0, CXS(S2sTO_REMOTE, "To remote redirect server. Current IP notification")},
{"noddns_time",0,0,(uint *)0, CXS(S2sDISABLE_C, "Disable current IP notification.")},
{"ddns_time"  ,0,0x1fFFFF,(uint *)&time_update, CXS(S2sTIME_TO_R, "Interval to update your IP on redirect server. (in secconds)")},
{"ddns",258,0,(uint *)&dns_user, CXS(S2sURL_ON_RE, "URL on redirect server. Full URL to update"
" your IP address on dynamic DNS server. \"$IP_ADDRESS\" to insert"
" your real IP address into URL.")},
#endif
*/

{0,0,0,0, CXS(S2sPROXY_SER, "Proxy server setting")},
{"noproxy_max",0,0,(uint *)0, CXS(S2sDISABLE_P, "Disable Proxy server.")},
{"proxy_max",0,1024,(uint *)&max_prx, CXS(S2sHOW_MANY_0, "Number of proxy requests working simultaneous.")},
{"proxy" ,1,0xFFFF,(uint *)&proxy_port, CXS(S2sTCP_IP_PO0, "TCP/IP port for proxy server.")},

OIPV6(proxy,1)

#ifdef MEMPRX
{"cache" ,0,0x3FFF, (uint *)&cash_max_kb, CXS(S2sMEMORY_CA, "Memory cache size (Kb). Set zero for disk cache only")},
#endif
{"noproxy_dir",0,0,(uint *)0, CXS(S2sDISABLE_T1, "Do not save proxy cache to hard disk.")},
{"proxy_dir",255,0,(uint *)&proxy_dir, CXS(S2sCACHE_PRO, "Proxy cache directory.")},
{"proxy_time",0,0x6000,(uint *)&(proxy_chk.time), CXS(S2sFOR_HOW_M, "Number of days to keep files in cache. (Zero for keep ever)")},
{"proxy_fsize",0,0x7FFFFFFF,(uint *)&max_pfile, CXS(S1t2T_5252181, "Don't save big files. Limit (bytes)" )},
{"proxy_laccess" ,1,FL1_LACPROXY, (uint *)0, CXS(S1t2T_4346165, "Calculate days from last access. (Otherwise from the day of download)")},

{"ignocache"  ,0,FL_IGNNOCH,(uint *)0, CXS(S2sIGNORE_NO, "Ignore NO-CACHE in control headers of pages.")},
{"proxy_hrd"  ,1,FL1_PRXHRD,(uint *)0, CXS(S1t2T_3002398, "Don't cache page if request content cookies." )},
{"proxyusers" ,0,FL_PRXUSER,(uint *)0, CXS(S2sPROXY_FOR, "Proxy for authorized users only.")},

RANGES(proxy,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))

 SPD(1,proxy)
{"proxy_big" ,1,FL1_BIGPROXY, (uint *)0, CXS(S1t2T_3282821, "Large mode. Useful to hold a lot of data traffic.")},
{"proxy_sbig" ,1,FL1_BBPROXY, (uint *)0, CXS(S1t2T_3391538, "Super large mode. New mode to minimize time for search when to many files stored.")},
{"proxy_tryes",0,0x2000,(uint *)&max_cont_st, CXS(S1t2T_4869017, "Number of tries to resume download file after error" )},
{"proxy_same",0,1024,(uint *)&cnt_same, CXS(S1t2T_3460001, "Limit for simultaneous requests from the same host to the same URL. Zero for unlimited." )},

{"noupproxy",3,0,(uint *)0, CXS(S2sDON_T_USE, "Do not use higher level proxy server.")},
{"upproxy" ,164,0,(uint *)&up_proxy, CXS(S2sUP_LEVEL_, "Higher level proxy server.")},
{"upproxy_port" ,1,0xFFFF,(uint *)&up_proxy_port, CXS(S2sTCP_IP_PO1, "TCP/IP port on up level proxy server.")},
{"noup_user",3,0,(uint *)0, CXS(S2sUP_LEVEL_0, "Higher level proxy server does not require authorization.")},
{"up_user" ,128,0,(uint *)&up_user, CXS(S2sUP_LEVEL_1, "Higher level proxy user:pasword")},
{"ever_upproxy" ,1,FL1_UPPRX, (uint *)0, CXS(S1t2T_3630380, "For POP3/SMTP/FTP proxy connect through HTTPS higher level proxy.")},
{"nouphosts" ,4096,0, (uint *)&nohosts, CXS(S1t2T_3118030, "No use higher level proxy for next hosts.")},

{"bad_hosts",4096,0,(uint *)&bad_hosts, CXS(S2sDISABLED_, "Disabled hosts")},
{"proxy_timeout",30,0xFFFFF,(uint *)&PRXTimeout, CXS(S1t2T_5697828, "Proxy session timeout (in second)." )},
{"proxy_gzip" ,1,FL1_GZPRX, (uint *)0, CXS(S1t2T_3133276, "Request gziped, and self unpack if browser don't support it. (Direct where is Zlib in HTTP part of options)")},

{"noproxy_antivirus",3,0,(uint *)0, CXS(S1t2T_3436009, "Don't use antivirus")},
{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, CXS(S1t2T_6602174, "Antivirus host (127.0.0.1 for local)"
//"Antivirus command for check active content (Checked filename will be added to end)"
 )},
{"proxy_avport" ,1,0xFFFF,(uint *)&proxy_antivirus_port, CXS(S1t2T_7871815, "Antivirus port" )},
{"proxy_avhtml" ,2,FL2_PAVHTML, (uint *)0, CXS(S1t2T_3955615, "Check HTML files. (Otherwise proxy will check application only)")},
{"proxy_avall" ,2,FL2_PAVALL, (uint *)0, CXS(S1t2T_3456905, "Check all files. (Otherwise proxy will check application only)")},


 XLIMIT(proxy, "Proxy",2),


{0,0,0,0, CXS(S2sFTP_SERVE, "FTP server setting")},
{"noftp_max",0,0,(uint *)0, CXS(S2sDISABLE_F, "Disable FTP server.")},
{"ftp_max",0,1024,(uint *)&max_ftp, CXS(S2sHOW_MANY_1, "Number of simultaneous requests.")},
{"ftp_port",1,0xFFFF,(uint *)&ftp_port, CXS(S2sTCP_IP_PO2, "TCP/IP port for FTP server. Usualy it's 21")},
{"ftp_timeout",30,0xFFFFF,(uint *)&FTPTimeout, CXS(S2sLIMIT_OF_0, "User session timeout. (in second) "
" Connection will close, if user is idle for this time.")},

OIPV6(ftp,2)
RANGES(ftp,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))
SPD(2,ftp)

{"noftp_pasvp",0,0,(uint *)0, CXS(S1t2T_2064283, "Use any free system provided port for a passive data connection" )},
{"ftp_pasvp",1,0xFFFF,(uint *)&first_pass_port, CXS(S1t2T_5234218, "First FTP port for passive data connection."
" (Range of used ports will be from and including this port depending on the number of simultanious FTP connections)")},

{"ftp_oone",2,FL2_NOMFTP, (uint *)0, CXS(S1t2T_2686706, "Disable multi stream for one IP" )},

{"ftp_always_pass",3,FL3_FTP_ALWPASS, (uint *)0, CXS(S1t2T_5561885, "Always ask for a password, even for users without a password" )},


{"ftp_wospace",0,FL_FTWOSPACE, (uint *)0, CXS(S2sCONVERT_N, "Convert names with space.")},
{"noftp_upload",3,0,(uint *)0, CXS(S2sDON_T_USE0, "Don't use upload directory.")},
{"ftp_upload",128,0,(uint *)&ftp_upload, CXS(S2sNAME_OF_U, "Name of upload subdirectory. If FTP directory"
" contents this subdirectory, users with \"read only\" access can"
" still upload files here. E.g. /pub/")},
{"ftp_vdirs",0,FL_FTPVDIR, (uint *)0, CXS(S2sENABLE_VI, "Enable virtual directories for FTP.")},

{"ftp_same" ,1,FL1_FTPSAME, (uint *)0, CXS(S1t2T_2789225, "Enable FTP PORT command to the client's host only. FTP to FTP mode may not work.")},
{"ftp_proxy" ,1,FL1_FTPROXY, (uint *)0, CXS(S1t2T_3196657, "Enable FTP proxy.")},

XLIMIT(ftpi, CXS(S1t2T_297279, "FTP download"),3),
XLIMIT(ftpo, CXS(S1t2T_297855, "FTP upload"),4),

{0,0,0,0, CXS(S2sPOP__SERV, "POP3 server setting")},
{"nopop3_max",0,0,(uint *)0, CXS(S2sDISABLE_P0, "Disable POP3 server.")},
{"pop3_max"  ,0,1024   ,(uint *)&max_pop, CXS(S2sHOW_MANY_1, "Number of simultaneous requests.")},
{"pop_port" ,1,0xFFFF,(uint *)&pop_port,     CXS(S2sTCP_IP_PO3, "TCP/IP port for POP3 server. Usually it's 110")},
{"pop_timeout"  ,30,0xFFFFF,(uint *)&POPTimeout, CXS(S2sPOP__SMTP, "POP3/SMTP session timeout. (in second)."
" Connection will close, if user is idle for this time.")},

RANGES(pop,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))
OIPV6(pop,4)
SPD(4,pop)

{"pop3_proxy",1,FL1_POPROXY,(uint *)0, CXS(S1t2T_3304985, "Enable POP3 proxy" )},

{"wmail",2,FL2_WMAIL,(uint *)0, CXS(S1t2T_1893843, "Enable Web mail" )},
{"nowmailsent",1,0,(uint *)0, CXS(S1t2T_2071505, "Don't save messages sent throught Web mail in user's folder" )},
{"wmailsent" ,64,0,(uint *)&loc_sent, CXS(S1t2T_2964100, "Subfolder to save sent messages" )},
{"nowmailtrash",1,0,(uint *)0, CXS(S1t2T_2262526, "Delete messages throught Web mail immediately" )},
{"wmailtrash" ,64,0,(uint *)&loc_trash, CXS(S1t2T_3387087, "Trash folder to move deleted messages" )},
{"wmail_utf",2,FL2_WMUTF, (uint *)0, CXS(S1t2T_2791726, "Convert pages to UTF-8" )},


{0,0,0,0, CXS(S2sSMTP_SERV, "SMTP server setting")},
{"nosmtp_max",0,0,(uint *)0, CXS(S2sDISABLE_S1, "Disable SMTP server.")},
{"smtp_max"  ,1,1024   ,(uint *)&max_smtp, CXS(S2sHOW_MANY_1, "Number of simultaneous requests.")},
{"smtp_name" ,128,0,(uint *)&smtp_name, CXS(S2sSMTP_SERV0, "SMTP server name. (Domain name)")},
OIPV6(smtp,3)
{"vhalias",0,FL_VHALIAS,(uint *)0, CXS(S2sUSE_ALL_V, "Use all virtual hosts as alias domain name.")},
{"smtp_dns" ,16,0,(uint *)&dns_server_for_mail, CXS(S2sDNS_SERVE0, "DNS server to get mail routing info. (May"
" be your default DNS server)")},
{"smtp_nomx",1,FL1_MHST,(uint *)0, CXS(S1t2T_2692044, "If mailhost of receptor absent, try host" )},
{"nosmtpproxy",1,0,(uint *)0, CXS(S1t2T_2158005, "It is normal SMTP relay. (Otherwise it is only SMTP proxy)" )},
{"smtpproxy" ,128,0,(uint *)&smtproxy , CXS(S1t2T_3232509, "Higher level SMTP. (SMTP proxy mode)" )},

{"smtp_port" ,1,0xFFFF,(uint *)&smtp_port,   CXS(S2sTCP_IP_PO4, "TCP/IP port for SMTP server. Usually it\'s 25")},
{"smtp_out",255,0,(uint *)&  out_path, CXS(S2sOUTPUT_PA, "Output path. Directory to store messages"
" before sending. Direct full patch.")},
{"nosmtp_sent",6,0,(uint *)0, CXS(S2sDISABLE_T2, "Do not save sent messages.")},
{"smtp_sent",255,0,(uint *)&sent_path, CXS(S2sSENT_PATH, "Sent path. Directory to store sent messages")},
{"sent_time",0,0x6000,(uint *)&(smtp_chk.time), CXS(S2sFOR_HOW_M0, "For how many days sent messages will be saved. (Zero for keep ever)")},
{"smtp_err",255,0,(uint *)&  err_path, CXS(S2sERROR_PAT, "Error path. Directory to store messages,"
" on failed send")},
{"smtp_any",0,FL_SMTPANY,(uint *)0, CXS(S2sRECEIVE_T, "Alow any \"From\" field."
" Otherwise server will send message from defined_user@domain.name only")},
RANGES(smtp, "Us IP ranges (allowed list)" /* sIP_RANGES */,sIP_RANGESD)
{"smtp_pop_ip",1,FL1_SMTP_AVT,(uint *)0, CXS(S1t2T_3845776, "Temporary add IP to allowed list after POP3 authorization" )},
{"smtp_msg_limit" ,0x1000,0x1C00000,(uint *)&max_msg_size, CXS(S2sLIMIT_OF_1, "Limit message size. (in bytes).")},
{"smtp_nobreak",0,FL_NOBRKSMTP,(uint *)0, CXS(S2sDONT_BREAK, "Don't break connection, when overflow size limit")},
{"blacklist" ,0x8000,0,(uint *)&blacklist, CXS(S2sDISABLED_0, "Blacklist of E-mail addresses of spamers."
" Separate addreses by space. Use *@host to block receiving"
" from any address of this host)")},
{"smtp_conform",1,FL1_CONFIRM,(uint *)0, CXS(S1t2T_3864377, "Enable Generate-Delivery-Report" )},
{"forward",0,FL_FORWARD,(uint *)0, CXS(S2sENABLE_TO1, "Use instructions from the \"forward\" file in a user's"
" directory.")},
{"fwdrun", 0,FL_FWDRUN ,(uint *)0, CXS(S2sENABLE_TO2, "Alow execution of applications from user's"
" \"forward\" file.")},

{"goodlist", 256,0 ,(uint *)&glst, CXS(S1t2T_2205754, "Goodlist. Common file with alowed source e-mails, IPs, hosts paterns")},
{"badlist", 256,0 ,(uint *)&blst, CXS(S1t2T_1940982, "Badlist. Common file with bad source e-mails, IPs, hosts paterns")},
{"graylist", 256,0 ,(uint *)&graylst, CXS(S1t2T_2688977, "Graylist. Common file with source e-mails, IPs, hosts paterns that required addvansed checking")},
{"chklists", 1,FL1_CHKUSR ,(uint *)0, CXS(S1t2T_2692031, "Check \"goodlist\",  \"badlist\" and  \"graylist\" files in user's home directory before receive message" )},
{"msgspam", 200,0 ,(uint *)&msg_spam, CXS(S1t2T_2571888, "Text that will be retrived in case when message declined. There you also may direct URL to Web form to direct send message" )},

{"noantivirus",1,0,(uint *)0, CXS(S1t2T_2094607, "Do not use script for incomming/outgoing mail" )},
{"antivirus" ,256,0,(uint *)&antiv , CXS(S1t2T_2616985, "Antivirus script" )},
{"run_timeout",30,0x100000,(uint *)&ttl_avr, CXS(S2sLIMIT_OF_, "Limit of time for script execution. (in seconds)")},
{"antispam" ,2048,0,(uint *)&antispam , CXS(S1t2T_2908859, "Break filter (expresion). Variables $msg,$sender,$hello,$control may be checked to stop reciving large message." )},
{"spamfltr" ,2048,0,(uint *)&spamfltr , CXS(S1t2T_2945553, "Spam filter (expresion). Variables $msg,$sender,$hello,$control may be checked to add IP to spamer's list." )},
{"nocheckback",1,FL1_NOCHKBCK,(uint *)0, CXS(S1t2T_3570899, "Accept messages with wrong return path" )},
{"fake" ,0x102,0,(uint *)&fake, CXS(S1t2T_1590071, "Fake e-mail addresses, through coma. If somebody try to send message to these addresses it will be added to spamer's list" )},
{"dnsbl" ,320,0,(uint *)&dnsbl, CXS(S1t2T_1681448, "DNSBL servers. Ask these external spamers list, about remote IP, before receive mail. (May be more then one server through space) " )},
{"checkmx",2,FL2_CHKMX,(uint *)0, CXS(S1t2T_2264384, "Check mailhost of sender (DNS MX record) before receive mail" )},
{"mxignbl",2,FL2_MX_GLIST,(uint *)0, CXS(S1t2T_2795522, "Ignore graylist if message incomme from source mailhost (DNS MX) " )},


{"spam_time" ,0,0x7FFFFFFF,(uint *)(ltime+1), CXS(S1t2T_4238571, "How long spamers IPs will active in spamer's list (in seconds)" )},

XLIMIT(smtp, "SMTP",0),

{"nolimitus",1,FL1_NOLUS,(uint *)0, CXS(S1t2T_2847040, "No limitation for alowed IPs" )},
{"uncheckip",2,FL2_MLNOIP,(uint *)0, CXS(S1t2T_2886929, "Enable receive from foregein IP messages from us domain" )},
{"time_btw"  ,2,1024   ,(uint *)&time_btw, CXS(S1t2T_2888615, "Minimal timeout betwen sending messages" )},



{0,0,0,0, CXS(S1t2T_103934, "DHCP server" )},
{"nodhcp_max",0,0,(uint *)0, CXS(S1t2T_1814608, "Disable DHCP" )},
{"dhcp_max",0,0xFEFEFE ,(uint *)&total_dhcp_ip, CXS(S1t2T_4657330, "Total IPs avilable to allocate" )},
{"dhcp_ip",16,0,(uint *)&dhcp_addr, CXS(S1t2T_2534733, "IP address of DHCP server" )},
{"dhcp_bcast",16,0,(uint *)&dhcp_bcast, CXS(S1t2T_3395438, "LAN broadcast address for DHCP reply" )},
{"dhcp_first",16,0,(uint *)&first_dhcpc, CXS(S1t2T_3689298, "First IPs for allocate" )},
{"dhcp_mask",16,0,(uint *)&netmaskc, "Netmask"},
{"dhcp_gate",16,0,(uint *)&gatewayc, "Gateway"},
{"dhcp_dns",64,0,(uint *)&dhcp_dnsc, CXS(S1t2T_2779449, "DNS servers" )},
{"dhcp_name",64,0,(uint *)&DhcpServerName, CXS(S1t2T_3881618, "Domain name" )},
{"dhcp_file",256,0,(uint *)&dhcp_fname, CXS(S1t2T_3263802, "File to save state" )},
{"dhcp_rdns",1,FL1_DHCPDNS,(uint *)0, CXS(S1t2T_2981372, "DNS should resolve hostnames for IPs that was allocated" )},
{"dhcp_lo",1,FL1_DHCPLO,(uint *)0, CXS(S1t2T_2361397, "Listen only, to store info from another servers for DNS. (never response)" )},

#ifdef TELNET


{0,0,0,0, CXS(S1t2T_1039340, "Telnet server" )},
{"notel_max",0,0,(uint *)0, CXS(S1t2T_1671671, "Disable telnet" )},
{"tel_max"  ,0,1024,(uint *)&max_tel, CXS(S2sHOW_MANY_1, "Number of simultaneous requests.")},
{"tel_port" ,1,0xFFFF,(uint *)&tel_port, CXS(S1t2T_3432320, "Telnet port" )},

RANGES(tel,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))
OIPV6(tel,6)
SPD(6,tel)
{"tel_cmd",256,0,(uint *)&tel_cmd, CXS(S1t2T_2348618, "Path to telnet shell (e.g. /bin/bash)" )},

#endif


{0,0,0,0, CXS(S2sTLS__SERV, "TLS/SSL server")},
{"notls_max",0,0,(uint *)0, CXS(S2sDISABLE_TLS, "Disable TLS/SSL server")},
{"tls_max"  ,0,1024,(uint *)&max_ssl, CXS(S2sHOW_MANY_1, "Number of simultaneous requests.")},
{"tls_port" ,1,0xFFFF,(uint *)&ssl_port,     CXS(S2sTCP_IP_TLS, "TCP/IP port for TLS/SSL server. Usually it's 443")},

RANGES(ssl,CXS(S2sIP_RANGES, "IPs that can access this server."
" Separe single IP by comma and IP ranges with hyphens."),CXS(S2sIP_RANGESD, "Deny IPs that can't access this server."
" Separe single IP by comma and IP ranges with hyphens."))
OIPV6(tls,5)
SPD(5,tls)
{"smtp_tls",1,FL1_SMTPTLS,(uint *)0, CXS(S1t2T_2938073, "Enable TLS for POP3/SMTP" )},
{"ftp_tls",2,FL2_FTPTLS,(uint *)0, CXS(S1t2T_2499029, "Enable TLS for FTP" )},


{"tls_lib"  ,256,0,(uint *)&TLSLibrary,
#ifndef  TLSWODLL
  CXS(S2sTLS__LIB, "DLL library with TLS/SSL. E.g. libsec111.dll")
#else
  0
#endif
},

{"tls_cert_file" ,256,0,(uint *)&s_cert_file, CXS(S2sTLS__SERT, "Certificate file")},
{"tls_key_file" ,256,0,(uint *)&s_key_file, CXS(S2sTLS__KEYF, "Key file")},
{"tls_capath" ,255,0,(uint *)&CApath, CXS(S2sTLS__CAPATH, "CA-Path")},
{"tls_cafile" ,256,0,(uint *)&CAfile, CXS(S2sTLS__CAFILE, "CA-file")},
{"tls_priority" ,2047,0,(uint *)&tls_priority, CXS(S1t2T_4948901, "Sets priorities for the ciphers, key exchange methods, and macs")},

{"admtls",1,FL1_ATLS,(uint *)0, CXS(S1t2T_1962848, "Remote administration through sequre HTTPS only" )},
{"tls_wmail",2,FL2_WMTLS,(uint *)0, CXS(S1t2T_2809214, "Web mail through  sequre HTTPS  only" )},


#ifdef TLSVPN

{0,0,0,0, CXS(S1t2T_1039341, "HTTP TLS VPN Server")},
{"notlsvpn",0,0,(uint *)0, CXS(S1t2T_1568467, "Disable TLS VPN" )},
{"tlsvpn_max",0,1024,(uint *)&vpn_max, CXS(S1t2T_3299313, "Maximum number of TLS VPN connections working simultaneous.")},

{"vpn_url" ,128,0,(uint *)&vpn_name, CXS(S1t2T_2629668, "TLS VPN URL name (direct only local part of URL e.g. \"/$_vpn_$\"). HTTPS requests to this URL will be redirected to VPN ")},

{"vpntun",3, FL3_VPN_TUN, (uint *)0, CXS(S1t2T_2535159, "Enable TLS VPN on Tun device" )},
{"vpntap",3, FL3_VPN_TAP, (uint *)0, CXS(S1t2T_2471473, "Enable TLS VPN on Tap device" )},
{"vpn_tun_number",0,1024,(uint *)&tuntap_number[0], CXS(S1t2T_6497414, "Tun device number")},
{"vpn_tap_number",0,1024,(uint *)&tuntap_number[1], CXS(S1t2T_6425577, "Tap device number")},
#ifndef VPN_WIN
{"vpn_tun_mtu", MIN_MTU, MAX_MTU,(uint *)&vpn_mtu[0], CXS(S1t2T_6533180, "TLS VPN MTU for tun.")},
{"vpn_tap_mtu", MIN_MTU, MAX_MTU,(uint *)&vpn_mtu[1], CXS(S1t2T_6461343, "TLS VPN MTU for tap.")},
{"tundev" ,128,0,(uint *)&tundev, CXS(S1t2T_2095850, "Tun device pathname")},
#else
{"vpn_tunif" ,128,0,(uint *)&vpnIfNames[0], CXS(S1t2T_4008588, "Tun interface name")},
{"vpn_tapif" ,128,0,(uint *)&vpnIfNames[1], CXS(S1t2T_3952141, "Tap interface name")},
{"tundev" ,128,0,(uint *)&tundev, CXS(S1t2T_20958500, "Tun driver Id (tap0901 for TAP-Windows Adaptor 9.24)")},
#endif
{"vpnpub",3, FL3_VPN_PUBLIC, (uint *)0, CXS(S1t2T_2917144, "Public access without password. (Otherwise only users with Proxy access can use this service) " )},


{"tun_ip" ,20,0,(uint *)&tuntap_ipv4[0], CXS(S1t2T_3296505, "Set Tun interface IP address")},
{"tun_nmask" ,20,0,(uint *)&tuntap_ipv4nmask[0], CXS(S1t2T_5261510, "Set Tun interface netmask")},

{"tap_ip" ,20,0,(uint *)&tuntap_ipv4[1], CXS(S1t2T_3238348, "Set Tap interface IP address")},
{"tap_nmask" ,20,0,(uint *)&tuntap_ipv4nmask[1], CXS(S1t2T_5189673, "Set Tap interface netmask")},

{"tun_script_up" ,255,0,(uint *)&vpn_scripts_up[0], CXS(S1t2T_6317118, "Run init script for Tun device")},
{"tap_script_up" ,255,0,(uint *)&vpn_scripts_up[1], CXS(S1t2T_6240151, "Run init script for Tap device")},

#ifdef VPN_WIN
{"vpn_script_keep",3, FL3_VPN_SCRKEEP, (uint *)0, CXS(S1t2T_5577742, "Keep open console window after run script for debug" )},
#endif

{"tun_remote_ip", 20, 0,(uint *)&vpn_first_remote_ipc[0], CXS(S1t2T_7776209, "First IP address to allocate for remote client that connected to Tun. (Optional)")},
{"tun_remote_max", 0, 1024,(uint *)&vpn_total_remote_ip[0], CXS(S1t2T_8186840, "Total IP addresses to allocate for remote client that connected to Tun. (Optional. Set to 0 to use external DHCP server, or another methods)")},
{"tun_remote_dns", 0, 1024,(uint *)&vpn_dns[0], CXS(S1t2T_4946453, "DNS servers that will be offered to the TUN client.")},

{"tap_remote_ip" ,20, 0,(uint *)&vpn_first_remote_ipc[1], CXS(S1t2T_7690692, "First IP address to allocate for remote client that connected to Tap. (Optional)")},
{"tap_remote_max", 0, 1024,(uint *)&vpn_total_remote_ip[1], CXS(S1t2T_8097903, "Total IP addresses to allocate for remote client that connected to Tap. (Optional. Set to 0 to use external DHCP server, or another methods)")},
{"tap_remote_dns", 0, 1024,(uint *)&vpn_dns[0], CXS(S1t2T_4877845, "DNS servers that will be offered to the TAP client. (Optional) ")},


{0,0,0,0, CXS(S1t2T_1039342, "HTTP TLS VPN Client")},

{"vpnclient",3,FL3_VPN_CLIENT,(uint *)0, CXS(S1t2T_3647717, "Enable to connect to TLS VPN remote host" )},
{"vpn_remote_host",512, 0,(uint *)&vpn_remote_host, CXS(S1t2T_6407430, "Host to connect to remote TLS VPN server")},
{"vpn_client_port",1,0xFFFE,(uint *)&vpn_client_port, CXS(S1t2T_7244850, "TLS VPN remote port. (Usually 443)")},
{"vpn_client_url" ,128,0,(uint *)&vpncln_name, CXS(S1t2T_5037784, "TLS VPN URL name (direct only local part of URL e.g. \"/$_vpn_$\"). Must be the same as directed on the remote server")},

{"vpn_remote_user",32, 0,(uint *)&vpn_user, CXS(S1t2T_4550693, "TLS VPN User name")},
{"vpn_remote_passw",32, 0,(uint *)&vpn_passw, CXS(S1t2T_5067174, "TLS VPN Password")},

{"vpncln_tap",3, FL3_TAP_CLIENT, (uint *)0, CXS(S1t2T_3867597, "VPN client to Tap. (Otherwise Tun)" )},

{"vpn_tuntap_number",0,1024,(uint *)&tuntap_number[2], CXS(S1t2T_7581972, "TLS VPN client Tun/Tap device number")},
#ifndef VPN_WIN
{"vpn_client_mtu", MIN_MTU, MAX_MTU,(uint *)&vpn_mtu[2], CXS(S1t2T_7490219, "TLS VPN MTU for client.")},
#else
{"vpn_tapif" ,128,0,(uint *)&vpnIfNames[2], CXS(S1t2T_3952141, "Tap interface name")},
#endif

{"tuntap_ip" ,32,0,(uint *)&tuntap_ipv4[2], CXS(S1t2T_4130456, "Set client VPN interface IP address")},
{"tuntap_nmask" ,32,0,(uint *)&tuntap_ipv4nmask[2], CXS(S1t2T_6268646, "Set  client VPN interface netmask")},
{"vpncln_script_up" ,255,0,(uint *)&vpn_scripts_up[2], CXS(S1t2T_7330605, "Run init script when VPN connection estabilished")},
{"vpncln_script_down" ,255,0,(uint *)&vpn_scripts_down[2], CXS(S1t2T_8671725, "Run deinit script when VPN connection closed")},

{"vpncln_chktls",3, FL3_VPN_CHKTLS    , (uint *)0, CXS(S1t2T_4758472, "Validate remote TLS sertificate, check host name" )},
{"vpncln_tlsigntime",3, FL3_VPN_TLSIGNTIME, (uint *)0, CXS(S1t2T_7019198, "Don't check remote sertificate time. Ignore expired. (GNUTLS only)" )},
{"vpncln_tlsssign",3, FL3_VPN_TLSSSIGN  , (uint *)0, CXS(S1t2T_5926682, "Accept self signed sertificate. (GNUTLS only)" )},
{"vpncln_tlssshstyle",3, FL3_VPN_TLSSHSTYLE  , (uint *)0, CXS(S1t2T_7549822, "SSH style of sertificate validate. (GNUTLS only. Public keys of new untracted remote will be stored in ~/.gnutls/known_hosts)" )},



#endif // TLSVPN

#endif //V_FULL
#ifndef FREEVER

{"registr_user",128,0,(uint *)&user_name,0},
{"registr_code",0,0xFFFFffff,(uint *)&chk_code3a,0},
{"registr_code1",0,0xFFFFffff,(uint *)&chk_code3b,0},
#endif

{0,0,0,(uint *)0,0}
};
