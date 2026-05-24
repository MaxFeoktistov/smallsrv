/*
 * Copyright (C) 1999-2026 Maksim Feoktistov.
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

#ifndef SYSUNIX

#ifndef SELECT1
#ifdef MINGW
#undef fd_set
#endif
int RESelect(long tv_sec, long tv_usec, int n, ...)
{return select(0, (fd_set *)&n, 0, 0, (timeval*)&tv_sec);};
int WESelect(long tv_sec, long tv_usec, int n, ...)
{return select(0, 0, (fd_set *)&n, 0, (timeval*)&tv_sec);};

#else // SELECT1


int RESelect1(long tv_sec, long tv_usec, int s)
{
#ifdef MINGW
#undef fd_set
#define win_fd_set fd_set
#endif
  fd_set t;

  t.fd_count = 1;
  t.fd_array[0] = s;
  return select(0, (fd_set *)&t, 0, 0, (timeval*)&tv_sec);
};
int RESelect2(long tv_sec, long tv_usec, int s1, int s2)
{
  fd_set t;
  int r;

  t.fd_count = 2;
  t.fd_array[0] = s1;
  t.fd_array[0] = s2;
  if( ! (r = select(0, (fd_set *)&t, 0, 0, (timeval*)&tv_sec)) )return 0;
  if(r > 1)return s1;
  return FD_ISSET(s1, &t) ? s1 : s2;
};
//#define RESelect(a,b,c,d) RESelect1(a,b,d)
#endif // SELECT1

#ifdef MINGW
#undef fd_set
#define fd_set win_fd_set

void win_fd_clr(int s, fd_set *set)
{
  for(int i = 0; i < set->fd_count; i++)
  {
    if(set->fd_array[i] == s)
    {
      set->fd_count --;
      if(set->fd_count != i) set->fd_array[i] = set->fd_array[set->fd_count];
    }
  }
}

#endif // MINGW


#endif // ! SYSUNIX


void maxFdSet::Set(int fd)
{
  FD_SET(fd, &set);
  if(max_fd < fd) max_fd = fd;
}

void maxFdSet::Fix()
{
  while(max_fd > 0)
  {
    max_fd --;
    if(FD_ISSET(max_fd, &set) ) break;
  }
}

void maxFdSet::Clear(int fd)
{
  FD_CLR(fd, &set);
  if(max_fd == fd) Fix();
}

void CloseSocket(int s)
{
#undef shutdown
  shutdown(s, 2);
  closesocket(s);
}

#ifdef USE_IPV6
int CmpIP(TSOCKADDR *a, TSOCKADDR *b)
{
  if(a->sin6_family == AF_INET6) {
    if(a->sin6_family == AF_INET6) {
      return !memcmp( a->sin6_addr.s6_addr, b->sin6_addr.s6_addr, 16);
    }
    // TODO:  check IPv4 in IPv6 case
    return 0;
  }
  return ((sockaddr_in *) a)->sin_addr. S_ADDR == ((sockaddr_in *)b)->sin_addr. S_ADDR;
}
#endif

char* IPv6Addr(ushort *t, char *s)
{ int i, j, k, n;
  char *p;
  j = 0;
  i = 0;
  k = 0;
  while(*s && i < 8)
  {
    if(*s == ':')
    {
      if(s[1] == ':' )
      { s += 2; t[i] = 0; k = ++i;
        //     debug("+A %u %.25s ",i,s);
        continue;
      }
      ++s;
    }
    if(*s < '0' || (*s | 0x20) > 'f')break;
    n = strtoul(s, &p, 16);
    if(s == p)break;
    if(*p == '.' && i)
    { n = ConvertIP(s); //inet_addr(s);
      DWORD_PTR(t[i]) = n;
      i += 2;
      break;
    }
    s = p;
    t[i++] = htons(n);
  }
  if(i < 8 && k)
  { n = i - 1;
    for(j = 7; j > n; --j)
    { t[j] = t[n];
      if(n >= k)--n;
    }
  }
  return s;
}

#ifdef USE_IPV6
int IsIPv6(sockaddr_in *sa)
{
  if(sa->sin_family != AF_INET6) return 0;
  if(
    ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0] == 0 &&
    ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1] == 0 &&
    (
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2] == 0xFFFF0000
      //|| ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16[2]==0
    )
  ) return 0;
  return 1;
}

uint IPv4addr(sockaddr_in *sa)
{
  if(sa->sin_family == AF_INET) return sa->sin_addr. S_ADDR;
  if(sa->sin_family == AF_INET6 &&
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[0] == 0 &&
      ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[1] == 0 &&
      (
        ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[2] == 0xFFFF0000
        //|| ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr16[2]==0
      )
    ) return        ((sockaddr_in6 *)sa)->sin6_addr.s6_addr32[3];
  return 1;
}
#endif


void dec_no_close_req()
{
#ifdef USE_FUTEX
  if( --no_close_req <= 0)
  {
    no_close_req = 0;
    if(close_wait)
    {
      close_wait = 0;
      futex((int *)&no_close_req, FUTEX_WAKE, 1, 0, 0, 0);
    }
  }
#else
  if(--no_close_req < 0)no_close_req = 0;
#endif
}


ulong DTick(ulong tick1, ulong tick2)
{
  if(tick1 >= tick2) return tick1 - tick2;
  return (0xFFFFffff - tick2 + tick1);
}

#ifndef VPNCLIENT_ONLY
ulong tmSpd;
shs_mutex_t SpdMut = SHS_MUTEX_INITIALIZER;
int Req::SleepSpeed()
{
  ulong uspd;
  if(!(uspd = ipspeed[flsrv[1] & 7]))return 0;

  Req *r;
  ulong sum = 0, j;
  ulong tt;
  long x, i, y;
  ulong z;

  MyLock(SpdMut);
  tt = GetTickCount();
  z = tt - tmSpd;
  if(z > 1024 || !tmSpd)tmSpd = tt;
  MyUnlock(SpdMut);
  j = 0;
  ++no_close_req;
  for(i = 0; i < max_tsk; ++i)
    if( (u_long)(r = rreq[i]) > 1 )
    {
      x = DTick(tt, r->tmout);
      y = r->Tout;
      if(x > 0x4000)
      {
        y -= r->bSpd;
        if(y < 0)y = 0;
        else if(z > 1024)
        { r->bSpd += MULDIV(z, y, 0x4000 + z);
          if((y = r->Tout - r->bSpd) < 0)y = 0;
        }
      }
      if(r->sa_c.sin_addr.s_addr == sa_c.sin_addr.s_addr)sum += y;
      else if(y > 0x400)++j;
    }
  dec_no_close_req();
  sum >>= 8;
  if((x = sum - uspd) > 0 && j >= ipspdusr[flsrv[1] & 7])
  {
    Sleep(MULDIV(x, 0x4000, uspd));
    return 1;
  }
  return 0;
}
#endif

int anychars(char *name)
{ for(; *name; name++) if(*name >= 'A') return 1;
  return 0;
}

#ifdef SYSUNIX
void SetCloseExec(int s)
{
  fcntl(s, F_SETFD, fcntl(s, F_GETFD) | FD_CLOEXEC);
}
#endif

int call_socket(char *lhstname, int portnum)
{
#ifdef  USE_IPV6
  union {
#endif
    struct sockaddr_in sa;
#ifdef  USE_IPV6
    struct sockaddr_in6 sa6;
  };
#endif

  struct hostent *hp;
  char *hhh;
  u32 *p, n, a;
  int s;

#ifdef  USE_IPV6
  memset((char *)&sa, 0, sizeof(sa6));
  if(lhstname[0] == '[' )
  {
    IPv6Addr((ushort *) & sa6.sin6_addr, lhstname + 1);
    sa.sin_family = AF_INET6;
    s = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  }
  else
#else
  memset((char *)&sa, 0, sizeof(sa));
#endif
  {
    if(anychars(lhstname))
    {
      MyLock(ip_cach_mtx);
      if( !(p = memchr4(ip_cach, n = MkName(lhstname), 16) ) )
      {
        MyUnlock(ip_cach_mtx);
        hp = gethostbyname(lhstname);
        if(!hp)
        {
lbErr:
          debug("Cannot get host by name %s", lhstname);
          return -1;
        }

        if( (a = *(ulong *)hp->h_addr) == 0x100007F)
        {
          debug("*** localhost  *** %u", hp->h_length);
          if(!(hp = gethostbyname(lhstname)) ) goto lbErr;
        }
lbQErr:
        MyLock(ip_cach_mtx);
        *(p = ip_cach + iip_cach) = n; iip_cach = (iip_cach + 1) & 15;
        p[16] = a;
      }
      sa.sin_addr.s_addr = p[16];
      if( sa.sin_addr.s_addr == 0x100007F)
      {
        debug("***2 localhost  *** %u", p - iip_cach); //,hp->h_addr_list[1]);
        if((hp = gethostbyname(lhstname)) && (a = *(ulong *)hp->h_addr) != 0x100007F ) goto lbQErr;
      }

      MyUnlock(ip_cach_mtx);
    } else sa.sin_addr.s_addr = inet_addr(hhh = lhstname);
    sa.sin_family = AF_INET;
    if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)return -1;
  }
  SetCloseExec(s);
  sa.sin_port = htons((ushort)portnum);
  /*
   * DBGLA("s=%d %X %X %X [%X:%X:%X:%X:%X:%X:%X:%X]",s,sa.sin_port,sa6.sin6_port,sa.sin_family,
   *       ((ushort *) & sa6.sin6_addr)[0],
   *       ((ushort *) & sa6.sin6_addr)[1],
   *       ((ushort *) & sa6.sin6_addr)[2],
   *       ((ushort *) & sa6.sin6_addr)[3],
   *       ((ushort *) & sa6.sin6_addr)[4],
   *       ((ushort *) & sa6.sin6_addr)[5],
   *       ((ushort *) & sa6.sin6_addr)[6],
   *       ((ushort *) & sa6.sin6_addr)[7]
   *
   *      );
   */
  if(connect(s, (struct sockaddr *)&sa,
#ifdef USE_IPV6
             (sa.sin_family == AF_INET6) ? sizeof(sa6) :
#endif
             sizeof(sa)) < 0)
  { debug("Call to connect to %s failed (%d) " SER, lhstname, errno Xstrerror(errno));
    CloseSocket(s);
    return -1;
  }
  return s;
}

