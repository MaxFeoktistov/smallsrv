#
# Copyright (C) 1999-2024 Maksim Feoktistov.
#
# This file is part of Small HTTP server project.
# Author: Maksim Feoktistov
#
#
# Small HTTP server is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
#
# Small HTTP server is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see https://www.gnu.org/licenses/
#
# Contact addresses for Email:  support@smallsrv.com
#
#
#

VERSION=3.06.32
VERSIONT=3.06.32

prefix ?=/usr/local/
CONFIG_BASE   ?= $(prefix:%/=%)/
CONFIG_APPDIR ?= $(CONFIG_BASE)lib/smallsrv/
CONFIG_SHARE  ?= $(CONFIG_BASE)share/smallsrv/
CONFIG_CONFIG ?= /etc/smallsrv/
CONFIG_LOG    ?= /var/log/smallsrv/
CONFIG_DATA   ?= /var

ifneq ($(DESTDIR),)
DESTDIR := $(DESTDIR:%/=%)/
endif

ICONFIG_BASE   := $(DESTDIR)$(CONFIG_BASE)
ICONFIG_APPDIR := $(DESTDIR)$(CONFIG_APPDIR)
ICONFIG_SHARE  := $(DESTDIR)$(CONFIG_SHARE)
ICONFIG_CONFIG := $(DESTDIR)$(CONFIG_CONFIG)
ICONFIG_LOG    := $(DESTDIR)$(CONFIG_LOG)

INSTALLFROM ?= oo/

G=  -g  -Os

TMPRAM ?= /dev/shm/shttps/

# OPT= -m32 -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
# -fno-verbose-asm -fno-implicit-templates -mno-align-double \
# -mno-fancy-math-387 -fwritable-strings -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated  -fno-nonansi-builtins  -fno-optional-diags -momit-leaf -frame-pointer  -mno-red-zone -DFREEVER  -fno-pic

# best opt for gcc 3.x
# OPT= -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
# -fno-verbose-asm -fno-implicit-templates -mno-align-double \
# -mno-fancy-math-387 -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated  -fno-nonansi-builtins  -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -DFREEVER  -fno-pic
# -fno-implicit-templates

OPT= -pipe  \
  -fno-stack-protector -fno-stack-check -fno-verbose-asm -fno-nonansi-builtins -fno-access-control  -fno-optional-diags -momit-leaf-frame-pointer\
 -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions  -Wno-deprecated -Wno-address-of-packed-member -fno-bounds-check -fno-tree-bit-ccp -fno-builtin -mno-red-zone -DFREEVER  -DWITHMD5 -DFIX_EXCEPT -DPF_LONG_LONG \
  -DUSEVALIST -DSEPLOG $(ADVOPT) -DVPN_LINUX -DTLSVPN -DUSE_SYSPASS -DSHS_VERSION=$(VERSION)

OPTTLS= $(OPT) -DTLSWODLL

#-fdelete-null-pointer-checks  -fdelayed-branch  -fdelete-dead-exceptions
# -fnobounds-check
#-mcpu=i486  -march=i486
# -fno-threadsafe-statics


OPT64=  -Dx86_64 -DNOTINTEL -DA_64 $(OPT)
OPT64TLS= $(OPT64) -DTLSWODLL

GPP=gcc -m32
GCC=gcc -m32
GPP64=gcc -m64
GCC64=gcc -m64

ULGCC= PATH=/usr/local/arm-2010q1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin ; arm-uclinuxeabi-gcc
ULGPP= PATH=/usr/local/arm-2010q1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin ; arm-uclinuxeabi-g++

ULCFLAGS= -mlittle-endian -marm  -mno-thumb-interwork -mthumb -march=armv7 -mfix-cortex-m3-ldrd  -mlong-calls -msoft-float -Uarm  -nostdinc  -I/usr/local/arm-2010q1/arm-uclinuxeabi/libc/usr/include -I/usr/local/arm-2010q1/lib/gcc/arm-uclinuxeabi/4.4.1/include -I/usr/local/arm-2010q1/lib/gcc/arm-uclinuxeabi/4.4.1/include-fixed  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs  -Wno-deprecated-declarations -Wno-conversion  -Wno-write-strings  -DUSE_IPV6

# -fno-enforce-eh-specs -fno-nonansi-builtins  -fno-rtti -fno-elide-constructors -fno-ms-extensions -fno-stats -fno-nonansi-builtins  -nostdinc++  -fno-access-control
# -fconserve-space -O2



S= -s

#WINEGPP=wine c:\\MINGW2\\bin\\gcc.exe
#WINEGCC=wine c:\\MINGW2\\bin\\gcc.exe
#WINEAS=wine  c:\\MINGW2\\bin\\as.exe
#WINERES=wine c:\\MINGW2\\bin\\windres.exe
#MGDIR=c:\\MINGW2
#WININC= -I$(MGDIR)\\include -I$(MGDIR)\\lib\\gcc\\mingw32\\8.2.0\\include
#WINLIB= -L$(MGDIR)\\lib

# WINEGPP=wine c:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/gcc.exe -m32 -DMINGW64
# WINEGCC=wine c:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/gcc.exe -m32 -DMINGW64
# WINEAS=wine  c:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/as.exe
# WINERES=wine c:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/bin/windres.exe
# MGDIR=c:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/
# WININC= -Ic:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/include -Ic:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/include
# WINLIB= -Lc:/mingw-w64/i686-8.1.0-win32-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/lib



# WINEGPP=wine c:\\MinGW\\bin\\gcc.exe
# WINEGCC=wine c:\\MinGW\\bin\\gcc.exe
# WINEAS=wine  c:\\MinGW\\bin\\as.exe
# WINERES=wine c:\\MinGW\\bin\\windres.exe

# WINEGPP=x86_64-w64-mingw32-gcc -m32
# WINEGCC=x86_64-w64-mingw32-gcc -m32
# WINEAS=x86_64-w64-mingw32-as
# WINERES=x86_64-w64-mingw32-windres

WINEGPP=i686-w64-mingw32-gcc -m32 -DMINGW64
WINEGCC=i686-w64-mingw32-gcc -m32 -DMINGW64
WINEAS=i686-w64-mingw32-as
WINERES=i686-w64-mingw32-windres
MGDIR=/usr/i686-w64-mingw32
WININC= -I$(MGDIR)/include -I/usr/lib/gcc/i686-w64-mingw32/5.3-win32/include
WINLIB= -L$(MGDIR)/lib




WINE=wine
#WINEBIN2S=wine wo\\bin2s.exe
WINEBIN2S=./bin2s

#WINECFLG=  -Iwinclude  -I$(MGDIR)\\include -fpack-struct -O2 -fno-verbose-asm -fno-implicit-templates -mno-svr3-shlib -mno-align-double -mno-fancy-math-387 -fconserve-space  -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mrtd
#  -fno-implicit-templates

WINECFLG= -Os -nostdinc -Iwinclude  $(WININC) -mno-align-double -mno-fancy-math-387 -fconserve-space  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mwindows -fno-optional-diags -momit-leaf-frame-pointer -mno-red-zone -fno-exceptions  -fno-stack-protector -no-pie -fno-ms-extensions -fno-stack-check -mno-stack-arg-probe -fmax-errors=10 -DSEPLOG -DSELECT1  -DVPN_WIN -DTLSVPN -DWITHMD5 -DSHS_VERSION=$(VERSION) -DUSEVALIST

WINELFLG=  -mwindows $(WINLIB) -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Xlinker --heap  -Xlinker 0x20000000 -Wl,--subsystem,windows  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe

#WINECFLG=  -nostdinc -Iwinclude  -I$(MGDIR)\\include -I$(MGDIR)\\lib\\gcc\\mingw32\\8.2.0\\include -fno-implicit-templates -mno-align-double -mno-fancy-math-387 -fconserve-space  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mwindows -fno-optional-diags -momit-leaf-frame-pointer -mno-red-zone -fno-exceptions  -fno-stack-protector -no-pie -fno-ms-extensions -fno-stack-check -mno-stack-arg-probe

#WINELFLG=    -mwindows -L$(MGDIR)\\lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Xlinker --heap  -Xlinker 0x20000000 -Wl,--subsystem,windows  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe

#
# WINECFLG=  -Os -nostdinc -Iwinclude  -I$(MGDIR)/include -I/usr/lib/gcc/i686-w64-mingw32/5.3-win32/include -fno-implicit-templates -mno-align-double -mno-fancy-math-387 -fconserve-space  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mwindows -fno-optional-diags -momit-leaf-frame-pointer -mno-red-zone -fno-exceptions  -fno-stack-protector -no-pie -fno-ms-extensions -fno-stack-check -mno-stack-arg-probe
#
# WINELFLG=    -mwindows -L$(MGDIR)/lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Xlinker --heap  -Xlinker 0x20000000 -Wl,--subsystem,windows  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe
#


WIN64EGPP=x86_64-mingw32-gcc -m64 -DMINGW64
WIN64EGCC=x86_64-mingw32-gcc -m64 -DMINGW64
WIN64EAS=x86_64-mingw32-as
WIN64ERES=x86_64-mingw32-windres
MG64DIR=/usr/x86_64-mingw32
WIN64INC= -I$(MG64DIR)/include -I/usr/lib/gcc/x86_64-mingw32/5.3-win32/include
WIN64LIB= -L$(MG64DIR)/lib




WIN64E=wine
WIN64EBIN2S=./bin2s

