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


#define STRING_CONST_H
#define G4STRING_CONST_H

#ifdef CD_VER
//#define ABOUT_STR sSMALL_HTT FORLINUX "\r\nby Max Feoktistov\r\nE-mail: max@mf.inc.ru\r\n (C) 1999-2004\r\n"

//"builded for Universidad Nacional de La Plata de Argentina\r\n"
//"builded for Stephan Krauss\r\n"
#else
#define ABOUT_STR sSMALL_HTT FORLINUX "\r\nby Max Feoktistov\r\nE-mail: support@smallsrv.com\r\n (C) 1999-2019\r\n" /* DONT_AUTO_CHANGE */
#endif

#ifdef CD_VER
// #define sSMALL_HTT "Small HTTP Server CD version" "\r\nby Max Feoktistov\r\nhttp://smallsrv.com\r\n (C) 1999-2010\r\n\r\n" \
// "\r\nСпециальная редакция для sitefactor.ru\r\nМожет распростронятся Кириллом Болдыревым только в составе программных средств sitefactor.ru. Перепродажа и распространение в составе других программных средств запрещена."
// #define ABOUT_STR sSMALL_HTT

//"Корпоративный сайт. Версия 1.0"

//"Small HTTP Server CD version based on ver. 3.05.01"

#else
#define STRVER  "3.05.95test1"   /* DONT_AUTO_CHANGE */
#define sSMALL_HTT "Small HTTP Server ver. " STRVER /* DONT_AUTO_CHANGE */
#endif

#ifdef FREEVER
#ifdef MAINSTR
 char S2sUNREGISTR[] = "Version without registration!\r\n";
 char *lS2sUNREGISTR = S2sUNREGISTR;
#else
 extern char *lS2sUNREGISTR;
#endif
#define sUNREGISTR lS2sUNREGISTR
#else
#ifdef MAINSTR
 char S2sUNREGISTR[] = "UNREGISTRED SOFTWARE\r\nPlease register!\r\n";
 char *lS2sUNREGISTR = S2sUNREGISTR;
#else
 extern char *lS2sUNREGISTR;
#define sUNREGISTR lS2sUNREGISTR
#endif
#ifdef MAINSTR
 char S2sFREE____D[] = "Free 21 days finished.";
 char *lS2sFREE____D = S2sFREE____D;
#else
 extern char *lS2sFREE____D;
#endif
#define sFREE____D lS2sFREE____D
#ifdef MAINSTR
 char S2sYOU_USED_[] = "You used crack... :~(.\n"
 " If all users had registered, then price might be less"
 " then $1 USD, but current fee is little to ceil all expense.\n";
 char *lS2sYOU_USED_ = S2sYOU_USED_;
#else
 extern char *lS2sYOU_USED_;
#endif
#define sYOU_USED_ lS2sYOU_USED_
#ifdef MAINSTR
 char S2sREGISTRED[] = "Registered  to %.72s";
 char *lS2sREGISTRED = S2sREGISTRED;
#else
 extern char *lS2sREGISTRED;
#endif
#define sREGISTRED lS2sREGISTRED
#ifdef MAINSTR
 char S2sBAD_CODE[] = "Bad code";
 char *lS2sBAD_CODE = S2sBAD_CODE;
#else
 extern char *lS2sBAD_CODE;
#endif
#define sBAD_CODE lS2sBAD_CODE
#ifdef MAINSTR
 char S2sTHANK_YOU[] = "Thank you for your support!\r\n%s\r\n";
 char *lS2sTHANK_YOU = S2sTHANK_YOU;
#else
 extern char *lS2sTHANK_YOU;
#endif
#define sTHANK_YOU lS2sTHANK_YOU
#ifdef MAINSTR
 char S2sTHANK_YOU0[] = "Thank you for your support!";
 char *lS2sTHANK_YOU0 = S2sTHANK_YOU0;
#else
 extern char *lS2sTHANK_YOU0;
#endif
#define sTHANK_YOU0 lS2sTHANK_YOU0
#ifdef MAINSTR
 char S2sFREE_TRIA[] = "Free trial finished. Do you want to register?";
 char *lS2sFREE_TRIA = S2sFREE_TRIA;
#else
 extern char *lS2sFREE_TRIA;
#endif
#define sFREE_TRIA lS2sFREE_TRIA
#ifdef MAINSTR
 char S2sFREE_TRIA0[] = "Free trial finished";
 char *lS2sFREE_TRIA0 = S2sFREE_TRIA0;
#else
 extern char *lS2sFREE_TRIA0;
#endif
#define sFREE_TRIA0 lS2sFREE_TRIA0

#ifndef RUS
#ifdef MAINSTR
 char S2sNOW_THE_PROGRAM[] = "Now the program working in evalution mode.\n"
 "In this mode only 3 users will be alowed, other will be ignored.\n"
 "Please register, to shut off this limitation.\n";
 char *lS2sNOW_THE_PROGRAM = S2sNOW_THE_PROGRAM;
#else
 extern char *lS2sNOW_THE_PROGRAM;
#endif
#define sNOW_THE_PROGRAM lS2sNOW_THE_PROGRAM

#else
#ifdef MAINSTR
 char S2sNOW_THE_PROGRAM[] = "Программа  работает в ознакомитнльном режиме.\n"
 "В этом режиме будут восприниматься только 3 пользователя.\n"
 "Остальные пользователи игнорируются.\n"
 "Пожалуйста зарегистрируйтесь чтобы выключить это ограничение.";
 char *lS2sNOW_THE_PROGRAM = S2sNOW_THE_PROGRAM;
#else
 extern char *lS2sNOW_THE_PROGRAM;
#endif
#define sNOW_THE_PROGRAM lS2sNOW_THE_PROGRAM

#endif
 
 
 
 
#endif  //FREEVER
#ifdef MAINSTR
 char S2sSTART_AS_[] = "Start as service...";
 char *lS2sSTART_AS_ = S2sSTART_AS_;
#else
 extern char *lS2sSTART_AS_;
#endif
#define sSTART_AS_ lS2sSTART_AS_
#ifdef MAINSTR
 char S2sRUN_AS_AP[] = "Run as application";
 char *lS2sRUN_AS_AP = S2sRUN_AS_AP;
#else
 extern char *lS2sRUN_AS_AP;
#endif
#define sRUN_AS_AP lS2sRUN_AS_AP
#ifdef MAINSTR
 char S2s_FOR_UNIX[] = " for UNIX";
 char *lS2s_FOR_UNIX = S2s_FOR_UNIX;
#else
 extern char *lS2s_FOR_UNIX;
#endif
#define s_FOR_UNIX lS2s_FOR_UNIX



#ifdef MAINSTR
 char S2sCAN__T_SE[] = "Can\'t send..";
 char *lS2sCAN__T_SE = S2sCAN__T_SE;
#else
 extern char *lS2sCAN__T_SE;
#endif
#define sCAN__T_SE lS2sCAN__T_SE
#ifdef MAINSTR
 char S2s_R_N____R[] = "\r\n*** Restarting. Wait for clossing all"
 " incomming connections and terminating.\r\n";
 char *lS2s_R_N____R = S2s_R_N____R;
#else
 extern char *lS2s_R_N____R;
#endif
#define s_R_N____R lS2s_R_N____R
#ifdef MAINSTR
 char S2sCOULD_NOT[] = "could not get socket";
 char *lS2sCOULD_NOT = S2sCOULD_NOT;
#else
 extern char *lS2sCOULD_NOT;
#endif
#define sCOULD_NOT lS2sCOULD_NOT
#ifdef MAINSTR
 char S2sCOULD_NOT0[] = "could not bind socket";
 char *lS2sCOULD_NOT0 = S2sCOULD_NOT0;
#else
 extern char *lS2sCOULD_NOT0;
#endif
#define sCOULD_NOT0 lS2sCOULD_NOT0
#ifdef MAINSTR
 char S2sROOT_DIR_[] = "HTTP: Root dir=%s def=%s ";
 char *lS2sROOT_DIR_ = S2sROOT_DIR_;
#else
 extern char *lS2sROOT_DIR_;
#endif
#define sROOT_DIR_ lS2sROOT_DIR_
#ifdef MAINSTR
 char S2s_S_PORT__[] = "%s port=%u Number of connections=%d\r\n";
 char *lS2s_S_PORT__ = S2s_S_PORT__;
