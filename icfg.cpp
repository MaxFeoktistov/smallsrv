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

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, CS("Bind to all addapters"  )},\
{#n "_bind",MAX_ADAPT*15,0,(uint *)& (bind_a[f]), CS("IPs and IPv6 to bind, through coma. (0.0.0.0 - bind to all IP; ::0 bind to all IPv6)" )},\
{#n "ipv6",2,1<<f,(uint *)0, CS("Also work through IPv6" )},

// #define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), t " E.g. 192.168.0.1-192.168.0.16,127.0.0.1" )},\
// {#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), td )},\
// {#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), CS("IPv6 " t " E.g. ::1,FE80::-FEFF::" )},\
// {#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]), CS("IPv6 "  td )},

#define RANGES(a,t,td)  {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), "%s E.g. 192.168.0.1-192.168.0.16,127.0.0.1", x_##t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]),  x_##td },\
{#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), x_##"IPv6 %s E.g. ::1,FE80::-FEFF::",x_##t },\
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
{"no" #a "_ltime",0,0,(uint *)0, CS("No limitation for %s"),b },\
{ #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), CS("Time per that will calculating limits (in seconds)" )},\
{ #a "_ip_limit" ,0x400,0x40000000,(uint *) (ip_limit+c), CS("Limit per IP (Kb)" )},\
{ #a "_net_limit" ,0x400,0x40000000,(uint *) (net_limit+c), CS("Limit per network (Kb)" )}, \
{ #a "_limit" ,0x400,0x40000000,(uint *) (limit+c), CS("Total limit for server (Kb)" )}


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
{"nolimitlog",1,0,(uint *)0, CS("Don't trim log lines")},
{"limitlog",0,4096,(uint *)&trim_log_lines, CS("Limit the length of the log lines. The length of each line should not exceed this value"  )},


#endif

{"nofrom_same_host",0,0,(uint *)0, CS(sDISABLE_T )},
{"from_same_host",0,256,(uint *)&max_cln_host, CS(sUSERS_FRO  )},

#define SPD(a,b) {"no" #b  "_speed",0,0,(uint *)0, CS("Don't restrict speed of outgoing transfer" )},\
{ #b "_speed",0, 0x200000,(uint *) & (ipspeed[a]), CS("Limit for summary speed of outgoing transfer for all connections from the same IP (KBytes/minute)")},\
{ #b "_spdusr",0,0x200000,(uint *) & (ipspdusr[a]),CS( "How many another connections must have activity, to check on speed limitation" )},

RANGES(ip,sIP_RANGES,sIP_RANGESD)

{"cryptpwd",1,FL1_CRYPTPWD,(uint *)0, CS("Don't save uncrypted passwords in config file" )},
{"delpwd",1,FL1_DELPAS,(uint *)0, CS("Remove passwords from the log" )},
#ifdef WITHMD5
{"md5pwd",2,FL2_MD5PASS,(uint *)0, CS("Save passwords as MD5 Digest (RFC2069/RFC2617)" )},
{"md5realm",255,0,(uint *)&realm, CS("Realm - string for MD5 Digest (RFC2069/RFC2617)" )},
{"md5digest",2,FL2_USEMD5D,(uint *)0, CS("Using MD5 Digest for authorization if posible (RFC2069/RFC2617)" )},
{"md5paranoidal",2,FL2_PARANOIDMD5,(uint *)0, CS("Using paranoidal variant of MD5 Digest for authorization if posible (RFC2617 qop=auth)" )},

#endif

#ifndef CD_VER
{"radmin",0,FL_RADMIN,(uint *)0, CS(sENABLE_TO )},
RANGES(adm,sIP_RANGES1,sIP_RANGESD1)
{"twopoint",1,FL1_2P,(uint *)0, CS("Enable 2 point in filenames (may be dangerous)" )},

#ifndef SYSUNIX
//{"utf8",2,FL2_UTF,(uint *)0, CS("Convert filenames from UTF-8" )},
{"utf8",2,FL2_UTF,(uint *)0, CS("For the log window: Check the log for UTF-8 characters and convert the log to Unicode, if any. (take a little longer)" )},
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
{"nbrkcgi"  ,1,FL1_NBRK,(uint *)0, CS("Don't break CGI, when connection closed"  )},

{"header",2048,0,(uint *)&header, CS(sADVANCED_ )},
#ifndef SYSUNIX
{"dntshowhiden",1,FL1_DNTSHOWHIDEN,(uint *)0, CS("Don't retry hidden files"  )},
{"runsystem"  ,1,FL1_RUNSYSTEM,(uint *)0, CS("Run 'system' files"  )},
#endif

{"ssi_chunk",2,FL2_CHUNKED,(uint *)0, CS("Use 'chunked' transfer for SSI." )},

{"nomsd" ,1,FL1_NOM, (uint *)0, CS("Disable multi stream download for one file.")},
{"http_gzip" ,1,FL1_GZ, (uint *)0, CS("Use gzip packing, if posible.")},
{"gz_lib",256,0,(uint *)& zlib,   CS("DLL library ZLib.")},
{"gz_low"  ,0,0x1000000,(uint *)&GZSizeLow, CS("Pack if size of file great then")},
{"nogz_ext"  ,1024,0,(uint *)&NoGZSuf, CS("Don't pack files with next sufixes")},

{"ip_base",256,0,(uint *)& ipbsfile,  CS( "IP database file for countries features.")},
{"ip_cntr"  ,1,FL1_IPCNTR,(uint *)0, CS("Add REMOTE_COUNTRY variable to CGI/SSI enviroment.")},
{"ip2cntr_srv"  ,2,FL2_IPCONTRY,(uint *)0, CS("Enable return country info for '/$_ip2country_$?ip=x.x.x.x' request")},
{"ip2cntr_aut"  ,2,FL2_IPCNTRAUT,(uint *)0, CS("$_ip2country_$ service for authorized users only")},
{"http_doh"  ,2,FL2_DOH,(uint *)0, CS("Enable DNS over HTTP(S).")},
{"noerrout"  ,2,FL2_NOERROUT,(uint *)0, CS("Don't out error stream (STDERR) from CGI scripts to remote users")},


XLIMIT(http,"HTTP",7),

#if V_FULL
{0,0,0,0, CS(sDNS_SERVE )},
{"nohosts",2,0,(uint *)0, CS(sDISABLE_D )},
{"hosts",256,0,(uint *)&dns_file, CS(sHOSTS_FIL )},
{"noreqursion",2,0,(uint *)0, CS(sDISABLE_R )},
{"dnscache",0,0x4000,(uint *)&dns_cach_size, CS("Size of DNS cache (in records)." )},
{"dnstimeout",100,2000,(uint *)&DnsTms, CS("Timeout, before resend request again. In milliseconds" )},
OIPV6(dns,SDNS_IND)

{"dnsupl",0,FL_DNSUPLVL,(uint *)0, CS(sRECURSION )},
{"dnstcp",0,FL_DNSTCP ,(uint *)0, CS(sENABLE_DN )},
                        //{"dnsaptr",1,FL1_AUTOPTR ,(uint *)0, CS("Auto make PTR from A record"  )},

RANGES(dns,sIP_RANGES,sIP_RANGESD)

{"nodnscachefile",2,0,(uint *)0, CS(sDON_T_SAV )},
{"dnscachefile",256,0,(uint *)&dnscachefile, CS(sDNS_CACHE )},
{"dnsno6",2,FL2_DNSNO6 ,(uint *)0, CS("Don't try to recursive find AAAA records. (for networks that don't use Internet through IPv6)" )},

{"nodns_bld",1,0,(uint *)0, CS("Disable build in DNSBL server" )},
{"dns_bld",63,0,(uint *)&dnsblname, CS("Host name of build in DNSBL server" )},
{"dns_detect_dos"  ,0,0x1000000,(uint *)&dns_dos_limit, CS("Detect DoS request. Number of DoS-like requests to block IP (0 - disable)")},

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
{"proxy_fsize",0,0x7FFFFFFF,(uint *)&max_pfile, CS("Don't save big files. Limit (bytes)" )},
{"proxy_laccess" ,1,FL1_LACPROXY, (uint *)0, CS("Calculate days from last access. (Otherwise from the day of download)")},

{"ignocache"  ,0,FL_IGNNOCH,(uint *)0, CS(sIGNORE_NO )},
{"proxy_hrd"  ,1,FL1_PRXHRD,(uint *)0, CS("Don't cache page if request content cookies." )},
{"proxyusers" ,0,FL_PRXUSER,(uint *)0, CS(sPROXY_FOR )},

RANGES(proxy,sIP_RANGES,sIP_RANGESD)

 SPD(1,proxy)
{"proxy_big" ,1,FL1_BIGPROXY, (uint *)0, CS("Large mode. Useful to hold a lot of data traffic.")},
{"proxy_sbig" ,1,FL1_BBPROXY, (uint *)0, CS("Super large mode. New mode to minimize time for search when to many files stored.")},
{"proxy_tryes",0,0x2000,(uint *)&max_cont_st, CS("Number of tries to resume download file after error" )},
{"proxy_same",0,1024,(uint *)&cnt_same, CS("Limit for simultaneous requests from the same host to the same URL. Zero for unlimited." )},

{"noupproxy",3,0,(uint *)0, CS(sDON_T_USE )},
{"upproxy" ,164,0,(uint *)&up_proxy, CS(sUP_LEVEL_ )},
{"upproxy_port" ,1,0xFFFF,(uint *)&up_proxy_port, CS(sTCP_IP_PO1 )},
{"noup_user",3,0,(uint *)0, CS(sUP_LEVEL_0 )},
{"up_user" ,128,0,(uint *)&up_user, CS(sUP_LEVEL_1 )},
{"ever_upproxy" ,1,FL1_UPPRX, (uint *)0, CS("For POP3/SMTP/FTP proxy connect through HTTPS higher level proxy.")},
{"nouphosts" ,4096,0, (uint *)&nohosts, CS("No use higher level proxy for next hosts.")},

{"bad_hosts",4096,0,(uint *)&bad_hosts, CS(sDISABLED_ )},
{"proxy_timeout",30,0xFFFFF,(uint *)&PRXTimeout, CS("Proxy session timeout (in second)." )},
{"proxy_gzip" ,1,FL1_GZPRX, (uint *)0, CS("Request gziped, and self unpack if browser don't support it. (Direct where is Zlib in HTTP part of options)")},

{"noproxy_antivirus",3,0,(uint *)0, CS("Don't use antivirus")},
{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, CS("Antivirus host (127.0.0.1 for local)"
//"Antivirus command for check active content (Checked filename will be added to end)"
 )},
{"proxy_avport" ,1,0xFFFF,(uint *)&proxy_antivirus_port, CS("Antivirus port" )},
{"proxy_avhtml" ,2,FL2_PAVHTML, (uint *)0, CS("Check HTML files. (Otherwise proxy will check application only)")},
{"proxy_avall" ,2,FL2_PAVALL, (uint *)0, CS("Check all files. (Otherwise proxy will check application only)")},


 XLIMIT(proxy, "Proxy",2),

{0,0,0,0, CS(sFTP_SERVE )},
{"noftp_max",0,0,(uint *)0, CS(sDISABLE_F )},
{"ftp_max",0,1024,(uint *)&max_ftp, CS(sHOW_MANY_1 )},
{"ftp_port",1,0xFFFF,(uint *)&ftp_port, CS(sTCP_IP_PO2 )},
{"ftp_timeout",30,0xFFFFF,(uint *)&FTPTimeout, CS(sLIMIT_OF_0 )},

OIPV6(ftp,2)
RANGES(ftp,sIP_RANGES,sIP_RANGESD)
SPD(2,ftp)

{"noftp_pasvp",0,0,(uint *)0, CS("Use any free system provided port for a passive data connection" )},
{"ftp_pasvp",1,0xFFFF,(uint *)&first_pass_port, CS("First FTP port for passive data connection."
" (Range of used ports will be from and including this port depending on the number of simultanious FTP connections)")},

{"ftp_oone",2,FL2_NOMFTP, (uint *)0, CS("Disable multi stream for one IP" )},


{"ftp_wospace",0,FL_FTWOSPACE, (uint *)0, CS(sCONVERT_N )},
{"noftp_upload",3,0,(uint *)0, CS(sDON_T_USE0 )},
{"ftp_upload",128,0,(uint *)&ftp_upload, CS(sNAME_OF_U )},
{"ftp_vdirs",0,FL_FTPVDIR, (uint *)0, CS(sENABLE_VI )},

{"ftp_same" ,1,FL1_FTPSAME, (uint *)0, CS("Enable FTP PORT command to the client's host only. FTP to FTP mode may not work.")},
{"ftp_proxy" ,1,FL1_FTPROXY, (uint *)0, CS("Enable FTP proxy.")},

XLIMIT(ftpi, CS("FTP download"),3),
XLIMIT(ftpo, CS("FTP upload"),4),

{0,0,0,0, CS(sPOP__SERV )},
{"nopop3_max",0,0,(uint *)0, CS(sDISABLE_P0 )},
{"pop3_max"  ,0,1024   ,(uint *)&max_pop, CS(sHOW_MANY_1 )},
{"pop_port" ,1,0xFFFF,(uint *)&pop_port,     CS(sTCP_IP_PO3 )},
{"pop_timeout"  ,30,0xFFFFF,(uint *)&POPTimeout, CS(sPOP__SMTP )},

RANGES(pop,sIP_RANGES,sIP_RANGESD)
OIPV6(pop,4)
SPD(4,pop)

{"pop3_proxy",1,FL1_POPROXY,(uint *)0, CS("Enable POP3 proxy" )},

{"wmail",2,FL2_WMAIL,(uint *)0, CS("Enable Web mail" )},
{"nowmailsent",1,0,(uint *)0, CS("Don't save messages sent throught Web mail in user's folder" )},
{"wmailsent" ,64,0,(uint *)&loc_sent, CS("Subfolder to save sent messages" )},
{"nowmailtrash",1,0,(uint *)0, CS("Delete messages throught Web mail immediately" )},
{"wmailtrash" ,64,0,(uint *)&loc_trash, CS("Trash folder to move deleted messages" )},
{"wmail_utf",2,FL2_WMUTF, (uint *)0, CS("Convert pages to UTF-8" )},


{0,0,0,0, CS(sSMTP_SERV )},
{"nosmtp_max",0,0,(uint *)0, CS(sDISABLE_S1 )},
{"smtp_max"  ,1,1024   ,(uint *)&max_smtp, CS(sHOW_MANY_1 )},
{"smtp_name" ,128,0,(uint *)&smtp_name, CS(sSMTP_SERV0 )},
OIPV6(smtp,3)
{"vhalias",0,FL_VHALIAS,(uint *)0, CS(sUSE_ALL_V )},
{"smtp_dns" ,16,0,(uint *)&dns_server_for_mail, CS(sDNS_SERVE0 )},
{"smtp_nomx",1,FL1_MHST,(uint *)0, CS("If mailhost of receptor absent, try host" )},
{"nosmtpproxy",1,0,(uint *)0, CS("It is normal SMTP relay. (Otherwise it is only SMTP proxy)" )},
{"smtpproxy" ,128,0,(uint *)&smtproxy , CS("Higher level SMTP. (SMTP proxy mode)" )},

{"smtp_port" ,1,0xFFFF,(uint *)&smtp_port,   CS( sTCP_IP_PO4 )},
{"smtp_out",255,0,(uint *)&  out_path, CS(sOUTPUT_PA )},
{"nosmtp_sent",6,0,(uint *)0, CS(sDISABLE_T2 )},
{"smtp_sent",255,0,(uint *)&sent_path, CS(sSENT_PATH )},
{"sent_time",0,0x6000,(uint *)&(smtp_chk.time), CS(sFOR_HOW_M0 )},
{"smtp_err",255,0,(uint *)&  err_path, CS(sERROR_PAT )},
{"smtp_any",0,FL_SMTPANY,(uint *)0, CS(sRECEIVE_T )},
RANGES(smtp, "Us IP ranges (allowed list)" /* sIP_RANGES */,sIP_RANGESD)
{"smtp_pop_ip",1,FL1_SMTP_AVT,(uint *)0, CS("Temporary add IP to allowed list after POP3 authorization" )},
{"smtp_msg_limit" ,0x1000,0x1C00000,(uint *)&max_msg_size, CS(sLIMIT_OF_1 )},
{"smtp_nobreak",0,FL_NOBRKSMTP,(uint *)0, CS(sDONT_BREAK )},
{"blacklist" ,0x8000,0,(uint *)&blacklist, CS(sDISABLED_0 )},
{"smtp_conform",1,FL1_CONFIRM,(uint *)0, CS("Enable Generate-Delivery-Report" )},
{"forward",0,FL_FORWARD,(uint *)0, CS(sENABLE_TO1 )},
{"fwdrun", 0,FL_FWDRUN ,(uint *)0, CS(sENABLE_TO2 )},

{"goodlist", 256,0 ,(uint *)&glst, CS("Goodlist. Common file with alowed source e-mails, IPs, hosts paterns")},
{"badlist", 256,0 ,(uint *)&blst, CS("Badlist. Common file with bad source e-mails, IPs, hosts paterns")},
{"graylist", 256,0 ,(uint *)&graylst, CS("Graylist. Common file with source e-mails, IPs, hosts paterns that required addvansed checking")},
{"chklists", 1,FL1_CHKUSR ,(uint *)0, CS("Check \"goodlist\",  \"badlist\" and  \"graylist\" files in user's home directory before receive message" )},
{"msgspam", 200,0 ,(uint *)&msg_spam, CS("Text that will be retrived in case when message declined. There you also may direct URL to Web form to direct send message" )},

{"noantivirus",1,0,(uint *)0, CS("Do not use script for incomming/outgoing mail" )},
{"antivirus" ,256,0,(uint *)&antiv , CS("Antivirus script" )},
{"run_timeout",30,0x100000,(uint *)&ttl_avr, CS(sLIMIT_OF_ )},
{"antispam" ,2048,0,(uint *)&antispam , CS("Break filter (expresion). Variables $msg,$sender,$hello,$control may be checked to stop reciving large message." )},
{"spamfltr" ,2048,0,(uint *)&spamfltr , CS("Spam filter (expresion). Variables $msg,$sender,$hello,$control may be checked to add IP to spamer's list." )},
{"nocheckback",1,FL1_NOCHKBCK,(uint *)0, CS("Accept messages with wrong return path" )},
{"fake" ,0x102,0,(uint *)&fake, CS("Fake e-mail addresses, through coma. If somebody try to send message to these addresses it will be added to spamer's list" )},
{"dnsbl" ,320,0,(uint *)&dnsbl, CS("DNSBL servers. Ask these external spamers list, about remote IP, before receive mail. (May be more then one server through space) " )},
{"checkmx",2,FL2_CHKMX,(uint *)0, CS("Check mailhost of sender (DNS MX record) before receive mail" )},
{"mxignbl",2,FL2_MX_GLIST,(uint *)0, CS("Ignore graylist if message incomme from source mailhost (DNS MX) " )},


{"spam_time" ,0,0x7FFFFFFF,(uint *)(ltime+1), CS("How long spamers IPs will active in spamer's list (in seconds)" )},

XLIMIT(smtp, "SMTP",0),

{"nolimitus",1,FL1_NOLUS,(uint *)0, CS("No limitation for alowed IPs" )},
{"uncheckip",2,FL2_MLNOIP,(uint *)0, CS("Enable receive from foregein IP messages from us domain" )},
{"time_btw"  ,2,1024   ,(uint *)&time_btw, CS("Minimal timeout betwen sending messages" )},



{0,0,0,0, CS("DHCP server" )},
{"nodhcp_max",0,0,(uint *)0, CS("Disable DHCP" )},
{"dhcp_max",0,0xFEFEFE ,(uint *)&total_dhcp_ip, CS("Total IPs avilable to allocate" )},
{"dhcp_ip",16,0,(uint *)&dhcp_addr, CS("IP address of DHCP server" )},
{"dhcp_bcast",16,0,(uint *)&dhcp_bcast, CS("LAN broadcast address for DHCP reply" )},
{"dhcp_first",16,0,(uint *)&first_dhcpc, CS("First IPs for allocate" )},
{"dhcp_mask",16,0,(uint *)&netmaskc, "Netmask"},
{"dhcp_gate",16,0,(uint *)&gatewayc, "Gateway"},
{"dhcp_dns",64,0,(uint *)&dhcp_dnsc, CS("DNS servers" )},
{"dhcp_name",64,0,(uint *)&DhcpServerName, CS("Domain name" )},
{"dhcp_file",256,0,(uint *)&dhcp_fname, CS("File to save state" )},
{"dhcp_rdns",1,FL1_DHCPDNS,(uint *)0, CS("DNS should resolve hostnames for IPs that was allocated" )},
{"dhcp_lo",1,FL1_DHCPLO,(uint *)0, CS("Listen only, to store info from another servers for DNS. (never response)" )},

#ifdef TELNET


{0,0,0,0, CS("Telnet server" )},
{"notel_max",0,0,(uint *)0, CS("Disable telnet" )},
{"tel_max"  ,0,1024,(uint *)&max_tel, CS(sHOW_MANY_1 )},
{"tel_port" ,1,0xFFFF,(uint *)&tel_port, CS("Telnet port" )},

RANGES(tel,sIP_RANGES,sIP_RANGESD)
OIPV6(tel,6)
SPD(6,tel)
{"tel_cmd",256,0,(uint *)&tel_cmd, CS("Path to telnet shell (e.g. /bin/bash)" )},

#endif


{0,0,0,0, CS(sTLS__SERV )},
{"notls_max",0,0,(uint *)0, CS(sDISABLE_TLS )},
{"tls_max"  ,0,1024,(uint *)&max_ssl, CS(sHOW_MANY_1 )},
{"tls_port" ,1,0xFFFF,(uint *)&ssl_port,     CS(sTCP_IP_TLS )},

RANGES(ssl,sIP_RANGES,sIP_RANGESD)
OIPV6(tls,5)
SPD(5,tls)
{"smtp_tls",1,FL1_SMTPTLS,(uint *)0, CS("Enable TLS for POP3/SMTP" )},
{"ftp_tls",2,FL2_FTPTLS,(uint *)0, CS("Enable TLS for FTP" )},

{"tls_lib"  ,256,0,(uint *)&TLSLibrary, CS(sTLS__LIB )},

{"tls_cert_file" ,256,0,(uint *)&s_cert_file, CS(sTLS__SERT )},
{"tls_key_file" ,256,0,(uint *)&s_key_file, CS(sTLS__KEYF )},
{"tls_capath" ,255,0,(uint *)&CApath, CS(sTLS__CAPATH )},
{"tls_cafile" ,256,0,(uint *)&CAfile, CS(sTLS__CAFILE )},
{"tls_priority" ,2047,0,(uint *)&tls_priority, CS("Sets priorities for the ciphers, key exchange methods, and macs")},

{"admtls",1,FL1_ATLS,(uint *)0, CS("Remote administration through sequre HTTPS only" )},
{"tls_wmail",2,FL2_WMTLS,(uint *)0, CS("Web mail through  sequre HTTPS  only" )},


#endif
#ifndef FREEVER

{"registr_user",128,0,(uint *)&user_name,0},
{"registr_code",0,0xFFFFffff,(uint *)&chk_code3a,0},
{"registr_code1",0,0xFFFFffff,(uint *)&chk_code3b,0},
#endif

{0,0,0,(uint *)0,0}
};