ulong MkName(char *t, int brk)
{
  uint i = 0, j = 0, k = 0;
  for(; *t && *t != brk; t++) {i += ((int) * t) << (k & 0x7); j ^= *t; k++;};
  return  (i << 16) | (j << 8) | k;
};

char *PrFinVar(char *s, const char *v)
{
  int l;
  char *r;
  char bb[64];
  l = sprintf(bb, "%s=", v);
  if(!(r = stristr(s, bb) )) return 0;
  if(r[l] == '"')l++;
  return r + l;
}

void SetKeepAliveSock(int s)
{
  setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *)&one, sizeof(int));
  if(keepalive_idle)
  { int v = 3;
    setsockopt(s, IPPROTO_TCP, TCP_KEEPIDLE, (char *)&keepalive_idle, sizeof(int));
    setsockopt(s, IPPROTO_TCP, TCP_KEEPINTVL, (char *)&keepalive_idle, sizeof(int));
    setsockopt(s, IPPROTO_TCP, TCP_KEEPCNT, (char *)&v, sizeof(int));
  }
}


ulong OldRnd, RndCounter;
ulong Rnd()
{
  ulong r, v1, v2;
#ifndef SYSUNIX
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);

  v1 = (GetTickCount() ^ (ulong)&r);
  v2 = ft.dwHighDateTime * ft.dwLowDateTime;
  r = rol( (OldRnd ^ ft.dwLowDateTime)
           ^ (v1 << (v2 % 24)) + v2
           , ((OldRnd >> (v1 & 15))^ft.dwLowDateTime) + ++RndCounter );
