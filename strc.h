/*
 * Copyright (C) 1999-2025 Maksim Feoktistov.
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


#define STRING_CONST_H

#define ABOUT_STR sSMALL_HTT FORLINUX "\r\nby Max Feoktistov\r\nE-mail: support@smallsrv.com\r\n (C) 1999-2025\r\nMD5 cashe function derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm\r\n" /* DONT_AUTO_CHANGE */

#ifdef SHS_VERSION

#define STRVERV(a) #a  /* DONT_AUTO_CHANGE */
#define STRVERVS(a) STRVERV(a)  /* DONT_AUTO_CHANGE */
#define STRVER STRVERVS(SHS_VERSION)  /* DONT_AUTO_CHANGE */

#else
#define STRVER  "3.06.35test3"   /* DONT_AUTO_CHANGE */
#endif
#define sSMALL_HTT "Small HTTP Server ver. " STRVER /* DONT_AUTO_CHANGE */

#define sUNREGISTR "Version without registration!\r\n"
#define sDON_T_SHO "Do not show directory listing"


#define sSTART_AS_ "Start as service..."
#define sRUN_AS_AP "Run as application"
#define s_FOR_UNIX " for UNIX"



#define sCAN__T_SE "Can\'t send.."
#define s_R_N____R "\r\n*** Restarting. Wait for clossing all"\
" incomming connections and terminating.\r\n"
#define sCOULD_NOT "could not get socket"
#define sCOULD_NOT0 "could not bind socket"
#define sROOT_DIR_ "HTTP: Root dir=%s def=%s "
#define s_S_PORT__ "%s port=%u Number of connections=%d\r\n"
#define sMAIN_DOMA "Main domain name=\"%s\" Folder: out=\"%s\""\
" sent=\"%s\" err=\"%s\"\r\n"
#define sDINAMIC_D "Dinamic DNS is enable. URL=\r\n"
#define sDNS_IS_EN "DNS is enable. %u hosts now are loaded.\r\n"

#define sGENERAL_S "General setting"
#define sDON_T_OUT "Don't out log to terminal."
#define sSTART_AS_0 "Start as daimon."
#define sDISABLE_I "Disable icon in notification area."
#define sMINIMIZE_ "Minimize on startup."
#define sDETAIL_LO "Create detailed log for POP/SMTP/FTP. (By default only"\
" basic events are added to the log)"
#define sDISABLE_S "Disable saving log."
#define sSTORE_LOG "Select filename and location of log."
#define sNEW_LOG_F "Create a daily log (new log created after each day). It's necessary"\
" to get statistics for a day."
#define sENABLE_TO "Enable web administration."
#define sDISABLE_T "No restrict the number of simultaneous connections from"\
" each host."
#define sUSERS_FRO "Number of simultaneous requests from each host. "\
"(Note: Restriction includes all TCP connections (HTTP,FTP,POP,SMTP,Proxy)"
#define sIP_RANGES "IPs that can access this server."\
" Separe single IP by comma and IP ranges with hyphens."
#define sIP_RANGESD "Deny IPs that can't access this server."\
" Separe single IP by comma and IP ranges with hyphens."
#define sIP_RANGES1 "IPs from that can administrate this server."\
" Separe single IP by comma and IP ranges with hyphens."
#define sIP_RANGESD1 "Deny IPs from that can't administrate this server."\
" Separe single IP by comma and IP ranges with hyphens."
#define sHTTP_SERV "HTTP server setting"
#define sDISABLE_H "Disable HTTP server."
#define sHOW_MANY_ "Number of HTTP requests working simultaneous."\
" Approcsimately 20Kb of memory is reserved for each"\
" thread. Usually 12 connections are enought for 3-8"\
" visitors per minute."
#define sTCP_IP_PO "TCP/IP port for HTTP server. Usualy it's 80"
#define sDEFAULT_W "Default web folder."
#define sDEFAULT_F "Default file name. (Wildcards are accepted, such as"\
" index.* to allow any index file in folder)"
#define sERROR_FIL "Error file. Full path to file or script that"\
" will be returned if requested file is not found"
#define sCGI_IDENT "CGI ident. The part of a URL that indicates a CGI script"\
" Default is \"\\cgi-bin\\\", but you could use \"\\cgi-\","\
" \"\\local-bin\\\", \".cgi\", etc."
#define sPERL__IF_ "Perl. Specify location of \"perl.exe\" or \"perlis.dll\""
#define sPHP__IF_Y "PHP. Specify location of \"php-cgi.exe\" or \"phpisapi.dll\""
#define sPHP_INI_D "php.ini directory."
#define sDISABLE_T0 "Do not detect CGI application with Windows types."
#define sENABLE_TO0 "Enable Server Side Includes (SSI) checking"\
" in HTML files. By default SSI checking in .sht*,.sml*,.asp*"\
" files only.  Warning: SSI processing uses more memory,"\
" and and creates a small delay"
#define sDISABLE_S0 "Disable share dir."
#define sSHARE_DIR "Share dir. Specify location for CGI"\
" current dir. By default CGI current dir will be the CGI script dir."
#define sTHE_LIMIT "Limit bytes received by POST method to."\
" Note: Large value may use excessive PC and network resources."