# -fno-implicit-templates
WIN64ECFLG= -Os -nostdinc -Iwinclude  $(WIN64INC) -mno-align-double -mno-fancy-math-387 -fconserve-space  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mwindows -fno-optional-diags -momit-leaf-frame-pointer -mno-red-zone -fno-exceptions  -fno-stack-protector -no-pie -fno-ms-extensions -fno-stack-check -mno-stack-arg-probe -fmax-errors=10  -DSEPLOG  -DSELECT1  -DSHS_VERSION=$(VERSION) -DUSEVALIST

WIN64ELFLG=  -mwindows $(WIN64LIB) -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Xlinker --heap  -Xlinker 0x20000000 -Wl,--subsystem,windows  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe







LOPT=   -dynamic-linker   -Xlinker -Map -Xlinker o/flxmap  -nodefaultlibs
LOPT64=   -dynamic-linker   -Xlinker -Map -Xlinker o64/flxmap  -nodefaultlibs

LIB= $(LIBDIR32) -lpthread  $(LIBDIR32) -ldl $(LIBDIR32) -lc $(LIBDIR32) -lgcc -lc_nonshared  $(LIBDIR32) -lcrypt
LIB64= $(LIBDIR64) -lpthread  $(LIBDIR64) -ldl $(LIBDIR64) -lc  $(LIBDIR32) -lgcc -lc_nonshared $(LIBDIR32) -lcrypt
LIB64u=  -ldl -lpthread  -lc -lgcc -lc_nonshared -lcrypt

BINFILES := httpd.exe httpd.exopenssl httpd.exgnutls sndmsg


#LIB= -lpthread -ldl -lc -lc_nonshared
#LIB64= -lpthread -ldl -lc -lc_nonshared


COBJS= o/to_linux.o o/bvprintfv.o
COBJS64= o64/to_linux.o

AOBJS=mstring1.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o md5.o msprintfchk.o fcgi.o vpn.o

AOBJS64=srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o md5.o msprintfchk.o fcgi.o vpn.o

POBJS=$(AOBJS)
# telnet.o
OBJS=$(COBJS) $(POBJS)


OOBJS=$(COBJS) $(addprefix o/,$(POBJS))

OOBJS64=$(COBJS64) $(addprefix o64/,$(AOBJS64))

WINOOBJS= wo/fwnd.o $(addprefix wo/,$(AOBJS)) wo/bvprintfv.o wo/srvdat.o wo/isapi.o wo/qsort.o wo/updr.res

#GENERATED=S2_lf.hh lS2_lf.cfg g4strc.h S3_lf.hh lS3_lf.cfg g4strcwm.h t2icfg.cpp S1_lf.hh lS1_lf.cfg g4s1.hh o/s1.hh
GENERATED=S2_lf.hh lS2_lf.cfg g4strc.h S3_lf.hh lS3_lf.cfg g4strcwm.h

OOBJS_TLS=$(COBJS) $(addprefix o/tls_,$(POBJS))

OOBJS64_TLS=$(COBJS64) $(addprefix o64/tls_,$(AOBJS64))

#FLIBS=libc.so libdl.so libpthread.so libgnutls.so libssl.so
FLIBS=libc.so libpthread.so libdl.so libgnutls.so libssl.so libcrypto.so libgnutls.so.30 libcrypt.so
FAKELIBS=$(addprefix fakelibs/,$(FLIBS))
FAKELIBS64=$(addprefix fakelibs64/,$(FLIBS))

FLIBSARM= libgnutls.so libssl.so libcrypto.so libgnutls.so.30 libcrypt.so
FAKELIBSARM=$(addprefix fakelibsarm/,$(FLIBSARM))
N_FAKELIBS=$(addprefix oo/fakelibs/,$(FLIBSARM))


ifdef $(CROSS_COMPILE)

all: n_all

else

#all: wo/http.exe

#all: o64/httpdu.exe o/httpd.exe

# all: o/httpd.exe

#o64/libsecgnutls.so o64/libsec.so

#all: wo/tstwin.o
# all: i64

#all: wo/libsec111.dll
#all: wo/libsecgnutls.dll
#all: /dev/shm/shttps/o/1.x o/1.x getstr $(FAKELIBS) $(FAKELIBS64) i32 i64 win

#all: arm64

#all: wo/libsec321.dll
all: /dev/shm/shttps/o/1.x o/1.x getstr i32 i64 i32f i64f win arm arm64

endif


$(OOBJS) $(OOBJS64) $(WINOOBJS): $(GENERATED)

# $(GENERATED): $(TMPRAM)o

i32: $(TMPRAM)o o o/httpd.exe o/libsec111.so o/libsecgnutls.so o/sndmsg o/httpd.exgnutls o/httpd.exopenssl

i64: $(TMPRAM)o64 o o64 o64/httpd.exe o64/libsecgnutls.so o64/libsec111.so o64/sndmsg o64/httpd.exgnutls o64/httpd.exopenssl

i32f: $(TMPRAM)o/of o $(FAKELIBS) o/of/httpd.exe o/of/libsec111.so o/of/libsecgnutls.so o/of/httpd.exgnutls o/of/httpd.exopenssl o/of/sndmsg

#o/libsecgnutls.so o/libsec.so o/libsec111.so

i64f: $(TMPRAM)o64/of o o64 $(FAKELIBS64) o64/of/httpd.exe o64/of/libsecgnutls.so o64/of/libsec111.so o64/of/httpd.exgnutls o64/of/httpd.exopenssl o64/of/sndmsg

#o64/httpdu.exe o64/libsecgnutls.so o64/libsec.so o64/libsec111.so o64/libsec.so

arm: $(TMPRAM)at o at at/atobjdir at/httpd.exe at/libsec111.so at/libsecgnutls.so at/httpd.exopenssl at/httpd.exgnutls

win: $(TMPRAM)wo o wo wo/http.exe wo/httpg.exe bin2s wo/shttps_mg.exe wo/shttpsr_mg.exe wo/shttps_mgi.exe wo/shttpsr_mgi.exe wo/libsecgnutls.dll


arm64: CROSS_COMPILE:=aarch64-linux-gnu-
arm64: A_OPT:=-DA_64
arm64: N_LFAKELIBS:=-Loo/fakelibs
arm64: N_LFLAGS:=-mlittle-endian -Wl,-Bdynamic
arm64: $(TMPRAM)oo oo/fakelibs $(N_FAKELIBS)
arm64: n_all

$(TMPRAM)% :
	mkdir -p $@
	[ -d $* ] || ln -sf $@ $*

%/of: $(TMPRAM)%
	mkdir -p $@

o64/of/%.o : o64/of

o/of/%.o : o/of


o/%.o :  %.cpp
	   $(GPP) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@

o/tls_%.o :  %.cpp
	   $(GPP) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPTTLS)  $< -o $@

o/%.o :  %.c
	   $(GCC) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@

o/tls_%.o :  %.c
	   $(GCC) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPTTLS)  $< -o $@


o64/%.o :  %.cpp
	   $(GPP64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT64)  $< -o $@


o64/%.o :  %.c
	   $(GCC64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT64)  $< -o $@


o64/tls_%.o :  %.cpp
	   $(GPP64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT64TLS)  $< -o $@


o64/tls_%.o :  %.c
	   $(GCC64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT64TLS)  $< -o $@


r/%.o :  %.cpp
	   $(GPP) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPT) -DRUS  $< -o $@


wo/%.o :  %.cpp
	   $(WINEGPP) -c -g $(WINECFLG)  $< -o $@

wo/sethttp3r.o: sethttp3.cpp
	   $(WINEGPP) -c -g $(WINECFLG) -DRUS $< -o $@

wo/sethttp3ri.o: sethttp3.cpp
	   $(WINEGPP) -c  -g $(WINECFLG) -DRUS -DIPCONTRY $< -o $@

wo/sethttp3i.o: sethttp3.cpp
	   $(WINEGPP) -c  -g $(WINECFLG) -DIPCONTRY $< -o $@

# wo/%.o :  %.c
# 	   $(WINEGCC) -c  $(G) $(WINECFLG)  $< -o $@

wo/%.res :  %.rc
	   $(WINERES) $<  -O coff -o $@

fakelibs/1.x:
	mkdir -p fakelibs
	echo  ok >$@

fakelibs64/1.x:
	mkdir -p fakelibs64
	echo  ok >$@

fakelibsarm/1.x:
	mkdir -p fakelibsarm
	echo  ok >$@

$(FAKELIBS) : fakelibs/1.x

$(FAKELIBS64) : fakelibs64/1.x

$(FAKELIBSARM) : fakelibsarm/1.x

fakelibs/libc.so: /usr/lib/i386-linux-gnu/libc.so.6
	./mkfakelib.pl $< fakelibs/

fakelibs/libdl.so: /lib/i386-linux-gnu/libdl.so
	./mkfakelib.pl $< fakelibs/

fakelibs/%.so: /usr/lib/i386-linux-gnu/%.so
	./mkfakelib.pl $< fakelibs/

fakelibs64/libc.so: /usr/lib/x86_64-linux-gnu/libc.so.6
	./mkfakelib64.pl $< fakelibs64/ -c

fakelibs64/libdl.so: /lib/x86_64-linux-gnu/libdl.so
	./mkfakelib64.pl $< fakelibs64/ -c

fakelibs64/%.so: /usr/lib/x86_64-linux-gnu/%.so
	./mkfakelib64.pl $< fakelibs64/ -c