#else
  struct timeval tv;

  gettimeofday(&tv, 0);
  v1 = tv.tv_usec ^ (u_long)&r;
  v2 = tv.tv_usec * tv.tv_sec;
  r = rol( (OldRnd ^ tv.tv_usec)
           ^ (v1 << (v2 % 24)) + v2
           , ((OldRnd >> (v1 & 15))^tv.tv_usec) + ++RndCounter );

#endif
  OldRnd = r;
  return r;
};


ulong D64X(uchar i)
{ if(i == '+')return 62;
  if(i == '/')return 63;
  if(i < '0')return 64;
  if(i <= '9')return i - '0' + 52;
  if(i <= 'Z')return i - 'A';
  return i - 'a' + 26;
};

char * Decode64(char *t, char *s, int max_size)
{
  char *y = t;
  uint i, j;

  while((i = D64X(*s)) < 64)
  {
    s++;
    if( (j = D64X(*s)) >= 64 )break;
    *y = (i << 2) | (j >> 4);
    s++; y++;
    if( (i = D64X(*s)) >= 64 )break;
    *y = (j << 4) | (i >> 2);
    s++; y++;
    if( (j = D64X(*s)) >= 64 )break;
    *y = (i << 6) | (j);
    s++; y++;
    if((y - t) >= max_size) return 0;
  }
  *y = 0;
  return y;
}