#define sNEVER_RUN "Never execute .htm,.gif,.jpg files."
#define sLIMIT_OF_ "Limit of time for script execution. (in seconds)"
#define sADVANCED_ "Advanced code for control header."

#define sDNS_SERVE "DNS server setting"
#define sDISABLE_D "Disable DNS server."
#define sHOSTS_FIL "Hosts file. File with hosts names and IP"\
" addresses for DNS server."
#define sDISABLE_R "Disable recursion."
#define sSIZE_OF_C "Size of cache for names. (in bytes)"
#define sRECURSION "Recursion calls allways begin from two first defined root servers. (If you use DNS servers of your provider instead real root DNS)"
#define sRETURN_MA "Return mailhost as host name, if MX record"\
" is not found."
#define sENABLE_DN "Enable DNS over TCP."
#define sDON_T_SAV "Don't save DNS cache on exit."
#define sDNS_CACHE "DNS cache file name."

#define sTO_REMOTE "To remote redirect server. Current IP notification"
#define sDISABLE_C "Disable current IP notification."
#define sTIME_TO_R "Interval to update your IP on redirect server. (in secconds)"
#define sURL_ON_RE "URL on redirect server. Full URL to update"\
" your IP address on dynamic DNS server. \"$IP_ADDRESS\" to insert"\
" your real IP address into URL."

#define sPROXY_SER "Proxy server setting"
#define sDISABLE_P "Disable Proxy server."
#define sHOW_MANY_0 "Number of proxy requests working simultaneous."
#define sTCP_IP_PO0 "TCP/IP port for proxy server."
#define sMEMORY_CA "Memory cache size (Kb). Set zero for disk cache only"
#define sDISABLE_T1 "Do not save proxy cache to hard disk."
#define sCACHE_PRO "Proxy cache directory."
#define sFOR_HOW_M "Number of days to keep files in cache. (Zero for keep ever)"
#define sIGNORE_NO "Ignore NO-CACHE in control headers of pages."
#define sPROXY_FOR "Proxy for authorized users only."
#define sDON_T_USE "Do not use higher level proxy server."
#define sUP_LEVEL_ "Higher level proxy server."
#define sTCP_IP_PO1 "TCP/IP port on up level proxy server."
#define sUP_LEVEL_0 "Higher level proxy server does not require authorization."
#define sUP_LEVEL_1 "Higher level proxy user:pasword"
#define sDISABLED_ "Disabled hosts"


#define sFTP_SERVE "FTP server setting"
#define sDISABLE_F "Disable FTP server."
#define sHOW_MANY_1 "Number of simultaneous requests."
#define sTCP_IP_PO2 "TCP/IP port for FTP server. Usualy it's 21"
#define sLIMIT_OF_0 "User session timeout. (in second) "\
" Connection will close, if user is idle for this time."
#define sCONVERT_N "Convert names with space."
#define sDON_T_USE0 "Don't use upload directory."
#define sNAME_OF_U "Name of upload subdirectory. If FTP directory"\
" contents this subdirectory, users with \"read only\" access can"\
" still upload files here. E.g. /pub/"
#define sENABLE_VI "Enable virtual directories for FTP."


#define sPOP__SERV "POP3 server setting"
#define sDISABLE_P0 "Disable POP3 server."
#define sTCP_IP_PO3 "TCP/IP port for POP3 server. Usually it's 110"
#define sPOP__SMTP "POP3/SMTP session timeout. (in second)."\
" Connection will close, if user is idle for this time."


#define sSMTP_SERV "SMTP server setting"
#define sDISABLE_S1 "Disable SMTP server."
#define sHOW_MANY_2 sHOW_MANY_1


