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


#ifndef SRV_H
#include "srv.h"
#endif

short MnuOffset[20];
int iofs;

#ifdef CD_VER
char *phtml_ini="";
#endif

#define rIP_RANGE  " ������, ��� ������� ������ ����� ��������. ���������� IP ������ ����� ������� � ��������� ������� ����� ����� �������."
#define rIP_DENY "����������� ������, ��� ������� ������ ����� �� ��������. ���������� IP ������ ����� ������� � ��������� ������� ����� ����� �������."

#ifdef USE_IPV6

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, "����������� �� ���� ���������"  },\
{#n "_bind",MAX_ADAPT*15,0,(uint *)& (bind_a[f]),"������������� ������ � ������������� IP � IPv6. ����� �������. (0.0.0.0 - ��� IP; ::0 ��� IPv6)" },\
{#n "ipv6",2,1<<f,(uint *)0, "����� �������� ����� IPv6" },

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), "IP " t " ��������: 192.168.0.1-192.168.0.16,127.0.0.1" },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), "IP " td },\
{#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), "IPv6 " t " �������� ::1,FE80::-FEFF::" },\
{#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]),"IPv6 "  td },

#else
#define OIPV6(n,f)

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), td },


#endif

#define XLIMIT(a,b,c)  \
{"no" #a "_ltime",0,0,(uint *)0,"�� ������������ " b },\
{ #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), "����� �� ������� ����� ������������ ����������� (� ��������)" },\
{ #a "_ip_limit" ,0x400,0x10000000,(uint *) (ip_limit+c), "����������� �� IP (Kb)" },\
{ #a "_net_limit" ,0x400,0x10000000,(uint *) (net_limit+c), "����������� ��  ���� (Kb)" }, \
{ #a "_limit" ,0x400,0x10000000,(uint *) (limit+c), "����� ����������� �� ���� ������ (Kb)" }



CfgParam ConfigParams[]={

{0,0,0,0,"����� ���������"},

#ifndef CD_VER
{"noicon",0,FL_NOICON,(uint *)0,"�� ���������� ������ � ��������."},
{"hide"  ,0,FL_HIDE,(uint *)0,"������������� ��� ������."},
#endif

{"detail",0,FL_FULLLOG,(uint *)0,"��������� ����� ��� POP/SMTP/FTP. ����� ����������� ������ �������� �������."},
{"nolog",1,0,(uint *)0,"�� ��������� ���."},
{"log",256,0,(uint *)&flog,"��������� ���. �������� ��� ���-�����."},
{"logday",0,FL_LOGDAY,(uint *)0,"����� ��� ���� ������ ����. ��� �����, ����� �������� ���������� �� ����."},

{"nofrom_same_host",0,0,(uint *)0,"�� ������������ ���������� ������������� ����������� � ������ �����."},
{"from_same_host",0,256,(uint *)&max_cln_host,"������������ ���������� ������������� ����������� � ������ � ���� �� �����."
 "��� ������ ����������� ����� ������������ 0."
 " ����������� ���������������� �� ��� TCP ���������� (HTTP,FTP,POP,SMTP,Proxy)" },

#ifndef CD_VER

#define SPD(a,b) {"no" #b  "_speed",0,0,(uint *)0, "�� ������������ �������� ��������" },\
{ #b "_speed",0,0x200000,(uint *) & (ipspeed[a]), "������ �������� �������� �������� ������ ��� ���� ���������� � ������ IP (K����/������)"},\
{ #b "_spdusr",0,0x2000,(uint *) & (ipspdusr[a]), "������� ������ ���������� ������ ���� ���������, ����� ���������� ��� ����������� ��������" },


RANGES(ip,rIP_RANGE,rIP_DENY)

{"cryptpwd",1,FL1_CRYPTPWD,(uint *)0, "�� ������� �� ������������ ������ � ���������������� �����." },
{"delpwd",1,FL1_DELPAS,(uint *)0, "������� ������ �� ����" },

{"radmin",0,FL_RADMIN,(uint *)0,"��������� ��������� �����������������."},

 RANGES(adm,"IP ������, � ������� �������� �����������������.","IP ������, � ������� ����������������� ���������.")
#endif


{"twopoint",1,FL1_2P,(uint *)0, "��������� ��� ����� � ������ ������ (����� ���� ������)" },

#ifndef SYSUNIX
{"utf8",2,FL2_UTF,(uint *)0, "�������������� ����� ������ �� UTF-8" },
#endif


{0,0,0,0,"��������� HTTP �������"},
{"nomax",0,0,(uint *)0,"��������� HTTP ������."},
{"max"  ,0,1024,(uint *)&max_cln,"��� ����� HTTP �������� ����� �������������� ������������. ����� 20Kb ������ ������������� ��� ������ ����."
 " ������ 12-24 ���������� ����� ��� ���������� ��� 3-8 ����������� � ������."},
{"port",1,0xFFFF,(uint *)&http_port,"TCP/IP ���� ��� HTTP. ������ 80"},

#ifndef CD_VER

RANGES(http,rIP_RANGE,rIP_DENY)
OIPV6(http,0)

SPD(0,http)
#endif

{"dir",255,0,(uint *)&def_dir,"Web ����� �� ���������."},
{"def",128,0,(uint *)&def_name,"��� ����� �� ���������. (����� ������������ ������ '*'. �������� index.* ��� ������ index-�)"},
{"error",256,0,(uint *)&error_file,"���� ������. ������ ���� � ����� ��� ������� ������������� ��� ���������� ������������ �����"},
{"cgi_ident",254, 0,(uint *)&cgi_detect,"CGI �������������. ����� URL ��� ����������� CGI. �� ��������� ��� \"\\cgi-bin\\\", �� ����� ������������, ��������, \"\\cgi-\", \"\\local-bin\\\", \".cgi\", � �.�."},
{"perl",256,0,(uint *)&perl,"Perl. ���� �� ����������� Perl, �� ������ �������, ��� ��������� perl.exe ��� perlis.dll."},
{"php",256,0,(uint *)&phtml_dir,"PHP. ���� �� �����������  PHP �� ������ ������� ��� ��������� php-cgi.exe ���  phpisapi.dll..."},

{"nowintypes",0,3,(uint *)0,"�� ������������ ���������������� ���� ��� ����������� CGI ����������."},
{"ssihtm",0,0x80, (uint *)0,"��������� ������������ Server Side Includes (SSI) � HTML ������."
 " �� ��������� ������ ��������� SSI ������ � .sht*,.sml*,.asp* ������. ���������������: ��������� SSI ������� ������ ������ � ��� ���������"},
{"noshare",1,0,(uint *)0,"�� ������������ ����������� �������."},
{"share",255,0,(uint *)&doc_dir,"����������� �������. ���� �� ������ ����� ��� ���� CGI ��� ���� � ��� �� ������� �������, ������� ��� �����. ����� ��������� �������, ����� ������� ������ �������."},
{"post_limit",0x800,0x2000000,(uint *)&post_limit,"���������� ������ ������ ����������� ������� POST � ������. �� ���������� ������� �������� ��� ��� ��� ����� ���� ��������� �������, ����������� ������ ��������� ���� ����."},
{"nooutdir",0,FL_NODIR,(uint *)0,"�� ���������� ��������. (������ � ��������������� ������)"},
{"norunhtm",0,FL_NORUNHTM,(uint *)0,"������� �� ��������� .htm,.gif,.jpg ������."},

{"cgi_timeout",30,0x100000,(uint *)&cgi_timeout,"������ ������� �� ���������� �������. � ��������."},
{"nbrkcgi"  ,1,FL1_NBRK,(uint *)0,"�� ��������� ���������� CGI, � ������ �������� ����������"  },
{"header",2048,0,(uint *)&header,"�������������� ������ ������������ ���������."},
#ifndef SYSUNIX
{"dntshowhiden"  ,1,FL1_DNTSHOWHIDEN,(uint *)0,"�� �������� ������� �����."  },
{"runsystem"  ,1,FL1_RUNSYSTEM,(uint *)0,"��������� ��������� �����."  },
#endif

#ifndef CD_VER

{"ssi_chunk",2,FL2_CHUNKED,(uint *)0, "������������ 'chunked' �������� ��� SSI." },

{"nomsd" ,1,FL1_NOM, (uint *)0, "��������� �������������� ����������."},
{"http_gzip" ,1,FL1_GZ, (uint *)0, "������������ gzip ������, ����� ��� ��������."},
{"gz_lib",256,0,(uint *)& zlib,  "DLL ���������� ZLib."},
{"gz_low"  ,0,0x1000000,(uint *)&GZSizeLow,"���������� ������ �����, ������� ����� ��������"},
{"nogz_ext"  ,1024,0,(uint *)&NoGZSuf,"�� �������� ����� �� ���������� ������������"},

{"ip_base",256,0,(uint *)& ipbsfile,  "���� ���� IP ������� ��� ������������� ����������."},
{"ip_cntr"  ,1,FL1_IPCNTR,(uint *)0,  "��������� REMOTE_COUNTRY ���������� � CGI/SSI ���������"},

XLIMIT(http,"HTTP",7),

#endif




#if V_FULL
{0,0,0,0,"��������� DNS �������"},
{"nohosts",0,0,(uint *)0,"��������� DNS ������."},
{"hosts",256,0,(uint *)&dns_file,"����-����. ���� � ������� ������ � ���������������� IP �������� ��� DNS �������."},
{"noreqursion",2,0,(uint *)0,"��������� ��������."},
{"dnscache",0,92160,(uint *)&dns_cach_size,"������ ���� ���� � �������."},
{"dnstimeout",100,2000,(uint *)&DnsTms,"�������� ����� �������� �������� �������. � ������������." },
{"dnsupl"  ,0,FL_DNSUPLVL,(uint *)0,"����������� ������ ������ �������� � ���� ������ ��� ��������. (���� �� ������������ DNS ������� ������ ���������� ������ ��������� ��� ��������)"},
//{"dnsmx"  ,0,FL_DNSMXA  ,(uint *)0,"���������� mailhost ��� host ���� MX ������ �����������."},
{"dnstcp"  ,0,FL_DNSTCP ,(uint *)0,"��������� DNS �� TCP."},
//{"dnsaptr"  ,1,FL1_AUTOPTR ,(uint *)0,"������������� ��������� PTR �� A ������"  },
RANGES(dns,rIP_RANGE,rIP_DENY)
OIPV6(dns,6)

{"nodnscachefile",2,0,(uint *)0,"�� ��������� ��� ���� ��� ������."},
{"dnscachefile",256,0,(uint *)&dnscachefile,"��� ����� ���� ����."},

{"dnsno6",2,FL2_DNSNO6 ,(uint *)0, "�� �������� ���������� ������ AAAA ������. (��� ����� �� ������������ �������� ����� IPv6)" },


{"nodns_bld",1,0,(uint *)0, "��������� ���������� DNSBL ������" },
{"dns_bld",63,0,(uint *)&dnsblname, "��� ����� ����������� DNSBL �������" },



{0,0,0,0,"��� ������� ���������� ��������������� ��������. ����������� � ������� IP ������"},
{"noddns_time",0,0,(uint *)0,"��������� ����������� � ������� IP ������."},
{"ddns_time"  ,0,0x1fFFFF,(uint *)&time_update,"�������� ���� �������������."},
{"ddns",258,0,(uint *)&dns_user,"URL �������. "
 "������ URL ��� ��������� ������ IP ������ ������� ������������ ���������������.  ����� ������������ \"$IP_ADDRESS\" ���������� ����� �������� ��� IP ����� � URL."},

{0,0,0,0,"��������� ������ �������"},
{"noproxy_max",0,0,(uint *)0,"��������� ������ ������."},
{"proxy_max"   ,0,1024,(uint *)&max_prx,"��� ����� �������� ����� �������������� ������������."},
{"proxy" ,1,0xFFFF,(uint *)&proxy_port,   "TCP/IP ���� ��� ������ �������."},
{"noproxy_dir",0,0,(uint *)0,"���������  ��� �� ������� �����."},
{"proxy_dir",255,0,(uint *)&proxy_dir,"������� ��� ���������� ����."},
{"proxy_time",0,0x6000,(uint *)&proxy_time,"������� ���� ������ ��������� ������ � ����. (���� -- ������� �����)"},
{"proxy_fsize",0,0x7FFFFFFF,(uint *)&max_pfile, "�� ��������� ������� �����. ������ � ������" },
{"proxy_laccess" ,1,FL1_LACPROXY, (uint *)0, "����������� ��� �� ���������� �������."},

{"ignocache"  ,0,FL_IGNNOCH,(uint *)0,"������������ NO-CACHE � ����������� ���������� �������."},
{"proxy_hrd"  ,1,FL1_PRXHRD,(uint *)0, "�� ���������� �������� ���� � ������� ���� ����." },
{"proxyusers" ,0,FL_PRXUSER,(uint *)0,"Proxy ������ ��� �������������� �������������."},
RANGES(proxy,rIP_RANGE,rIP_DENY)
OIPV6(proxy,1)


SPD(1,proxy)
{"proxy_big" ,1,FL1_BIGPROXY, (uint *)0, "������� �����. �������� ���������� ������� ������ ���������."},
{"proxy_sbig" ,1,FL1_BBPROXY, (uint *)0, "��� ������� �����. ����� ������� ����������� ���������, ��� ���������� ������� ������ ������ �������� ����� ������."},
{"proxy_tryes",0,0x2000,(uint *)&max_cont_st, "����� ������� �������� ���� ����� ������� ����������" },
{"proxy_same",0,1024,(uint *)&cnt_same, "����������� �� ������������� ������� � ������ ����� � ����� � ��� �� URL. ���� -- ��� �����������." },
{"noupproxy",0,0,(uint *)0,"�� ������������ ������ ������ �������� ������."},
{"upproxy" ,128,0,(uint *)&up_proxy,"������ ������ �������� ������."},
{"upproxy_port" ,1,0xFFFF,(uint *)&up_proxy_port,   "TCP/IP ���� ������ ������� �������� ������."},

{"noup_user",3,0,(uint *)0,"������ ������ �������� ������ ��� �����������."},
{"up_user" ,128,0,(uint *)&up_user,"user:pasword ��� ������ ������� �������� ������."},
{"ever_upproxy" ,1,FL1_UPPRX, (uint *)0, "��� POP3/SMTP/FTP ������, ����������� ����� HTTPS ������ �������� ������."},
{"nouphosts" ,4096,0, (uint *)&nohosts, "�� ������������ ������ �������� ������ ��� ��������� ������"},


{"bad_hosts",4096,0,(uint *)&bad_hosts, "����������� �����" },
{"proxy_timeout",30,0xFFFFF,(uint *)&PRXTimeout, "������� ������� ��� ������ (� ��������)." },
{"proxy_gzip" ,1,FL1_GZPRX, (uint *)0, "����������� ���������� gzip-��, � ������������� ���� ������� �� ������������ �����."},

//{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, "������� ������ ���������� (��� ������������ ����� ����������� � ����� �������)" },
{"noproxy_antivirus",3,0,(uint *)0,"�� ������������ ���������."},
{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, "���� ����������. (127.0.0.1 ��� ���������� PC)"
//"Antivirus command for check active content (Checked filename will be added to end)"
 },
{"proxy_avport" ,1,0xFFFF,(uint *)&proxy_antivirus_port,"���� ����������" },
{"proxy_avhtml" ,2,FL2_PAVHTML, (uint *)0, "��������� � HTML �����. (����� ���������� ����� ������ ����������)"},
{"proxy_avall" ,2,FL2_PAVALL, (uint *)0, "��������� ��� �����. (����� ���������� ����� ������ ����������)"},






 XLIMIT(proxy,"Proxy",2),

{0,0,0,0,"��������� FTP �������"},
{"noftp_max",0,0,(uint *)0,"��������� FTP ������."},
{"ftp_max"   ,0,1024,(uint *)&max_ftp,"��� ����� �������� ����� �������������� ������������."},
{"ftp_port"  ,1,0xFFFF,(uint *)&ftp_port,"TCP/IP ���� ��� FTP �������. ������ 21"},
{"ftp_timeout",30,0xFFFFF,(uint *)&FTPTimeout,"������� �� ����������� � ��������. �� ���������� ����� �������, ����� ��������� ��������, ���������� ����� �������."},

RANGES(ftp,rIP_RANGE,rIP_DENY)
OIPV6(ftp,2)
SPD(2,ftp)

{"noftp_pasvp",0,0,(uint *)0, "��� ���������� ������ � �������� ������ ������������ ����� ��������� ����." },
{"ftp_pasvp",1,0xFFFF,(uint *)&first_pass_port, "������ ���� ��� ���������� ������ � �������� ������. ( �������� ������������ ������ ����� �� ����� ����� �� ����� + ����� ������������� FTP ����������)" },

{"ftp_oone",2,FL2_NOMFTP, (uint *)0, "��������� ��������� ������� ��� ������ IP" },


{"ftp_wospace"  ,0,FL_FTWOSPACE, (uint *)0,"��������������� ����� � ���������."},
{"noftp_upload",3,0,(uint *)0,"�� ������������ ������� ��� ��������."},
{"ftp_upload" ,128,0,(uint *)&ftp_upload,"��� ��� ��������. ���� FTP ������� �������� ����� ���������� �� ������������ � �������� ������ ��� ������ ����� ��������� ���� �����. ������: /pub/"},
{"ftp_vdirs" ,0,FL_FTPVDIR, (uint *)0,"��������� ����������� ���������� ��� FTP."},

{"ftp_same"  ,1,FL1_FTPSAME, (uint *)0, "��������� FTP PORT ������� ������ �� ���������� ����. ��� ���� ����� FTP �� FTP ����� �� ��������."},
{"ftp_proxy" ,1,FL1_FTPROXY, (uint *)0, "��������� FTP proxy."},

 XLIMIT(ftpi,"FTP download",3),
 XLIMIT(ftpo,"FTP upload",4),



{0,0,0,0,"��������� POP3 �������"},
{"nopop3_max",0,0,(uint *)0,"��������� POP3 ������."},
{"pop3_max" ,0,1024,(uint *)&max_pop,"��� ����� �������� ����� �������������� ������������."},
{"pop_port" ,1,0xFFFF,(uint *)&pop_port,   "TCP/IP ���� ��� POP3 �������. ������ 110"},
{"pop_timeout"  ,30,0xFFFFF,(uint *)&POPTimeout,"POP3/SMTP ������� �� ����������� � ��������. �� ���������� ����� �������, ����� ��������� ��������, ���������� ����� �������."},

RANGES(pop,rIP_RANGE,rIP_DENY)
OIPV6(pop,4)


{"pop3_proxy",1,FL1_POPROXY,(uint *)0, "��������� POP3 proxy" },


{"wmail",2,FL2_WMAIL,(uint *)0, "��������� Web mail" },
{"nowmailsent",1,0,(uint *)0, "�� ��������� ��������� ������������ ����� Web mail � ����� ������������" },
{"wmailsent" ,64,0,(uint *)&loc_sent, "���������� ��� ���������� ������������ ���������" },
{"nowmailtrash",1,0,(uint *)0, "����� Web mail ������� ��������� ����������" },
{"wmailtrash" ,64,0,(uint *)&loc_trash, "�������� ������� ��� ����������� ��������� ����������" },
{"wmail_utf",2,FL2_WMUTF, (uint *)0, "�������������� �������� � UTF8" },



SPD(4,pop)

 {0,0,0,0,"��������� SMTP �������"},
{"nosmtp_max",0,0,(uint *)0,"��������� SMTP ������."},
{"smtp_max"  ,1,1024   ,(uint *)&max_smtp,"��� ����� �������� ����� �������������� ������������."},
{"smtp_name" ,128,0,(uint *)&smtp_name,"��� SMTP �������. (��� ������)"},
{"vhalias",0,FL_VHALIAS,(uint *)0,"������������ ��� ����������� ����� ��� �������� ����� ������."},
{"smtp_dns" ,128,0,(uint *)&dns_server_for_mail,"DNS ������ ����� ������� ����� �������� ���������� � �������� ����. (��� ����� ���� ��� ������� DNS ������)"},
{"smtp_nomx",1,FL1_MHST,(uint *)0, "��� ���������� ��������� �����, ����������� ����" },
{"nosmtpproxy",1,0,(uint *)0, "��� ���������� SMTP relay. (����� ��� ����� ���� SMTP ������)" },
{"smtpproxy" ,128,0,(uint *)&smtproxy , "SMTP �������� ������. (����� SMTP ������)" },

{"smtp_port" ,1,0xFFFF,(uint *)&smtp_port,   "TCP/IP ���� ��� SMTP �������. ������ 25"},
{"smtp_out",255,0,(uint *)&  out_path,"Output �������. ������� ��� ���������� ��������� ����� ���������. ���������� ������ ����."},
{"nosmtp_sent",6,0,(uint *)0,"�� ��������� ������������ ���������."},
{"smtp_sent",255,0,(uint *)&sent_path,"Sent �������. ������� ��� ���������� ��������� ����� ��������."},
{"sent_time",1,0x6000,(uint *)&(smtp_chk.time),"������� ���� ������ ��������� ������ � ����. (���� -- ������� �����)"},

{"smtp_err",255,0,(uint *)&  err_path,"Error �������. ������� ��� ���������� ��������� ������� �� ������� ��������� ��������."},
{"smtp_any",0,FL_SMTPANY,(uint *)0,"��������� ��� �������� ��������� � ����� ����� \"From\". ����� ������ ����� �������� ��������� ������ �� ��������_������������@���.������ "},

RANGES(smtp,"���� IP ������ (����������� ������)",rIP_DENY)
OIPV6(smtp,3)

{"smtp_pop_ip",1,FL1_SMTP_AVT,(uint *)0, "�������� ��������� IP � ����������� ������ ����� ����������� �� POP3" },

{"smtp_msg_limit" ,0x1000,0x1C00000,(uint *)&max_msg_size,"������������ ������ ���������. � ������."},
{"smtp_nobreak",0,FL_NOBRKSMTP,(uint *)0,"�� ��������� ���������� � ������ ���������� ������������� �������." },

{"blacklist" ,0x8000,0,(uint *)&blacklist,"������ ������. E-mail ������ �������� ����� ������. (����� *@���.����� ����� ��������� ��������� ��������� �� ���� � ����� �����)"},
{"smtp_conform",1,FL1_CONFIRM,(uint *)0, "��������� Generate-Delivery-Report" },
{"forward",0,FL_FORWARD,(uint *)0,"��������� ������� \"forward\" ����� � �������� ������������ � ��������� ���������� �� ����."},
{"fwdrun", 0,FL_FWDRUN ,(uint *)0,"��������� ���������� ���������� �� ����������������� \"forward\" �����."},

{"goodlist", 256,0 ,(uint *)&glst, "����� ������. ����� ���� � ����������� � e-mail � IP �������� (���������) ������������"},
{"badlist", 256,0 ,(uint *)&blst, "������ ������. ����� ���� �� ����������� e-mail � IP �������� (���������) ������������"},
{"graylist", 256,0 ,(uint *)&graylst, "����� ������. ����� ���� � e-mail � IP �������� (���������) ���������� �������������� ��������"},

{"chklists",1,FL1_CHKUSR ,(uint *)0, "��������� ����� \"goodlist\", \"badlist\" � \"graylist\"  � �������� ������������ ����� ������� ��������" },
{"msgspam",200,0 ,(uint *)&msg_spam, "����� ������� ����� ������������ � ������ ������ ������ ���������. ����� ����� ����� ������� URL Web ����� ��� ������ �������� ���������" },


{"noantivirus",1,0,(uint *)0, "�� ������������ �������� ��� ��������/��������� �����" },
{"antivirus" ,256,0,(uint *)&antiv , "������������ ������." },
{"run_timeout",30,0x100000,(uint *)&ttl_avr, "������ ������� �� ���������� �������. � ��������." },
{"antispam" ,0x1000,0,(uint *)&antispam , "������ ������� (���������). ���������� $msg,$sender,$hello,$control ����� ���� ���������." },
{"spamfltr" ,2048,0,(uint *)&spamfltr , "������ ����� (���������). ���������� $msg,$sender,$hello,$control ����� ���� ��������� � IP����� ���� �������� � ������ ��������." },

{"nocheckback",1,FL1_NOCHKBCK,(uint *)0,"��������� ��������� � ������������ �������� �������" },
{"fake" ,0x102,0,(uint *)&fake, "��������� e-mail ������, ����� �������. ���� ���-�� ���������� ������� ���������� �� ��� ������, �� ����� �������� � ������ ��������" },
{"dnsbl" ,320,0,(uint *)&dnsbl, "DNSBL ������. ����������� IP � ����� �������� ���������� ������, ����� ������� ���������." },
{"checkmx",2,FL2_CHKMX,(uint *)0, "����� ������� ��������� ��������� �������� ���� �����������" },
{"mxignbl",2,FL2_MX_GLIST,(uint *)0, "������������ ����� ������ ���� ��������� ������ � ������������� ��������� ������ (DNS MX) " },

{"spam_time" ,0,0x7FFFFFFF,(uint *)(ltime+1), "��� ����� ���������� IPs ����� ������� � ���������� ������ (� ��������)" },

 XLIMIT(smtp,"SMTP",0),

{"nolimitus",1,FL1_NOLUS,(uint *)0, "�� ������������ ����������� IP" },
{"uncheckip",2,FL2_MLNOIP,(uint *)0, "��������� ��������� �� ������ ������, � ����� IP �������" },
{"time_btw"  ,2,1024   ,(uint *)&time_btw, "����������� ����� ����� ��������� ���������" },



{0,0,0,0, "DHCP ������" },
{"nodhcp_max",0,0,(uint *)0, "��������� DHCP" },
{"dhcp_max"   ,0,0xFEFEFE ,(uint *)&total_dhcp_ip, "����� ���������� ���������� IP ������" },
{"dhcp_ip",16,0,(uint *)&dhcp_addr, "IP ����� DHCP �������" },
{"dhcp_bcast",16,0,(uint *)&dhcp_bcast, "����������������� ����� ��� DHCP �������" },
{"dhcp_first" ,16,0,(uint *)&first_dhcpc, "��������� IP �����" },
{"dhcp_mask" ,16,0,(uint *)&netmaskc, "����� �������" },
{"dhcp_gate" ,16,0,(uint *)&gatewayc, "���� (Gateway)" },
{"dhcp_dns" ,64,0,(uint *)&dhcp_dnsc, "DNS �������" },
{"dhcp_name" ,64,0,(uint *)&DhcpServerName, "��� ������" },
{"dhcp_file" ,256,0,(uint *)&dhcp_fname, "���� ���������� ���������" },
{"dhcp_rdns" ,1,FL1_DHCPDNS,(uint *)0, "DNS ������ ��������� ����� ������ �� ���������� IP �������" },
{"dhcp_lo" ,1,FL1_DHCPLO,(uint *)0, "������ �������, ��������� ����� ��� NS. (�� ��������)" },


{0,0,0,0, sTLS__SERV },
{"notls_max",0,0,(uint *)0, "��������� SSL/TLS ������."},
{"tls_max"  ,0,1024 ,(uint *)&max_ssl,"��� ����� �������� ����� �������������� ������������."},
{"tls_port" ,1,0xFFFF,(uint *)&ssl_port,"TCP/IP ���� ��� SSL/TLS �������. ������ 443"},

RANGES(ssl,rIP_RANGE,rIP_DENY)
OIPV6(tls,5)


SPD(5,tls)

{"smtp_tls",1,FL1_SMTPTLS,(uint *)0,"��������� TLS ��� POP3/SMTP" },

{"tls_lib",256,0,(uint *)&TLSLibrary,  "DLL ���������� TLS/SSL. �������� seclib.dll"},
{"tls_cert_file",256,0,(uint *)&s_cert_file,"���� �����������"},
{"tls_key_file",256,0,(uint *)&s_key_file, "Key-����"},
{"tls_capath",256,0,(uint *)&CApath,"CA-file"},
{"tls_cafile",256,0,(uint *)&CAfile,"CA-Path"},
{"admtls",1,FL1_ATLS,(uint *)0, "��������� ����������������� ������ ����� ��������� HTTPS" },
{"tls_wmail",2,FL2_WMTLS,(uint *)0, "Web mail ������ ����� ��������� HTTPS" },


#endif

#ifndef FREEVER

{"registr_user",128,0,(uint *)&user_name,0},
{"registr_code",0,0xFFFFffff,(uint *)&chk_code3a,0},
{"registr_code1",0,0xFFFFffff,(uint *)&chk_code3b,0},
#endif

{0,0,0,(uint *)0,0}
};

#undef RSTNEED
#define RSTNEED "(��������� ����������)"