#else
 extern char *lS2s_S_PORT__;
#endif
#define s_S_PORT__ lS2s_S_PORT__
#ifdef MAINSTR
 char S2sMAIN_DOMA[] = "Main domain name=\"%s\" Folder: out=\"%s\""
 " sent=\"%s\" err=\"%s\"\r\n";
 char *lS2sMAIN_DOMA = S2sMAIN_DOMA;
#else
 extern char *lS2sMAIN_DOMA;
#endif
#define sMAIN_DOMA lS2sMAIN_DOMA
#ifdef MAINSTR
 char S2sDINAMIC_D[] = "Dinamic DNS is enable. URL=\r\n";
 char *lS2sDINAMIC_D = S2sDINAMIC_D;
#else
 extern char *lS2sDINAMIC_D;
#endif
#define sDINAMIC_D lS2sDINAMIC_D
#ifdef MAINSTR
 char S2sDNS_IS_EN[] = "DNS is enable. %u hosts now are loaded.\r\n";
 char *lS2sDNS_IS_EN = S2sDNS_IS_EN;
#else
 extern char *lS2sDNS_IS_EN;
#endif
#define sDNS_IS_EN lS2sDNS_IS_EN

#ifdef MAINSTR
 char S2sGENERAL_S[] = "General setting";
 char *lS2sGENERAL_S = S2sGENERAL_S;
#else
 extern char *lS2sGENERAL_S;
#endif
#define sGENERAL_S lS2sGENERAL_S
#ifdef MAINSTR
 char S2sDON_T_OUT[] = "Don't out log to terminal.";
 char *lS2sDON_T_OUT = S2sDON_T_OUT;
#else
 extern char *lS2sDON_T_OUT;
#endif
#define sDON_T_OUT lS2sDON_T_OUT
#ifdef MAINSTR
 char S2sSTART_AS_0[] = "Start as daimon.";
 char *lS2sSTART_AS_0 = S2sSTART_AS_0;
#else
 extern char *lS2sSTART_AS_0;
#endif
#define sSTART_AS_0 lS2sSTART_AS_0
#ifdef MAINSTR
 char S2sDISABLE_I[] = "Disable icon in notification area.";
 char *lS2sDISABLE_I = S2sDISABLE_I;
#else
 extern char *lS2sDISABLE_I;
#endif
#define sDISABLE_I lS2sDISABLE_I
#ifdef MAINSTR
 char S2sMINIMIZE_[] = "Minimize on startup.";
 char *lS2sMINIMIZE_ = S2sMINIMIZE_;
#else
 extern char *lS2sMINIMIZE_;
#endif
#define sMINIMIZE_ lS2sMINIMIZE_
#ifdef MAINSTR
 char S2sDETAIL_LO[] = "Create detailed log for POP/SMTP/FTP. (By default only"
 " basic events are added to the log)";
 char *lS2sDETAIL_LO = S2sDETAIL_LO;
#else
 extern char *lS2sDETAIL_LO;
#endif
#define sDETAIL_LO lS2sDETAIL_LO
#ifdef MAINSTR
 char S2sDISABLE_S[] = "Disable saving log.";
 char *lS2sDISABLE_S = S2sDISABLE_S;
#else
 extern char *lS2sDISABLE_S;
#endif
#define sDISABLE_S lS2sDISABLE_S
#ifdef MAINSTR
 char S2sSTORE_LOG[] = "Select filename and location of log.";
 char *lS2sSTORE_LOG = S2sSTORE_LOG;
#else
 extern char *lS2sSTORE_LOG;
#endif
#define sSTORE_LOG lS2sSTORE_LOG
#ifdef MAINSTR
 char S2sNEW_LOG_F[] = "Create a daily log (new log created after each day). It's necessary"
 " to get statistics for a day.";
 char *lS2sNEW_LOG_F = S2sNEW_LOG_F;
#else
 extern char *lS2sNEW_LOG_F;
#endif
#define sNEW_LOG_F lS2sNEW_LOG_F
#ifdef MAINSTR
 char S2sENABLE_TO[] = "Enable web administration.";
 char *lS2sENABLE_TO = S2sENABLE_TO;
#else
 extern char *lS2sENABLE_TO;
#endif
#define sENABLE_TO lS2sENABLE_TO
#ifdef MAINSTR
 char S2sDISABLE_T[] = "No restrict the number of simultaneous connections from"
 " each host.";
 char *lS2sDISABLE_T = S2sDISABLE_T;
#else
 extern char *lS2sDISABLE_T;
#endif
#define sDISABLE_T lS2sDISABLE_T
#ifdef MAINSTR
 char S2sUSERS_FRO[] = "Number of simultaneous requests from each host. "
 "(Note: Restriction includes all TCP connections (HTTP,FTP,POP,SMTP,Proxy)";
 char *lS2sUSERS_FRO = S2sUSERS_FRO;
#else
 extern char *lS2sUSERS_FRO;
#endif
#define sUSERS_FRO lS2sUSERS_FRO
#ifdef MAINSTR
 char S2sIP_RANGES[] = "IPs that can access this server."
 " Separe single IP by comma and IP ranges with hyphens.";
 char *lS2sIP_RANGES = S2sIP_RANGES;
#else
 extern char *lS2sIP_RANGES;
#endif
#define sIP_RANGES lS2sIP_RANGES
#ifdef MAINSTR
 char S2sIP_RANGESD[] = "Deny IPs that can't access this server."
 " Separe single IP by comma and IP ranges with hyphens.";
 char *lS2sIP_RANGESD = S2sIP_RANGESD;
#else
 extern char *lS2sIP_RANGESD;
#endif
#define sIP_RANGESD lS2sIP_RANGESD
#ifdef MAINSTR
 char S2sIP_RANGES1[] = "IPs from that can administrate this server."
 " Separe single IP by comma and IP ranges with hyphens.";
 char *lS2sIP_RANGES1 = S2sIP_RANGES1;
#else
 extern char *lS2sIP_RANGES1;
#endif
#define sIP_RANGES1 lS2sIP_RANGES1
#ifdef MAINSTR
 char S2sIP_RANGESD1[] = "Deny IPs from that can't administrate this server."
 " Separe single IP by comma and IP ranges with hyphens.";
 char *lS2sIP_RANGESD1 = S2sIP_RANGESD1;
#else
 extern char *lS2sIP_RANGESD1;
#endif
#define sIP_RANGESD1 lS2sIP_RANGESD1
#ifdef MAINSTR
 char S2sHTTP_SERV[] = "HTTP server setting";
 char *lS2sHTTP_SERV = S2sHTTP_SERV;
#else
 extern char *lS2sHTTP_SERV;
#endif
#define sHTTP_SERV lS2sHTTP_SERV
#ifdef MAINSTR
 char S2sDISABLE_H[] = "Disable HTTP server.";
 char *lS2sDISABLE_H = S2sDISABLE_H;
#else
 extern char *lS2sDISABLE_H;
#endif
#define sDISABLE_H lS2sDISABLE_H
#ifdef MAINSTR
 char S2sHOW_MANY_[] = "Number of HTTP requests working simultaneous."
 " Approcsimately 20Kb of memory is reserved for each"
 " thread. Usually 12 connections are enought for 3-8"
 " visitors per minute.";
 char *lS2sHOW_MANY_ = S2sHOW_MANY_;
#else
 extern char *lS2sHOW_MANY_;
#endif
#define sHOW_MANY_ lS2sHOW_MANY_
#ifdef MAINSTR
 char S2sTCP_IP_PO[] = "TCP/IP port for HTTP server. Usualy it's 80";
 char *lS2sTCP_IP_PO = S2sTCP_IP_PO;
#else
 extern char *lS2sTCP_IP_PO;
#endif
#define sTCP_IP_PO lS2sTCP_IP_PO
#ifdef MAINSTR
 char S2sDEFAULT_W[] = "Default web folder.";
 char *lS2sDEFAULT_W = S2sDEFAULT_W;
#else
 extern char *lS2sDEFAULT_W;
#endif
#define sDEFAULT_W lS2sDEFAULT_W
#ifdef MAINSTR
 char S2sDEFAULT_F[] = "Default file name. (Wildcards are accepted, such as"
 " index.* to allow any index file in folder)";
 char *lS2sDEFAULT_F = S2sDEFAULT_F;
