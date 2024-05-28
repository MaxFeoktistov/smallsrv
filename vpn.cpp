/*
 * Copyright (C) 1999-2024 Maksim Feoktistov.
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
#include "mstring1.h"
#include "vpn.h"

#ifdef SEPLOG

#undef debug
#undef AddToLog

#define debug(a...)  sepLog[VPN_LOG]->Ldebug(a)
#define AddToLog(a...)  sepLog[VPN_LOG]->LAddToLog(a)

#endif

const char FmtShortVPN []=">>%.256s VPN   in:%llu out:%llu time: %us %s\r\n";



//#include <stdio.h>

extern "C" {

#ifndef VPN_WIN
int tuntap_fds[3] = {-1,-1,-1};
#undef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE (-1)
#define CancelIo(a)
#undef HANDLE
#define HANDLE int
#else
HANDLE tuntap_fds[3] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE};
OVERLAPPED vpnOverlapped[3];
void  *tap_waitbfr[3];
#endif

#define tun_fd tuntap_fds[0]
#define tap_fd tuntap_fds[1]

#define INDEX_TUN  0
#define INDEX_TAP  1
#define INDEX_CLIENT  2
#define IS_TAP(index)  ( (index) == INDEX_TAP || ((index)==INDEX_CLIENT && TAP_CLIENT) )
#define IS_TUN(index)  ( (index) == INDEX_TUN || ((index)==INDEX_CLIENT && !(TAP_CLIENT) ) )

//const char * TUNTAPNames[3] = {"TUN","TAP","TAP"};
const char * TUNTAPNames[3] = {"tun","tap","tap"};

#ifdef VPN_WIN
char * vpnIfNames[3];
char *tundev="tap0901";

struct AsincReadHelper_t
{
  HANDLE hfile;
  HANDLE hevent;
  OVERLAPPED ovd;
  int index;
  int current;
  union {
    short pkt_len;
    uchar pkt[MAX_MTU];
  } u[2];
};
static AsincReadHelper_t * AsincReadHelper[3];

#endif

#define MCAST_DETECT  0xE0
#define MCAST_MASK    0xF0

#define BROADCAST_ANY 0xFFFFffff


VPNclient **vpn_list;

maxFdSet maxVPNset;

int vpn_max;
int vpn_count;
//volatile
int vpn_mutex;
#ifdef VPN_LINUX
char *tundev="/dev/net/tun";
#ifndef USE_TUN_PI
#define TUN_PI  IFF_NO_PI
#else
#define TUN_PI  0
#endif
static int tuntap_flags[3] = {IFF_TUN | TUN_PI, IFF_TAP|IFF_NO_PI, IFF_TAP|IFF_NO_PI };
#endif
int  tuntap_number[3];
char* tuntap_ipv4[3]= {"192.168.111.1","192.168.112.1"};
char* tuntap_ipv4nmask[3] = {"255.255.255.0","255.255.255.0"};
char* tuntap_ipv6[3];
uint tuntap_ipv6plen[3];
uint vpn_rescan_us=500000;
char* vpn_first_remote_ipc[2]={"192.168.111.20","192.168.112.20"};
uint vpn_first_remote_ip[2];
uint vpn_total_remote_ip[2] = {128,128} ;
uint vpn_next_remote_ip[2];
//unsigned long long vpn_amask_remote_ip[2];
uint vpn_allocated_remote_ip;
//char* vpn_gwc[2] = {"192.168.111.1","192.168.112.1"};
//uint vpn_gw[2];
//uint vpn_nmask[2];
char *vpn_dns[2] = {"192.168.111.1, 8.8.8.8, 4.4.4.4","192.168.112.1, 8.8.8.8, 4.4.4.4"};
char *vpn_scripts_up[3]
#ifdef VPN_WIN
 = { "vpn_if_up.bat", "vpn_if_up.bat", "vpn_if_client_up.bat"}
#endif
;
char *vpn_scripts_down[3]
#ifdef VPN_WIN
= {0,0, "vpn_if_client_down.bat"}
#endif
;
VPNclient * vpn_cln_connected;
int  vpn_mtu[3]={9000,9000,9000};

long long vpn_mac[3];

unsigned long long VPNdroped;
unsigned long long VPNsendet;
unsigned long long VPNreceved;
unsigned long long VPNsendet_pkt;
unsigned long long VPNreceved_pkt;

int  rt_fd;

////// Client Vars

char *vpn_remote_host;
int vpn_client_port=443;
char * vpn_passw=NullString, *vpn_user=NullString;
char vpn_notice[48];
char vpn_opaque[48];
char vpn_realm[32];
uint  vpn_client_ip;
long long vpn_client_mac;
uint client_nmask;
char *client_dns;


VPNUserLimit *vpn_limits;


void OnPktFromIf(uchar *pkt, int i);
int tun_up(uint index, uint ip, uint mask, uint gw, char *dns);



int  RunScript(char *cmd)
{
#ifdef VPN_LINUX
  int r = system(cmd);
  if(r == -1) debug("vpn: Can't run script '%s' error:%d %s\r\n", cmd, errno, strerror(errno) );
  return r;
#elif defined(VPN_WIN)
  DBGLS(cmd);
  char *p;
  char dir[256];
  strncpy(dir,cmdline,254);
  p=strrchr(dir,'\\');
  if(p){
     if(dir[0] == '"' || dir[0] == '\'') *p++ = dir[0];
     *p=0;
  }
  else
  {
    GetCurrentDirectory(255,dir);
  }

  int r;
  r = (int) ShellExecute(0,"runas","cmd.exe",cmd, dir,
                             (s_flgs[3]&FL3_VPN_SCRKEEP)? SW_SHOWNORMAL : SW_HIDE);
#if 0
  p=strchr(cmd,' ');
  if(!p) p="";
  else
  {
    *p++=0;
  }
  int r = (int) ShellExecute(0,"runas",cmd,p,dir,SW_SHOWNORMAL /*SW_HIDE !!!*/);
#endif
  if(r < 32)
  {
    int err=GetLastError();
    debug("**** Error run script %s ; error code=%d ; %d %s\r\n",cmd,r,err,strerror(err));
  }
  return r;
#endif
}

int RunDownScript(int index, int ip)
{
  char cmd[300];
  char  client_ip[48];
  client_ip[0] = 0;
  if(INDEX_CLIENT == index && vpn_cln_connected)
  {
    IP2S(client_ip + 1, &vpn_cln_connected->sa_c);
    client_ip[0] = ' ';
  }

  if(vpn_scripts_down[index] && vpn_scripts_down[index][0])
  {
#ifdef VPN_LINUX
    sprintf(cmd, "%s %s%u %u.%u.%u.%u%s", vpn_scripts_down[index],
            TUNTAPNames[index], tuntap_number[index],
            ip>>24, (ip>>16)&0xFF, (ip>>8)&0xFF, ip&0xFF,
            client_ip
    );
#elif defined(VPN_WIN)
    char  *qt="";
    if(vpn_scripts_down[index][0] != '"' && strchr(vpn_scripts_down[index],' ') ) qt="\"";

    sprintf(cmd, "/S /%c %s%s%s \"%s\" %u.%u.%u.%u",
            ((s_flgs[3]&FL3_VPN_SCRKEEP)? 'K':'C'),
            qt,vpn_scripts_down[index],qt,
            vpnIfNames[index],
            ip>>24, (ip>>16)&0xFF, (ip>>8)&0xFF, ip&0xFF,
            client_ip
    );
#else
#error "TODO: "
#endif
    DBGLS(cmd);

    return RunScript(cmd);
  }
  return 0;
};