fakelibsarm/%.so: /usr/lib/i386-linux-gnu/%.so
	./mkfakelib_arm.pl $< fakelibsarm/ -c

%/libgnutls.so.30: %/libgnutls.so
	ln -s libgnutls.so $@

.cpp.S:
	   $(GPP) -S -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $<


LIBDIR32F=-Lfakelibs/
LIBDIR64F=-Lfakelibs64/
LIBDIRARMF=-Lfakelibsarm/

LIBF= $(LIBDIR32F) -lpthread  $(LIBDIR32F) -ldl $(LIBDIR32F) -lc $(LIBDIR32F) -lgcc -lc_nonshared  $(LIBDIR64F) -lcrypt
LIB64F= $(LIBDIR64F) -lpthread  $(LIBDIR64F) -ldl $(LIBDIR64F) -lc $(LIBDIR64F) -lgcc -lc_nonshared  $(LIBDIR64F) -lcrypt

getstr: getstr.cpp
	g++ -m32 -pipe -fpack-struct -fconserve-space -O2 -s getstr.cpp -o getstr

md5:   md5.c
	gcc -s -pipe -m32 -Os md5.c -DTEST  -DSYSUNIX -DV_FULL=1  -o md5

o/1.x: /dev/shm/shttps/o/1.x
	ln -sf /dev/shm/shttps/o .
	ln -sf /dev/shm/shttps/o64 .
	ln -sf /dev/shm/shttps/wo .
	ln -sf /dev/shm/shttps/arm .
	mkdir -p  o/dist o/distu o/adist/ o64/dist o64/distu o/of o64/of
	mkdir -p  o/dist/langpacks/en o/adist/langpacks/en o64/dist/langpacks/en o64/distu/langpacks/en
	mkdir -p  o/dist/langpacks/ru o/adist/langpacks/ru o64/dist/langpacks/ru o64/distu/langpacks/ru

/dev/shm/shttps/o/1.x :
	mkdir -p /dev/shm/shttps/o /dev/shm/shttps/o64 /dev/shm/shttps/wo /dev/shm/shttps/arm ; echo 1 >$@

o/httpd.exe: $(OOBJS)
	$(GCC)  $(G)  $(OOBJS) -o $@  $(LOPT) $(LIB)

o/of/httpd.exe: $(OOBJS)
	$(GCC)  $(G)  $(OOBJS) -o $@  $(LOPT) $(LIBF)


o/httpd.exopenssl: $(OOBJS_TLS) o/runssl111.o
	$(GCC)  $(G)  $^ -o $@ -Wl,--copy-dt-needed-entries $(LOPT) $(LIB) $(LIBDIR32) -lssl -lcrypto

o/of/httpd.exopenssl: $(OOBJS_TLS) o/runssl111.o
	$(GCC)  $(G)  $^ -o $@  $(LOPT) $(LIBF) $(LIBDIR32F) -lssl  $(LIBDIR32F) -lcrypto


o/httpd.exgnutls: $(OOBJS_TLS) o/rungnutls.o
	$(GCC)  $(G)  $^ -o $@ -Wl,--copy-dt-needed-entries $(LOPT) $(LIB) $(LIBDIR32) -lgnutls

o/of/httpd.exgnutls: $(OOBJS_TLS) o/rungnutls.o
	$(GCC)  $(G)  $^ -o $@  $(LOPT) $(LIBF)  $(LIBDIR32F) -l:libgnutls.so.30


tclean:
	rm $(OOBJS) $(OOBJS_TLS)

o64/httpd.exe: $(OOBJS64)
	$(GCC64)  $(G)  $(OOBJS64) -o $@  $(LOPT64) $(LIB64)

o64/of/httpd.exe: $(OOBJS64)
	$(GCC64)  $(G)  $(OOBJS64) -o $@  $(LOPT64) $(LIB64F)


o64/httpd.exopenssl: $(OOBJS64_TLS) o64/runssl111.o
	$(GCC64)  $(G)  $^ -o $@  -Wl,--copy-dt-needed-entries $(LOPT64) $(LIB64) $(LIBDIR64) -lssl $(LIBDIR64F) -lcrypto

o64/of/httpd.exopenssl: $(OOBJS64_TLS) o64/runssl111.o
	$(GCC64)  $(G)  $^ -o $@  $(LOPT64) $(LIB64F) $(LIBDIR64F) -lssl $(LIBDIR64F) -lcrypto


o64/httpd.exgnutls: $(OOBJS64_TLS) o64/rungnutls.o
	$(GCC64)  $(G)  $^ -o $@  -Wl,--copy-dt-needed-entries $(LOPT64) $(LIB64) $(LIBDIR64) -lgnutls

o64/of/httpd.exgnutls: $(OOBJS64_TLS) o64/rungnutls.o
	$(GCC64)  $(G)  $^ -o $@  $(LOPT64) $(LIB64F) $(LIBDIR64F) -l:libgnutls.so.30



wo/http.exe: $(WINOOBJS)
	$(WINEGCC) -s  $(WINOOBJS) -o $@ $(WINELFLG)

wo/httpg.exe: $(WINOOBJS)
	$(WINEGCC) -g  $(WINOOBJS) -o $@ $(WINELFLG)

o/sndmsg: o/sndmsg.o o/mstring1.o
	$(GCC)  $(G)  $^ -o $@  $(LOPT) $(LIB)

o/of/sndmsg: o/sndmsg.o o/mstring1.o
	$(GCC) $(G) $^ -o $@  $(LOPT) $(LIBF)

o64/sndmsg: o64/sndmsg.o
	$(GCC64)  $(G)  $^ -o $@  $(LOPT64) $(LIB64)

o64/of/sndmsg: o64/sndmsg.o
	$(GCC64)  $(G)  $^ -o $@  $(LOPT64) $(LIB64F)




#wo/bin2s.exe:  wo/bin2s.o
#	$(WINEGCC)  $(S)  $^ -o $@  -L$(MGDIR)\\lib -luser32 -lkernel32 -Wl,--subsystem,console -lmsvcrt_fix -lgcc -nodefaultlibs

bin2s :  bin2s.cpp
	gcc -s -O2 $^  -o $@


wo/shttps_mg.exe: wo/stpdta.o wo/sethttp3.o wo/mstring1.o wo/updr.res
	$(WINEGCC)  -s -Os $^ -o $@  -L$(MGDIR)\\lib -luser32 -lkernel32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -Wl,--subsystem,windows -nostartfiles  -Xlinker -Map -Xlinker wo/flxmaps  -Xlinker --entry=_start  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe
	upx -9 $@

wo/shttpsr_mg.exe: wo/stpdtar.o wo/sethttp3r.o wo/mstring1.o wo/updr.res
	$(WINEGCC)  -s -Os $^ -o $@  -L$(MGDIR)\\lib -luser32 -lkernel32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -Wl,--subsystem,windows -nostartfiles  -Xlinker -Map -Xlinker wo/flxmaps  -Xlinker --entry=_start  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe
	upx -9 $@

wo/shttps_mgi.exe: wo/stpdtai.o wo/sethttp3i.o wo/mstring1.o wo/updr.res
	$(WINEGCC)  -s -Os $^ -o $@  -L$(MGDIR)\\lib -luser32 -lkernel32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -Wl,--subsystem,windows -nostartfiles  -Xlinker -Map -Xlinker wo/flxmaps  -Xlinker --entry=_start  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe
	upx -9 $@

wo/shttpsr_mgi.exe: wo/stpdtari.o wo/sethttp3ri.o wo/mstring1.o wo/updr.res
	$(WINEGCC)  -s -Os $^ -o $@  -L$(MGDIR)\\lib -luser32 -lkernel32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -Wl,--subsystem,windows -nostartfiles  -Xlinker -Map -Xlinker wo/flxmaps  -Xlinker --entry=_start  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe
	upx -9 $@


wo/uninst.exe: wo/uninst.o
	$(WINEGCC)  -s  $^ -o $@  -nodefaultlibs -L$(MGDIR)\\lib -luser32 -lkernel32 -lgdi32 -lcomdlg32 -ladvapi32 -lshell32 -Wl,--subsystem,windows -nostartfiles  -Xlinker -Map -Xlinker wo/flxmaps  -Xlinker --entry=_start  -nostartfiles -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe



wo/stpdta.o: wo/stpdta.s wo/uninst.bin wo/http.bin wo/ind1.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/lic.bin wo/vpn_if_up.bin wo/vpn_if_client_up.bin wo/vpn_if_client_down.bin wo/manifest.bin
	 cd wo ;  $(WINEAS)  stpdta.s -o stpdta.o

wo/stpdtar.o: wo/stpdtar.s wo/uninst.bin wo/http.bin wo/ind1r.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/licr.bin wo/vpn_if_up.bin wo/vpn_if_client_up.bin wo/vpn_if_client_down.bin wo/manifest.bin
	 cd wo ;  $(WINEAS)  stpdtar.s -o stpdtar.o

wo/stpdtai.o: wo/stpdtai.s wo/uninst.bin wo/http.bin wo/ind1.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/lic.bin wo/ipbase.s wo/vpn_if_up.bin wo/vpn_if_client_up.bin wo/vpn_if_client_down.bin wo/manifest.bin
	 cd wo ;  $(WINEAS)  stpdtai.s -o stpdtai.o

wo/stpdtari.o: wo/stpdtari.s wo/uninst.bin wo/http.bin wo/ind1r.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/licr.bin wo/ipbase.s wo/vpn_if_up.bin wo/vpn_if_client_up.bin wo/vpn_if_client_down.bin wo/manifest.bin
	 cd wo ;  $(WINEAS)  stpdtari.s -o stpdtari.o