#else
 extern char *lS2sDEFAULT_F;
#endif
#define sDEFAULT_F lS2sDEFAULT_F
#ifdef MAINSTR
 char S2sERROR_FIL[] = "Error file. Full path to file or script that"
 " will be returned if requested file is not found";
 char *lS2sERROR_FIL = S2sERROR_FIL;
#else
 extern char *lS2sERROR_FIL;
#endif
#define sERROR_FIL lS2sERROR_FIL
#ifdef MAINSTR
 char S2sCGI_IDENT[] = "CGI ident. The part of a URL that indicates a CGI script"
 " Default is \"\\cgi-bin\\\", but you could use \"\\cgi-\","
 " \"\\local-bin\\\", \".cgi\", etc.";
 char *lS2sCGI_IDENT = S2sCGI_IDENT;
#else
 extern char *lS2sCGI_IDENT;
#endif
#define sCGI_IDENT lS2sCGI_IDENT
#ifdef MAINSTR
 char S2sPERL__IF_[] = "Perl. Specify location of \"perl.exe\" or \"perlis.dll\"";
 char *lS2sPERL__IF_ = S2sPERL__IF_;
#else
 extern char *lS2sPERL__IF_;
#endif
#define sPERL__IF_ lS2sPERL__IF_
#ifdef MAINSTR
 char S2sPHP__IF_Y[] = "PHP. Specify location of \"php-cgi.exe\" or \"phpisapi.dll\"";
 char *lS2sPHP__IF_Y = S2sPHP__IF_Y;
#else
 extern char *lS2sPHP__IF_Y;
#endif
#define sPHP__IF_Y lS2sPHP__IF_Y
#ifdef MAINSTR
 char S2sPHP_INI_D[] = "php.ini directory.";
 char *lS2sPHP_INI_D = S2sPHP_INI_D;
#else
 extern char *lS2sPHP_INI_D;
#endif
#define sPHP_INI_D lS2sPHP_INI_D
#ifdef MAINSTR
 char S2sDISABLE_T0[] = "Do not detect CGI application with Windows types.";
 char *lS2sDISABLE_T0 = S2sDISABLE_T0;
#else
 extern char *lS2sDISABLE_T0;
#endif
#define sDISABLE_T0 lS2sDISABLE_T0
#ifdef MAINSTR
 char S2sENABLE_TO0[] = "Enable Server Side Includes (SSI) checking"
 " in HTML files. By default SSI checking in .sht*,.sml*,.asp*"
 " files only.  Warning: SSI processing uses more memory,"
 " and and creates a small delay";
 char *lS2sENABLE_TO0 = S2sENABLE_TO0;
#else
 extern char *lS2sENABLE_TO0;
#endif
#define sENABLE_TO0 lS2sENABLE_TO0
#ifdef MAINSTR
 char S2sDISABLE_S0[] = "Disable share dir.";
 char *lS2sDISABLE_S0 = S2sDISABLE_S0;
#else
 extern char *lS2sDISABLE_S0;
#endif
#define sDISABLE_S0 lS2sDISABLE_S0
#ifdef MAINSTR
 char S2sSHARE_DIR[] = "Share dir. Specify location for CGI"
 " current dir. By default CGI current dir will be the CGI script dir.";
 char *lS2sSHARE_DIR = S2sSHARE_DIR;
#else
 extern char *lS2sSHARE_DIR;
#endif
#define sSHARE_DIR lS2sSHARE_DIR
#ifdef MAINSTR
 char S2sTHE_LIMIT[] = "Limit bytes received by POST method to."
 " Note: Large value may use excessive PC and network resources.";
 char *lS2sTHE_LIMIT = S2sTHE_LIMIT;
#else
 extern char *lS2sTHE_LIMIT;
#endif
#define sTHE_LIMIT lS2sTHE_LIMIT
#ifdef MAINSTR
 char S2sDON_T_SHO[] = "Do not show directory listing "
#ifndef FREEVER 
 "(avilable in registered"
 " version only)"
#endif 
 ;
 char *lS2sDON_T_SHO = S2sDON_T_SHO;
#else
 extern char *lS2sDON_T_SHO;
#endif
#define sDON_T_SHO lS2sDON_T_SHO
#ifdef MAINSTR
 char S2sNEVER_RUN[] = "Never execute .htm,.gif,.jpg files.";
 char *lS2sNEVER_RUN = S2sNEVER_RUN;
#else
 extern char *lS2sNEVER_RUN;
#endif
#define sNEVER_RUN lS2sNEVER_RUN
#ifdef MAINSTR
 char S2sLIMIT_OF_[] = "Limit of time for script execution. (in seconds)";
 char *lS2sLIMIT_OF_ = S2sLIMIT_OF_;
#else
 extern char *lS2sLIMIT_OF_;
#endif
#define sLIMIT_OF_ lS2sLIMIT_OF_
#ifdef MAINSTR
 char S2sADVANCED_[] = "Advanced code for control header.";
 char *lS2sADVANCED_ = S2sADVANCED_;
#else
 extern char *lS2sADVANCED_;
#endif
#define sADVANCED_ lS2sADVANCED_

#ifdef MAINSTR
 char S2sDNS_SERVE[] = "DNS server setting";
 char *lS2sDNS_SERVE = S2sDNS_SERVE;
#else
 extern char *lS2sDNS_SERVE;
#endif
#define sDNS_SERVE lS2sDNS_SERVE
#ifdef MAINSTR
 char S2sDISABLE_D[] = "Disable DNS server.";
 char *lS2sDISABLE_D = S2sDISABLE_D;
#else
 extern char *lS2sDISABLE_D;
#endif
#define sDISABLE_D lS2sDISABLE_D
#ifdef MAINSTR
 char S2sHOSTS_FIL[] = "Hosts file. File with hosts names and IP"
 " addresses for DNS server.";
 char *lS2sHOSTS_FIL = S2sHOSTS_FIL;
#else
 extern char *lS2sHOSTS_FIL;
#endif
#define sHOSTS_FIL lS2sHOSTS_FIL
#ifdef MAINSTR
 char S2sDISABLE_R[] = "Disable recursion.";
 char *lS2sDISABLE_R = S2sDISABLE_R;
#else
 extern char *lS2sDISABLE_R;
#endif
#define sDISABLE_R lS2sDISABLE_R
#ifdef MAINSTR
 char S2sSIZE_OF_C[] = "Size of cache for names. (in bytes)";
 char *lS2sSIZE_OF_C = S2sSIZE_OF_C;
#else
 extern char *lS2sSIZE_OF_C;
#endif
#define sSIZE_OF_C lS2sSIZE_OF_C
#ifdef MAINSTR
 char S2sRECURSION[] = "Recursion calls allways begin from two first defined root servers. (If you use DNS servers of your provider instead real root DNS)";
 char *lS2sRECURSION = S2sRECURSION;
#else
 extern char *lS2sRECURSION;
#endif
#define sRECURSION lS2sRECURSION
#ifdef MAINSTR
 char S2sRETURN_MA[] = "Return mailhost as host name, if MX record"
 " is not found.";
 char *lS2sRETURN_MA = S2sRETURN_MA;
#else
 extern char *lS2sRETURN_MA;
#endif
#define sRETURN_MA lS2sRETURN_MA
#ifdef MAINSTR
 char S2sENABLE_DN[] = "Enable DNS over TCP.";
 char *lS2sENABLE_DN = S2sENABLE_DN;
#else
 extern char *lS2sENABLE_DN;
#endif
#define sENABLE_DN lS2sENABLE_DN
#ifdef MAINSTR
 char S2sDON_T_SAV[] = "Don't save DNS cache on exit.";
 char *lS2sDON_T_SAV = S2sDON_T_SAV;
#else
 extern char *lS2sDON_T_SAV;
#endif
#define sDON_T_SAV lS2sDON_T_SAV
#ifdef MAINSTR
 char S2sDNS_CACHE[] = "DNS cache file name.";
 char *lS2sDNS_CACHE = S2sDNS_CACHE;
#else
 extern char *lS2sDNS_CACHE;
#endif
#define sDNS_CACHE lS2sDNS_CACHE