uint GetIPv4(sockaddr_in* xsa)
{
  if(((sockaddr_in6 *)xsa)->sin6_family == AF_INET6)
    return (
             ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[0] == 0 &&
             ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[1] == 0 &&
             ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[2] == 0xFFFF0000
           ) ? ((sockaddr_in6 *)xsa)->sin6_addr.s6_addr32[3] : 0;

#ifndef SYSUNIX
  return xsa->sin_addr.S_un.S_addr;
#else
  return xsa->sin_addr.s_addr;
#endif
}

int utf2unicode(uchar *s, ushort *cm)
{
  int r = 0;
  uint a, b, c, d;
  do
  {
    a = *s++;
    if(a < 0x80)
    {
      d = a;
      b = 0;
    }
    else if(a >= 0xC0 && a <= 0xDF)
    {
      b = *s++;
      if( b < 0x80 || b > 0xbf )
      {
        return -1;
      }
      d = ((a & 0x1F) << 6) | (b & 0x3F);
      r++;
    }
    else if(a >= 0xE0 && a <= 0xEF)
    {
      b = *s++;
      c = *s++;
      if( b < 0x80 || b > 0xbf || c < 0x80 || c > 0xbf )
      {
        return -1;
      }
      d = ((a & 0xF) << 12) | ((b & 0x3F) << 6) | (c & 0x3F);
      r++;
    }
    else if(a >= 0xF0 && a <= 0xF5)
    {
      b = *s++;
      c = *s++;
      d = *s++;
      if( b < 0x80 || b > 0xbf || c < 0x80 || c > 0xbf || d < 0x80 || d > 0xbf )
      {
        return -1;
      }
      d = ((a & 0xF) << 18) | ((b & 0x3F) << 12) | ((c & 0x3F) << 6) | (d & 0x3F);
      r++;
    }
    else return -1;
#define UNI_SUR_HIGH_START  0xD800
#define UNI_SUR_HIGH_END    0xDBFF
#define UNI_SUR_LOW_START   0xDC00
#define UNI_SUR_LOW_END     0xDFFF
    if(d < 0xFFFE)
    {
      if(d >= UNI_SUR_HIGH_START && d < UNI_SUR_LOW_END)
      {
        return -2;
      }
      else {
        if(d < 0x80 && b)
          return -3;
        if(cm)
          *cm++ = d;
      }
    }
    else
    {
      if(cm) {
        *cm++ = (d >> 10)  + UNI_SUR_HIGH_START ;
        *cm++ = (d & 0x3FF)  + UNI_SUR_LOW_START ;
      }
    }

  } while( a );

  return r;
};


#ifdef USE_FUTEX
const  struct timespec timeout_50ms = {0, 50000000};
#endif
volatile int lock_cnt;

#ifdef USE_SEM

int MyLockTimeout(shs_mutex_t &x, int dead_lock_chk)
{
  int a = (int) GetCurrentThreadId();
  if(a == x.lock) return 0;

  int ms = dead_lock_chk * 50;

  while(! (x.sem_state & SEM_INITED))
  {
    if(x.sem_state & SEM_TRY_INIT)
    {
      sched_yield();
      continue;
    }
    x.sem_state |= SEM_TRY_INIT;
    sched_yield();
    if(x.sem_state & SEM_INITED) break;
    sem_init(&x.sem, 0, 1);
    x.sem_state |= SEM_INITED;
  }

  while(sem_trywait(&x.sem))
  {
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += ms >> 10;
    ts.tv_nsec += (ms & 0xFFF) * 1000000;
    while(ts.tv_nsec >= NSEC_PER_SEC)
    {
      ts.tv_nsec -= NSEC_PER_SEC;
      ts.tv_sec ++;
    }
    if(!sem_timedwait(&x.sem, &ts))
      break;
    if(dead_lock_chk-- < 0)
      return -1;
    MyUnlock(x);
  }
  x.lock = a;
  return 0;
}
void MyUnlock(shs_mutex_t &x)
{
  if(! x.lock) return;
  x.lock = 0;
  sem_post(&x.sem);
}

