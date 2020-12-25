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

#define rIP_RANGE  " адреса, для которых сервер будет доступен. Указывайте IP адреса через запятую и диапазоны младший через дефис старший."
#define rIP_DENY "Запрещенные адреса, для которых сервер будет не доступен. Указывайте IP адреса через запятую и диапазоны младший через дефис старший."

#ifdef USE_IPV6

#define OIPV6(n,f) {"no" #n "_bind",1,0,(uint *)0, "Привязаться ко всем адаптерам"  },\
{#n "_bind",MAX_ADAPT*15,0,(uint *)& (bind_a[f]),"Привязываться только к перечисленным IP и IPv6. через запятую. (0.0.0.0 - все IP; ::0 все IPv6)" },\
{#n "ipv6",2,1<<f,(uint *)0, "Также работать через IPv6" },

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), "IP " t " Например: 192.168.0.1-192.168.0.16,127.0.0.1" },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), "IP " td },\
{#a "6_range",1024,0,(uint *)& (Rangec6[a##_range]), "IPv6 " t " Например ::1,FE80::-FEFF::" },\
{#a "6_deny" ,1024,0,(uint *)& (Rangec6[a##_range+1]),"IPv6 "  td },

#else
#define OIPV6(n,f)

#define RANGES(a,t,td) {#a "_range",1024,0,(uint *)& (Rangec[a##_range]), t },\
{#a "_deny" ,1024,0,(uint *)& (Rangec[a##_range+1]), td },


#endif

#define XLIMIT(a,b,c)  \
{"no" #a "_ltime",0,0,(uint *)0,"Не ограничивать " b },\
{ #a "_ltime" ,0,0x10000000,(uint *) (ltime+c), "Время за которое будет определяться ограничение (в секундах)" },\
{ #a "_ip_limit" ,0x400,0x10000000,(uint *) (ip_limit+c), "Ограничение на IP (Kb)" },\
{ #a "_net_limit" ,0x400,0x10000000,(uint *) (net_limit+c), "Ограничение на  сеть (Kb)" }, \
{ #a "_limit" ,0x400,0x10000000,(uint *) (limit+c), "Общие ограничение на весь сервер (Kb)" }



CfgParam ConfigParams[]={

{0,0,0,0,"Общие параметры"},

#ifndef CD_VER
{"noicon",0,FL_NOICON,(uint *)0,"Не показывать иконку в таскбаре."},
{"hide"  ,0,FL_HIDE,(uint *)0,"Сворачиваться при старте."},
#endif

{"detail",0,FL_FULLLOG,(uint *)0,"Подробный отчет для POP/SMTP/FTP. Иначе фиксируются только основные события."},
{"nolog",1,0,(uint *)0,"Не сохранять лог."},
{"log",256,0,(uint *)&flog,"Сохранять лог. Выберите имя лог-файла."},
{"logday",0,FL_LOGDAY,(uint *)0,"Новый лог файл каждый день. Это нужно, чтобы получать статистику по дням."},

{"nofrom_same_host",0,0,(uint *)0,"Не ограничивать количество одновременных подключений с одного хоста."},
{"from_same_host",0,256,(uint *)&max_cln_host,"Максимальное количество одновременных подключений с одного и того же хоста."
 "Для снятия ограничений можно использовать 0."
 " Ограничения распространяются на все TCP соединения (HTTP,FTP,POP,SMTP,Proxy)" },

#ifndef CD_VER

#define SPD(a,b) {"no" #b  "_speed",0,0,(uint *)0, "Не ограничивать скорость передачи" },\
{ #b "_speed",0,0x200000,(uint *) & (ipspeed[a]), "Предел сумарной скорости передачи данных для всех соединений с одного IP (KБайт/минуту)"},\
{ #b "_spdusr",0,0x2000,(uint *) & (ipspdusr[a]), "Сколько других соединений должно быть активными, чтобы включалось это ограничение скорости" },


RANGES(ip,rIP_RANGE,rIP_DENY)

{"cryptpwd",1,FL1_CRYPTPWD,(uint *)0, "Не хранить не криптованные пароли в конфигурационном файле." },
{"delpwd",1,FL1_DELPAS,(uint *)0, "Удалять пароли из лога" },

{"radmin",0,FL_RADMIN,(uint *)0,"Разрешить удаленное администрирование."},

 RANGES(adm,"IP адреса, с которых возможно администрирование.","IP адреса, с которых администрирование запрещено.")
#endif


{"twopoint",1,FL1_2P,(uint *)0, "Разрешить две точки в именах файлов (может быть опасно)" },

#ifndef SYSUNIX
{"utf8",2,FL2_UTF,(uint *)0, "Конвертировать имена файлов из UTF-8" },
#endif


{0,0,0,0,"Параметры HTTP сервера"},
{"nomax",0,0,(uint *)0,"Запретить HTTP сервер."},
{"max"  ,0,1024,(uint *)&max_cln,"Как много HTTP запросов могут обрабатываться одновременно. Около 20Kb памяти резервируется для каждой нити."
 " Обычно 12-24 соединений более чем достаточно для 3-8 посетителей в минуту."},
{"port",1,0xFFFF,(uint *)&http_port,"TCP/IP порт для HTTP. Обычно 80"},

#ifndef CD_VER

RANGES(http,rIP_RANGE,rIP_DENY)
OIPV6(http,0)

SPD(0,http)
#endif

{"dir",255,0,(uint *)&def_dir,"Web папка по умолчанию."},
{"def",128,0,(uint *)&def_name,"Имя файла по умолчанию. (Можно использовать шаблон '*'. Например index.* для любого index-а)"},
{"error",256,0,(uint *)&error_file,"Файл ошибки. Полный путь к файлу или скрипту возвращаемому при отсутствии запрошенного файла"},
{"cgi_ident",254, 0,(uint *)&cgi_detect,"CGI идентификатор. Часть URL для определения CGI. По умолчанию это \"\\cgi-bin\\\", но можно использовать, например, \"\\cgi-\", \"\\local-bin\\\", \".cgi\", и т.д."},
{"perl",256,0,(uint *)&perl,"Perl. Если вы используйте Perl, вы можете указать, где находится perl.exe или perlis.dll."},
{"php",256,0,(uint *)&phtml_dir,"PHP. Если вы используйте  PHP вы можете указать где находится php-cgi.exe или  phpisapi.dll..."},

{"nowintypes",0,3,(uint *)0,"Не использовать регистрированные типы для определения CGI приложений."},
{"ssihtm",0,0x80, (uint *)0,"Разрешить обрабатывать Server Side Includes (SSI) в HTML файлах."
 " По умолчанию сервер проверяет SSI только в .sht*,.sml*,.asp* файлах. Предостережение: Обработка SSI требует больше памяти и это медленнее"},
{"noshare",1,0,(uint *)0,"Не использовать разделяемый каталог."},
{"share",255,0,(uint *)&doc_dir,"Разделяемый каталог. Если вы хотите чтобы для всех CGI был один и тот же каталог запуска, укажите его здесь. Иначе каталогом запуска, будет каталог самого скрипта."},
{"post_limit",0x800,0x2000000,(uint *)&post_limit,"Предельный размер данных принимаемых методом POST в байтах. Не указывайте большое значение так как это может дать атакующим хакерам, возможность сильно загрузить вашу сеть."},
{"nooutdir",0,FL_NODIR,(uint *)0,"Не показывать каталоги. (Только в регистрированой версии)"},
{"norunhtm",0,FL_NORUNHTM,(uint *)0,"Никогда не запускать .htm,.gif,.jpg файлов."},

{"cgi_timeout",30,0x100000,(uint *)&cgi_timeout,"Предел времени на выполнение скрипта. В секундах."},
{"nbrkcgi"  ,1,FL1_NBRK,(uint *)0,"Не прерывать исполнение CGI, в случае закрытия соединения"  },
{"header",2048,0,(uint *)&header,"Дополнительные строки управляющего заголовка."},
#ifndef SYSUNIX
{"dntshowhiden"  ,1,FL1_DNTSHOWHIDEN,(uint *)0,"Не отдавать скрытые файлы."  },
{"runsystem"  ,1,FL1_RUNSYSTEM,(uint *)0,"Запускать системные файлы."  },
#endif

#ifndef CD_VER

{"ssi_chunk",2,FL2_CHUNKED,(uint *)0, "Использовать 'chunked' передачу для SSI." },

{"nomsd" ,1,FL1_NOM, (uint *)0, "Запретить многопотоковое скачивание."},
{"http_gzip" ,1,FL1_GZ, (uint *)0, "Использовать gzip сжатие, когда это возможно."},
{"gz_lib",256,0,(uint *)& zlib,  "DLL библиотека ZLib."},
{"gz_low"  ,0,0x1000000,(uint *)&GZSizeLow,"Наименьший размер файла, который стоит паковать"},
{"nogz_ext"  ,1024,0,(uint *)&NoGZSuf,"Не паковать файлы со следующими расширениями"},

{"ip_base",256,0,(uint *)& ipbsfile,  "Файл базы IP адресов для гографических дополнений."},
{"ip_cntr"  ,1,FL1_IPCNTR,(uint *)0,  "Добавлять REMOTE_COUNTRY переменную к CGI/SSI окружению"},

XLIMIT(http,"HTTP",7),

#endif




#if V_FULL
{0,0,0,0,"Параметры DNS сервера"},
{"nohosts",0,0,(uint *)0,"Запретить DNS сервер."},
{"hosts",256,0,(uint *)&dns_file,"Хост-файл. Файл с именами хостов и соответствующими IP адресами для DNS сервера."},
{"noreqursion",2,0,(uint *)0,"Запретить рекурсию."},
{"dnscache",0,92160,(uint *)&dns_cach_size,"Размер кеша имен в записях."},
{"dnstimeout",100,2000,(uint *)&DnsTms,"Задержка перед пвторной посылкой запроса. В милисекундах." },
{"dnsupl"  ,0,FL_DNSUPLVL,(uint *)0,"Рекурсивные вызовы всегда начинать с двух первых рут серверов. (Если вы испоользуйте DNS сервера вашего провайдера вместо настоящих рут серверов)"},
//{"dnsmx"  ,0,FL_DNSMXA  ,(uint *)0,"Возвращать mailhost как host если MX запись отсутствует."},
{"dnstcp"  ,0,FL_DNSTCP ,(uint *)0,"Разрешить DNS по TCP."},
//{"dnsaptr"  ,1,FL1_AUTOPTR ,(uint *)0,"Автоматически создавать PTR из A записи"  },
RANGES(dns,rIP_RANGE,rIP_DENY)
OIPV6(dns,6)

{"nodnscachefile",2,0,(uint *)0,"Не сохранять кеш имен при выходе."},
{"dnscachefile",256,0,(uint *)&dnscachefile,"Имя файла кеша имен."},

{"dnsno6",2,FL2_DNSNO6 ,(uint *)0, "Не пытаться рекурсивно искать AAAA записи. (для сетей не использующих Интернет через IPv6)" },


{"nodns_bld",1,0,(uint *)0, "Запретить встроенный DNSBL сервер" },
{"dns_bld",63,0,(uint *)&dnsblname, "Имя хоста встроенного DNSBL сервера" },



{0,0,0,0,"Для сервера удаленного перенаправления запросов. Уведомления о текущем IP адресе"},
{"noddns_time",0,0,(uint *)0,"Запретить уведомления о текущем IP адресе."},
{"ddns_time"  ,0,0x1fFFFF,(uint *)&time_update,"Интервал пере подтверждения."},
{"ddns",258,0,(uint *)&dns_user,"URL запроса. "
 "Полный URL для сообщения вашего IP адреса серверу динамических перенаправлений.  Можно использовать \"$IP_ADDRESS\" переменную чтобы вставить ваш IP адрес в URL."},

{0,0,0,0,"Параметры прокси сервера"},
{"noproxy_max",0,0,(uint *)0,"Запретить прокси сервер."},
{"proxy_max"   ,0,1024,(uint *)&max_prx,"Как много запросов могут обрабатываться одновременно."},
{"proxy" ,1,0xFFFF,(uint *)&proxy_port,   "TCP/IP порт для прокси сервера."},
{"noproxy_dir",0,0,(uint *)0,"Запретить  кэш на жестком диске."},
{"proxy_dir",255,0,(uint *)&proxy_dir,"Каталог для сохранения кэша."},
{"proxy_time",0,0x6000,(uint *)&proxy_time,"Сколько дней должны храниться данные в кеше. (Ноль -- хранить вечно)"},
{"proxy_fsize",0,0x7FFFFFFF,(uint *)&max_pfile, "Не сохранять большие файлы. Предел в байтах" },
{"proxy_laccess" ,1,FL1_LACPROXY, (uint *)0, "Отсчитывать дни от последнего доступа."},

{"ignocache"  ,0,FL_IGNNOCH,(uint *)0,"Игнорировать NO-CACHE в управляющих заголовках страниц."},
{"proxy_hrd"  ,1,FL1_PRXHRD,(uint *)0, "Не кэшировать страницы если в запросе есть куки." },
{"proxyusers" ,0,FL_PRXUSER,(uint *)0,"Proxy только для авторизованных пользователей."},
RANGES(proxy,rIP_RANGE,rIP_DENY)
OIPV6(proxy,1)


SPD(1,proxy)
{"proxy_big" ,1,FL1_BIGPROXY, (uint *)0, "Большой режим. Огромное количество трафика должно храниться."},
{"proxy_sbig" ,1,FL1_BBPROXY, (uint *)0, "Еще больший режим. Новый вариант организации хранилища, для уменьшения времени поиска срреди большого числа файлов."},
{"proxy_tryes",0,0x2000,(uint *)&max_cont_st, "Число попыток докачать файл после разрыва соединения" },
{"proxy_same",0,1024,(uint *)&cnt_same, "Ограничение на одновременные запросы с одного хоста к одной и той же URL. Ноль -- без ограничений." },
{"noupproxy",0,0,(uint *)0,"Не использовать прокси сервер верхнего уровня."},
{"upproxy" ,128,0,(uint *)&up_proxy,"Прокси сервер верхнего уровня."},
{"upproxy_port" ,1,0xFFFF,(uint *)&up_proxy_port,   "TCP/IP порт прокси сервера верхнего уровня."},

{"noup_user",3,0,(uint *)0,"Прокси сервер верхнего уровня без авторизации."},
{"up_user" ,128,0,(uint *)&up_user,"user:pasword для прокси сервера верхнего уровня."},
{"ever_upproxy" ,1,FL1_UPPRX, (uint *)0, "Для POP3/SMTP/FTP прокси, соединяться через HTTPS прокси верхнего уровня."},
{"nouphosts" ,4096,0, (uint *)&nohosts, "Не использовать прокси верхнего уровня для следующих хостов"},


{"bad_hosts",4096,0,(uint *)&bad_hosts, "Запрещенные хосты" },
{"proxy_timeout",30,0xFFFFF,(uint *)&PRXTimeout, "Таймаут простоя для прокси (в секундах)." },
{"proxy_gzip" ,1,FL1_GZPRX, (uint *)0, "Запрашивать упакованые gzip-ом, и распаковывать если браузер не поддерживает этого."},

//{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, "Команда вызова антивируса (Имя проверяемого файла добовляется в конец команды)" },
{"noproxy_antivirus",3,0,(uint *)0,"Не использовать антивирус."},
{"proxy_antivirus" ,256,0,(uint *)&proxy_antivirus, "Хост антивируса. (127.0.0.1 для локального PC)"
//"Antivirus command for check active content (Checked filename will be added to end)"
 },
{"proxy_avport" ,1,0xFFFF,(uint *)&proxy_antivirus_port,"Порт антивируса" },
{"proxy_avhtml" ,2,FL2_PAVHTML, (uint *)0, "Проверять и HTML файлы. (Иначе проверятся будут только приложения)"},
{"proxy_avall" ,2,FL2_PAVALL, (uint *)0, "Проверять все файлы. (Иначе проверятся будут только приложения)"},






 XLIMIT(proxy,"Proxy",2),

{0,0,0,0,"Параметры FTP сервера"},
{"noftp_max",0,0,(uint *)0,"Запретить FTP сервер."},
{"ftp_max"   ,0,1024,(uint *)&max_ftp,"Как много запросов могут обрабатываться одновременно."},
{"ftp_port"  ,1,0xFFFF,(uint *)&ftp_port,"TCP/IP порт для FTP сервера. Обычно 21"},
{"ftp_timeout",30,0xFFFFF,(uint *)&FTPTimeout,"Таймаут по бездействию в секундах. По прошествии этого времени, после последней операции, соединение будет закрыто."},

RANGES(ftp,rIP_RANGE,rIP_DENY)
OIPV6(ftp,2)
SPD(2,ftp)

{"noftp_pasvp",0,0,(uint *)0, "Для соединения данных в пасивном режиме использовать любой свободный порт." },
{"ftp_pasvp",1,0xFFFF,(uint *)&first_pass_port, "Первый порт для соединения данных в пасивном режиме. ( Диапазон используемых портов будет от этого порта до этого + число одновременных FTP соединений)" },

{"ftp_oone",2,FL2_NOMFTP, (uint *)0, "Запретить несколько потоков для одного IP" },


{"ftp_wospace"  ,0,FL_FTWOSPACE, (uint *)0,"Преобразовывать имена с пробелами."},
{"noftp_upload",3,0,(uint *)0,"Не использовать каталог для загрузок."},
{"ftp_upload" ,128,0,(uint *)&ftp_upload,"Имя для загрузок. Если FTP каталог содержит такой подкаталог то пользователи с доступом только для чтения могут загружать туда файлы. Пример: /pub/"},
{"ftp_vdirs" ,0,FL_FTPVDIR, (uint *)0,"Разрешить виртуальные директории для FTP."},

{"ftp_same"  ,1,FL1_FTPSAME, (uint *)0, "Разрешать FTP PORT команду только на клиентский хост. При этом режим FTP на FTP может не работать."},
{"ftp_proxy" ,1,FL1_FTPROXY, (uint *)0, "Разрешать FTP proxy."},

 XLIMIT(ftpi,"FTP download",3),
 XLIMIT(ftpo,"FTP upload",4),



{0,0,0,0,"Параметры POP3 сервера"},
{"nopop3_max",0,0,(uint *)0,"Запретить POP3 сервер."},
{"pop3_max" ,0,1024,(uint *)&max_pop,"Как много запросов могут обрабатываться одновременно."},
{"pop_port" ,1,0xFFFF,(uint *)&pop_port,   "TCP/IP порт для POP3 сервера. Обычно 110"},
{"pop_timeout"  ,30,0xFFFFF,(uint *)&POPTimeout,"POP3/SMTP таймаут по бездействию в секундах. По прошествии этого времени, после последней операции, соединение будет закрыто."},

RANGES(pop,rIP_RANGE,rIP_DENY)
OIPV6(pop,4)


{"pop3_proxy",1,FL1_POPROXY,(uint *)0, "Разрешить POP3 proxy" },


{"wmail",2,FL2_WMAIL,(uint *)0, "Разрешить Web mail" },
{"nowmailsent",1,0,(uint *)0, "Не сохранять сообщения отправленные через Web mail в папке пользователя" },
{"wmailsent" ,64,0,(uint *)&loc_sent, "Подкаталог для сохранения отправленных сообщений" },
{"nowmailtrash",1,0,(uint *)0, "Через Web mail удалять сообщения немедленно" },
{"wmailtrash" ,64,0,(uint *)&loc_trash, "Мусорная корзина для перемещения удаляемых сообщенний" },
{"wmail_utf",2,FL2_WMUTF, (uint *)0, "Конвертировать страницы в UTF8" },



SPD(4,pop)

 {0,0,0,0,"Параметры SMTP сервера"},
{"nosmtp_max",0,0,(uint *)0,"Запретить SMTP сервер."},
{"smtp_max"  ,1,1024   ,(uint *)&max_smtp,"Как много запросов могут обрабатываться одновременно."},
{"smtp_name" ,128,0,(uint *)&smtp_name,"Имя SMTP сервера. (Имя домена)"},
{"vhalias",0,FL_VHALIAS,(uint *)0,"Использовать все виртуальные хосты как синонимы имени домена."},
{"smtp_dns" ,128,0,(uint *)&dns_server_for_mail,"DNS сервер через который будет искаться информация о почтовом пути. (Это может быть ваш обычный DNS сервер)"},
{"smtp_nomx",1,FL1_MHST,(uint *)0, "При отсудствии почтового хоста, попробовать хост" },
{"nosmtpproxy",1,0,(uint *)0, "Это нормальный SMTP relay. (Иначе это всего лишь SMTP прокси)" },
{"smtpproxy" ,128,0,(uint *)&smtproxy , "SMTP верхнего уровня. (режим SMTP прокси)" },

{"smtp_port" ,1,0xFFFF,(uint *)&smtp_port,   "TCP/IP порт для SMTP сервера. Обычно 25"},
{"smtp_out",255,0,(uint *)&  out_path,"Output каталог. Каталог для сохранения сообщений перед отправкой. Указывайте полный путь."},
{"nosmtp_sent",6,0,(uint *)0,"Не сохранять отправленные сообщения."},
{"smtp_sent",255,0,(uint *)&sent_path,"Sent каталог. Каталог для сохранения сообщений после отправки."},
{"sent_time",1,0x6000,(uint *)&(smtp_chk.time),"Сколько дней должны храниться данные в кеше. (Ноль -- хранить вечно)"},

{"smtp_err",255,0,(uint *)&  err_path,"Error каталог. Каталог для сохранения сообщений которые не удалось доставить адресату."},
{"smtp_any",0,FL_SMTPANY,(uint *)0,"Принимать для отправки сообщения с любым полем \"From\". Иначе сервер будет отсылать сообщения только от извесный_пользователь@имя.домена "},

RANGES(smtp,"Наши IP адреса (разрешенный список)",rIP_DENY)
OIPV6(smtp,3)

{"smtp_pop_ip",1,FL1_SMTP_AVT,(uint *)0, "Временно добавлять IP в разрешенный список после авторизации по POP3" },

{"smtp_msg_limit" ,0x1000,0x1C00000,(uint *)&max_msg_size,"Максимальный размер сообщения. В байтах."},
{"smtp_nobreak",0,FL_NOBRKSMTP,(uint *)0,"Не прерывать соединение в случае привышения максимального размера." },

{"blacklist" ,0x8000,0,(uint *)&blacklist,"Черный список. E-mail адреса спамеров через пробел. (Можно *@имя.хоста чтобы запретить принимать сообщения от всех с этого хоста)"},
{"smtp_conform",1,FL1_CONFIRM,(uint *)0, "Разрешить Generate-Delivery-Report" },
{"forward",0,FL_FORWARD,(uint *)0,"Проверять наличие \"forward\" файла в каталоге пользователя и выполнять инструкции из него."},
{"fwdrun", 0,FL_FWDRUN ,(uint *)0,"Разрешить исполнение приложений из пользовательского \"forward\" файла."},

{"goodlist", 256,0 ,(uint *)&glst, "Белый список. Общий файл с допустимыми ш e-mail и IP адресами (шаблонами) отправителей"},
{"badlist", 256,0 ,(uint *)&blst, "Черный список. Общий файл со спамерскими e-mail и IP адресами (шаблонами) отправителей"},
{"graylist", 256,0 ,(uint *)&graylst, "Серый список. Общий файл с e-mail и IP адресами (шаблонами) требующими дополнительную проверку"},

{"chklists",1,FL1_CHKUSR ,(uint *)0, "Проверять файлы \"goodlist\", \"badlist\" и \"graylist\"  в каталоге пользователя перед приемом собщения" },
{"msgspam",200,0 ,(uint *)&msg_spam, "Текст который будет возвращаться в случае отказа приема сообщения. Здесь также можно указать URL Web формы для прямой отправки сообщения" },


{"noantivirus",1,0,(uint *)0, "Не использовать скриптов для входящей/исходящей почты" },
{"antivirus" ,256,0,(uint *)&antiv , "Антивирусный скрипт." },
{"run_timeout",30,0x100000,(uint *)&ttl_avr, "Предел времени на выполнение скрипта. В секундах." },
{"antispam" ,0x1000,0,(uint *)&antispam , "Фильтр разрыва (выражение). Переменные $msg,$sender,$hello,$control могут быть проверены." },
{"spamfltr" ,2048,0,(uint *)&spamfltr , "Фильтр спама (выражение). Переменные $msg,$sender,$hello,$control могут быть проверены и IPможет быть добавлен в список спамеров." },

{"nocheckback",1,FL1_NOCHKBCK,(uint *)0,"Принимать сообщения с неправильным обратным адресом" },
{"fake" ,0x102,0,(uint *)&fake, "Фальшивые e-mail адреса, через запятую. Если кто-то попытается послать сообщеение на эти адреса, он будет добавлен в список спамеров" },
{"dnsbl" ,320,0,(uint *)&dnsbl, "DNSBL сервер. Запрашивать IP у этого внешнего спамерскго списка, перед приемом сообщений." },
{"checkmx",2,FL2_CHKMX,(uint *)0, "Перед приемом сообщения проверить почтовый хост отправителя" },
{"mxignbl",2,FL2_MX_GLIST,(uint *)0, "Игнорировать серый список если сообщение пришло с орегинального почтового домена (DNS MX) " },

{"spam_time" ,0,0x7FFFFFFF,(uint *)(ltime+1), "Как долго спамерские IPs будут активны в спамерском списке (в секундах)" },

 XLIMIT(smtp,"SMTP",0),

{"nolimitus",1,FL1_NOLUS,(uint *)0, "Не ограничивать разрешенные IP" },
{"uncheckip",2,FL2_MLNOIP,(uint *)0, "Принимать сообщения от нашего домена, с чужих IP адресов" },
{"time_btw"  ,2,1024   ,(uint *)&time_btw, "Минимальное время между отправкой сообщений" },



{0,0,0,0, "DHCP сервер" },
{"nodhcp_max",0,0,(uint *)0, "Запретить DHCP" },
{"dhcp_max"   ,0,0xFEFEFE ,(uint *)&total_dhcp_ip, "Общие количество выделяемых IP адресв" },
{"dhcp_ip",16,0,(uint *)&dhcp_addr, "IP адрес DHCP сервера" },
{"dhcp_bcast",16,0,(uint *)&dhcp_bcast, "Широковещательный адрес для DHCP ответов" },
{"dhcp_first" ,16,0,(uint *)&first_dhcpc, "Начальный IP адрес" },
{"dhcp_mask" ,16,0,(uint *)&netmaskc, "Маска подсети" },
{"dhcp_gate" ,16,0,(uint *)&gatewayc, "Гейт (Gateway)" },
{"dhcp_dns" ,64,0,(uint *)&dhcp_dnsc, "DNS сервера" },
{"dhcp_name" ,64,0,(uint *)&DhcpServerName, "Имя домена" },
{"dhcp_file" ,256,0,(uint *)&dhcp_fname, "Файл сохранения состояния" },
{"dhcp_rdns" ,1,FL1_DHCPDNS,(uint *)0, "DNS должен резолвить имена хостов от выделенных IP адресов" },
{"dhcp_lo" ,1,FL1_DHCPLO,(uint *)0, "Только слушать, сохранять хосты для NS. (не отвечать)" },


{0,0,0,0, sTLS__SERV },
{"notls_max",0,0,(uint *)0, "Запретить SSL/TLS сервер."},
{"tls_max"  ,0,1024 ,(uint *)&max_ssl,"Как много запросов могут обрабатываться одновременно."},
{"tls_port" ,1,0xFFFF,(uint *)&ssl_port,"TCP/IP порт для SSL/TLS сервера. Обычно 443"},

RANGES(ssl,rIP_RANGE,rIP_DENY)
OIPV6(tls,5)


SPD(5,tls)

{"smtp_tls",1,FL1_SMTPTLS,(uint *)0,"Разрешить TLS для POP3/SMTP" },

{"tls_lib",256,0,(uint *)&TLSLibrary,  "DLL библиотека TLS/SSL. Например seclib.dll"},
{"tls_cert_file",256,0,(uint *)&s_cert_file,"Файл сертификата"},
{"tls_key_file",256,0,(uint *)&s_key_file, "Key-файл"},
{"tls_capath",256,0,(uint *)&CApath,"CA-file"},
{"tls_cafile",256,0,(uint *)&CAfile,"CA-Path"},
{"admtls",1,FL1_ATLS,(uint *)0, "Удаленное администрирование только через защищеный HTTPS" },
{"tls_wmail",2,FL2_WMTLS,(uint *)0, "Web mail только через защищеный HTTPS" },


#endif

#ifndef FREEVER

{"registr_user",128,0,(uint *)&user_name,0},
{"registr_code",0,0xFFFFffff,(uint *)&chk_code3a,0},
{"registr_code1",0,0xFFFFffff,(uint *)&chk_code3b,0},
#endif

{0,0,0,(uint *)0,0}
};

#undef RSTNEED
#define RSTNEED "(Необходим перезапуск)"

