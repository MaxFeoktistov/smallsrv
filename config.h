
#ifndef CONFIG_H
#define CONFIG_H

#ifdef CONFIG_CURRENT_DIR

#define CONFIG_BASE   ""
#define CONFIG_APPDIR ""
#define CONFIG_LANG   ""
#define CONFIG_DATA   ""
#define CONFIG_CONFIG ""
#define CONFIG_LOG    ""
#define CONFIG_TMP    ""

#elif ! defined(SYSUNIX)
// Windows definitions

#define CONFIG_BASE   "c:\\shttps\\"
#define CONFIG_APPDIR CONFIG_BASE
#define CONFIG_SHARE  CONFIG_BASE
#define CONFIG_DATA   CONFIG_BASE
#define CONFIG_CONFIG CONFIG_BASE
#define CONFIG_LOG    CONFIG_BASE
#define CONFIG_TMP    CONFIG_BASE

#else

#define CONFIG_CONFIG "/etc/smallsrv/"
#define CONFIG_BASE "/usr/local/"
#define CONFIG_DATA "/var"
#define CONFIG_SHARE "/usr/local/share/smallsrv/"
#define CONFIG_LOG "/var/log/smallsrv/"
#define CONFIG_APPDIR "/usr/local/lib/smallsrv/"

#endif

#endif