#ifdef MAINSTR
 char S2sTO_REMOTE[] = "To remote redirect server. Current IP notification";
 char *lS2sTO_REMOTE = S2sTO_REMOTE;
#else
 extern char *lS2sTO_REMOTE;
#endif
#define sTO_REMOTE lS2sTO_REMOTE
#ifdef MAINSTR
 char S2sDISABLE_C[] = "Disable current IP notification.";
 char *lS2sDISABLE_C = S2sDISABLE_C;
#else
 extern char *lS2sDISABLE_C;
#endif
#define sDISABLE_C lS2sDISABLE_C
#ifdef MAINSTR
 char S2sTIME_TO_R[] = "Interval to update your IP on redirect server. (in secconds)";
 char *lS2sTIME_TO_R = S2sTIME_TO_R;
#else
 extern char *lS2sTIME_TO_R;
#endif
#define sTIME_TO_R lS2sTIME_TO_R
#ifdef MAINSTR
 char S2sURL_ON_RE[] = "URL on redirect server. Full URL to update"
 " your IP address on dynamic DNS server. \"$IP_ADDRESS\" to insert"
 " your real IP address into URL.";
 char *lS2sURL_ON_RE = S2sURL_ON_RE;
#else
 extern char *lS2sURL_ON_RE;
#endif
#define sURL_ON_RE lS2sURL_ON_RE

#ifdef MAINSTR
 char S2sPROXY_SER[] = "Proxy server setting";
 char *lS2sPROXY_SER = S2sPROXY_SER;
#else
 extern char *lS2sPROXY_SER;
#endif
#define sPROXY_SER lS2sPROXY_SER
#ifdef MAINSTR
 char S2sDISABLE_P[] = "Disable Proxy server.";
 char *lS2sDISABLE_P = S2sDISABLE_P;
#else
 extern char *lS2sDISABLE_P;
#endif
#define sDISABLE_P lS2sDISABLE_P
#ifdef MAINSTR
 char S2sHOW_MANY_0[] = "Number of proxy requests working simultaneous.";
 char *lS2sHOW_MANY_0 = S2sHOW_MANY_0;
#else
 extern char *lS2sHOW_MANY_0;
#endif
#define sHOW_MANY_0 lS2sHOW_MANY_0
#ifdef MAINSTR
 char S2sTCP_IP_PO0[] = "TCP/IP port for proxy server.";
 char *lS2sTCP_IP_PO0 = S2sTCP_IP_PO0;
#else
 extern char *lS2sTCP_IP_PO0;
#endif
#define sTCP_IP_PO0 lS2sTCP_IP_PO0
#ifdef MAINSTR
 char S2sMEMORY_CA[] = "Memory cache size (Kb). Set zero for disk cache only";
 char *lS2sMEMORY_CA = S2sMEMORY_CA;
#else
 extern char *lS2sMEMORY_CA;
#endif
#define sMEMORY_CA lS2sMEMORY_CA
#ifdef MAINSTR
 char S2sDISABLE_T1[] = "Do not save proxy cache to hard disk.";
 char *lS2sDISABLE_T1 = S2sDISABLE_T1;
#else
 extern char *lS2sDISABLE_T1;
#endif
#define sDISABLE_T1 lS2sDISABLE_T1
#ifdef MAINSTR
 char S2sCACHE_PRO[] = "Proxy cache directory.";
 char *lS2sCACHE_PRO = S2sCACHE_PRO;
#else
 extern char *lS2sCACHE_PRO;
#endif
#define sCACHE_PRO lS2sCACHE_PRO
#ifdef MAINSTR
 char S2sFOR_HOW_M[] = "Number of days to keep files in cache. (Zero for keep ever)";
 char *lS2sFOR_HOW_M = S2sFOR_HOW_M;
#else
 extern char *lS2sFOR_HOW_M;
#endif
#define sFOR_HOW_M lS2sFOR_HOW_M
#ifdef MAINSTR
 char S2sIGNORE_NO[] = "Ignore NO-CACHE in control headers of pages.";
 char *lS2sIGNORE_NO = S2sIGNORE_NO;
#else
 extern char *lS2sIGNORE_NO;
#endif
#define sIGNORE_NO lS2sIGNORE_NO
#ifdef MAINSTR
 char S2sPROXY_FOR[] = "Proxy for authorized users only.";
 char *lS2sPROXY_FOR = S2sPROXY_FOR;
#else
 extern char *lS2sPROXY_FOR;
#endif
#define sPROXY_FOR lS2sPROXY_FOR
#ifdef MAINSTR
 char S2sDON_T_USE[] = "Do not use higher level proxy server.";
 char *lS2sDON_T_USE = S2sDON_T_USE;
#else
 extern char *lS2sDON_T_USE;
#endif
#define sDON_T_USE lS2sDON_T_USE
#ifdef MAINSTR
 char S2sUP_LEVEL_[] = "Higher level proxy server.";
 char *lS2sUP_LEVEL_ = S2sUP_LEVEL_;
#else
 extern char *lS2sUP_LEVEL_;
#endif
#define sUP_LEVEL_ lS2sUP_LEVEL_
#ifdef MAINSTR
 char S2sTCP_IP_PO1[] = "TCP/IP port on up level proxy server.";
 char *lS2sTCP_IP_PO1 = S2sTCP_IP_PO1;
#else
 extern char *lS2sTCP_IP_PO1;
#endif
#define sTCP_IP_PO1 lS2sTCP_IP_PO1
#ifdef MAINSTR
 char S2sUP_LEVEL_0[] = "Higher level proxy server does not require authorization.";
 char *lS2sUP_LEVEL_0 = S2sUP_LEVEL_0;
#else
 extern char *lS2sUP_LEVEL_0;
#endif
#define sUP_LEVEL_0 lS2sUP_LEVEL_0
#ifdef MAINSTR
 char S2sUP_LEVEL_1[] = "Higher level proxy user:pasword";
 char *lS2sUP_LEVEL_1 = S2sUP_LEVEL_1;
#else
 extern char *lS2sUP_LEVEL_1;
#endif
#define sUP_LEVEL_1 lS2sUP_LEVEL_1
#ifdef MAINSTR
 char S2sDISABLED_[] = "Disabled hosts";
 char *lS2sDISABLED_ = S2sDISABLED_;
#else
 extern char *lS2sDISABLED_;
#endif
#define sDISABLED_ lS2sDISABLED_


#ifdef MAINSTR
 char S2sFTP_SERVE[] = "FTP server setting";
 char *lS2sFTP_SERVE = S2sFTP_SERVE;
#else
 extern char *lS2sFTP_SERVE;
#endif
#define sFTP_SERVE lS2sFTP_SERVE
#ifdef MAINSTR
 char S2sDISABLE_F[] = "Disable FTP server.";
 char *lS2sDISABLE_F = S2sDISABLE_F;
#else
 extern char *lS2sDISABLE_F;
#endif
#define sDISABLE_F lS2sDISABLE_F
#ifdef MAINSTR
 char S2sHOW_MANY_1[] = "Number of simultaneous requests.";
 char *lS2sHOW_MANY_1 = S2sHOW_MANY_1;
#else
 extern char *lS2sHOW_MANY_1;
#endif
#define sHOW_MANY_1 lS2sHOW_MANY_1
#ifdef MAINSTR
 char S2sTCP_IP_PO2[] = "TCP/IP port for FTP server. Usualy it's 21";
 char *lS2sTCP_IP_PO2 = S2sTCP_IP_PO2;
#else
 extern char *lS2sTCP_IP_PO2;
#endif
#define sTCP_IP_PO2 lS2sTCP_IP_PO2
#ifdef MAINSTR
 char S2sLIMIT_OF_0[] = "User session timeout. (in second) "
 " Connection will close, if user is idle for this time.";
 char *lS2sLIMIT_OF_0 = S2sLIMIT_OF_0;
#else
 extern char *lS2sLIMIT_OF_0;
#endif
#define sLIMIT_OF_0 lS2sLIMIT_OF_0
#ifdef MAINSTR
 char S2sCONVERT_N[] = "Convert names with space.";
 char *lS2sCONVERT_N = S2sCONVERT_N;
#else
 extern char *lS2sCONVERT_N;
#endif
#define sCONVERT_N lS2sCONVERT_N
#ifdef MAINSTR
 char S2sDON_T_USE0[] = "Don't use upload directory.";
 char *lS2sDON_T_USE0 = S2sDON_T_USE0;