#ifdef VPN_LINUX
int tun_up(uint index, uint ip, uint mask, uint gw, char *dns )
{
  int aflag = 0;
  int r;
  char *t;
  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));

  sprintf(ifr.ifr_name, "%s%u", TUNTAPNames[index], tuntap_number[index]);


  if(ip)
  {

    DBGLS(ifr.ifr_name);

    ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
    ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip;
    if( ioctl(rt_fd, SIOCSIFADDR, &ifr)  )
      debug("VPN: can't set IP %X for interface %s: %d %s\r\n", htonl(ip), ifr.ifr_name, errno, strerror(errno) );


    ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
    ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = mask;
    if( ioctl(rt_fd, SIOCSIFNETMASK, &ifr) )
      debug("VPN: can't set netmask %X for interface %s: %d %s\r\n", mask , ifr.ifr_name, errno, strerror(errno) );

    if(~mask)
    {
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip | ~mask;
      if( ioctl(rt_fd, SIOCSIFBRDADDR, &ifr) )
        debug("VPN: can't set broadcast address for interface %s: %d %s\r\n", ifr.ifr_name, errno, strerror(errno) );
    }

    if(index == INDEX_CLIENT)
    {
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = gw;
      ioctl(rt_fd, SIOCSIFDSTADDR, &ifr);
    }
    else  aflag |= IFF_BROADCAST;
  }

  if(vpn_mtu[index] > MIN_MTU)
  {
    ifr.ifr_mtu = vpn_mtu[index];
    if( (r = ioctl(rt_fd, SIOCSIFMTU, &ifr) ) < 0 )
      debug("VPN: can't set MTU interface %s: %d %d %s\r\n", ifr.ifr_name, r, errno, strerror(errno) );
  }
  //ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_RUNNING | aflag;
  if( (r = ioctl(rt_fd, SIOCGIFFLAGS, &ifr) ) < 0 )
    debug("VPN: can't get flags interface %s: %d %d %s\r\n", ifr.ifr_name, r, errno, strerror(errno) );
  else aflag |= ifr.ifr_flags;

  //    ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_ECHO | aflag;
  ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_DYNAMIC  | IFF_MULTICAST // | IFF_BROADCAST
  | IFF_AUTOMEDIA | aflag;

  if( (r=ioctl(rt_fd, SIOCSIFFLAGS, &ifr) ) )
  {
    debug("VPN: can't UP %X interface %s: %d %d %s\r\n", ifr.ifr_flags, ifr.ifr_name, r, errno, strerror(errno) );
  }

  // TODO: get IPv6

  DBGLA("%u '%s'", index, vpn_scripts_up[index])

  if(vpn_scripts_up[index] && vpn_scripts_up[index][0] )
  {
    char  cmd[512];
    char  client_ip[48];
    char  *dnsqt= NullString;
    client_ip[0] = 0;

    if(INDEX_CLIENT == index && vpn_cln_connected)
    {

      IP2S(client_ip + 1, &vpn_cln_connected->sa_c);
      client_ip[0] = ' ';
      DBGLA("Client %s %s", client_ip + 1, client_ip)
    }

    sprintf(cmd, "%s %s%u %u.%u.%u.%u %u.%u.%u.%u %u.%u.%u.%u '%s'%s", vpn_scripts_up[index],
            TUNTAPNames[index], tuntap_number[index],
            ip&0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF,  ip>>24,
            mask&0xFF,  (mask>>8)&0xFF,  (mask>>16)&0xFF,   mask>>24,
            gw&0xFF,  (gw>>8)&0xFF,  (gw>>16)&0xFF,   gw>>24,
            ((dns)?dns:NullString), client_ip
    );

    DBGLS(cmd);

    RunScript(cmd);
  }

  return r;

}
#endif

#ifdef VPN_LINUX

int tun_alloc(int index)
{
    struct ifreq ifr;
    int fd, err;
    uint ip=0,m=0;
    uint aflag=0;
    int r;
    char *t;

    if( (fd = open(tundev, O_RDWR | O_CLOEXEC)) < 0 )
    {
       sleep(5);
       if( (fd = open(tundev, O_RDWR | O_CLOEXEC)) < 0 )
         return -1;
    }
    tuntap_fds[index] = fd;

    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags =  tuntap_flags[index]; //IFF_TUN;
    sprintf(ifr.ifr_name, "%s%u", TUNTAPNames[index], tuntap_number[index]);

    DBGLS(ifr.ifr_name);

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 )
    {
      debug("Can't open tun device %s. %d %d %s", ifr.ifr_name, err, errno, strerror(errno));
      close(fd);
      return err;
    }

    if(rt_fd<=0) rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);
    if( tuntap_flags[index] & IFF_TAP )
    {
      if( (r=ioctl(rt_fd,  SIOCGIFHWADDR, &ifr) ) )
        debug("VPN: can't get hw address interface %s: %d %d %s\r\n", ifr.ifr_flags, ifr.ifr_name, r, errno, strerror(errno) );
      else
      {
#ifdef ARM
        memcpy(vpn_mac+index, ifr.ifr_hwaddr.sa_data, 6);
#else
        vpn_mac[index] = DDWORD_PTR(ifr.ifr_hwaddr.sa_data[0]) & 0xFFFFffffFFLL;
#endif
        DBGLA("Tap MAC: %llX", vpn_mac[index])
      }
    }
    if(index < 2)
    {
      maxVPNset.Set(fd);
      ip = 0;
      m = 0;
      if(tuntap_ipv4[index] && tuntap_ipv4nmask[index] )
      {
        ip = ConvertIP(t=tuntap_ipv4[index]);
        m = ConvertIP(t=tuntap_ipv4nmask[index]);
      }
      tun_up(index, ip, m, ip, 0 );
    }
    return fd;
}
#endif

#if defined(VPN_WIN)

//int get_guid(const char *src_name, char *ret_bfr)
int get_guid(int index, char *ret_bfr)
{

  HKEY  adapter_key;
  HKEY unit_key;
  LONG  status;
  DWORD len;
  DWORD data_type;
  HKEY network_connections_key;

  HKEY connection_key;



  char enum_name[300];

  char component_id[256];
  BYTE net_cfg_instance_id[256];
  BYTE name_data[256];
  char name[200];
  char connections_str[256];

  int  i = 0;
  int  ii = 0;
  int  n;
  char *p;
  int r=0;


  if (RegOpenKeyEx( HKEY_LOCAL_MACHINE, ADAPTER_KEY, 0, KEY_READ, &adapter_key) != ERROR_SUCCESS)
  {
    DBGL("ADAPTER not open")
    return 0;
  }
  if (RegOpenKeyEx( HKEY_LOCAL_MACHINE, NETWORK_CONNECTIONS_KEY, 0, KEY_READ, &network_connections_key) == ERROR_SUCCESS)
  {
    n=sprintf(enum_name,"%s\\", ADAPTER_KEY);
    DBGLS(enum_name)
    while(1)
    {
      len = sizeof(enum_name) - n;
      status = RegEnumKeyEx( adapter_key, i, enum_name + n, &len, NULL, NULL, NULL, NULL);

      if (status == ERROR_NO_MORE_ITEMS) break;
      // DBGLA("enum %s %X", enum_name, status )

      if (status == ERROR_SUCCESS)
      {
        if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, enum_name, 0, KEY_READ, &unit_key) == ERROR_SUCCESS)
        {
          //            DBGL("")
          len = sizeof(component_id);
          if(
            (
              RegQueryValueEx( unit_key, "ComponentId", NULL, &data_type, (LPBYTE)component_id, &len) == ERROR_SUCCESS &&
              data_type == REG_SZ
              && (p=strcasestr(component_id, tundev) )
              && (p == component_id || p[-1]=='\\' )
              //&& p[3]<='9'
            ) ||
            (
              RegQueryValueEx( unit_key, "DriverDesc", NULL, &data_type, (LPBYTE)component_id, &len) == ERROR_SUCCESS &&
              data_type == REG_SZ &&
              strcasestr(component_id, "TAP-Windows")
            )
          )
          {
            DBGLA("found %s!", component_id)
            len = sizeof(net_cfg_instance_id);
            if(RegQueryValueEx( unit_key, "NetCfgInstanceId", NULL, &data_type, net_cfg_instance_id, &len)  == ERROR_SUCCESS &&
              data_type == REG_SZ)
            {
              RegCloseKey(unit_key);
              sprintf(connections_str, "%s\\%s\\Connection",NETWORK_CONNECTIONS_KEY, net_cfg_instance_id);
              //DBGLS(connections_str)
              if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, connections_str, 0, KEY_READ, &connection_key) == ERROR_SUCCESS )
              {
                len = sizeof(name_data);
                status = RegQueryValueExW( connection_key, L"Name", NULL, &data_type, (LPBYTE) name_data, &len);
                RegCloseKey(connection_key);
                //DBGLA("Open key ok, QueryValue = %X", status)

                if (status != ERROR_SUCCESS || data_type != REG_SZ)
                {
                  DBGLA("Error read registry key: %s\\Name",  connections_str);
                }
                else
                {
                  WideCharToMultiByte(CP_UTF8, 0, (WCHAR *)name_data, -1, name, sizeof(name), NULL, NULL);

                  DBGLA("name = '%s'", name)

                  if( vpnIfNames[index] && vpnIfNames[index][0] &&
                     ( !strcasecmp((char *)name, vpnIfNames[index]) ) )
                  {
                    strncpy(ret_bfr, (char *) net_cfg_instance_id, 256);
                    r = 1;
                    break;
                  }
                  if(ii == tuntap_number[index])
                  {
                    len = strlen(name);
                    vpnIfNames[index] = (char *) malloc(len+2);
                    strcpy(vpnIfNames[index],name);
                    strncpy(ret_bfr, (char *) net_cfg_instance_id, 256);
                    r = 1;
                    break;
                  }
                  ii++;
                }
              }
              else
              {

                DBGLA("Error opening registry key: %s",  connections_str);

              }

            }
          }
        }
        else
        {
          DBGLA("Cant open key %s %X", enum_name, status )
        }
      }
      i++;
    }
    RegCloseKey(network_connections_key);
  }
  // lb_close_ad:
  RegCloseKey(adapter_key);

  return r;
}