#define sSMTP_SERV0 "SMTP server name. (Domain name)"
#define sUSE_ALL_V "Use all virtual hosts as alias domain name."
#define sDNS_SERVE0 "DNS server to get mail routing info. (May"\
" be your default DNS server)"
#define sTCP_IP_PO4 "TCP/IP port for SMTP server. Usually it\'s 25"
#define sOUTPUT_PA "Output path. Directory to store messages"\
" before sending. Direct full patch."
#define sDISABLE_T2 "Do not save sent messages."
#define sSENT_PATH "Sent path. Directory to store sent messages"
#define sFOR_HOW_M0 "For how many days sent messages will be saved. (Zero for keep ever)"
#define sERROR_PAT "Error path. Directory to store messages,"\
" on failed send"
#define sRECEIVE_T "Alow any \"From\" field."\
" Otherwise server will send message from defined_user@domain.name only"
#define sLIMIT_OF_1 "Limit message size. (in bytes)."
#define sDISABLED_0 "Blacklist of E-mail addresses of spamers."\
" Separate addreses by space. Use *@host to block receiving"\
" from any address of this host)"
#define sENABLE_TO1 "Use instructions from the \"forward\" file in a user's"\
" directory."
#define sENABLE_TO2 "Alow execution of applications from user's"\
" \"forward\" file."


#define sCANT_ACCE "Cant accept..."
#define sACCESS_DE "Access denyed"
#define sTOO_MANY_ "Too many simultaneous requests from the same host"
#define sACCESS_DE0 "Access denid"
#define sERROR__BA "Error. Bad request."
#define sBAD_USER_ "Bad user data"
#define sUNABLE_TO "Unable to open configuration"
#define sERROR__CA "Error. Can't create %s to save configuration"
#define s__UNINSPE "# Uninspected end of SSI variable (\") #"
#define s__UNINSPE0 "# Uninspected back bracket (}) #"
#define s__SSI_VAR "# SSI variable too large #"
#define sTO_MANY_I "To many includes..."
#define sERROR__FI "Error. File not found."
#define s__FILE_NO "# File not found.<p>"
#define sERROR__NO "Error. Not enought memory..."
#define sUNINSPECT "Uninspected end of SSI line... (-->)"
#define sUNINSPECT0 "Uninspected format.."
#define s___BAD_NA " # Bad name #"
#define s___ENDIF_ "# #endif not found... #"
#define s__TO_MANY "# To many vars.. #"
#define sWARNING__ "Warning, it's to open window only, to pause"\
" service push \"Stop\" button."
#define sDO_YOU_WA "Do you want to close HTTP server?"
#define sBAD_PATHN "Bad pathname. Type full pathname."
#define sCHECK___E "Check \"Enable to remote administration\" first."
#define sUSER_NOT_ "User not found."
#define s_HR__DIV_ "<hr><div align=right>[<a name=\"%s\" href=#top>Contents</a>] &nbsp; &nbsp; [<a href=\"#end_%s\">Next</a>]</div><h2><a href=\"javascript:SHTbl('t%x');\"><span id=lt%x >[-]</span></a>  %s:</h2> "\
 "<table id=t%x bgcolor=#F8FCFC border=1>"\
  "<tr bgcolor=#E0F8FF><td><b>Name:</b></td><td><b>Count</b></td><td"\
  " width=210>Percentage:</td></tr>"
// #define s_TR_BGCOL "<tr bgcolor=#E0F8FF><td><b>Name:</b></td><td><b>Count</b></td><td"\
// " width=210>Percentage:</td></tr>"