#else
 extern char *lS2sDON_T_USE0;
#endif
#define sDON_T_USE0 lS2sDON_T_USE0
#ifdef MAINSTR
 char S2sNAME_OF_U[] = "Name of upload subdirectory. If FTP directory"
 " contents this subdirectory, users with \"read only\" access can"
 " still upload files here. E.g. /pub/";
 char *lS2sNAME_OF_U = S2sNAME_OF_U;
#else
 extern char *lS2sNAME_OF_U;
#endif
#define sNAME_OF_U lS2sNAME_OF_U
#ifdef MAINSTR
 char S2sENABLE_VI[] = "Enable virtual directories for FTP.";
 char *lS2sENABLE_VI = S2sENABLE_VI;
#else
 extern char *lS2sENABLE_VI;
#endif
#define sENABLE_VI lS2sENABLE_VI


#ifdef MAINSTR
 char S2sPOP__SERV[] = "POP3 server setting";
 char *lS2sPOP__SERV = S2sPOP__SERV;
#else
 extern char *lS2sPOP__SERV;
#endif
#define sPOP__SERV lS2sPOP__SERV
#ifdef MAINSTR
 char S2sDISABLE_P0[] = "Disable POP3 server.";
 char *lS2sDISABLE_P0 = S2sDISABLE_P0;
#else
 extern char *lS2sDISABLE_P0;
#endif
#define sDISABLE_P0 lS2sDISABLE_P0
#ifdef MAINSTR
 char S2sTCP_IP_PO3[] = "TCP/IP port for POP3 server. Usually it's 110";
 char *lS2sTCP_IP_PO3 = S2sTCP_IP_PO3;
#else
 extern char *lS2sTCP_IP_PO3;
#endif
#define sTCP_IP_PO3 lS2sTCP_IP_PO3
#ifdef MAINSTR
 char S2sPOP__SMTP[] = "POP3/SMTP session timeout. (in second)."
 " Connection will close, if user is idle for this time.";
 char *lS2sPOP__SMTP = S2sPOP__SMTP;
#else
 extern char *lS2sPOP__SMTP;
#endif
#define sPOP__SMTP lS2sPOP__SMTP


#ifdef MAINSTR
 char S2sSMTP_SERV[] = "SMTP server setting";
 char *lS2sSMTP_SERV = S2sSMTP_SERV;
#else
 extern char *lS2sSMTP_SERV;
#endif
#define sSMTP_SERV lS2sSMTP_SERV
#ifdef MAINSTR
 char S2sDISABLE_S1[] = "Disable SMTP server.";
 char *lS2sDISABLE_S1 = S2sDISABLE_S1;
#else
 extern char *lS2sDISABLE_S1;
#endif
#define sDISABLE_S1 lS2sDISABLE_S1
#define sHOW_MANY_2 sHOW_MANY_1


#ifdef MAINSTR
 char S2sSMTP_SERV0[] = "SMTP server name. (Domain name)";
 char *lS2sSMTP_SERV0 = S2sSMTP_SERV0;
#else
 extern char *lS2sSMTP_SERV0;
#endif
#define sSMTP_SERV0 lS2sSMTP_SERV0
#ifdef MAINSTR
 char S2sUSE_ALL_V[] = "Use all virtual hosts as alias domain name.";
 char *lS2sUSE_ALL_V = S2sUSE_ALL_V;
#else
 extern char *lS2sUSE_ALL_V;
#endif
#define sUSE_ALL_V lS2sUSE_ALL_V
#ifdef MAINSTR
 char S2sDNS_SERVE0[] = "DNS server to get mail routing info. (May"
 " be your default DNS server)";
 char *lS2sDNS_SERVE0 = S2sDNS_SERVE0;
#else
 extern char *lS2sDNS_SERVE0;
#endif
#define sDNS_SERVE0 lS2sDNS_SERVE0
#ifdef MAINSTR
 char S2sTCP_IP_PO4[] = "TCP/IP port for SMTP server. Usually it\'s 25";
 char *lS2sTCP_IP_PO4 = S2sTCP_IP_PO4;
#else
 extern char *lS2sTCP_IP_PO4;
#endif
#define sTCP_IP_PO4 lS2sTCP_IP_PO4
#ifdef MAINSTR
 char S2sOUTPUT_PA[] = "Output path. Directory to store messages"
 " before sending. Direct full patch.";
 char *lS2sOUTPUT_PA = S2sOUTPUT_PA;
#else
 extern char *lS2sOUTPUT_PA;
#endif
#define sOUTPUT_PA lS2sOUTPUT_PA
#ifdef MAINSTR
 char S2sDISABLE_T2[] = "Do not save sent messages.";
 char *lS2sDISABLE_T2 = S2sDISABLE_T2;
#else
 extern char *lS2sDISABLE_T2;
#endif
#define sDISABLE_T2 lS2sDISABLE_T2
#ifdef MAINSTR
 char S2sSENT_PATH[] = "Sent path. Directory to store sent messages";
 char *lS2sSENT_PATH = S2sSENT_PATH;
#else
 extern char *lS2sSENT_PATH;
#endif
#define sSENT_PATH lS2sSENT_PATH
#ifdef MAINSTR
 char S2sFOR_HOW_M0[] = "For how many days sent messages will be saved. (Zero for keep ever)";
 char *lS2sFOR_HOW_M0 = S2sFOR_HOW_M0;
#else
 extern char *lS2sFOR_HOW_M0;
#endif
#define sFOR_HOW_M0 lS2sFOR_HOW_M0
#ifdef MAINSTR
 char S2sERROR_PAT[] = "Error path. Directory to store messages,"
 " on failed send";
 char *lS2sERROR_PAT = S2sERROR_PAT;
#else
 extern char *lS2sERROR_PAT;
#endif
#define sERROR_PAT lS2sERROR_PAT
#ifdef MAINSTR
 char S2sRECEIVE_T[] = "Alow any \"From\" field."
 " Otherwise server will send message from defined_user@domain.name only";
 char *lS2sRECEIVE_T = S2sRECEIVE_T;
#else
 extern char *lS2sRECEIVE_T;
#endif
#define sRECEIVE_T lS2sRECEIVE_T
#ifdef MAINSTR
 char S2sLIMIT_OF_1[] = "Limit message size. (in bytes).";
 char *lS2sLIMIT_OF_1 = S2sLIMIT_OF_1;
#else
 extern char *lS2sLIMIT_OF_1;
#endif
#define sLIMIT_OF_1 lS2sLIMIT_OF_1
#ifdef MAINSTR
 char S2sDISABLED_0[] = "Blacklist of E-mail addresses of spamers."
 " Separate addreses by space. Use *@host to block receiving"
 " from any address of this host)";
 char *lS2sDISABLED_0 = S2sDISABLED_0;
#else
 extern char *lS2sDISABLED_0;
#endif
#define sDISABLED_0 lS2sDISABLED_0
#ifdef MAINSTR
 char S2sENABLE_TO1[] = "Use instructions from the \"forward\" file in a user's"
 " directory.";
 char *lS2sENABLE_TO1 = S2sENABLE_TO1;
#else
 extern char *lS2sENABLE_TO1;
#endif
#define sENABLE_TO1 lS2sENABLE_TO1
#ifdef MAINSTR
 char S2sENABLE_TO2[] = "Alow execution of applications from user's"
 " \"forward\" file.";
 char *lS2sENABLE_TO2 = S2sENABLE_TO2;
#else
 extern char *lS2sENABLE_TO2;
#endif
#define sENABLE_TO2 lS2sENABLE_TO2


#ifdef MAINSTR
 char S2sCANT_ACCE[] = "Cant accept...";
 char *lS2sCANT_ACCE = S2sCANT_ACCE;
#else
 extern char *lS2sCANT_ACCE;
#endif
#define sCANT_ACCE lS2sCANT_ACCE
#ifdef MAINSTR
 char S2sACCESS_DE[] = "Access denyed";
 char *lS2sACCESS_DE = S2sACCESS_DE;
#else
 extern char *lS2sACCESS_DE;
#endif
#define sACCESS_DE lS2sACCESS_DE
#ifdef MAINSTR
 char S2sTOO_MANY_[] = "Too many simultaneous requests from the same host";
 char *lS2sTOO_MANY_ = S2sTOO_MANY_;