const char * windevdir[] = {
  USERMODEDEVICEDIR,
  SYSDEVICEDIR,
  USERDEVICEDIR
};
HANDLE win_tun_open(int index)
{
  HANDLE h;
  DWORD len;
  int i;
  char guid[256];
  char path[256];

  if(! get_guid(index,guid) )
  {
    debug("Can't get GUID of %u", index);
    return INVALID_HANDLE_VALUE;
  }


  sprintf(path, USERMODEDEVICEDIR "%s" TAP_WIN_SUFFIX, guid);


  DBGLA("Using device interface: %s", path);

  h = CreateFileA(path,
                  //MAXIMUM_ALLOWED,//
                  GENERIC_READ | GENERIC_WRITE,
                  0,         /* was: FILE_SHARE_READ */
                  //&secat
                  0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED, 0);
  if (h != INVALID_HANDLE_VALUE)
  {
    SECURITY_ATTRIBUTES sa;
    SECURITY_DESCRIPTOR sd;
    memset(&sa, 0, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = TRUE;
    if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
    {
      debug("initialize security descriptor failed\r\n");
    }
    else if (!SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE)) {
      debug("Set security descriptor dacl failed\r\n");
    }
    else  SetKernelObjectSecurity(h, DACL_SECURITY_INFORMATION, &sd);

    // get info from the fd
    {
      ULONG info[3];
      if (DeviceIoControl(h, TAP_WIN_IOCTL_GET_VERSION, info, sizeof(info), &info, sizeof(info), &len, NULL))
      {
        debug("%s:%s: TAP-Windows Driver Version %d.%d %s\r\n", vpnIfNames[index], guid , (int)info[0], (int)info[0], (info[2] ? "(DEBUG)" : NullString));
      } else {
        debug("%s:%s: Device io control GET_VERSION failed\r\n", vpnIfNames[index], guid);
      }
    }
  }
  else
  {
    int err;
    err = GetLastError();
    debug("Can't open device: %s; Error code:%d %s", path, err, strerror(err));
  }

  return h;

}


HANDLE tun_alloc(int index)
{
  HANDLE h = win_tun_open( index ); //vpnIfNames[index]);
  char *t;
  if(h != INVALID_HANDLE_VALUE && index != INDEX_CLIENT)
  {
      uint ip = 0;
      uint m = 0;
      DBGLA("%d %lX",index, (long)h)

      tuntap_fds[index] = h;

      if(tuntap_ipv4[index] && tuntap_ipv4nmask[index] )
      {
        ip = ConvertIP(t=tuntap_ipv4[index]);
        m = ConvertIP(t=tuntap_ipv4nmask[index]);
      }
      if(index<2) tun_up(index, ip, m, ip, 0 );
  }
  return h;
};



void AsincReadServCB(AsincReadHelper_t *thi)
{
  int next = thi->current^1;
  DWORD len;
  if(GetOverlappedResult(thi->hfile, &thi->ovd, &len, 0) )
  {
    ResetEvent(thi->hevent);
    memset(&thi->ovd, 0, offset(OVERLAPPED, hEvent));
    ReadFile(thi->hfile, thi->u[next].pkt+2, MAX_MTU, 0, &thi->ovd);
    thi->u[thi->current].pkt_len = len;
    OnPktFromIf(thi->u[thi->current].pkt, thi->index);
    thi->current = next;
  }
}

void AsincReadClientCB(AsincReadHelper_t *thi)
{
  int next = thi->current^1;
  DWORD len=0;
  if(GetOverlappedResult(thi->hfile, &thi->ovd, &len, 0) )
  {
    ResetEvent(thi->hevent);
    memset(&thi->ovd, 0, offset(OVERLAPPED, hEvent));
    if(vpn_cln_connected)
    {
      ReadFile(thi->hfile, thi->u[next].pkt+2, MAX_MTU, 0, &thi->ovd);
      thi->u[thi->current].pkt_len = len;
      vpn_cln_connected->Send(thi->u[thi->current].pkt,len+2);
    }
    thi->current = next;
  }
}

static void AddAsincCB(int index)
{
  AsincReadHelper_t *p;
  if(! AsincReadHelper[index])
  {
    p = (AsincReadHelper_t *) malloc(sizeof(AsincReadHelper_t));
    if(p)
    {
      p->index = index;
      p->hevent = CreateEvent(NULL, FALSE, FALSE, NULL);
      AddASyncIO( (index==INDEX_CLIENT)?
                  (tfASyncIOHelperCB) AsincReadClientCB :
                  (tfASyncIOHelperCB) AsincReadServCB, p, p->hevent);
      AsincReadHelper[index] = p;
   lbInitRead:
      p->current = 0;
      p->hfile = tuntap_fds[index];
      memset(&p->ovd, 0, offset(OVERLAPPED, hEvent));
      p->ovd.hEvent = p->hevent;
      ReadFile(p->hfile, p->u[0].pkt+2, MAX_MTU, 0, &p->ovd);
    }
  }
  else
  {
    p = AsincReadHelper[index];
    ResetEvent(p->hevent);
    goto lbInitRead;
  }
}

int tun_up(uint index, uint ip, uint mask, uint gw, char *dns)
{

  DWORD len;
  ULONG x;
  HANDLE handle = tuntap_fds[index];
  int r;
  int lst;
  DWORD ep[4];

  DBGLA("index = %u ip=%X mask=%X gw=%X", index, ip, mask, gw);

    if(ip)
    {
      if( IS_TAP(index) )
      {
        DBGLA("TAP %u", index)
        if(ip) {
          /* We will answer DHCP requests with a reply to set IP/subnet to these values */
          ep[0] = ip;
          ep[1] = mask;
          ep[2] = gw;
          ep[3] = 0x7FFFffff;

          if(!DeviceIoControl(handle, TAP_WIN_IOCTL_CONFIG_DHCP_MASQ, ep, sizeof(ep), ep, sizeof(ep), &len, NULL))
          {

            lst = GetLastError();

            debug("*****ERROR: The TAP-Windows driver rejected a call to set TAP_WIN_IOCTL_CONFIG_DHCP_MASQ mode %d %s",lst, strerror(lst));
          }
        }

        if(!DeviceIoControl(handle, TAP_WIN_IOCTL_GET_MAC, &vpn_mac[index] , 6 , &vpn_mac[index], 6, &len, NULL))
        {
          lst = GetLastError();
          debug("ERROR: The TAP-Windows driver rejected a call to get TAP_WIN_IOCTL_GET_MAC  %d %s",lst, strerror(lst));
        }
      }
      else //if(ip == gw)
      {
        ep[0] = ip;
        ep[1] = ip & mask;
        ep[2] = mask;

        DBGLA("TUN server %u", index)

        if(!DeviceIoControl(handle, TAP_WIN_IOCTL_CONFIG_TUN, ep, 12, ep, 12, &len, NULL))
        {
          lst = GetLastError();
          debug("****Setting device to TAP_WIN_IOCTL_CONFIG_TUN failed %d %d %s\r\n",r,lst, strerror(lst) );
        }

      }
#if 0
      else // tun ip!=gw
      {
        ep[0] = ip;
        ep[1] = gw;
       // ep[2] = mask;

        if(!DeviceIoControl(handle, TAP_WIN_IOCTL_CONFIG_POINT_TO_POINT, ep, 8, ep, 8, &len, NULL))
        {
          lst = GetLastError();
          debug("****Setting device to CONFIG_POINT_TO_POINT failed %d %s\r\n",lst, strerror(lst) );
        }
      }
#endif
    }

    // set to connected
    x = 1;
    if((r=DeviceIoControl(handle, TAP_WIN_IOCTL_SET_MEDIA_STATUS, &x, sizeof(x), &x, sizeof(x), &len, NULL)))
    {
      debug("Setting device to CONNECTED\r\n");
    } else {
      lst = GetLastError();
      debug("*****Setting device to CONNECTED failed %d %d %s\r\n",r,lst, strerror(lst) );
      return -1;
    }

    if(vpn_scripts_up[index] && vpn_scripts_up[index][0] )
    {
      char  *qt=NullString;
      char  cmd[512];
      char  client_ip[32];
      client_ip[0] = 0;
      if(INDEX_CLIENT == index && vpn_cln_connected)
      {
        client_ip[0] = ' ';
        IP2S(client_ip+1, &vpn_cln_connected->sa_c);
      }
      if(vpn_scripts_up[index][0] != '"' && strchr(vpn_scripts_up[index],' ') ) qt="\"";
      sprintf(cmd, "/S /%c %s%s%s \"%s\" %u.%u.%u.%u %u.%u.%u.%u %u.%u.%u.%u \"%s\"%s",
              ((s_flgs[3]&FL3_VPN_SCRKEEP)? 'K':'C'),
              qt,vpn_scripts_up[index],qt,
              vpnIfNames[index],
              ip&0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF,  ip>>24,
              mask&0xFF,  (mask>>8)&0xFF,  (mask>>16)&0xFF,   mask>>24,
              gw&0xFF,  (gw>>8)&0xFF,  (gw>>16)&0xFF,   gw>>24,
              ((dns)?dns:NullString), client_ip
      );

      DBGLS(cmd);

      RunScript(cmd);
    }
    AddAsincCB(index);
    return r;
}