wo/stpdta.s: stpdta.s
	cp -f $< $@

wo/stpdtar.s: stpdtar.s
	cp -f $< $@

wo/stpdtai.s: stpdtai.s
	cp -f $< $@

wo/stpdtari.s: stpdtari.s
	cp -f $< $@

wo/%.bin:  wo/%.exe
	$(WINEBIN2S)  $< $@

# wo/%.bin:  %.htm
# 	$(WINEBIN2S)  $< $@

wo/ind1.bin:  desc.htm
	$(WINEBIN2S)  $< $@

wo/manifest.bin: http.exe.manifest
	$(WINEBIN2S)  $< $@

wo/%.bin: script_examples/%.bat
	$(WINEBIN2S)  $< $@

wo/ind1r.bin:  descr.htm
	$(WINEBIN2S)  $< $@

wo/licr.bin : licr06.txt
	$(WINEBIN2S)  $< $@

wo/lic.bin :  license06.txt
	$(WINEBIN2S)  $< $@

wo/shs_lang.bin :  ru/shs_lang.cfg
	$(WINEBIN2S)  $< $@

wo/eshs_lang.bin : en/shs_lang.cfg
	$(WINEBIN2S)  $< $@

wo/lnotes.bin :  lang_notes.txt
	$(WINEBIN2S)  $< $@

wo/ipbase.s : ipbase.bin
	$(WINEBIN2S)  $< $@


# o/t2icfg.o: t2icfg.cpp g4s1.hh



fixicfg: icfg.cpp
	./def4str2.pl S1 icfg.cpp g4s1.hh

fixicfghtm: icfghtm.cpp
	./def4str2.pl S5 icfghtm.cpp g4strhtm.hh

#t2icfg.cpp o/s1.hh: icfg.cpp

# t2icfg.cpp: icfg.cpp
# 	rm -f o/s1.hh
# 	./getstr -f o/s1.hh -y -n t2 icfg.cpp
# 	./def4str.pl S1 <o/s1.hh >g4s1.hh


#t2icfghtm.cpp o/strhtm.hh g4strhtm.hh: icfghtm.cpp

t2icfghtm.cpp: icfghtm.cpp
	rm -f o/strhtm.hh
	./getstr -f o/strhtm.hh -y -n t2 icfghtm.cpp
	./def4str.pl S5 <o/strhtm.hh >g4strhtm.hh


#GENERATED_HEADERS= g4s1.hh g4strc.h g4strcwm.h
GENERATED_HEADERS= g4strc.h g4strcwm.h

# g4s1.hh: t2icfg.cpp
# 	./def4str.pl S1 < $< > $@


#g4strc.h S2_lf.hh: strc.h

g4strc.h S2_lf.hh: strc.h
	./def4str.pl S2 < $< > g4strc.h
	echo "#define G4STRING_CONST_H\n\n" >> g4strc.h


S3_lf.hh: g4strcwm.h

g4strcwm.h: strcwm.h
	./def4str.pl S3 < $< > g4strcwm.h


t2icfgrus.cpp: icfgrus.cpp
	rm -f o/s1r.hh
	./getstr -f o/s1r.hh -y -n -r t2 icfgrus.cpp
	./def4str.pl S1 o/s1r1 <o/s1r.hh >o/g4s1r1.hh


# lang_rus1.cfg:  strcwmr.h
# 	./def4str.pl G4 o/strcwmr  <strcwmr.h  >lang_rus1.cfg

#cp httpd.exe d/httpd.exe

#
#	   ./packelf httpd.exe d/httpd.exe
#	   chmod 0777 d/httpd.exe


httpd_.exe: $(OBJS)
	   $(GCC) $(G)   $(OOBJS) -o $@ $(LIB) $(LOPT) -nostartfiles -Telfi386.x -nodefaultlibs
	   ./packelf httpd_.exe d/httpd.exe
	   chmod 0777 d/httpd.exe

o/httpd.exr: $(ROBJS)
	   $(GCC) $(G)   $(ROBJS) -o $@ $(LIB) $(LOPT) -nostartfiles -Telfi386.x -nodefaultlibs
	   cp httpd.exr d/httpd.exr



#o/dist/%.so: o/%.so
#	cp -f $^ $@

o/dist/%.so: o/of/%.so
	cp -f $^ $@

#o/dist/libsecgnutls.so: o/libsecgnutls.so
#	cp -f $^ $@

# o64/dist/libsec.so: o64/libsec.so
# 	cp -f $^ $@

o64/dist/%.so: o64/of/%.so
	cp -f $^ $@

o64/dist/libsecgnutls.so: o64/libsecgnutls.so
	cp -f $^ $@

o64/distu/libsec.so: o64/libsec.so
	cp -f $^ $@

o64/distu/%.so: o64/%.so
	cp -f $^ $@

o/distu/%.so: o/%.so
	cp -f $^ $@

o64/distu/libsecgnutls.so: o64/libsecgnutls.so
	cp -f $^ $@

o/dist/descu.htm o/adist/descu.htm o64/dist/descu.htm o64/distu/descu.htm: descu.htm
	cp -f $^ $@

o/dist/%.sh: script_examples/%.sh
	cp -f $^ $@

oo/dist/%.sh: script_examples/%.sh
	cp -f $^ $@

o/distu/%.sh: script_examples/%.sh
	cp -f $^ $@

o/adist/%.sh: script_examples/%.sh
	cp -f $^ $@

o64/dist/%.sh: script_examples/%.sh
	cp -f $^ $@

o64/distu/%.sh: script_examples/%.sh
	cp -f $^ $@

o/dist/license06.txt o/adist/license06.txt o64/dist/license06.txt o64/distu/license06.txt: license06.txt
	cp -f $^ $@

o/dist/notes.ssl o/adist/notes.ssl o64/dist/notes.ssl o64/distu/notes.ssl: notes.ssl
	cp -f $^ $@

o/adist/%.so: at/%.so
	cp -f $^ $@


en/shs_lang.cfg : lS1_lf.cfg
	./add_to_langcfg.pl lS1_lf.cfg $@

ru/shs_lang.cfg : lS1_lf.cfg
	./add_to_langcfg.pl lS1_lf.cfg $@