#else
 extern char *lS2sTOO_MANY_;
#endif
#define sTOO_MANY_ lS2sTOO_MANY_
#ifdef MAINSTR
 char S2sACCESS_DE0[] = "Access denid";
 char *lS2sACCESS_DE0 = S2sACCESS_DE0;
#else
 extern char *lS2sACCESS_DE0;
#endif
#define sACCESS_DE0 lS2sACCESS_DE0
#ifdef MAINSTR
 char S2sERROR__BA[] = "Error. Bad request.";
 char *lS2sERROR__BA = S2sERROR__BA;
#else
 extern char *lS2sERROR__BA;
#endif
#define sERROR__BA lS2sERROR__BA
#ifdef MAINSTR
 char S2sBAD_USER_[] = "Bad user data";
 char *lS2sBAD_USER_ = S2sBAD_USER_;
#else
 extern char *lS2sBAD_USER_;
#endif
#define sBAD_USER_ lS2sBAD_USER_
#ifdef MAINSTR
 char S2sUNABLE_TO[] = "Unable to open configuration";
 char *lS2sUNABLE_TO = S2sUNABLE_TO;
#else
 extern char *lS2sUNABLE_TO;
#endif
#define sUNABLE_TO lS2sUNABLE_TO
#ifdef MAINSTR
 char S2sERROR__CA[] = "Error. Can't create %s to save configuration";
 char *lS2sERROR__CA = S2sERROR__CA;
#else
 extern char *lS2sERROR__CA;
#endif
#define sERROR__CA lS2sERROR__CA
#ifdef MAINSTR
 char S2s__UNINSPE[] = "# Uninspected end of SSI variable (\") #";
 char *lS2s__UNINSPE = S2s__UNINSPE;
#else
 extern char *lS2s__UNINSPE;
#endif
#define s__UNINSPE lS2s__UNINSPE
#ifdef MAINSTR
 char S2s__UNINSPE0[] = "# Uninspected back bracket (}) #";
 char *lS2s__UNINSPE0 = S2s__UNINSPE0;
#else
 extern char *lS2s__UNINSPE0;
#endif
#define s__UNINSPE0 lS2s__UNINSPE0
#ifdef MAINSTR
 char S2s__SSI_VAR[] = "# SSI variable too large #";
 char *lS2s__SSI_VAR = S2s__SSI_VAR;
#else
 extern char *lS2s__SSI_VAR;
#endif
#define s__SSI_VAR lS2s__SSI_VAR
#ifdef MAINSTR
 char S2sTO_MANY_I[] = "To many includes...";
 char *lS2sTO_MANY_I = S2sTO_MANY_I;
#else
 extern char *lS2sTO_MANY_I;
#endif
#define sTO_MANY_I lS2sTO_MANY_I
#ifdef MAINSTR
 char S2sERROR__FI[] = "Error. File not found.";
 char *lS2sERROR__FI = S2sERROR__FI;
#else
 extern char *lS2sERROR__FI;
#endif
#define sERROR__FI lS2sERROR__FI
#ifdef MAINSTR
 char S2s__FILE_NO[] = "# File not found.<p>";
 char *lS2s__FILE_NO = S2s__FILE_NO;
#else
 extern char *lS2s__FILE_NO;
#endif
#define s__FILE_NO lS2s__FILE_NO
#ifdef MAINSTR
 char S2sERROR__NO[] = "Error. Not enought memory...";
 char *lS2sERROR__NO = S2sERROR__NO;
#else
 extern char *lS2sERROR__NO;
#endif
#define sERROR__NO lS2sERROR__NO
#ifdef MAINSTR
 char S2sUNINSPECT[] = "Uninspected end of SSI line... (-->)";
 char *lS2sUNINSPECT = S2sUNINSPECT;
#else
 extern char *lS2sUNINSPECT;
#endif
#define sUNINSPECT lS2sUNINSPECT
#ifdef MAINSTR
 char S2sUNINSPECT0[] = "Uninspected format..";
 char *lS2sUNINSPECT0 = S2sUNINSPECT0;
#else
 extern char *lS2sUNINSPECT0;
#endif
#define sUNINSPECT0 lS2sUNINSPECT0
#ifdef MAINSTR
 char S2s___BAD_NA[] = " # Bad name #";
 char *lS2s___BAD_NA = S2s___BAD_NA;
#else
 extern char *lS2s___BAD_NA;
#endif
#define s___BAD_NA lS2s___BAD_NA
#ifdef MAINSTR
 char S2s___ENDIF_[] = "# #endif not found... #";
 char *lS2s___ENDIF_ = S2s___ENDIF_;
#else
 extern char *lS2s___ENDIF_;
#endif
#define s___ENDIF_ lS2s___ENDIF_
#ifdef MAINSTR
 char S2s__TO_MANY[] = "# To many vars.. #";
 char *lS2s__TO_MANY = S2s__TO_MANY;
#else
 extern char *lS2s__TO_MANY;
#endif
#define s__TO_MANY lS2s__TO_MANY
#ifdef MAINSTR
 char S2sWARNING__[] = "Warning, it's to open window only, to pause"
 " service push \"Stop\" button.";
 char *lS2sWARNING__ = S2sWARNING__;
#else
 extern char *lS2sWARNING__;
#endif
#define sWARNING__ lS2sWARNING__
#ifdef MAINSTR
 char S2sDO_YOU_WA[] = "Do you want to close HTTP server?";
 char *lS2sDO_YOU_WA = S2sDO_YOU_WA;
#else
 extern char *lS2sDO_YOU_WA;
#endif
#define sDO_YOU_WA lS2sDO_YOU_WA
#ifdef MAINSTR
 char S2sBAD_PATHN[] = "Bad pathname. Type full pathname.";
 char *lS2sBAD_PATHN = S2sBAD_PATHN;
#else
 extern char *lS2sBAD_PATHN;
#endif
#define sBAD_PATHN lS2sBAD_PATHN
#ifdef MAINSTR
 char S2sCHECK___E[] = "Check \"Enable to remote administration\" first.";
 char *lS2sCHECK___E = S2sCHECK___E;
#else
 extern char *lS2sCHECK___E;
#endif
#define sCHECK___E lS2sCHECK___E
#ifdef MAINSTR
 char S2sUSER_NOT_[] = "User not found.";
 char *lS2sUSER_NOT_ = S2sUSER_NOT_;
#else
 extern char *lS2sUSER_NOT_;
#endif
#define sUSER_NOT_ lS2sUSER_NOT_
#ifdef MAINSTR
 char S2s_HR__DIV_[] = "<hr><div align=right>[<a name=\"%s\" href=#top>Contents</a>] &nbsp; &nbsp; [<a href=\"#end_%s\">Next</a>]</div><h2><a href=\"javascript:SHTbl('t%x');\"><span id=lt%x >[-]</span></a>  %s:</h2> "
  "<table id=t%x bgcolor=#F8FCFC border=1>"
  "<tr bgcolor=#E0F8FF><td><b>Name:</b></td><td><b>Count</b></td><td"
 " width=210>Percentage:</td></tr>"
 ;
 char *lS2s_HR__DIV_ = S2s_HR__DIV_;
#else
 extern char *lS2s_HR__DIV_;
#endif
#define s_HR__DIV_ lS2s_HR__DIV_
// #ifdef MAINSTR
//  char S2s_TR_BGCOL[] = "<tr bgcolor=#E0F8FF><td><b>Name:</b></td><td><b>Count</b></td><td"
//  " width=210>Percentage:</td></tr>";
//  char *lS2s_TR_BGCOL = S2s_TR_BGCOL;
// #else
//  extern char *lS2s_TR_BGCOL;
// #endif
// #define s_TR_BGCOL lS2s_TR_BGCOL
#ifdef MAINSTR
 char S2s_TR__TD_C[] = "<tr><td colspan=3 bgcolor=#C0F0E0><b>Total:"
 " %u%s / (%u items)</b></td></tr></table><a name=\"end_%s\" ></a> <br>";
 char *lS2s_TR__TD_C = S2s_TR__TD_C;
#else
 extern char *lS2s_TR__TD_C;