int SynhWait(int index)
{
  if(! (tap_waitbfr[index]) ) return 1;

  DWORD n=0;
  if(WaitForSingleObject(vpnOverlapped[index].hEvent,500) != WAIT_OBJECT_0 )
  {
    debug("*****TAP Write timeout %d %d", index, n);
  }
  tap_waitbfr[index] = 0;
  if(!GetOverlappedResult(tuntap_fds[index], &vpnOverlapped[index], (DWORD *) &n, 0) )
  {
    int err = GetLastError();
    debug("*****TAP Write error %d %d %d %s", index, n, err, strerror(err) );
    return 0;
  }

  return n;
}

int SynhWrite(int index, void *b, int n)
{
  DWORD nn;
  if(!vpnOverlapped[index].hEvent) {
    vpnOverlapped[index].hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  }
  SynhWait(index);
  memset(&vpnOverlapped[index],0, offset( OVERLAPPED, hEvent) );
  ResetEvent(vpnOverlapped[index].hEvent);
  if(!WriteFile(tuntap_fds[index],b,n,(DWORD *)&nn, &vpnOverlapped[index]))
  {
    if(GetLastError() !=  ERROR_IO_PENDING)
    {
      int err = GetLastError();
      debug("TAP%u write error %d %s\r\n", index, err, strerror(err));
      return 0;
    }
    tap_waitbfr[index] = b;
  }
  return n;
}


#endif

#ifdef DEBUG_VERSION
void print_pkt(int index, uchar *pktl)
{
  iphdr *iph;
  if(IS_TAP(index))
  {
#define ppkt ((VPN_TAPPacket *) pktl)
#ifdef ARM
//     debug("%X:%08X%02X > %08X%02X: ", ppkt->eth.ether_type,
//           DWORD_PTR(ppkt->eth.ether_shost[0]) , WORD_PTR(ppkt->eth.ether_shost[4])
//           DWORD_PTR(ppkt->eth.ether_dhost[0]) , WORD_PTR(ppkt->eth.ether_dhost[4])
//          );

#else
    debug("%X:%llX > %llX: ", ppkt->eth.ether_type,
          DDWORD_PTR(ppkt->eth.ether_shost[0]) & 0xFFffffFFFF,
          DDWORD_PTR(ppkt->eth.ether_dhost[0]) & 0xFFffffFFFF
         );
#endif
    iph=&ppkt->ip4;
#undef ppkt
  }
  else
  {
    iph=&((VPN_TUNPacket *) pktl)->ip4;
  }
  debug("%u:%d ip%u (%X:%X) %X>%X\r\n", index, WORD_PTR(*pktl),
        iph->version,
        iph->protocol,
        iph->id,
        iph->saddr,
        iph->daddr
  );
}
#endif

void CloseVPNClient(int i)
{

  AddToLog(0, vpn_list[i]->s, & vpn_list[i]->sa_c46, FmtShortVPN,"Connection closed.", vpn_list[i]->Tin, vpn_list[i]->Tout, (GetTickCount() - vpn_list[i]->tmout)/1000, vpn_list[i]->a_user? vpn_list[i]->a_user->name: "" );

  maxVPNset.Clear(vpn_list[i]->s);
  //SecClose((OpenSSLConnection*) vpn_list[i]->Adv);
  //CloseSocket(vpn_list[i]->s);
  vpn_list[i]->Close();


  MyLock(vpn_mutex);
  free(vpn_list[i]);
  vpn_count --;
  if(i != vpn_count)
  {
    vpn_list[i] = vpn_list[vpn_count];
  }
  MyUnlock(vpn_mutex);
}

//////////////////////////////////////////////
int IsVPN_IP_Free(uint ip)
{
  int i;
  for(i=0; i<vpn_count; i++)
    if(vpn_list[i]->ipv4 == ip) return 0;
  return 1;
}
///////////////////
int Req::InsertVPNclient()
{
  VPNclient *cl;
  char *rnd, *pwdcode;
  int l;
  User *tuser = 0;
  char *t,*t1;
  char *p;
  int isTap;
  ulong ip = 0;
  long long mac;
  int  reconnect = 0;
  uint id_ip = 0;
  VPNUserLimit *lmt=0;


  //DBGL("");

  if(vpn_count >= vpn_max)
  {
    debug("Too many VPN clients. Can't insert new\r\n");
    return -1;
  }
  //DBGL("");

   if( ( ! (s_flgs[3] & FL3_VPN_PUBLIC) )  && ! (tuser=ChUser(UserHTTP)) )
   {
 // lbLogout:

      //DBGL("");

#ifdef WITHMD5
       SendDigestAuthReq(loc);
#else
       Send(AuthErr, strlen(AuthErr)  ) ;// sizeof(AuthErr)-1);
#endif
    return -1;
   }

   DBGL("");

   isTap = 0;
   if( (t =  GetVar(http_var,"tap") ) ) isTap++;
   else t =  GetVar(http_var,"tun");
   if( (!t) || tuntap_fds[isTap] == INVALID_HANDLE_VALUE )
   {
     HttpReturnError("Error. Selected VPN type disabled...");
     return -1;
   }

   if( s_flgs[3] & (FL3_VPN_ULIMIT|FL3_VPN_ULIMIT ) ) {
     lmt = ((VPNclient *)this)->SetLimit();
     if(((VPNclient *)this)->CheckVPNLimits()) {
       HttpReturnError("Limit overflow...");
       return -1;
     }
   }

   DBGLS(t);
   if( (t1=GetVar(http_var,"reconnect")) )
   {
     id_ip = atouix(t1);
     for(int i=0; i<vpn_count; i++)
     {
       if(id_ip == vpn_list[i]->ipv4)
       {
         if(tuser == vpn_list[i]->a_user)
         {
           cl = vpn_list[i];
           maxVPNset.Clear(cl->s);
           cl->Close();
           ip = cl->ipv4;
           reconnect++;
           debug("Reconnect...\r\n");
           goto lb_reconnect;
         }

         debug("Bad user for reconnect...\r\n");

         break;
       }
     }


     debug("Not found for reconnect %u.%u.%u.%u...\r\n", id_ip&0xFF, (id_ip>>8)&0xFF, (id_ip>>16)&0xFF, id_ip>>24);
     if(id_ip < vpn_first_remote_ip[isTap] || id_ip >= (vpn_first_remote_ip[isTap] + vpn_total_remote_ip[isTap]))
     {
       id_ip = 0;
     }
   }

//   cl = new VPNclient;
   cl = (VPNclient *) malloc(sizeof(VPNclient));
   if(!cl) return -1;
lb_reconnect:
   memcpy(cl, this, sizeof(Req));
   memcpy(&cl->tls, Adv, sizeof(OpenSSLConnection) );
   cl->Adv = &cl->tls;
   cl->tls.CallbackParam = cl;
   cl->a_user = tuser;
   SecUpdateCB(&cl->tls);
   cl->limits = lmt;

   cl->fl = F_VPNTUN << isTap;
   cl->tun_index = isTap;
   if(!reconnect)
   {
     ip = strtoul(t, &p, 16);
     if(id_ip) ip = id_ip;
     if( (!ip) || (vpn_total_remote_ip[isTap] && ! IsVPN_IP_Free(ip)) )
     {
       if(vpn_first_remote_ip[isTap] && vpn_total_remote_ip[isTap])
       {
         if(vpn_next_remote_ip[isTap] < vpn_first_remote_ip[isTap]) vpn_next_remote_ip[isTap] = vpn_first_remote_ip[isTap];
         ip = vpn_next_remote_ip[isTap];
         do{
           if(IsVPN_IP_Free(ip))
           {
             vpn_next_remote_ip[isTap] = ip;
             goto found_free_ip;
           }

           #ifdef BIG_ENDIAN
           ip++;
           if( (ip-vpn_first_remote_ip[isTap]) >= vpn_total_remote_ip[isTap] ) ip = vpn_first_remote_ip[isTap];
           #else
           ip = htonl(ip);
           ip++;
           if( (ip-htonl(vpn_first_remote_ip[isTap])) >= vpn_total_remote_ip[isTap] ) ip = vpn_first_remote_ip[isTap];
           else ip = htonl(ip);
           #endif

         } while( ip != vpn_next_remote_ip[isTap] );
         ip = 0;
         debug("Not found free IP for %s\r\n", TUNTAPNames[isTap]);
         found_free_ip:;
       }
     }
   }
   t=loc + 1024;
   l = sprintf(t,"HTTP/1.0 200\r\n");
   if(ip)
   {
       char *tt;
       uint msk;
       msk = ConvertIP(tt=tuntap_ipv4nmask[isTap]);
       l += sprintf(t + l,"ip: %X\r\n"
                         "mask: %X\r\n"
                         "gw: %X\r\n"
                         "mtu: %X\r\n"
       ,ip,
       //vpn_nmask[isTap], //vpn_gw[isTap]
       msk,
       ConvertIP(tt=tuntap_ipv4[isTap]), vpn_mtu[isTap]
      );
       if(vpn_dns[isTap] && vpn_dns[isTap][0]) {
         l += sprintf(t + l,"dns: %s\r\n", vpn_dns[isTap]);
       cl->ipv4 = ip;
       cl->ipv4bcast = ip | ~msk; // ~vpn_nmask[isTap];
       cl->fl |= F_VPN_IPSET;
      }
   }
   l += sprintf(t + l,"\r\n");
   cl->Send(t,l);
   if(p && (mac = strtoll(SkipSpace(p) , 0, 16)) )
   {
     cl->macl = mac;
     //! cl->fl |= F_VPN_MACSET;
     DBGLA("set mac %s %X-%X", p, cl->mac[0],cl->mac[1]);
   }

   DBGL("Ok");
   cl->pos_pkt = 0;


   if(! reconnect) vpn_list[vpn_count++] = cl;
   maxVPNset.Set(s);
   SetKeepAliveSock(s);

   fl = F_KEEP_ALIVE | F_VPNANY;
   s = -1;
   Adv = 0;

   AddToLog(0, cl->s, & cl->sa_c46, ">>VPN Connection open %s %s %u.%u.%u.%u\r\n", cl->a_user? cl->a_user->name: "",
            TUNTAPNames[isTap], ip&0xFF, (ip>>8)&0xFF,(ip>>16)&0xFF, ip>>24); // TODO: Big endian fix

   return 0;
}