#define s_TR__TD_C "<tr><td colspan=3 bgcolor=#C0F0E0><b>Total:"\
" %u%s / (%u items)</b></td></tr></table><a name=\"end_%s\" ></a> <br>"
#define sFTP_LOGIN "FTP Login"
#define sPOP_LOGIN "POP Login"
#define sSUMMARY_B "Summary by port"
#define sHTTP_CALL "HTTP calls"
#define sSSL_CALL "SSL/TLS calls"
#define sREQUESTED "Requested hosts"
#define sHTTP__INC "HTTP. Incomming traffic (bytes)"
#define sHTTP__OUT "HTTP. Outgoing traffic (bytes)"
#define sHTTP_BY_H "HTTP by hosts. Incomming traffic (bytes)"
#define sHTTP_BY_H0 "HTTP by hosts. Outgoing traffic (bytes)"
#define sPROXY_CAL "Proxy calls"
#define sPROXY__RE "Proxy. Requested incomming traffic (bytes)"
#define sPROXY__IN "Proxy. Incomming traffic without cached (bytes)"
#define sPROXY__OU "Proxy. Outgoing traffic (bytes)"
#define sPROXY_USE "Proxy users. Requested incomming traffic (bytes)"
#define sPROXY_USE0 "Proxy users. Incomming traffic without cached"\
" (bytes)"
#define sPROXY_USE1 "Proxy users. Outgoing traffic (bytes)"
#define sPROXY_PAG "Proxy Pages"
#define sPROXY_REF "Proxy Referer"
#define sFTP_CALLS "FTP calls"
#define sFTP_DOWNL "FTP download"
#define sFTP_UPLOA "FTP upload"
#define sFTP_DOWNL1 "FTP download (bytes)"
#define sFTP_UPLOA1 "FTP upload (bytes)"
#define sPOP__CALL "POP3 calls"
#define sPOP_DATA_ "POP data traffic (bytes)"
#define sSMTP_CALL "SMTP calls"
#define sSMTP_DATA "SMTP data traffic (bytes)"
#define sSMTP_DATA0 "SMTP data from users (bytes)"
#define sSMTP_DATA1 "SMTP data for users (bytes)"
#define sDNS_CALLS "DNS calls"
#define sDNS_HOSTS "DNS hosts"
#define s_LI__A_HR "<li><a href=\"#Files\"><b>Log files for other"\
" days</b></a> (If option <i>New log file every new day</i> is selected)"\
" </ul><hr><form method=GET><input type=hidden name=n value='%u'><input type=hidden name=t value='%u'>Filter for:<input type=text name=for value='%s' size=25 maxlength=64> &nbsp; for IP:<input type=text name=fip value='%s' size=17 maxlength=18 > <input type=submit name=sb value='Apply filters'></form>  <center> "
#define s_HR__DIV_0 "<hr><div align=right><a name=\"Files\" href=#top>Contents</a></div><h2>Log"\
" files avilable:</h2><font size=2 class=f><b>"
#define s__B___FON "</b></font><hr><div align=right><a name=Connected"\
" href=#top>Contents</a></div>"
#define MSG_LD "<h2>This is avilable only if option <i>\"Create a daily log\"</i> has been selected</h2>"

#define sDONT_BREAK "Don't break connection, when overflow size limit"
#define sTLS__SERV   "TLS/SSL server"
#define sDISABLE_TLS "Disable TLS/SSL server"
#define sTCP_IP_TLS  "TCP/IP port for TLS/SSL server. Usually it's 443"
#define sTLS__LIB    "DLL library with TLS/SSL. E.g. libsec111.dll"
#define sTLS__SERT   "Certificate file"
#define sTLS__KEYF   "Key file"
#define sTLS__CAFILE "CA-file"
#define sTLS__CAPATH "CA-Path"

#define sUSER_COULD "User could have only E-Mail access or only FTP access without mail "\
"account. Also you can restrict FTP access, only read, only write, "\
"disable upload files with CGI- ident in path."\
" If you give <b><i>Admin</i></b> access and check "\
"<b><i> Enable remote administration </i></b> in configuration, "\
"then user could edit this page, and will have"\
" access to all administrations pages."

#define sVIRTUAL_HOSTS "<h2>Virtual hosts and virtual directories:</h2>"\
"Server can support many virtual hosts. Each virtual host has its own root "\
"directory, and also can include virtual directories common for all. "\
"To add virtual host direct <i>any.host.name</i> and full path to root. "\
"To add virtual directory direct <i><b>/</b>directory<b>/</b></i> "\
"(between two slash) and full path for substitute. To make this private, direct "\
"users for that resurse will be available, through coma. After user you may direct type of access: "\
"R - read, W-write, NOFTP, NOHTTP .<BR> Example:"\
" <I>admin:RW,anonymous:RNOHTTP,user1,user2:R,user3:W,user4:RWNOHTTP</I><BR>" \
"Instead of root dir you may direct link to another Web server: http://other.host In this case server will work like proxy"\
"<table bgcolor=#bcf8cc border=1><tr bgcolor=#bcf0ff>"\
"<td>Host name or <b>/</b>directory<b>/</b></td>"\
"<td>Directory</td><td>Users</td>"

#define AUTH_ERR "HTTP/1.0 401 \r\nWWW-Authenticate: Basic realm=\"Authorization request\"\r\nContent-Type: text/html\r\n\r\n<h1>Authorization request\r\n"
#define RSTNEED ". (Restart need)"