#elif defined(USE_PTHREAD_MUTEX)
int MyLockTimeout(shs_mutex_t &x, int dead_lock_chk)
{
  int a = (int) GetCurrentThreadId();
  if(a == x.lock) return 0;

  uint ms = dead_lock_chk * 50;

  while(pthread_mutex_trylock(&mutex))
  {
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += ms >> 10;
    ts.tv_nsec += (ms & 0xFFF) * 1000000;
    while(ts.tv_nsec >= NSEC_PER_SEC)
    {
      ts.tv_nsec -= NSEC_PER_SEC;
      ts.tv_sec ++;
    }
    if(!pthread_mutex_timedlock(&mutex, &ts))
      break;
    if(dead_lock_chk-- < 0)
      return -1;
    MyUnlock(x);
  }
  x.lock = a;
  return 1;
}
static pthread_mutex_t sig_mutex = PTHREAD_MUTEX_INITIALIZER;
static shs_mutex_t *dead_mutex;

void MyUnlock(shs_mutex_t &x)
{
  if(! x.lock) return;
  pthread_t a = GetCurrentThreadId();
  if(a == x.lock)
  {
    x.lock = 0;
    pthread_mutex_unlock(&mutex);
  }
  else
  {
    while(1)
    {
      pthread_mutex_lock(&sig_mutex);

      if(dead_mutex) {
        bool cond;

        cond = dead_mutex == &x;
        pthread_mutex_unlock(&sig_mutex);
        if(cond)
          break;
        sched_yield();
        continue;
      }
      dead_mutex = &x;
      pthread_kill(x.lock, SIGRTMIN);
      pthread_mutex_unlock(&sig_mutex);
      sched_yield();
      break;
    }
  }
}

void SignalFreeMutex(int)
{
  if (dead_mutex && dead_mutex->lock == GetCurrentThreadId())
  {
    dead_mutex->lock = 0;
    pthread_mutex_unlock(&dead_mutex->mutex);
    dead_mutex = 0;
  }
  signal(SIGRTMIN, SignalFreeMutex);
}

#else // USE_SEM, USE_PTHREAD_MUTEX
int MyLockTimeout(shs_mutex_t &x, int dead_lock_chk)
{
  pthread_t a = GetCurrentThreadId();
  if(a == x.lock) return 0;
#ifdef USE_WINMUTEX
  if(!x.hSem)
    x.hSem = CreateSemaphore(0, 1, 1, 0);

  while(x.lock)
  {
    if(WaitForSingleObject(x.hSem, 50 * dead_lock_chk) != WAIT_OBJECT_0)
      MyUnlock(x);
  }
  x.lock = a;
#else
  if( ++lock_cnt > 1)
  {
#ifdef SYSUNIX
    sched_yield();
#else
    Sleep(1);
#endif
  }

  do {
    while(x.lock && x.lock != a && --dead_lock_chk > 0)
#ifdef USE_FUTEX
      futex((int *)&x.lock, FUTEX_WAIT, x.lock, &timeout_50ms, 0, 0);
#else
      Sleep(50);
#endif
#if defined(SYSUNIX) && defined(DEBUG_VERSION)
    if(dead_lock_chk <= 0) {
      printf("Lock timeout %lX %X %X\r\n", (long) &x, x.lock, a);
    }
#endif
    x.lock = a;
    if(lock_cnt > 1)
    {
#ifdef SYSUNIX
      sched_yield();
#else
      Sleep(1);
#endif
    }
  } while(x.lock != a);
//Sleep(0); do{ while(x){ Sleep(30);}  if(++x==1)break; --x; }while(1);
  if(--lock_cnt < 0) lock_cnt = 0;

#endif
  return 1;
}
void MyUnlock(shs_mutex_t &x)
{
  x.lock = 0;
#ifdef USE_FUTEX
  futex((int *)&x, FUTEX_WAKE, 1, 0, 0, 0);
#elif defined(USE_WINMUTEX)
  ReleaseSemaphore(x.hSem, 1, NULL);
#endif // USE_FUTEX, USE_WINMUTEX
}
#endif // USE_SEM, USE_PTHREAD_MUTEX

int MyLock(shs_mutex_t &x)
{
  return MyLockTimeout(x, 128);
}