int VPNclient::RecvPkt()
{
  int l,ll;
  //HANDLE h;
//  DBGLA("recv %d", pos_pkt);
#ifdef VPN_WIN
  if( tap_waitbfr[tun_index] == (pkt+2) ) SynhWait(tun_index);
#endif

  if(MAX_MTU > (uint)pos_pkt)
  {
    fl |= F_JUSTPOOL;
    l=Recv(pkt + pos_pkt, MAX_MTU - pos_pkt);
    if(l<=0) return -1;
    pos_pkt += l;
    if(pos_pkt < 2) return 0;
  }
//  DBGLA("l=%d pos=%d pkt_len=%d",l, pos_pkt, pkt_len);

  if(pkt_len >= MAX_MTU ||
         (pkt_len < ( (fl&F_VPNTAP)? sizeof(ether_header) : sizeof(iphdr) ) )
         || ((uint)pos_pkt) > MAX_MTU )
  {
    debug("VPN synchronization error %d %d\r\n",pos_pkt, pkt_len );
    pos_pkt = 0;
    if(fl & F_VPN_LASTSINHERROR) return -1;
    fl |= F_VPN_LASTSINHERROR;
  }
  fl &= ~F_VPN_LASTSINHERROR;
  while(pos_pkt>2 && (uint)pos_pkt >= (ll = pkt_len+2))
  {

#ifdef VPN_WIN
    l = SynhWrite(tun_index, pkt + 2, pkt_len);
#else
    l=write(tuntap_fds[tun_index], pkt + 2, pkt_len);
#endif
    //DBGLA("tun_proto = %X l=%d", tunpkt.tun_proto, l )
    if(l != pkt_len )
    {
       debug("TUN(%d,%X) write %d error %d %d %s\r\n", tun_index, (fl&F_VPNTAP), pkt_len, l, errno, strerror(errno));
       pos_pkt = 0;
       return 0; // -1;
    }

    if(fl & F_VPNTUN)
    {
      if( (! (fl & F_VPN_IPSET) ) &&
        #ifdef USE_TUN_PI
        tunpkt.tun_proto == ETHERTYPE_IP_LE &&
        #else
        (tunpkt.ip4.version) == 4 &&
        #endif
        tunpkt.ip4.saddr)
      {
        ipv4 = tunpkt.ip4.saddr;
        fl |= F_VPN_IPSET;
        DBGLA("Set IP: %X",tunpkt.ip4.saddr)
      }
      if( (! (fl & F_VPN_IP6SET) ) &&
        #ifdef USE_TUN_PI
        tunpkt.tun_proto == ETHERTYPE_IPV6_LE &&
        #else
        (tunpkt.ip4.version) == 6 &&
        #endif
        WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) == 0xfe80 )
      {
        DBGL("")

        memcpy(& ipv6[0], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET;
      }
      if( (! (fl & F_VPN_IP6SET2) ) &&
        #ifdef USE_TUN_PI
        tunpkt.tun_proto == ETHERTYPE_IPV6_LE &&
        #else
        (tunpkt.ip4.version) == 6 &&
        #endif
          WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) < 0xfe00 &&
          WORD_PTR(tunpkt.ip6.ip6_src.s6_addr[0]) & ~0x40  )
      {

        DBGL("")

        memcpy(& ipv6[1], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET2;
      }
    }
    else if(fl & F_VPNTAP)
    {
      //DBGLA("tap proto = %X", tappkt.eth.ether_type)

      if( //tappkt.eth.ether_type == ETHERTYPE_ARP_LE  ||
          ! (fl & F_VPN_MACSET) )
      {
        mac[1]=0;
        memcpy(&macb, &tappkt.eth.ether_shost, 6);
        //macl = DDWORD_PTR(tappkt.eth.ether_shost[0]) & 0xFFFFffffFFLL;
        fl |= F_VPN_MACSET;
        DBGLA("ARP: mac=%X-%X", mac[0], mac[1])
      }

      if( (! (fl & F_VPN_IPSET) ) && tappkt.eth.ether_type == ETHERTYPE_IP_LE && tappkt.ip4.saddr )
      {
        ipv4 = tunpkt.ip4.saddr;
        fl |= F_VPN_IPSET;
        DBGLA("ip=%X",ipv4)
      }
      if( (! (fl & F_VPN_IP6SET) ) && tappkt.eth.ether_type == ETHERTYPE_IPV6 && WORD_PTR(tappkt.ip6.ip6_src.s6_addr[0]) ==
#ifdef BIG_ENDIAN
        0xFE80
#else
        0x80fe // LE machine!!!
#endif
      )
      {
        memcpy(& ipv6[0], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET;
      }
      if( (! (fl & F_VPN_IP6SET2) ) && tappkt.eth.ether_type == ETHERTYPE_IPV6 &&
          tappkt.ip6.ip6_src.s6_addr[0] < 0xfe &&
          WORD_PTR(tappkt.ip6.ip6_src.s6_addr[0]) &
#ifdef BIG_ENDIAN
          ~0x40
#else
          ~0x4000
#endif
        )
      {
        memcpy(& ipv6[1], tappkt.ip6.ip6_src.s6_addr, sizeof(ipv6[0]) );
        fl |= F_VPN_IP6SET2;
      }
    }

    if((uint)pos_pkt > (uint)ll)
    {
      pos_pkt -= ll;
      memcpy(pkt, pkt+ll, pos_pkt);
    }
    else pos_pkt = 0;
    VPNreceved += l;
    VPNreceved_pkt ++;

  }
  return 1;
}

int ReInitTUNTAP(char *reason, int i)
{
  debug("VPN Error %s for %s %d %d %s\r\n", reason, TUNTAPNames[i], errno, strerror(errno));
  CancelIo(tuntap_fds[i]);
  close(tuntap_fds[i]);
  tun_alloc(i);
  if(i == INDEX_CLIENT && vpn_cln_connected) tun_up(i, vpn_cln_connected->ipv4, client_nmask, vpn_cln_connected->ipv4gw, client_dns );
  return (int) tuntap_fds[i];
}

void OnPktFromIf(uchar *pkt, int i)
{
  int r,l,k;
 // DBGLA("%u read %u %u",TUNTAPNames[i], pkt_len,vpn_count);
  l = 0;
  for(k=0; k<vpn_count; k++)
  {
    r = vpn_list[k]->SendIsUs(pkt,i);
    if( r < 0 ) CloseVPNClient(k);
    else if(r > 0)
    {
      l++;
      if(r == PKT_US) break;
    }
  }
  if(!l)
  {
#ifdef DEBUG_VERSION
    DBGL("DROPED")
    print_pkt(i,pkt);
#endif
    VPNdroped ++;  //TODO: bytes stat
  }
  else {
    VPNsendet += l;
    VPNsendet_pkt ++;
  }
}

ulong WINAPI VPN_Thread(void *)
{
  fd_set set;
  fd_set er_set;
  timeval  TVal;
  int    i,j,l,k,r;
  union {
    short pkt_len;
    uchar pkt[MAX_MTU];
  };

  while(is_no_exit)
  {
#ifdef VPN_WIN
    if(!vpn_count)
    {
      Sleep(500);
      continue;
    }
#endif
    memcpy(&set, &VPNset, sizeof(set) );
    memcpy(&er_set,&set,sizeof(er_set));
    TVal.tv_sec=1;
    TVal.tv_usec=vpn_rescan_us;
    j=select(vpn_max_fd+1,&set,0,&er_set,&TVal);
    if(j>0)
    {

     // DBGL("");
#ifndef VPN_WIN

      for(i=0; i<2; i++)
        if(tuntap_fds[i] > 0 && FD_ISSET(tuntap_fds[i], &set) )
        {
          if( (pkt_len=read(tuntap_fds[i],pkt+2,MAX_MTU)) <= 0 )
          {
              ReInitTUNTAP("read",i);
          }
          else
          {
            OnPktFromIf(pkt,i);
          }
          j--;
        }
        else  if( FD_ISSET(tuntap_fds[i], &er_set) )
        {
          ReInitTUNTAP("set",i);
          j--;
        }
#endif
      for(i=0; i<vpn_count && j>0; i++)
        if( FD_ISSET(vpn_list[i]->s, &set) )
        {
          if(vpn_list[i]->RecvPkt()<0)  CloseVPNClient(i);
          j--;
        }
        else if( FD_ISSET(vpn_list[i]->s, &er_set) )
        {
          CloseVPNClient(i);
          j--;
        }
    }
  }

  return 0;
}


void  CloseTunTap()
{
  for(int i=0; i<3; i++)
     if(tuntap_fds[i] >= 0) {
     CancelIo(tuntap_fds[i]);
     close(tuntap_fds[i]);
     tuntap_fds[i] = INVALID_HANDLE_VALUE;
  }
}

int VPN_Init()
{
  int  i;
  char *t;
  for(i=0; i<2; i++)
  {
    if(vpn_first_remote_ipc[i] && vpn_first_remote_ipc[i][0])
      vpn_next_remote_ip[i] = vpn_first_remote_ip[i] = ConvertIP(t=vpn_first_remote_ipc[i]);
 /*
    if(vpn_gwc[i] && vpn_gwc[i][0])
      vpn_gw[i] = ConvertIP(t=vpn_gwc[i]);
*/
    //if(tuntap_ipv4nmask[i] && tuntap_ipv4nmask[i][0])
    //  vpn_nmask[i] = ConvertIP(t=tuntap_ipv4nmask[i]);
  }


  if(USE_TUN) tun_alloc(0);
  if(USE_TAP) tun_alloc(1);
  if(tun_fd == INVALID_HANDLE_VALUE && tap_fd == INVALID_HANDLE_VALUE) return -1;

  vpn_list = (VPNclient **) malloc( sizeof(VPNclient *) * (vpn_max+1) );
  if(!vpn_list) {
    CloseTunTap();
    return -1;
  }

  return (int) CreateThread(&secat,0x5000 + MAX_MTU,(TskSrv)VPN_Thread, (void *)0, 0, &trd_id);
}

int VPNclient::SendIsUs(uchar *pktl, int tuntap)
{
  int ret = PKT_NOT_US;

  if(!tuntap)
  { // TUN
  //  DBGLA("tun %u %X %X", tun_index,fl,fl & F_VPNTUN )
    if(fl & F_VPNTUN)
    {
#define ppkt ((VPN_TUNPacket *) pktl)
#ifdef USE_TUN_PI
      proto = ppkt->tun_proto;
      DBGLA("tun ppkt: %X", ppkt->tun_proto);
      switch(ppkt->tun_proto)
#else
      switch(ppkt->ip4.version)
#endif
      {
#ifdef USE_TUN_PI
        case ETHERTYPE_IP_LE:
#else
        case 4:
#endif
             //DBGLA("IP: %X %X",ppkt->ip4.daddr, ipv4)

             if(ppkt->ip4.daddr == ipv4) ret = PKT_US;
             else if( (ppkt->ip4.daddr & MCAST_MASK) == MCAST_DETECT ||
               ppkt->ip4.daddr == ipv4bcast ||
               ppkt->ip4.daddr == BROADCAST_ANY
             ) ret = PKT_BCAST;
             else  break;
             if(0){
#ifdef USE_TUN_PI
        case ETHERTYPE_IPV6_LE:
#else
        case 6:
#endif
             if(
                (!memcmp( & ppkt->ip6.ip6_dst, &ipv6[0], sizeof(ppkt->ip6.ip6_dst) ) ) ||
                (!memcmp( & ppkt->ip6.ip6_dst, &ipv6[1], sizeof(ppkt->ip6.ip6_dst) ) )
               ) ret = PKT_US;
             else if(
#if __BYTE_ORDER__  !=  __ORDER_LITTLE_ENDIAN__
                WORD_PTR(ppkt->ip6.ip6_dst.s6_addr[0]) == 0xFF02  // Multicast
#else
                WORD_PTR(ppkt->ip6.ip6_dst.s6_addr[0]) == 0x20FF  // Multicast
#endif
             ) ret = PKT_BCAST;
             else  break;
             }
             if(Send(pktl, ppkt->len + 2)<=0) ret = -1;
             break;
        default:

          DBGLA("Unknow tun ppkt: %X", DWORD_PTR(ppkt->ip4) );

      }
#undef  ppkt
    }
    // TODO: else Tun/Tap gate
  }
  else
  { //TAP
    if(fl & F_VPNTAP)
    {
#define ppkt ((VPN_TAPPacket *) pktl)
//       DBGLA("tap ppkt: %X %08X%04X %08X%04X  mcst:%X", ppkt->eth.ether_type,
//             DWORD_PTR(ppkt->eth.ether_dhost[0]), WORD_PTR(ppkt->eth.ether_dhost[4]) ,
//             mac[0], mac[1],  ppkt->eth.ether_dhost[0] & 0x1);

//      DBGLA("tap ppkt: %X %llX %llX mcst:%u", ppkt->eth.ether_type, DDWORD_PTR(ppkt->eth.ether_dhost[0]) & 0xFFffffFFFF, macl & 0xFFffffFFFF,   ppkt->eth.ether_dhost[0] & 0x1   );
      if(
#ifdef OPT64
        macl == (DDWORD_PTR(ppkt->eth.ether_dhost[0]) & 0xFFFFffffFFLL)
#else
       ( !memcmp(ppkt->eth.ether_dhost, &macb, 6) )
#endif
      ) ret = PKT_US;
      else if(
           //(!memcmp(ppkt->ether_dhost, &NullLongLong, 6) ) ||
           ppkt->eth.ether_dhost[0] & 0x1   // multicast
      )  ret = PKT_BCAST;
      else
      {
        DBGLA("tap ppkt: %X %08X-%04X != %08X-%04X  mcst:%X", ppkt->eth.ether_type,
            DWORD_PTR(ppkt->eth.ether_dhost[0]), WORD_PTR(ppkt->eth.ether_dhost[4]) ,
            mac[0], mac[1],  ppkt->eth.ether_dhost[0] & 0x1);
        return ret;
      }
      if(Send(pktl, ppkt->len + 2)<=0) ret = -1;
#undef  ppkt
    }
    // TODO: else Tun <=> Tap gate
  }
  return ret;
}

void VPN_Done()
{
  int i;
  MyLock(vpn_mutex);

  for(i=0; i<vpn_count; i++)
  {
    CloseSocket(vpn_list[i]->s);
  }
  vpn_count = 0;
  MyUnlock(vpn_mutex);

  CloseTunTap();
  free(vpn_list);

}


uint limitPeriods[]={3600, 86400, 86400*30};
uint VPNInLimitMb[3];
uint VPNOutLimitMb[3];
u64 VPNInLimit[3];
u64 VPNOutLimit[3];

int VPNclient::CheckVPNLimits()
{
  time_t current;
  u64 li;
  u64 lo;
  int i;
  limitPerTime *lpt;
  if(!limits) return 0;
  if(limits->in_fast > Tin && limits->out_fast > Tout) return 0;
  limits->in_fast = (u64)-1;
  limits->out_fast = (u64)-1;
  current = time(0);
  for(i=0; i<3; i++)
  {
    lpt = limits->lim + i;
    if( (!lpt->end) || current > lpt->end)
    {
      lpt->end = current + limitPeriods[i];
      lpt->out_bytes = Tout;
      lpt->in_bytes = Tin;
    }
    li = lpt->in_bytes + VPNInLimit[i];
    lo = lpt->out_bytes + VPNOutLimit[i];
    if(!VPNInLimit[i]) li = (u64)-1;
    else if(limits->in_fast > li) limits->in_fast = li;
    if(!VPNOutLimit[i]) lo = (u64)-1;
    else if(limits->out_fast > lo) limits->out_fast = lo;

    if(Tin > li) return -1;
    if(Tout > lo) return -1;
  }
  return 0;
}

int vpn_limit_mutex;
VPNUserLimit *VPNclient::SetLimit()
{
  VPNUserLimit* p;
  MyLock(vpn_limit_mutex);
  for(p=vpn_limits; p; p=p->next)
  {
    if( (s_flgs[3] & FL3_VPN_ULIMIT) && a_user == p->usr ) break;
    if( (s_flgs[3] & FL3_VPN_IPLIMIT) && CmpIP(&p->sa_c46, &sa_c46) ) break;
  }
  if(!p) {
    p = (VPNUserLimit *) Malloc(sizeof(VPNUserLimit));
    if(p) {
      p->usr = a_user;
      memcpy(&p->sa_c46, &sa_c46, sizeof(p->sa_c46));
      p->next = vpn_limits;
      vpn_limits = p;
    }
  }
  MyUnlock(vpn_limit_mutex);
  //limits = p;
  return p;
}

#if 0
void addRoute(uint dstip, uint dstmsk, uint gw)
{
    //int rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);
    if(rt_fd<=0) rt_fd = socket( PF_INET, SOCK_DGRAM,  IPPROTO_IP);

    struct rtentry route;
    memset( &route, 0, sizeof( route ) );

    ((struct sockaddr_in*) &route.rt_gateway)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_gateway)->sin_addr.s_addr = gw;

    ((struct sockaddr_in*) &route.rt_dst)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_dst)->sin_addr.s_addr = dstip;

    ((struct sockaddr_in*) &route.rt_genmask)->sin_family = AF_INET;
    ((struct sockaddr_in*) &route.rt_genmask)->sin_addr.s_addr = dstmsk;
    route.rt_flags = RTF_UP | RTF_GATEWAY;

    route.rt_metric = 0;

    if (ioctl( rt_fd, SIOCADDRT, &route ) < 0)
    {
        debug("ioctl failed and returned errno %s \n", strerror(errno));
    }
    //close( fd );
}