install: # all
	mkdir -p $(ICONFIG_APPDIR) $(ICONFIG_SHARE) $(ICONFIG_CONFIG) $(ICONFIG_LOG) $(ICONFIG_BASE)/bin $(ICONFIG_BASE)/share/man/man1
	for i in $(BINFILES) libsecgnutls.so libsec111.so ; do  if [ -e $(INSTALLFROM)$$i ] ; then cp $(INSTALLFROM)$$i $(ICONFIG_APPDIR) ; fi; done
	for i in httpd.cfg script_examples/*.sh ; do [ -e $(ICONFIG_CONFIG)/$$i ] || cp $$i $(ICONFIG_CONFIG) ; done
	for i in $(BINFILES) ; do  if [ -e $(ICONFIG_APPDIR)/$$i ] ; then  ln -sf $(CONFIG_APPDIR)/$$i $(ICONFIG_BASE)/bin/ ; fi ; done
	for i in ru en ; do mkdir -p $(ICONFIG_SHARE)/$$i ; cp $$i/shs_lang.cfg $(ICONFIG_SHARE)/$$i/ ; done
	for i in license.ssl notes.ssl license06.txt notes.ssl descu.htm ; do cp $$i $(ICONFIG_SHARE) ; done
	for i in httpd.exe.1 sndmsg.1 ; do cp $$i $(ICONFIG_BASE)/share/man/man1/ ; done
	for i in httpd.exgnutls.1 httpd.exopenssl.1 ; do ln -sf httpd.exe.1 $(ICONFIG_BASE)/share/man/man1/$$i ; done


DISTFILES=vpn_if_client_down.sh vpn_if_client_up.sh vpn_if_up.sh descu.htm httpd.cfg lang_notes.txt libsec111.so license.ssl notes.ssl libsecgnutls.so  license06.txt langpacks/ru langpacks/ru/shs_lang.cfg langpacks/en langpacks/en/shs_lang.cfg sndmsg httpd.exe.1 sndmsg.1
ODISTFILES=$(addprefix o/dist/,$(DISTFILES))
OUDISTFILES=$(addprefix o/distu/,$(DISTFILES))
ADISTFILES=$(addprefix o/adist/,$(DISTFILES))
O64DISTFILES=$(addprefix o64/dist/,$(DISTFILES))
O64UDISTFILES=$(addprefix o64/distu/,$(DISTFILES))
OODISTFILES=$(addprefix oo/dist/,$(DISTFILES))

PREPAREDISTDIRS= o/dist o/distu o/adist o64/dist o64/distu oo/dist

dist_dirs: $(PREPAREDISTDIRS)
	mkdir -p $(PREPAREDISTDIRS)

$(addsuffix /httpd.exe.1,$(PREPAREDISTDIRS)) : httpd.exe.1
	cp $^ $@

$(addsuffix /sndmsg.1,$(PREPAREDISTDIRS)) : sndmsg.1
	cp $^ $@

CURRENT_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
DIST_DIR=$(CURRENT_DIR)o/alldist/


$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# dist:   o/dist/libsec.so o/dist/libsec111.so o/dist/license06.txt o/adist/license06.txt o/dist/libsecgnutls.so o/dist/langpacks/ru/shs_lang.cfg o/dist/langpacks/en/shs_lang.cfg o/dist/descu.htm o/adist/

dist:   $(DIST_DIR) o/dist/license06.txt o/adist/license06.txt $(ODISTFILES) $(OUDISTFILES) $(ADISTFILES) dist64 arm64dist
	cp o/of/httpd.ex* o/dist/
	cd o/dist/ ; strip httpd.ex* ; chmod -R go-w,u+rw,a+X * ; chmod 0755 httpd.ex* *.so sndmsg ; chmod 0644 *.1 ; chmod  0600 httpd.cfg ;  rm -f $(DIST_DIR)shttplnx.tgz ; tar --owner=root --group=root -czf $(DIST_DIR)shttplnx.tgz * ; chmod 0644 $(DIST_DIR)shttplnx.tgz
	cp at/httpd.ex* o/adist/
	cd o/adist/ ; arm-linux-gnueabi-strip httpd.ex* ; chmod -R go-w *  ; chmod -R a+X * ; chmod  0755 httpd.ex* *.so  sndmsg ; chmod 0644 *.1 ; chmod  0600 httpd.cfg ;  rm -f $(DIST_DIR)shttparmlnx.tgz ; tar --owner=root --group=root -czf $(DIST_DIR)shttparmlnx.tgz * ; chmod 0644 $(DIST_DIR)shttparmlnx.tgz
	cp o/httpd.exe o/distu/
	cd o/distu/ ; strip httpd.exe ; chmod -R go-w,u+rw,a+X * ; chmod  0755 httpd.exe *.so sndmsg ; chmod  0600 httpd.cfg ;  rm -f $(DIST_DIR)shttplnxu.tgz ; tar --owner=root --group=root -czf $(DIST_DIR)shttplnxu.tgz * ; chmod 0644 $(DIST_DIR)shttplnxu.tgz

#dist64: o64/dist/libsec.so o64/dist/libsec111.so o64/dist/libsecgnutls.so o64/distu/libsec.so o64/distu/libsecgnutls.so o64/dist/langpacks/ru/shs_lang.cfg o64/dist/langpacks/en/shs_lang.cfg


dist64: o64/dist/libsec111.so o64/dist/libsecgnutls.so o64/distu/libsec111.so o64/distu/libsecgnutls.so $(O64DISTFILES) $(O64UDISTFILES)
	cp o64/of/httpd.ex* o64/dist/
	cd o64/dist/ ; strip httpd.ex* ; chmod -R go-w,u+rw,a+X * ; chmod  0755 httpd.ex* *.so  sndmsg ; chmod 0644 *.1 ; chmod  0600 httpd.cfg ;  rm -f $(DIST_DIR)shttplnx64.tgz ; tar --owner=root --group=root -czf $(DIST_DIR)shttplnx64.tgz * ; chmod a+rw $(DIST_DIR)shttplnx64.tgz
	cp o64/httpd.exe o64/distu/httpd.exe
	cd o64/distu/ ; strip httpd.exe ; chmod -R go-w,u+rw,a+X * ; chmod  0755 httpd.exe *.so  sndmsg ; chmod  0600 httpd.cfg ;  rm -f $(DIST_DIR)shttplnx64u.tgz ; tar --owner=root --group=root -czf $(DIST_DIR)shttplnx64u.tgz * ; chmod 0644 $(DIST_DIR)shttplnx64u.tgz


arm64dist: arm64
arm64dist: CROSS_COMPILE:=aarch64-linux-gnu-
arm64dist: arm64 oo/dist $(addprefix oo/dist/,$(DISTFILES)) oo.prepare_dist
	rm -f $(DIST_DIR)shttparm64lnx.tgz
	cd oo/dist ; tar --owner=root --group=root -czf $(DIST_DIR)shttparm64lnx.tgz * ; chmod 0644 $(DIST_DIR)shttparm64lnx.tgz

%.prepare_dist: %/dist %/httpd.exe
	cp $*/httpd.ex* $*/dist/
	cd $*/dist/ ; $(CROSS_COMPILE)strip httpd.ex* sndmsg *.so ; chmod -R go-w,u+rw,a+X * ; chmod  0755 httpd.ex* *.so sndmsg ; chmod 0644 *.1; chmod 0600 httpd.cfg

oo/dist/% : oo/%
	cp $^ $@

oo/dist: $(TMPRAM)oo
	mkdir -p $@

sinst:  o/dist/httpd.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnxu.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz  $(DIST_DIR)shttplnx64u.tgz
	chmod 0666 wo/shttps*.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe $(DIST_DIR)*.tgz o/smallsrv_$(VERSION)_amd64.deb o/smallsrv-$(VERSION).tar.gz ; do echo $$i ; cp $$i /mnt/d/var/www/pre/ ; done
	if [ "$(VERSION)" != "$(VERSIONT)" ] ; then ln -sf smallsrv_$(VERSION)_amd64.deb /mnt/d/var/www/pre/smallsrv_$(VERSIONT)_amd64.deb ; fi

# 	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnxu.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz $(DIST_DIR)shttplnx64u.tgz ; do cp $$i /mnt/d/var/www/pre/ ; done

# 	mv /mnt/d/shttps/httpd.exe /mnt/d/shttps/oldhttpd_$(shell date "+%F_%R").exe
# 	cp o/distu/httpd.exe /mnt/d/shttps/

sinste:  o/dist/httpd.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnx64.tgz
	chmod 0666 wo/shttps*.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnx64.tgz ; do cp $$i /mnt/d/var/www/pre/ ; done
	mv /mnt/d/shttps/httpd.exe /mnt/d/shttps/oldhttpd.exe
	cp o/dist/httpd.exe /mnt/d/shttps/

sinstcpy:  o/dist/httpd.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnxu.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz  $(DIST_DIR)shttplnx64u.tgz
	chmod 0666 wo/shttps*.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttplnxu.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz $(DIST_DIR)shttplnx64u.tgz ; do cp $$i ./lastbin ; done

sinsta: sinst wo/shttps_mgi.exe wo/shttpsr_mgi.exe
	chmod 0666 wo/shttps_mgi.exe wo/shttpsr_mgi.exe
	for i in wo/shttps_mgi.exe wo/shttpsr_mgi.exe ; do cp $$i /mnt/d/var/www/pre/ ; done

sinstw: wo/shttps_mg.exe wo/shttpsr_mg.exe
	chmod 0666 wo/shttps_mg.exe wo/shttpsr_mg.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe ; do cp $$i /mnt/d1/var/www/pre/ ; done


sinstf:  o/dist/httpd.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz  $(DIST_DIR)shttplnx64u.tgz
	chmod 0666 wo/shttps*.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe wo/shttps_mgi.exe wo/shttpsr_mgi.exe $(DIST_DIR)shttplnx.tgz $(DIST_DIR)shttparmlnx.tgz $(DIST_DIR)shttplnx64.tgz $(DIST_DIR)shttplnx64u.tgz $(DIST_DIR)shttplnx64u.tgz shttp_src.tgz ; do cp $$i /mnt/d/var/www/pre/ ; done
	mv /mnt/d/shttps/httpd.exe /mnt/d/shttps/oldhttpd.exe
	cp o/distu/httpd.exe /mnt/d/shttps/
	for i in indexr.htm index.htm shttp3e.xml shttp3.xml ; do cp $$i /mnt/d/var/www/ ; done

%/langpacks/en:
	mkdir -p $@

%/langpacks/ru:
	mkdir -p $@

%/langpacks/en/shs_lang.cfg: en/shs_lang.cfg %/langpacks/en
	cp -f $< $@

%/langpacks/ru/shs_lang.cfg: ru/shs_lang.cfg %/langpacks/ru
	cp -f $< $@

o%/license.ssl: lbins/license.ssl
	cp -f $< $@


%/license06.txt: license06.txt
	cp -f $< $@

%/notes.ssl: notes.ssl
	cp -f $< $@

%/descu.htm: descu.htm
	cp -f $< $@

%/lang_notes.txt: lang_notes.txt
	cp -f $< $@

o%/httpd.cfg: lbins/httpd.cfg
	cp -f $< $@

o64/distu/sndmsg: lbins/o64u/sndmsg
	cp -f $< $@

o64/dist/sndmsg: lbins/o64/sndmsg
	cp -f $< $@

o/dist/sndmsg: lbins/o/sndmsg
	cp -f $< $@

o/distu/sndmsg: lbins/u/sndmsg
	cp -f $< $@

o/adist/sndmsg: lbins/arm/sndmsg
	cp -f $< $@

o/libsec.so: runssl.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32) -lssl

o64/libsec.so: runssl.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64) -lssl  $(LIBDIR64) -lc

o/libsecgnutls.so: rungnutls.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $<  -nodefaultlibs $(LIBDIR32) -l:libgnutls.so.30  $(LIBDIR32) -lc

o64/libsecgnutls.so: rungnutls.cpp
	 $(GCC64) -s -o $@   -fPIC $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64) -l:libgnutls.so.30  $(LIBDIR64) -lc

o/libsec111.so: runssl111.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32) -lssl -DOSSL111 -I/usr/src/openssl-1.1.1m/include

o/libsec3.so: runssl3.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32) -l:libssl.so.3 -DOSSL111 -DOSSL33 -I/usr/src/openssl-3.2.1/include

o64/libsec111.so: runssl111.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64) -lssl  $(LIBDIR64) -lc -DOSSL111 -I/usr/src/openssl-1.1.1m/include