int MyTryLock(shs_mutex_t &x)
{
  if(x.lock) return 0;
  return MyLockTimeout(x, 1);
}

void MyUnlockOwn(shs_mutex_t &x) {
  if(x.lock == GetCurrentThreadId())
  {
    MyUnlock(x);
  }
}


#ifdef SYSUNIX

#ifndef malloc
#error  "used original malloc"
#endif


shs_mutex_t MemMtx = SHS_MUTEX_INITIALIZER;
char * Malloc(int c)
{
  register char *r;
  MyLock(MemMtx);
  r = (char *)malloc(c);
  MyUnlock(MemMtx);
  if(r) memset(r, 0, c);
  return r;
};


#ifdef MEMDBG

#undef malloc
#undef realloc
#undef free

#ifndef STRVER
#define STRVERV(a) #a
#define STRVERVS(a) STRVERV(a)
#define STRVER STRVERVS(SHS_VERSION)
#endif  // STRVER

struct dbg_mem
{
  int sign;
  int l;
  u8 d[4];
};
#define SIGN_START  0x12345678
#define SIGN_END    0x12345670

void *dbg_malloc(int n)
{
  union {
    void *r;
    uint *d;
    dbg_mem *dbg;
  };
  r = malloc(n + sizeof(dbg_mem));
  if(!r) return r;

  dbg->sign = SIGN_START;
  dbg->l = n;
  DWORD_PTR(dbg->d[n]) = SIGN_END;

  return dbg->d;
}

#if 0
int chk_dbg_mem(void *p)
{
  dbg_mem *dbg = container_of(p, dbg_mem, d);

  if(dbg->sign == SIGN_START && DWORD_PTR(dbg->d[dbg->l]) == SIGN_END)
    return 0;

  {
    void *caller_address = __builtin_return_address(0);

    debug("MEMERROR: %lX l=%X signs: %X, %X  call from %lX ver: %s end: %lX", (long) p, dbg->l,
            dbg->sign,
            ((dbg->sign == SIGN_START) ? (int) DWORD_PTR(dbg->d[dbg->l]) : 0),
            (long) caller_address, STRVER, (long) &end);

    dbg_backtrace();
  }

  return -1;
}
#endif

void *dbg_realloc(void *p, int n)
{
  dbg_mem *dbg = container_of(p, dbg_mem, d);

  if(dbg->sign != SIGN_START || DWORD_PTR(dbg->d[n]) != SIGN_END)
  {
    void *caller_address = __builtin_return_address(0);

    debug("MEMERROR realloc %X -> %X: signs: %X, %X  call from %X,%X ver: %s\r\n",  dbg->l, n,
          dbg->sign,
          ( (dbg->sign == SIGN_START) ? (int) DWORD_PTR(dbg->d[dbg->l]) : 0 ),
          caller_address, STRVER);

    dbg_backtrace();
  }

  dbg = (dbg_mem *) realloc(dbg, n + sizeof(dbg_mem));

  if(!dbg) return 0;

  dbg->l = n;
  DWORD_PTR(dbg->d[n]) = SIGN_END;

  return  dbg->d;
}

void dbg_free(void *p)
{
  dbg_mem *dbg = container_of(p, dbg_mem, d);

  if(dbg->sign == SIGN_START && DWORD_PTR(dbg->d[dbg->l]) == SIGN_END)
  {
    dbg->sign = 0;
    free(dbg);
  }
  else
  {
    void *caller_address = __builtin_return_address(0);

    debug("MEMERROR: %lX l=%X signs: %X, %X  call from %lX ver: %s end: %lX", (long) p, dbg->l,
          dbg->sign,
          ((dbg->sign == SIGN_START) ? (int) DWORD_PTR(dbg->d[dbg->l]) : 0),
          (long) caller_address, STRVER, (long) &end);

#if 1
    dbg_backtrace();
#else
    void *btr[16];
    int  n;

    n = backtrace(btr, 16);
    for(int i = 0; i < n; i++) {
      if( btr[i] >  __executable_start && btr[i] < &end)
        debug(" from  %lX (+%lX)", (long) (btr[i]), (long) ((char *) (btr[i]) - __executable_start));
      else debug(" from %lX", (long) (btr[i]));
    }
#endif
  }

}

#endif // MEMDBG

#endif //SYSUNIX