uint GetDefaultRoute()
{
#ifdef VPN_LINUX
  uint ret = 0;
  int fd, l, nlines, n,i, g;
  char *bfr;
  char *lines[32];
  char *words[16];
  enum {Iface=0,   Destination,     Gateway,         Flags,   RefCnt,  Use,     Metric,  Mask,            MTU,     Window,  IRTT };
#define MAX_ROUTE_SIZE 0x10000
  bfr=(char *)malloc(MAX_ROUTE_SIZE);
  fd=open("/proc/net/route",O_RDONLY);
  if(fd > 0)
  {
    l = read(fd,bfr,MAX_ROUTE_SIZE-1);
    close(fd);
    if(l>0)
    {
      bfr[l]=0;
      nlines = split(bfr,"\r\n",lines,31);
      for(i=1; i< nlines; i++)
      {
        n = split(lines[i]," \t",words,15);
        if(n >= Mask)
        {
          if( (!atouix(words[Destination])) && (!atouix(words[Mask])) && (g=atouix(words[Mask])) && words[Destination][0]=='0' )
          {
            ret = g;
            goto ex_ret;
          }
        }
      }
    }
  }
 ex_ret:
  free(bfr);
  return ret;
#else
  //TODO
#endif
}
#endif

int split(char *src, char *separators, char **result, int max_result)
{
  int ret = 0;
  while(*src && ret<max_result)
  {
    if(result) *result=src;
    //while( ! strchr(separators, *src ) ) { if(!*src) goto ex2loop; src++ ; }
    if( (!(src = strpbrk(src, separators)) ) || ! *src )  goto ex2loop;
    *src++ = 0;
    while( strchr(separators, *src ) ) { if(!*src) goto ex2loop; src++ ; }
    if(result) result++;
    ret++;
  }
 ex2loop:
  return ret;
}