o64/libsec3.so: runssl111.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $< -nodefaultlibs $(LIBDIR64) -l:libssl.so.3 $(LIBDIR64) -lc -DOSSL111 -I/usr/src/openssl-3.2.1/include


o/of/libsec.so: runssl.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32F) -lssl

o64/of/libsec.so: runssl.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64F) -lssl  $(LIBDIR64F) -lc

o/of/libsecgnutls.so: rungnutls.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $<  -nodefaultlibs $(LIBDIR32F) -l:libgnutls.so.30  $(LIBDIR32F) -lc

o64/of/libsecgnutls.so: rungnutls.cpp
	 $(GCC64) -s -o $@   -fPIC $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64F) -l:libgnutls.so.30  $(LIBDIR64F) -lc

o/of/libsec111.so: runssl111.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32F) -lssl -DOSSL111 -I/usr/src/openssl-1.1.1m/include

o/of/libsec3.so: runssl3.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< $(LIBDIR32F) -lssl -DOSSL111 -DOSSL32 -I/usr/src/openssl-3.2.1/include


o64/of/libsec111.so: runssl111.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $<  -nodefaultlibs $(LIBDIR64F) -lssl  $(LIBDIR64F) -lc -DOSSL111 -I/usr/src/openssl-1.1.1m/include

at/runssl111.o: runssl111.cpp
	 $(ATGPP) -c -s -o $@  -DOSSL111 -I/usr/src/openssl-1.1.1m/include $(ATFLAGS) -O2  $<

at/libsec111.so: runssl111.cpp
	 $(ATGPP) -s -o $@  -DOSSL111 -I/usr/src/openssl-1.1.1m/include -fPIC $(ATFLAGS) -shared -O2  $<  -nodefaultlibs  -L$(ATLIB)/ -lc -L./fakelibsarm -lssl

at/libsec3.so: runssl3.cpp
	 $(ATGPP) -s -o $@  -fPIC  $(ATFLAGS) -shared -O2  $< $(ATLIB) -lssl -DOSSL111 -DOSSL32 -I/usr/src/openssl-3.2.1/include


at/libsecgnutls.so: rungnutls.cpp
	 $(ATGPP) -s -o $@   -fPIC $(ATFLAGS) -shared -O2  $<  -nodefaultlibs  -L$(ATLIB)/ -lc -L./fakelibsarm -l:libgnutls.so.30

at/libsec111.so at/libsecgnutls.so: $(FAKELIBSARM)


SSLIDIR=z:\\home\\adv\\openssl\\include

wo/libsec.o: wo/runssl.cpp
	$(WINEGCC)  $(G)  $< -c -o $@ -I$(MGDIR)\\include -fno-implicit-templates -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -I$(SSLIDIR) -nodefaultlibs -nostartfiles

SSLIDIR111=/usr/src/openssl-1.1.1w/include

wo/libsec111.o: runssl111.cpp
	$(WINEGCC)  $(G)  $< -c -o $@ $(WINECFLG) -DOSSL111 -I$(MGDIR)/include -fno-implicit-templates -DSERVICE -DFREE_VER -DFREEVER -DV_FULL=1 -DUSE_IPV6 -I$(SSLIDIR111) -nodefaultlibs -nostartfiles

SSLLIBDIR=z:\\home\\adv\\openssl\\lib
SSLLIBDIR111=/usr/src/openssl-1.1.1w
SSLLIBDIR321=/usr/src/win/openssl-3.2.1
#./Configure --cross-compile-prefix=i686-w64-mingw32-  mingw


wo/libsec.dll: wo/libsec.o
	$(WINEGCC)  -s  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(SSLLIBDIR) -lssl -lcrypto -L$(MGDIR)\\lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -lmsvcrt_fix -lgcc -Wl,--entry=_DllMainCRTStartup  -Xlinker --heap  -Xlinker 0x20000000   -Wl,--stack -Wl,0x800000 -Wl,-Map -Wl,wo/dllmap -Wl,--subsystem,windows

wo/libsec321.dll: wo/libsec111.o
	$(WINEGCC)  -g  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(SSLLIBDIR321) -Wl,-Bstatic -lssl -Wl,-Bstatic -lcrypto  -L$(MGDIR)/lib -luser32  -lws2_32 -lkernel32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -lmsvcrt -lcrypt32 -Wl,-Bstatic -lgcc -Wl,--entry=_DllMainCRTStartup -Wl,--heap -Xlinker 0x800000 -Wl,--stack -Wl,0x400000 -Wl,--subsystem,windows -Wl,-Map -Wl,wo/dllmap

wo/libsec111.dll: wo/libsec111.o
	$(WINEGCC)  -g  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(SSLLIBDIR111) -Wl,-Bstatic -lssl -Wl,-Bstatic -lcrypto  -L$(MGDIR)/lib -luser32  -lws2_32 -lkernel32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -lmsvcrt -Wl,-Bstatic -lgcc -Wl,--entry=_DllMainCRTStartup -Wl,--heap -Xlinker 0x800000 -Wl,--stack -Wl,0x400000 -Wl,--subsystem,windows -Wl,-Map -Wl,wo/dllmap

wo/libsec111dyn.dll: wo/libsec111.o
	$(WINEGCC)  -g  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(SSLLIBDIR111) -lssl -lcrypto -L$(MGDIR)/lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -lgcc -Wl,--entry=_DllMainCRTStartup -Wl,--heap -Xlinker 0x800000 -Wl,--stack -Wl,0x400000 -Wl,--subsystem,windows -Wl,-Map -Wl,wo/dllmap

wo/libsec.dll: /home/adv/openssl/lib/libssl.a /home/adv/openssl/lib/libcrypto.a


GNUTSLIDIR=/usr/src/gnutls-3.6.0-w32/lib


wo/libsecgnutls.o: rungnutls.cpp
	$(WINEGCC)  $(G)  $< -c -o $@ -I$(MGDIR)/include -fno-implicit-templates -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -I$(GNUTSLIDIR)


GNUTSLLIBDIR=/usr/src/gnutls-3.6.0-w32/lib


wo/libsecgnutls.dll: wo/libsecgnutls.o
	$(WINEGCC)  -s  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(GNUTSLLIBDIR) -l:libgnutls.dll.a -L$(MGDIR)/lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Wl,--entry=_DllMainCRTStartup  -Wl,--heap -Wl,0x20000000 -Wl,--stack -Wl,0x800000 -Wl,-Map -Wl,wo/dllmap -Wl,--subsystem,windows

SECOBJ=runssl111.o rungnutls.o sndmsg.o

OBJDIRSNAMES := o o64 wo at oo
OBJDIRS := o/ o64/ wo/ at/ oo/
OBJDIRS_TLS := $(addsuffix tls_,$(OBJDIRS))

${OBJDIRSNAMES} :
	mkdir -p ${TMPRAM}/$@
	ln -s ${TMPRAM}/$@ .