#endif
#define s_TR__TD_C lS2s_TR__TD_C
#ifdef MAINSTR
 char S2sFTP_LOGIN[] = "FTP Login";
 char *lS2sFTP_LOGIN = S2sFTP_LOGIN;
#else
 extern char *lS2sFTP_LOGIN;
#endif
#define sFTP_LOGIN lS2sFTP_LOGIN
#ifdef MAINSTR
 char S2sPOP_LOGIN[] = "POP Login";
 char *lS2sPOP_LOGIN = S2sPOP_LOGIN;
#else
 extern char *lS2sPOP_LOGIN;
#endif
#define sPOP_LOGIN lS2sPOP_LOGIN
#ifdef MAINSTR
 char S2sSUMMARY_B[] = "Summary by port";
 char *lS2sSUMMARY_B = S2sSUMMARY_B;
#else
 extern char *lS2sSUMMARY_B;
#endif
#define sSUMMARY_B lS2sSUMMARY_B
#ifdef MAINSTR
 char S2sHTTP_CALL[] = "HTTP calls";
 char *lS2sHTTP_CALL = S2sHTTP_CALL;
#else
 extern char *lS2sHTTP_CALL;
#endif
#define sHTTP_CALL lS2sHTTP_CALL
#ifdef MAINSTR
 char S2sSSL_CALL[] = "SSL/TLS calls";
 char *lS2sSSL_CALL = S2sSSL_CALL;
#else
 extern char *lS2sSSL_CALL;
#endif
#define sSSL_CALL lS2sSSL_CALL
#ifdef MAINSTR
 char S2sREQUESTED[] = "Requested hosts";
 char *lS2sREQUESTED = S2sREQUESTED;
#else
 extern char *lS2sREQUESTED;
#endif
#define sREQUESTED lS2sREQUESTED
#ifdef MAINSTR
 char S2sHTTP__INC[] = "HTTP. Incomming traffic (bytes)";
 char *lS2sHTTP__INC = S2sHTTP__INC;
#else
 extern char *lS2sHTTP__INC;
#endif
#define sHTTP__INC lS2sHTTP__INC
#ifdef MAINSTR
 char S2sHTTP__OUT[] = "HTTP. Outgoing traffic (bytes)";
 char *lS2sHTTP__OUT = S2sHTTP__OUT;
#else
 extern char *lS2sHTTP__OUT;
#endif
#define sHTTP__OUT lS2sHTTP__OUT
#ifdef MAINSTR
 char S2sHTTP_BY_H[] = "HTTP by hosts. Incomming traffic (bytes)";
 char *lS2sHTTP_BY_H = S2sHTTP_BY_H;
#else
 extern char *lS2sHTTP_BY_H;
#endif
#define sHTTP_BY_H lS2sHTTP_BY_H
#ifdef MAINSTR
 char S2sHTTP_BY_H0[] = "HTTP by hosts. Outgoing traffic (bytes)";
 char *lS2sHTTP_BY_H0 = S2sHTTP_BY_H0;
#else
 extern char *lS2sHTTP_BY_H0;
#endif
#define sHTTP_BY_H0 lS2sHTTP_BY_H0
#ifdef MAINSTR
 char S2sPROXY_CAL[] = "Proxy calls";
 char *lS2sPROXY_CAL = S2sPROXY_CAL;
#else
 extern char *lS2sPROXY_CAL;
#endif
#define sPROXY_CAL lS2sPROXY_CAL
#ifdef MAINSTR
 char S2sPROXY__RE[] = "Proxy. Requested incomming traffic (bytes)";
 char *lS2sPROXY__RE = S2sPROXY__RE;
#else
 extern char *lS2sPROXY__RE;
#endif
#define sPROXY__RE lS2sPROXY__RE
#ifdef MAINSTR
 char S2sPROXY__IN[] = "Proxy. Incomming traffic without cached (bytes)";
 char *lS2sPROXY__IN = S2sPROXY__IN;
#else
 extern char *lS2sPROXY__IN;
#endif
#define sPROXY__IN lS2sPROXY__IN
#ifdef MAINSTR
 char S2sPROXY__OU[] = "Proxy. Outgoing traffic (bytes)";
 char *lS2sPROXY__OU = S2sPROXY__OU;
#else
 extern char *lS2sPROXY__OU;
#endif
#define sPROXY__OU lS2sPROXY__OU
#ifdef MAINSTR
 char S2sPROXY_USE[] = "Proxy users. Requested incomming traffic (bytes)";
 char *lS2sPROXY_USE = S2sPROXY_USE;
#else
 extern char *lS2sPROXY_USE;
#endif
#define sPROXY_USE lS2sPROXY_USE
#ifdef MAINSTR
 char S2sPROXY_USE0[] = "Proxy users. Incomming traffic without cached"
 " (bytes)";
 char *lS2sPROXY_USE0 = S2sPROXY_USE0;
#else
 extern char *lS2sPROXY_USE0;
#endif
#define sPROXY_USE0 lS2sPROXY_USE0
#ifdef MAINSTR
 char S2sPROXY_USE1[] = "Proxy users. Outgoing traffic (bytes)";
 char *lS2sPROXY_USE1 = S2sPROXY_USE1;
#else
 extern char *lS2sPROXY_USE1;
#endif
#define sPROXY_USE1 lS2sPROXY_USE1
#ifdef MAINSTR
 char S2sPROXY_PAG[] = "Proxy Pages";
 char *lS2sPROXY_PAG = S2sPROXY_PAG;
#else
 extern char *lS2sPROXY_PAG;
#endif
#define sPROXY_PAG lS2sPROXY_PAG
#ifdef MAINSTR
 char S2sPROXY_REF[] = "Proxy Referer";
 char *lS2sPROXY_REF = S2sPROXY_REF;
#else
 extern char *lS2sPROXY_REF;
#endif
#define sPROXY_REF lS2sPROXY_REF
#ifdef MAINSTR
 char S2sFTP_CALLS[] = "FTP calls";
 char *lS2sFTP_CALLS = S2sFTP_CALLS;
#else
 extern char *lS2sFTP_CALLS;
#endif
#define sFTP_CALLS lS2sFTP_CALLS
#ifdef MAINSTR
 char S2sFTP_DOWNL[] = "FTP download";
 char *lS2sFTP_DOWNL = S2sFTP_DOWNL;
#else
 extern char *lS2sFTP_DOWNL;
#endif
#define sFTP_DOWNL lS2sFTP_DOWNL
#ifdef MAINSTR
 char S2sFTP_UPLOA[] = "FTP upload";
 char *lS2sFTP_UPLOA = S2sFTP_UPLOA;
#else
 extern char *lS2sFTP_UPLOA;
#endif
#define sFTP_UPLOA lS2sFTP_UPLOA
#ifdef MAINSTR
 char S2sFTP_DOWNL1[] = "FTP download (bytes)";
 char *lS2sFTP_DOWNL1 = S2sFTP_DOWNL1;
#else
 extern char *lS2sFTP_DOWNL1;
#endif
#define sFTP_DOWNL1 lS2sFTP_DOWNL1
#ifdef MAINSTR
 char S2sFTP_UPLOA1[] = "FTP upload (bytes)";
 char *lS2sFTP_UPLOA1 = S2sFTP_UPLOA1;
#else
 extern char *lS2sFTP_UPLOA1;
#endif
#define sFTP_UPLOA1 lS2sFTP_UPLOA1
#ifdef MAINSTR
 char S2sPOP__CALL[] = "POP3 calls";
 char *lS2sPOP__CALL = S2sPOP__CALL;
#else
 extern char *lS2sPOP__CALL;
#endif
#define sPOP__CALL lS2sPOP__CALL
#ifdef MAINSTR
 char S2sPOP_DATA_[] = "POP data traffic (bytes)";
 char *lS2sPOP_DATA_ = S2sPOP_DATA_;
#else
 extern char *lS2sPOP_DATA_;
#endif
#define sPOP_DATA_ lS2sPOP_DATA_
#ifdef MAINSTR
 char S2sSMTP_CALL[] = "SMTP calls";
 char *lS2sSMTP_CALL = S2sSMTP_CALL;
#else
 extern char *lS2sSMTP_CALL;
#endif
#define sSMTP_CALL lS2sSMTP_CALL
#ifdef MAINSTR
 char S2sSMTP_DATA[] = "SMTP data traffic (bytes)";
 char *lS2sSMTP_DATA = S2sSMTP_DATA;