#ifdef WITHMD5

int FindCopyVar(char *s, char *name, char *t, int l)
{
  char *v = PrFinVar(s,name);
  char *e;
  int ll;
  if(!v) return 0;
  e=strpbrk(v,"\t\r\n \",");
  if(!e) return 0;
  ll = e-v;
  if(l <= ll) ll = l - 1;
  memcpy(t,v,ll);
  t[ll] = 0;
  return 1;
};

void ChkEmpty(char *p)
{
  if(! *p )
    sprintf(p,"%X", Rnd() );
}
#endif

int VPNclient::ClientConnect(OpenSSLConnection *x)
{
  char bfr[2048];
  int  r;
  char *t;
  uint n,i;
  int  l;
 // struct ifreq  ifr;
  char *dgtvars[10];
  uint HA1[6];
  char HA2Hex[40];
  int  tryes = 0;
  static int AuthBasic;
  uint m=0xFFFFFF;

  vpn_cln_connected = 0;
  if(!vpn_remote_host) return -5;
#ifndef  TLSWODLL
  if( (!PSecConnect) || PSecConnect == SecConnectAbcent)
  {
    debug("Your version of 'libsec' library doesn't support functions required for VPN client. Please update it...\r\n");
    return -4;
  }
#endif
  timout = 60;
 // DBGL("")

agayn1:
  s=call_socket(vpn_remote_host, vpn_client_port);
  if(s <= 0)
  {
    debug("VPN client: Can't connect to %s...\r\n", vpn_remote_host);
    return -1;
  }

 // DBGL("")

  i = tbtAnon;
  if(s_flgs[3] & FL3_VPN_CHKTLS    ) i |= tbtVerfyRequired;
  if(s_flgs[3] & FL3_VPN_TLSIGNTIME) i |= tbtDontVerfyTyme;
  if(s_flgs[3] & FL3_VPN_TLSSSIGN  ) i |= tbtDontVerfySigner;
  if(s_flgs[3] & FL3_VPN_TLSSHSTYLE  ) i |= tbtSSHstyleVerfy;

  if(! TLSBegin(x,i,(s_flgs[3] & FL3_VPN_CHKTLS)? vpn_remote_host : 0 ) )
  {
    debug("VPN client: Can't setup TLS to %s...\r\n", vpn_remote_host);
    CloseSocket(s);
    return -1;
  }

  DBGL("TLS OK!")
  l = 1024;

#ifdef WITHMD5
  if(!AuthBasic)
  {
    l += sprintf(bfr + l,"Digest ");
    memset(dgtvars, 0, sizeof(dgtvars));
    dgtvars[digtVar_username] = vpn_user;
    dgtvars[digtVar_nonce   ] = vpn_notice;
    dgtvars[digtVar_uri     ] = vpncln_name;
    dgtvars[digtVar_opaque  ] = vpn_opaque;
    ChkEmpty(vpn_notice);
    ChkEmpty(vpn_opaque);

    //dgtvars[digtVar_qop] = 0;

    //CalkHA1(vpn_user, vpn_passw, (uchar *)HA1);
    ConvPwdMD5L4(HA1, vpn_user, vpn_passw, vpn_realm);
    CalkPwdMD5D(dgtvars, HA1, "GET", HA2Hex);
    dgtvars[digtVar_response] = HA2Hex;
    for(i=0; digetvars[i]; i++) if(dgtvars[i])
      l += sprintf(bfr+l,"%c%s=\"%s\"", (i)?',':' ', digetvars[i], dgtvars[i]);
    /*
     * l=sprintf(bfr+1024,"%X", r);
     * GenAPOP_dgst(vpn_passw, bfr+1124,bfr+1024,l);
     */
  }
  else
#endif
  {
    l += sprintf(bfr + l,"Basic ");
    l = Encode64(bfr + l, bfr + 1400, sprintf(bfr+1400, "%s:%s",vpn_user, vpn_passw ) ) - bfr;
  }
  l= msprintf(bfr,"GET %s HTTP/1.1\r\n"
    "Authorization: %s\r\n"
    "Host: %s\r\n"
    "%s: %X %llX\r\n"
    "Connection: keep-alive\r\n\r\n", vpncln_name,
    //vpn_user, r, bfr+1124,
    bfr+1024, vpn_remote_host,
    TUNTAPNames[tun_index], vpn_client_ip, vpn_mac[tun_index]
  );
  if(ipv4)
  {
    l+=sprintf(bfr+l-2,"reconnect: %X\r\n\r\n", ipv4) - 2;
  }

  DBGLA("ClSend:%u: %s",l,bfr);

  if(Send(bfr,l) != l )
  {
 err_close:
    debug("VPN client: TLS Connection error\n");
    //SecClose(x);
    //CloseSocket(s);
    Close();
    return -3;
  };
#if 0
  if( ! REPool(30000,s) )
  {
    debug("VPN Connection timeout\r\n");
    goto err_close;
  }
#endif
  if( (l=Recv(bfr,sizeof(bfr)-1) ) <= 0 )
  {
     DBGL("Recv error")
     goto err_close;
  }

  bfr[l] = 0;

  DBGLA("ClRecv: l=%d\r\n%s", l, bfr)

  if( (r=atoui(bfr+9)) != 200)
  {

    debug("VPN client: server return error %d %.64s...\r\n", r, bfr);

    if(r == 401)
    {
      http_var=(char **) (bfr + 1024);
      rq = bfr;
      prepare_HTTP_var();

      DBGL("")

      if( (t=GetVar(http_var,"WWW_Authenticate") ) )
      {
        DBGLS(t)
        if(strin(t, "Basic ") )
        {
          if(tryes) goto err_close;
          AuthBasic ++ ;
          DBGLS("Basic")
          goto agayn1;
        }

        FindCopyVar(t,"realm",  vpn_realm , sizeof(vpn_realm ));
        FindCopyVar(t,"nonce",  vpn_notice, sizeof(vpn_notice));
        FindCopyVar(t,"opaque", vpn_opaque, sizeof(vpn_opaque));

        DBGLA(" %s %s %s", vpn_realm, vpn_notice, vpn_opaque)

        if(!tryes) {
          //SecClose(x);
          //CloseSocket(s);
          Close();
          tryes ++ ;
          goto agayn1;
        }
      }
    }
    goto err_close;
  }

  http_var=(char **) (bfr + 1024);
  rq = bfr;
  prepare_HTTP_var();

  t=GetVar(http_var,"ip");
  if(t && t[0])
  {
    DBGLS(t)
    n=atouix(t); //ConvertIP(t);
   // DBGLA("%X" , n)
    ipv4 = n;
    if(n) fl |= F_VPN_IPSET;
  }
#if 0
  memset(&ifr, 0, sizeof(ifr) );
  //tun_index = (vpn.fl == F_VPNTAP)?0:1;
  DBGL("")
  sprintf(ifr.ifr_name,"%s%u",TUNTAPNames[tun_index], tuntap_number[tun_index]);

    ifr.ifr_mtu = vpn_mtu[2];
    if( (r = ioctl(rt_fd, SIOCSIFMTU, &ifr) ) < 0 )
      debug("VPN: can't set MTU interface %s: %d %d %s\r\n", ifr.ifr_name, r, errno, strerror(errno) );


    //if(ipv4 != n)
    {
      ipv4 = n;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
      ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ipv4;
      ioctl(rt_fd, SIOCSIFADDR, &ifr);

      t = GetVar(http_var,"mask");
      if(t && t[0])
      {
        n=atouix(t); //ConvertIP(t);
        if(n)
        {
          m = n;
          ipv4bcast = ipv4 | ~n;

          DBGLA("mask=%X bc=%X", m, ipv4bcast)

          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = n;
          ioctl(tuntap_fds[tun_index], SIOCSIFNETMASK, &ifr);

          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
          ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ipv4bcast;
          ioctl(rt_fd, SIOCSIFBRDADDR, &ifr);

          if(fl & F_VPNTUN)
          {
            t=GetVar(http_var, "gw");
            if(t && t[0]) n = atouix(t);

            if(n)
            {
              ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
              ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = n;
              ioctl(rt_fd, SIOCSIFDSTADDR, &ifr);
            }
          }

          if( (r = ioctl(rt_fd, SIOCGIFFLAGS, &ifr) ) < 0 )
          {
            debug("VPN: can't get flags interface %s: %d %d %s\r\n", ifr.ifr_name, r, errno, strerror(errno) );
            ifr.ifr_flags = 0;
          }

//    ifr.ifr_flags = IFF_UP | IFF_ALLMULTI | IFF_ECHO | aflag;
          ifr.ifr_flags |= IFF_UP | IFF_ALLMULTI | IFF_DYNAMIC | IFF_AUTOMEDIA | IFF_BROADCAST | IFF_MULTICAST;

          if( (r=ioctl(rt_fd, SIOCSIFFLAGS, &ifr) ) )
            debug("VPN: can't UP %X interface %s: %d %d %s\r\n", ifr.ifr_flags, ifr.ifr_name, r, errno, strerror(errno) );

        }
        else
        {
          DBGLS("NO MASK!!!!!!")
        }
      }
    }
  }
  t=GetVar(http_var, "gw");
  // TODO: set route
  if(t && t[0]) n = atouix(t);
  ipv4gw = n;
  if( vpn_scripts_up[2] && vpn_scripts_up[2][0] )
  {
    char *dns=GetVar(http_var,"dns");
    sprintf(bfr, "%s %s%u %u.%u.%u.%u %u.%u.%u.%u %u.%u.%u.%u '%s'", vpn_scripts_up[2],
            TUNTAPNames[2], tuntap_number[2],
            ipv4&0xFF, (ipv4>>8)&0xFF, (ipv4>>16)&0xFF,  ipv4>>24,
            m&0xFF,  (m>>8)&0xFF,  (m>>16)&0xFF,   m>>24,
            n&0xFF,  (n>>8)&0xFF,  (n>>16)&0xFF,   n>>24,
            (dns)?dns:""
    );
    DBGLS(bfr);
    RunScript(bfr);
  }