$(addsuffix srv0a.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h srv0a.cpp slloop.cpp srvars.cpp seplog.cpp onelog.cpp
$(addsuffix adminr.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h adminr.cpp icfghtm.cpp g4strhtm.hh
$(addsuffix stat.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h stat.cpp cstat.cpp statusr.cpp
$(addsuffix smptps.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h smptps.cpp pop3d.cpp
$(addsuffix conf.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h g4s1.hh g4strcwm.h S2_lf.hh
# $(addsuffix t2icfg.o,$(OBJDIRS) $(OBJDIRS_TLS)): g4strc.h g4s1.hh g4strcwm.h S2_lf.hh
$(addsuffix wmbx.o,$(OBJDIRS) $(OBJDIRS_TLS)):  g4strc.h g4s1.hh g4strcwm.h
$(addsuffix srv_ssi.o,$(OBJDIRS) $(OBJDIRS_TLS)):  g4strc.h regular.cpp

#o/srv0a.o o64/srv0a.o wo/srv0a.o r/srv0a.o lpc/srv0a.o at/srv0a.o : g4strc.h srv0a.cpp slloop.cpp srvars.cpp seplog.cpp onelog.cpp
#o/adminr.o o64/adminr.o r/adminr.o wo/adminr.o at/adminr.o : g4strc.h adminr.cpp t2icfghtm.cpp g4strhtm.hh
#o/stat.o o64/stat.o wo/stat.o r/stat.o at/stat.o mips/stat.o : g4strc.h stat.cpp cstat.cpp statusr.cpp
#o/smptps.o o64/smptps.o r/smptps.o mips/smptps.o at/smptps.o : g4strc.h smptps.cpp pop3d.cpp
wo/fwnd.o: wndcfg.cpp
#o/conf.o o64/conf.o o/t2icfg.o o64/t2icfg.o wo/conf.o wo/t2icfg.o at/t2icfg.o at/conf.o: g4strc.h g4s1.hh g4strcwm.h S2_lf.hh
#o/wmbx.o o64/wmbx.o o/t2icfg.o wo/wmbx.o at/t2icfg.o at/wmbx.o: g4strc.h g4s1.hh g4strcwm.h
#o/srv_ssi.o o64/srv_ssi.o wo/srv_ssi.o r/srv_ssi.o lpc/srv_ssi.o at/srv_ssi.o : g4strc.h regular.cpp

NIOBJS=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o msprintfchk.o

MIPSDIR=/opt/brcm/hndtools-mipsel-linux-3.2.3
MIPSPATH=$(MIPSDIR)/bin:$(PATH)
MIPSOBJ=$(addprefix mips/,$(NIOBJS))
MIPSOPT= -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs\
-fno-verbose-asm -fno-implicit-templates  \
 -fwritable-strings -DUSE_IPV6  -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated  -mno-stats   -mno-mips-tfile

MIPSLOPT= -Xlinker -Map -Xlinker flxmap


#ARMDIR=/usr/local/arm-linux
ARMDIR=/usr/arm-linux-gnueabi
ARMPATH=$(ARMDIR)/bin:$(PATH)
#ARMOBJ=$(addprefix arm/,$(NIOBJS))
ARMOBJ= arm/to_linux.o $(addprefix arm/,$(POBJS)) arm/telnet.o

ARMOPT= -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
-fno-verbose-asm -fno-implicit-templates  -DUSE_FUTEX -DUSE_POOL -DFREEVER -DWITHMD5 -DTELNET -DSHS_VERSION=$(VERSION) \
 -fwritable-strings -DUSE_IPV6  -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated

ARMLOPT= -Xlinker -Map -Xlinker flxmap



mips/%.o :  %.cpp
	export PATH=$(MIPSPATH) ; mipsel-linux-g++ -c -s -DSYSUNIX  -I $(MIPSDIR)/mipsel-linux/include  -DMIPS -DNOTINTEL -DV_FULL=1 $(G) $(MIPSOPT) $< -o $@

mips/httpd.exe :  $(MIPSOBJ)
	cd mips ; export PATH=$(MIPSPATH) ; mipsel-linux-gcc $(G) -s  $(NIOBJS) -o httpd.exe  -L$(MIPSDIR)/lib -L$(MIPSDIR)/mipsel-linux/lib $(MIPSLOPT) $(LIB)



arm/%.o :  %.cpp
	export PATH=$(ARMPATH) ; arm-linux-g++ -c -s -DSYSUNIX  -DARM -DNOTINTEL -DV_FULL=1 $(G) $(ARMOPT) $< -o $@

arm/httpd.exe :  $(ARMOBJ)
	cd arm ; export PATH=$(ARMPATH) ; arm-linux-gcc $(G) -s  $(NIOBJS) -o httpd.exe  $(ARMLOPT) $(LIB)


arm/runssl.o :  runssl.cpp
	export PATH=$(ARMPATH) ; arm-linux-g++ -c -s -DSYSUNIX  -DARM -DNOTINTEL -DV_FULL=1 $(G) $(ARMOPT) $< -o $@ -I/usr/include

#arm/runssl111.o :  runssl111.cpp
#	export PATH=$(ARMPATH) ; arm-linux-g++ -c -s -DSYSUNIX  -DARM -DNOTINTEL -DV_FULL=1 -DOSSL111 $(G) $(ARMOPT) $< -o $@ -I -I/usr/src/openssl-1.1.1m/include


arm/libsec.so:    arm/runssl.o
	 export PATH=$(ARMPATH) ; arm-linux-gcc $(G) -s $^  -o $@  -shared $(ARMLOPT) $(LIB)

#arm/libsec111.so:    arm/runssl111.o
#	 export PATH=$(ARMPATH) ; arm-linux-gcc $(G) -s $^  -o $@  -shared $(ARMLOPT) $(LIB)

LIOBJS=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o telnet.o md5.o msprintfchk.o

LIOBJS1=$(LIOBJS) pthread.o

#

LPCOBJS=$(addprefix lpc/,$(LIOBJS1))

lpc/%.o : %.cpp
	  $(ULGPP) -c $(S)  -DARM -DLINUX  -DSYSUNIX -DARM -DNOTINTEL -DLPC_ARM -DFREEVER -DTELNET -DV_FULL=1 $(ULCFLAGS) -o $@  $<

lpc/httpd.exe: $(LPCOBJS)
	$(ULGPP)  $(S) -o $@ $^ -mthumb -march=armv7 -mfix-cortex-m3-ldrd  -Xlinker  -elf2flt -Xlinker -EL -L/usr/local/arm-2010q1/arm-uclinuxeabi/libc/thumb2/usr/lib -lc -lutil


ATGPP=arm-linux-gnueabi-gcc

ATLIB2=/home/max/electr/atmel/SAM9-L9260_Rev.D/sources/rd/usr/lib
ATINCLUDE2=/home/max/electr/atmel/SAM9-L9260_Rev.D/sources/rd/usr/include

#ATLIB=/usr/arm-linux-gnueabi/lib
#ATINCLUDE=/usr/arm-linux-gnueabi/include
ATSYSINCLUDE=/usr/lib/gcc-cross/arm-linux-gnueabi/10/include
ATSYSLIB=/usr/lib/gcc-cross/arm-linux-gnueabi/10/

# ATLIB=/usr/src/at/musl-1.0.4/lib
# ATINCLUDE=/usr/src/at/musl-1.0.4/include

ATLIB=/usr/src/musl-1.2.3/lib
ATINCLUDE=/usr/src/musl-1.2.3/include
ATINCLUDE3=/usr/src/musl-1.2.3/arch/arm
ATINCLUDE4=/usr/src/musl-1.2.3/obj/include
ATINCLUDE5=/usr/src/musl-1.2.3/arch/generic

ATFLAGS= -mlittle-endian -marm  -mcpu=arm926ej-s -mthumb-interwork  -msoft-float -Uarm  -nostdinc -I$(ATINCLUDE) -I$(ATINCLUDE3) -I$(ATINCLUDE4) -I$(ATINCLUDE5) -I$(ATSYSINCLUDE)  -Wno-deprecated-declarations -Wno-conversion  -Wno-write-strings  -fno-access-control  -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs   -fno-rtti  -fno-weak -nostdinc++  -Wnoexcept   -fno-exceptions -Wno-format -fpermissive -DARM -DLINUX  -DSYSUNIX -DNOTINTEL -DAT_ARM -DFREEVER -DTELNET -DUSE_IPV6 -DV_FULL=1 -DUSE_FUTEX -DUSE_POOL -DWITHMD5 -DFIX_EXCEPT -DUSEVALIST -DVPN_LINUX -DTLSVPN -DSHS_VERSION=$(VERSION)

#   -fno-implicit-templates -fno-implicit-inline-templates  -fno-for-scope-fnothrow-opt  -fno-pretty-templates


ATOBJS0=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o telnet.o md5.o msprintfchk.o fcgi.o vpn.o

ATOBJS=$(addprefix at/,$(ATOBJS0))
ATOBJS_TLS=$(addprefix at/tls_,$(ATOBJS0))

$(ATOBJS): $(GENERATED)

at/%.o : %.cpp
	  $(ATGPP) -c $(S) $(ATFLAGS) -o $@  $<

at/tls_%.o : %.cpp
	  $(ATGPP) -c $(S) $(ATFLAGS) -DTLSWODLL -o $@  $<

at/%.o : %.c
	  $(ATGPP) -c $(S) $(ATFLAGS) -o $@  $<

at/tls_%.o : %.c
	  $(ATGPP) -c $(S) $(ATFLAGS) -DTLSWODLL -o $@  $<


at/atobjdir:
	mkdir -p /dev/shm/shttps/o/at
	[ -d at ] || ln -sf /dev/shm/shttps/o/at .
	echo   >$@


at/httpd.exe: $(ATOBJS)
	$(ATGPP)  $(S) -o $@ $^  -mlittle-endian  -msoft-float -march=armv5te -mtune=arm9tdmi -L$(ATLIB)/  -L$(ATSYSLIB)  -nostartfiles  $(ATLIB)/crt1.o $(ATLIB)/crti.o $(ATLIB)/crtn.o -static -lc -lutil -lpthread

at/httpd.exopenssl: $(ATOBJS_TLS) at/runssl111.o
	$(ATGPP)  $(S) -o $@ $^  -mlittle-endian  -msoft-float -march=armv5te -mtune=arm9tdmi -L$(ATLIB)/  -L$(ATSYSLIB)  -nostartfiles  $(ATLIB)/crt1.o $(ATLIB)/crti.o $(ATLIB)/crtn.o -static -lc -lutil -lpthread -Wl,-Bdynamic $(LIBDIRARMF) -lssl  $(LIBDIRARMF) -lcrypto

at/httpd.exgnutls: $(ATOBJS_TLS) at/rungnutls.o
	$(ATGPP)  $(S) -o $@ $^  -mlittle-endian  -msoft-float -march=armv5te -mtune=arm9tdmi -L$(ATLIB)/  -L$(ATSYSLIB)  -nostartfiles  $(ATLIB)/crt1.o $(ATLIB)/crti.o $(ATLIB)/crtn.o -static -lc -lutil -lpthread -Wl,-Bdynamic $(LIBDIRARMF) -l:libgnutls.so.30



N_OBJS0=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o tools.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o telnet.o md5.o msprintfchk.o fcgi.o vpn.o
N_OBJS=$(addprefix oo/,$(N_OBJS0))
N_OBJS_TLS=$(addprefix oo/tls_,$(N_OBJS0))

$(N_OBJS): $(GENERATED)

n_all: A_GCC=$(CROSS_COMPILE)gcc
n_all: N_FLAGS = -mlittle-endian -Wno-deprecated-declarations -Wno-conversion  -Wno-write-strings  -fno-access-control  -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs   -fno-rtti  -fno-weak -nostdinc++  -Wnoexcept -fno-exceptions -Wno-format -fpermissive -DLINUX -DSYSUNIX -DNOTINTEL -DFREEVER -DTELNET -DUSE_IPV6 -DV_FULL=1 -DUSE_FUTEX -DUSE_POOL -DWITHMD5 -DFIX_EXCEPT -DUSEVALIST -DVPN_LINUX -DTLSVPN -DUSE_SYSPASS -DSHS_VERSION=$(VERSION) $(A_OPT) $(ADVOPT)
n_all: $(TMPRAM)oo o oo $(N_DEP) oo/httpd.exe oo/sndmsg oo/libsec111.so oo/libsecgnutls.so oo/httpd.exopenssl oo/httpd.exgnutls


oo/%.o : %.cpp
	  $(A_GCC) -c $(S) $(N_FLAGS) -o $@  $<

oo/tls_%.o : %.cpp
	  $(A_GCC) -c $(S) $(N_FLAGS) -DTLSWODLL -o $@  $<

oo/%.o : %.c
	  $(A_GCC) -c $(S) $(N_FLAGS) -o $@  $<

oo/tls_%.o : %.c
	  $(A_GCC) -c $(S) $(N_FLAGS) -DTLSWODLL -o $@  $<

oo/runssl111.o: runssl111.cpp
	 $(A_GCC) -c -s -o $@  -DOSSL111 -I/usr/src/openssl-1.1.1m/include $(N_FLAGS) -O2  $<

oo/httpd.exe: $(N_OBJS)
	$(A_GCC)  $(S) -o $@ $^ $(N_LFLAGS) -lc -lutil -lpthread -ldl $(N_LFAKELIBS) -lcrypt

oo/httpd.exopenssl: $(N_OBJS_TLS) oo/runssl111.o
	$(A_GCC)  $(S) -o $@ $^  $(N_LFLAGS) -lc -lutil -lpthread -ldl -Wl,-Bdynamic $(N_LFAKELIBS) -lssl $(N_LFAKELIBS) -lcrypto $(N_LFAKELIBS) -lcrypt

oo/httpd.exgnutls: $(N_OBJS_TLS) oo/rungnutls.o
	$(A_GCC)  $(S) -o $@ $^ $(N_LFLAGS) -lc -lutil -lpthread -ldl -Wl,-Bdynamic  $(N_LFAKELIBS) -l:libgnutls.so.30 $(N_LFAKELIBS) -lcrypt

oo/libsecgnutls.so: rungnutls.cpp
	 $(A_GCC) -s -o $@   -fPIC -shared -O2 -I/usr/src/openssl-1.1.1m/include $(N_FLAGS) -O2  $<  $(N_LFAKELIBS) -l:libgnutls.so.30 -lc

oo/libsec111.so: runssl111.cpp
	 $(A_GCC) -s -o $@  -fPIC  -shared -O2 -DOSSL111 -I/usr/src/openssl-1.1.1m/include $(N_FLAGS) $< $(N_LFAKELIBS) -lssl

oo/sndmsg: oo/sndmsg.o
	$(A_GCC)  $(G)  $^ -o $@ -lc

oo/fakelibs:
	mkdir -p $@

oo/fakelibs/%.so: /usr/lib/x86_64-linux-gnu/%.so
	./mkfakelib_cros.pl $< $(CROSS_COMPILE) oo/fakelibs/ -c

httpd.exe.1: httpd_usage.1.in o64/httpd.exe
	help2man -i $< -o $@ -N ./o64/httpd.exe

sndmsg.1: sndmsg_usage.1.in o64/sndmsg
	help2man -i $< -o $@ -N ./o64/sndmsg


clean_n:
	rm -rf $(N_OBJS_TLS) $(N_OBJS) oo/fakelibs oo/httpd.exe oo/sndmsg oo/libsec111.so oo/libsecgnutls.so oo/httpd.exopenssl oo/httpd.exgnutls $(addprefix oo/,$(SECOBJ))



cleanobj:
	rm -f $(OOBJS) $(OOBJS64) $(OOBJS64_TLS) $(OOBJS_TLS) $(ATOBJS) $(ATOBJS_TLS) $(addprefix o/,$(SECOBJ)) $(addprefix o64/,$(SECOBJ)) $(addprefix at/,$(SECOBJ)) $(N_OBJS_TLS) $(N_OBJS)

clean:  clean_n
	rm -f $(OOBJS) $(OOBJS64) $(ROBJS) $(MIPSOBJ) $(WINOOBJS)  $(ATOBJS) $(ARMOBJ) wo/libsecgnutls.o wo/libsec111.o $(OOBJS64_TLS) $(OOBJS_TLS)  $(ATOBJS_TLS) $(addprefix o/,$(SECOBJ)) $(addprefix o64/,$(SECOBJ)) $(addprefix at/,$(SECOBJ)) $(WINOOBJS)

cleangen:
	rm -f $(GENERATED)

cleanall: clean cleangen
	rm -f o/httpd.exe o64/httpd.exe wo/http.exe http.exe wo/httpg.exe wo/shttps_mg.exe  wo/shttps_mgi.exe  wo/shttpsr_mg.exe  wo/shttpsr_mgi.exe wo/uninst.exe wo/libsecgnutls.dll o/libsec111.so o/libsecgnutls.so o64/httpd.exe o64/libsecgnutls.so o64/libsec111.so at/httpd.exe at/libsec111.so at/libsecgnutls.so
	if [ "$(TMPRAM)" = "/dev/shm/shttps/o/" ] ; then rm -rf $(TMPRAM) ; fi

SRC = accept.cpp cdde.cpp descu.htm fix_langpack.pl gz.h LICENSE mstring1.h pop3d.cpp secdll.h srv_ssi.cpp strtoul.cpp uninst.cpp \
add_to_langcfg.pl conf.cpp dgpp_quote.h fix_rdesk.pl heap.cpp license06.txt md5.cpp mstring2.cpp proxy.cpp rulst.cpp seplog.cpp stat.cpp strvar.cpp updr.rc \
adminr.cpp configure dhcpd.cpp ftpd.cpp hton.h licr06.txt md5.h musers.cpp prxcach.cpp rungnutls.cpp sethttp3.cpp statusr.cpp \
arhbb.cpp dhcp.h fwnd.cpp icfg.cpp lS1_lf.cfg mdef.h qsort.cpp runssl111.cpp slib.h stpdtai.s  wgnuext.cpp \
 dhhosts.h g4s1.hh icfghtm.cpp lS2_lf.cfg mkfakelib64.pl ndpdbg.s README.md runssl.cpp slloop.cpp stpdtamg.s t3_g4strc.h \
bin2s.cpp cres.cpp g4strc.h icfgjs.cpp lS3_lf.cfg mkfakelib_arm.pl realloc.s S1_lf.hh smptps.cpp stpdtari.s telnet.cpp winpr.cpp \
bvprintf.cpp g4strcwm.h lS5_lf.cfg mkfakelib.pl ndp.s S2_lf.hh smtpcl.cpp stpdtar.s tlsdll.h wmbx.cpp \
bvprintf.h crt0pe.s dnsd.cpp g4strhtm.hh ico2.h mailip.cpp mlist.h nommc_dlfcn.h regular.cpp S3_lf.hh srv0a.cpp stpdta.s tlsm.cpp wndcfg.cpp \
bvprintfv.cpp cstat.cpp get4def.pl ico.h mrc.cpp onelog.cpp regular.h S5_lf.hh srvars.cpp _strc.h to_linux.cpp xx31.ico \
 def4str.pl fcgi.cpp getstr isapi.cpp makefile.bsd mrc.h pe_m.h req.cpp srv_cgi.cpp strc.h to_linux.h \
 desc.htm fcgi.h getstr.cpp isapi.h makefile.dj msprintfchk.cpp pkbig.cpp ress1.s srvdat.cpp strcwm.h to_win.h \
c4char.pl descr.htm gz.cpp lang_notes.txt makefile.in mstring1.cpp pklz.cpp restart.cpp srv.h strcwmr.h vpn.cpp vpn.h tools.cpp mkfakelib_cros.pl sndmsg.cpp httpd.exe.1 sndmsg.1 sndmsg_usage.1.in httpd_usage.1.in def4str2.pl httpd.cfg

WSRC = $(addprefix winclude/,winsock_IPv6.h zconf.h zlib.h tap-windows.h nethdr.h)
LSRC = $(addsuffix shs_lang.cfg, ru/ en/)
SSRC = $(addprefix script_examples/,vpn_if_client_down.bat vpn_if_client_down.sh vpn_if_client_up.bat vpn_if_client_up.sh vpn_if_up.bat vpn_if_up.sh)
DSRC = $(addprefix debian/,changelog rules copyright smallsrv.manpages)
ASRC = license.ssl notes.ssl
src_name=o/smallsrv-$(VERSION).tar.gz
src_dir=smallsrv-$(VERSION)

sources:
	tar -czvf $(src_name) --transform="s,^,./$(src_dir)/," --exclude-backups $(SRC) $(WSRC) $(LSRC) $(SSRC) $(DSRC) $(ASRC)

deb: sources
	cd o ; tar -xzf $(src_dir).tar.gz
	mkdir -p o/$(src_dir)/debian ; sed "s/^Version:.*/Version: $(VERSION)/" debian/control >o/$(src_dir)/debian/control
	cd o/$(src_dir) ; DEBEMAIL="max@smallsrv.com" ; DEBFULLNAME="Maksim Feoktistov" ; debmake -b "binarypackage:bin" ; debuild -i -us -uc -b

srcdeb: deb
	cd o/$(src_dir) ; DEBEMAIL="max@smallsrv.com" ; DEBFULLNAME="Maksim Feoktistov" ; debmake -b "binarypackage:bin" ; debuild -i -us -uc -S