#else
 extern char *lS2sSMTP_DATA;
#endif
#define sSMTP_DATA lS2sSMTP_DATA
#ifdef MAINSTR
 char S2sSMTP_DATA0[] = "SMTP data from users (bytes)";
 char *lS2sSMTP_DATA0 = S2sSMTP_DATA0;
#else
 extern char *lS2sSMTP_DATA0;
#endif
#define sSMTP_DATA0 lS2sSMTP_DATA0
#ifdef MAINSTR
 char S2sSMTP_DATA1[] = "SMTP data for users (bytes)";
 char *lS2sSMTP_DATA1 = S2sSMTP_DATA1;
#else
 extern char *lS2sSMTP_DATA1;
#endif
#define sSMTP_DATA1 lS2sSMTP_DATA1
#ifdef MAINSTR
 char S2sDNS_CALLS[] = "DNS calls";
 char *lS2sDNS_CALLS = S2sDNS_CALLS;
#else
 extern char *lS2sDNS_CALLS;
#endif
#define sDNS_CALLS lS2sDNS_CALLS
#ifdef MAINSTR
 char S2sDNS_HOSTS[] = "DNS hosts";
 char *lS2sDNS_HOSTS = S2sDNS_HOSTS;
#else
 extern char *lS2sDNS_HOSTS;
#endif
#define sDNS_HOSTS lS2sDNS_HOSTS
#ifdef MAINSTR
 char S2s_LI__A_HR[] = "<li><a href=\"#Files\"><b>Log files for other"
 " days</b></a> (If option <i>New log file every new day</i> is selected)"
 " </ul><center> ";
 char *lS2s_LI__A_HR = S2s_LI__A_HR;
#else
 extern char *lS2s_LI__A_HR;
#endif
#define s_LI__A_HR lS2s_LI__A_HR
#ifdef MAINSTR
 char S2s_HR__DIV_0[] = "<hr><div align=right><a name=\"Files\" href=#top>Contents</a></div><h2>Log"
 " files avilable:</h2><font size=2 class=f><b>";
 char *lS2s_HR__DIV_0 = S2s_HR__DIV_0;
#else
 extern char *lS2s_HR__DIV_0;
#endif
#define s_HR__DIV_0 lS2s_HR__DIV_0
#ifdef MAINSTR
 char S2s__B___FON[] = "</b></font><hr><div align=right><a name=Connected"
 " href=#top>Contents</a></div>";
 char *lS2s__B___FON = S2s__B___FON;
#else
 extern char *lS2s__B___FON;
#endif
#define s__B___FON lS2s__B___FON
#ifdef MAINSTR
 char S2MSG_LD[] = "<h2>This is avilable only if option <i>\"Create a daily log\"</i> has been selected</h2>";
 char *lS2MSG_LD = S2MSG_LD;
#else
 extern char *lS2MSG_LD;
#endif
#define MSG_LD lS2MSG_LD

#ifdef MAINSTR
 char S2sDONT_BREAK[] = "Don't break connection, when overflow size limit";
 char *lS2sDONT_BREAK = S2sDONT_BREAK;
#else
 extern char *lS2sDONT_BREAK;
#endif
#define sDONT_BREAK lS2sDONT_BREAK
#ifdef MAINSTR
 char S2sTLS__SERV[] = "TLS/SSL server";
 char *lS2sTLS__SERV = S2sTLS__SERV;
#else
 extern char *lS2sTLS__SERV;
#endif
#define sTLS__SERV lS2sTLS__SERV
#ifdef MAINSTR
 char S2sDISABLE_TLS[] = "Disable TLS/SSL server";
 char *lS2sDISABLE_TLS = S2sDISABLE_TLS;
#else
 extern char *lS2sDISABLE_TLS;
#endif
#define sDISABLE_TLS lS2sDISABLE_TLS
#ifdef MAINSTR
 char S2sTCP_IP_TLS[] = "TCP/IP port for TLS/SSL server. Usually it's 443";
 char *lS2sTCP_IP_TLS = S2sTCP_IP_TLS;
#else
 extern char *lS2sTCP_IP_TLS;
#endif
#define sTCP_IP_TLS lS2sTCP_IP_TLS
#ifdef MAINSTR
 char S2sTLS__LIB[] = "DLL library with TLS/SSL. E.g. seclib.dll";
 char *lS2sTLS__LIB = S2sTLS__LIB;
#else
 extern char *lS2sTLS__LIB;
#endif
#define sTLS__LIB lS2sTLS__LIB
#ifdef MAINSTR
 char S2sTLS__SERT[] = "Certificate file";
 char *lS2sTLS__SERT = S2sTLS__SERT;
#else
 extern char *lS2sTLS__SERT;
#endif
#define sTLS__SERT lS2sTLS__SERT
#ifdef MAINSTR
 char S2sTLS__KEYF[] = "Key file";
 char *lS2sTLS__KEYF = S2sTLS__KEYF;
#else
 extern char *lS2sTLS__KEYF;
#endif
#define sTLS__KEYF lS2sTLS__KEYF
#ifdef MAINSTR
 char S2sTLS__CAFILE[] = "CA-file";
 char *lS2sTLS__CAFILE = S2sTLS__CAFILE;
#else
 extern char *lS2sTLS__CAFILE;
#endif
#define sTLS__CAFILE lS2sTLS__CAFILE
#ifdef MAINSTR
 char S2sTLS__CAPATH[] = "CA-Path";
 char *lS2sTLS__CAPATH = S2sTLS__CAPATH;
#else
 extern char *lS2sTLS__CAPATH;
#endif
#define sTLS__CAPATH lS2sTLS__CAPATH

#ifdef MAINSTR
 char S2sUSER_COULD[] = "User could have only E-Mail access or only FTP access without mail "
 "account. Also you can restrict FTP access, only read, only write, "
 "disable upload files with CGI- ident in path."
 " If you give <b><i>Admin</i></b> access and check "
 "<b><i> Enable remote administration </i></b> in configuration, "
 "then user could edit this page, and will have"
 " access to all administrations pages.";
 char *lS2sUSER_COULD = S2sUSER_COULD;
#else
 extern char *lS2sUSER_COULD;
#endif
#define sUSER_COULD lS2sUSER_COULD


#ifdef MAINSTR
 char S2sVIRTUAL_HOSTS[] = "<h2>Virtual hosts and virtual directories:</h2>"
 "Server can support many virtual hosts. Each virtual host has its own root "
 "directory, and also can include virtual directories common for all. "
 "To add virtual host direct <i>any.host.name</i> and full path to root. "
 "To add virtual directory direct <i><b>/</b>directory<b>/</b></i> "
 "(between two slash) and full path for substitute. To make this private, direct "
 "users for that resurse will be available, through coma. After user you may direct type of access: "
 "R - read, W-write, NOFTP, NOHTTP .<BR> Example:"
 " <I>admin:RW,anonymous:RNOHTTP,user1,user2:R,user3:W,user4:RWNOHTTP</I><BR>" 
 "Instead of root dir you may direct link to another Web server: http://other.host In this case server will work like proxy"
 "<table bgcolor=#bcf8cc border=1><tr bgcolor=#bcf0ff>"
 "<td>Host name or <b>/</b>directory<b>/</b></td>"
 "<td>Directory</td><td>Users</td>"
 ;
 char *lS2sVIRTUAL_HOSTS = S2sVIRTUAL_HOSTS;
#else
 extern char *lS2sVIRTUAL_HOSTS;
#endif
#define sVIRTUAL_HOSTS lS2sVIRTUAL_HOSTS


#ifdef MAINSTR
 char S2AUTH_ERR[] = "HTTP/1.0 401 \r\nWWW-Authenticate: Basic realm=\"Authorization request\"\r\nContent-Type: text/html\r\n\r\n<h1>Authorization request\r\n";
 char *lS2AUTH_ERR = S2AUTH_ERR;
#else
 extern char *lS2AUTH_ERR;
#endif
#define AUTH_ERR lS2AUTH_ERR
#ifdef MAINSTR
 char S2RSTNEED[] = ". (Restart need)";
 char *lS2RSTNEED = S2RSTNEED;
#else
 extern char *lS2RSTNEED;
#endif
#define RSTNEED lS2RSTNEED

