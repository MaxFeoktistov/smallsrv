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
#endif



#ifndef SRV_H
#include "srv.h"
#endif

#ifdef TLSVPN
#include "vpn.h"
#endif

#include "g4strcwm.h"
#include "g4strhtm.hh"

// #ifdef  G4STRING_CONST_H
//
// #define CS(a)  x_##a
// #else
// #define CS(a)  a
//
// #endif

#ifdef CD_VER
char *phtml_ini="";
#endif
static char *tmp_var;

#ifdef USE_IPV6

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, CXS(S1t2T_4362878, "Bind to all addapters"  ), 0, 0, onCfgChangeDisable},\
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

#define XLIMIT(a,b,c)  \
{"no" #a "_ltime",0,0,(uint *)0, CXS(S1t2T_1727805, "No limitation for %s"),b , 0, onCfgChangeDisable},\
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

{0,0,0,0, "TLS/SSL"},

#ifndef  TLSWODLL
{"tls_lib"  ,256,0,(uint *)&TLSLibrary, CXS(S2sTLS__LIB, "DLL library with TLS/SSL. E.g. libsec111.dll") },
#endif
{"tls_cert_file" ,256,0,(uint *)&s_cert_file, CXS(S2sTLS__SERT, "Certificate file")},
{"tls_key_file" ,256,0,(uint *)&s_key_file, CXS(S2sTLS__KEYF, "Key file")},
{"tls_capath" ,255,0,(uint *)&CApath, CXS(S2sTLS__CAPATH, "CA-Path")},
{"tls_cafile" ,256,0,(uint *)&CAfile, CXS(S2sTLS__CAFILE, "CA-file")},
{"tls_priority" ,2047,0,(uint *)&tls_priority, CXS(S1t2T_4948901, "Sets priorities for the ciphers, key exchange methods, and macs")},

#ifdef TLSVPN

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


#ifdef  VPN_UPDATE_NET

{"vpncln_setip", 3, FL3_VPNCL_FIXIP   , (uint *)0, CXS(S1T_15218830, "Set interface IP (must run from Administrator)" )},
{"vpncln_upd_route", 3, FL3_VPNCL_UPDRT , (uint *)0, CXS(S1T_28625631, "Update route table to redirect all IP4 trafic to VPN (must run from Administrator)" )},

#endif // VPN_UPDATE_NET


{"tuntap_ip" ,32,0,(uint *)&tuntap_ipv4[2], CXS(S1t2T_4130456, "Set client VPN interface IP address")},
{"tuntap_nmask" ,32,0,(uint *)&tuntap_ipv4nmask[2], CXS(S1t2T_6268646, "Set  client VPN interface netmask")},
{"vpncln_script_up" ,256,0,(uint *)&vpn_scripts_up[2], CXS(S1t2T_7330605, "Run init script when VPN connection estabilished")},
{"vpncln_script_down" ,256,0,(uint *)&vpn_scripts_down[2], CXS(S1t2T_8671725, "Run deinit script when VPN connection closed")},
#ifdef VPN_WIN
{"vpn_script_keep",3, FL3_VPN_SCRKEEP, (uint *)0, CXS(S1t2T_5577742, "Keep open console window after run script for debug" )},
#endif

{"vpncln_chktls",3, FL3_VPN_CHKTLS    , (uint *)0, CXS(S1t2T_4758472, "Validate remote TLS sertificate, check host name" )},
{"vpncln_tlsigntime",3, FL3_VPN_TLSIGNTIME, (uint *)0, CXS(S1t2T_7019198, "Don't check remote sertificate time. Ignore expired. (GNUTLS only)" )},
{"vpncln_tlsssign",3, FL3_VPN_TLSSSIGN  , (uint *)0, CXS(S1t2T_5926682, "Accept self signed sertificate. (GNUTLS only)" )},
{"vpncln_tlssshstyle",3, FL3_VPN_TLSSHSTYLE  , (uint *)0, CXS(S1t2T_7549822, "SSH style of sertificate validate. (GNUTLS only. Public keys of new untracted remote will be stored in ~/.gnutls/known_hosts)" )},

#endif // TLSVPN

#ifdef  TLSWODLL
/* Load/save TLSLibrary for compatible with common version, but not show it in web configuration */
{"tls_lib"  ,256,0,(uint *)&TLSLibrary, 0 },
#endif


{0,0,0,(uint *)0,0}
};


CfgParam ConfigParams2[]={
{0,0,0,(uint *)0,0}

};