#else
  t=GetVar(http_var, "gw");
  // TODO: set route
  if(t && t[0]) n = atouix(t);
  ipv4gw = n;

  t = GetVar(http_var,"mask");
  if(t && t[0])
  {
    n=atouix(t); //ConvertIP(t);
    if(n)
    {
      client_nmask = n;
      ipv4bcast = ipv4 | ~n;
    }
  }
#ifdef FORLINUX
  t = GetVar(http_var,"mtu");
  if(t && t[0])
  {
    n = atouix(t);
    if(n > MIN_MTU && n<MAX_MTU) vpn_mtu[2] = n;
  }
#endif

  tmout = GetTickCount();

#ifdef USE_IPV6
  l = sizeof(sa_c6);
#else
  l = sizeof(sa_c);
#endif
  getpeername(s,(sockaddr*) (&sa_c), &l);
  vpn_cln_connected = this;
  tun_up(INDEX_CLIENT, ipv4, client_nmask, ipv4gw, client_dns = GetVar(http_var,"dns"));

#endif
  pos_pkt = 0;

  SetKeepAliveSock(s);

  debug("VPN client: Connection to the server has been established.\n");
  return 1;
}

ulong WINAPI VPNClient(void *)
{
  VPNclient         vpn;
  timeval  TVal;

  int r;
#ifndef VPN_WIN
  int vpn_client_fd;
#endif
  int max_fd;
  fd_set set, er_set;
  union {
    short pkt_len;
    uchar pkt[MAX_MTU];
  };

  DBGL("")
  memset(&vpn,0,sizeof(vpn));

  //vpn.tun_index=0;
  //if(TAP_SEPARATE)
  {
    vpn.tun_index=2;
    if(TAP_CLIENT){
      vpn.fl = F_VPNTAP;
      TUNTAPNames[2] = "tap";
#ifdef VPN_LINUX
      tuntap_flags[2] = IFF_TAP|IFF_NO_PI;
#endif
    }
    else
    {
      vpn.fl = F_VPNTUN;
      TUNTAPNames[2] = "tun";
#ifdef VPN_LINUX
      tuntap_flags[2] = IFF_TUN | TUN_PI;
#endif
    }
  }

#ifndef VPN_WIN
  if(tuntap_fds[vpn.tun_index] < 0 ) tun_alloc(vpn.tun_index);
  if(tuntap_fds[vpn.tun_index] < 0 )  return -1;
  DBGL("")

  vpn_client_fd = tuntap_fds[vpn.tun_index];
#else
  if(tuntap_fds[vpn.tun_index] == INVALID_HANDLE_VALUE ) tuntap_fds[vpn.tun_index] = win_tun_open(INDEX_CLIENT);//vpnIfNames[vpn.tun_index]);
  if(tuntap_fds[vpn.tun_index] == INVALID_HANDLE_VALUE ) return -1;
#endif


  vpn.ipv4 = 0;
  while(is_no_exit)
  {
    if( (r=vpn.ClientConnect(&vpn.tls)) < 0 )
    {

      DBGL("")

      if(r < -4)
      {

        DBGL("client exit");

        return -1;
      }
      Sleep(60000);
    }
    else
    {
      FD_ZERO(&set);
      FD_ZERO(&er_set);
#ifdef VPN_WIN
      max_fd = vpn.s;
#else
      max_fd = vpn_client_fd;
      if(vpn_client_fd < vpn.s) max_fd = vpn.s;
#endif

      while(is_no_exit)
      {
#ifndef VPN_WIN
        FD_SET(vpn_client_fd, &set);
        FD_SET(vpn_client_fd, &er_set);
#endif
        FD_SET(vpn.s, &set);
        FD_SET(vpn.s, &er_set);
        TVal.tv_sec=1;
        TVal.tv_usec=500000;
        if( (select(max_fd+1,&set,0,&er_set,&TVal))>0 )
        {
          if(FD_ISSET(vpn.s, &er_set)) goto err_s;
#ifndef VPN_WIN
          if(FD_ISSET(vpn_client_fd, &er_set)) goto err_vpn;
          if(FD_ISSET(vpn_client_fd, &set))
          {
            if( (pkt_len=read(vpn_client_fd,pkt+2,MAX_MTU)) <= 0 )
            {
              DBGLA("VPN client error %d %s\r\n", errno, strerror(errno))
          err_vpn:
              vpn_client_fd = ReInitTUNTAP("read",vpn.tun_index);
              if(vpn_client_fd < 0)
                return -1;
              max_fd = vpn_client_fd;
              if(vpn_client_fd < vpn.s) max_fd = vpn.s;
            }
            else {
              DBGLA("send %u", pkt_len)
              vpn.Send(pkt, pkt_len+2);
            }
          }
#endif
          if(FD_ISSET(vpn.s, &set))
          {
            if(vpn.RecvPkt()<0)
            {
            err_s:
              AddToLog(0, vpn.s,&vpn.sa_c46, FmtShortVPN,"VPN client: connection closed. ", vpn.Tin, vpn.Tout, (GetTickCount() - vpn.tmout)/1000, "" );
              //SecClose(&vpn.tls);
              //CloseSocket(vpn.s);
              vpn.Close();
              RunDownScript(vpn.tun_index, vpn.ipv4);
              break;
            }
          }
        }
      }
    }
  }


  return 0;
}

}
