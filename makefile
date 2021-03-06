#
# Copyright (C) 1999-2020 Maksim Feoktistov.
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


G=  -g  -Os

# OPT= -m32 -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
# -fno-verbose-asm -fno-implicit-templates -mno-align-double \
# -mno-fancy-math-387 -fwritable-strings -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated  -fno-nonansi-builtins  -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -DFREEVER  -fno-pic

# best opt for gcc 3.x
# OPT= -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
# -fno-verbose-asm -fno-implicit-templates -mno-align-double \
# -mno-fancy-math-387 -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control  -Wno-deprecated  -fno-nonansi-builtins  -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -DFREEVER  -fno-pic

OPT= -pipe  \
 -fno-implicit-templates  -fno-stack-protector -fno-stack-check -fno-verbose-asm -fno-nonansi-builtins -fno-access-control  -fno-optional-diags -momit-leaf-frame-pointer\
 -DUSE_IPV6 -DUSE_FUTEX -DUSE_POOL -fno-exceptions  -Wno-deprecated  -fno-bounds-check -fno-tree-bit-ccp -fno-builtin -mno-red-zone -DFREEVER  -DWITHMD5 -DFIX_EXCEPT -DUSEVALIST

#-fdelete-null-pointer-checks  -fdelayed-branch  -fdelete-dead-exceptions
# -fnobounds-check
#-mcpu=i486  -march=i486
# -fno-threadsafe-statics

 
OPT64=  -Dx86_64 -DNOTINTEL $(OPT)

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

WINEGPP=wine c:\\MINGW2\\bin\\gcc.exe
WINEGCC=wine c:\\MINGW2\\bin\\gcc.exe
WINEAS=wine  c:\\MINGW2\\bin\\as.exe
WINERES=wine c:\\MINGW2\\bin\\windres.exe
MGDIR=c:\\MINGW2

# WINEGPP=wine c:\\MinGW\\bin\\gcc.exe
# WINEGCC=wine c:\\MinGW\\bin\\gcc.exe
# WINEAS=wine  c:\\MinGW\\bin\\as.exe
# WINERES=wine c:\\MinGW\\bin\\windres.exe



WINE=wine
#WINEBIN2S=wine wo\\bin2s.exe 
WINEBIN2S=./bin2s 

#WINECFLG=  -Iwinclude  -I$(MGDIR)\\include -fpack-struct -O2 -fno-verbose-asm -fno-implicit-templates -mno-svr3-shlib -mno-align-double -mno-fancy-math-387 -fconserve-space  -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mrtd

WINECFLG=  -Os -nostdinc -Iwinclude  -I$(MGDIR)\\include -I$(MGDIR)\\lib\\gcc\\mingw32\\8.2.0\\include -fno-implicit-templates -mno-align-double -mno-fancy-math-387 -fconserve-space  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -DRICON -mwindows -fno-optional-diags -momit-leaf-frame-pointer -mno-red-zone -fno-exceptions  -fno-stack-protector -no-pie -fno-ms-extensions -fno-stack-check -mno-stack-arg-probe 
          
WINELFLG=    -mwindows -L$(MGDIR)\\lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Xlinker --heap  -Xlinker 0x20000000 -Wl,--subsystem,windows  -nostartfiles -nodefaultlibs -Xlinker -Map -Xlinker wo/flxmap  -Xlinker --entry=_start    -fno-optional-diags -momit-leaf-frame-pointer  -mno-red-zone -fno-exceptions  -fno-stack-protector -fno-ms-extensions -no-pie -fno-stack-check -mno-stack-arg-probe




LOPT=   -dynamic-linker   -Xlinker -Map -Xlinker o/flxmap  -nodefaultlibs
LOPT64=   -dynamic-linker   -Xlinker -Map -Xlinker o64/flxmap  -nodefaultlibs

LIB= -Lfakelibs -lpthread  -Lfakelibs -ldl -Lfakelibs -lc -lc_nonshared
LIB64= -Lfakelibs64 -lpthread  -Lfakelibs64 -ldl -Lfakelibs64 -lc -lc_nonshared
LIB64u=  -ldl -lpthread  -lc -lc_nonshared

COBJS= o/to_linux.o o/bvprintfv.o
COBJS64= o64/to_linux.o

AOBJS=mstring1.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o adminr.o restart.o proxy.o musers.o conf.o t2icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o md5.o msprintfchk.o

AOBJS64=srv0a.o srv_ssi.o srv_cgi.o req.o accept.o adminr.o restart.o proxy.o musers.o conf.o t2icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintfv.o md5.o msprintfchk.o

POBJS=$(AOBJS)
# telnet.o
OBJS=$(COBJS) $(POBJS)



OOBJS=$(COBJS) $(addprefix o/,$(POBJS))

OOBJS64=$(COBJS64) $(addprefix o64/,$(AOBJS64))

WINOOBJS= wo/fwnd.o $(addprefix wo/,$(AOBJS)) wo/bvprintf.o wo/srvdat.o wo/isapi.o wo/qsort.o wo/updr.res


#all: wo/http.exe

#all: o64/httpdu.exe o/httpd.exe

#all: o/httpd.exe

#o64/libsecgnutls.so o64/libsec.so

all: /dev/shm/shttps/o/1.x o/1.x getstr i32 i64 arm win

# i32: o/httpd.exe

i32: o/httpd.exe o/libsecgnutls.so o/libsec.so

i64: o64/httpd.exe o64/httpdu.exe o64/libsecgnutls.so o64/libsec.so

arm: at/atobjdir at/httpd.exe

win:  wo/http.exe wo/httpg.exe bin2s wo/shttps_mg.exe wo/shttpsr_mg.exe wo/shttps_mgi.exe wo/shttpsr_mgi.exe


o/%.o :  %.cpp
	   $(GPP) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@


o/%.o :  %.c
	   $(GCC) -c -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@


o64/%.o :  %.cpp
	   $(GPP64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@


o64/%.o :  %.c
	   $(GCC64) -c -Dx86_64 -DNOTINTEL -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $< -o $@


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


fakelibs/%.so: /usr/lib/i386-linux-gnu/%.so
	./mkfakelib.pl $< fakelibs/

fakelibs64/%.so: /lib/x86_64-linux-gnu/%.so
	./mkfakelib64.pl $< fakelibs64/

.cpp.S:
	   $(GPP) -S -DSYSUNIX -DV_FULL=1 $(G) $(OPT)  $<



cleanobj:
	rm $(OOBJS) $(OOBJS64)

getstr: getstr.cpp
	g++ -m32 -pipe -fpack-struct -fconserve-space -O2 -s getstr.cpp -o getstr

md5:   md5.c 
	gcc -s -pipe -m32 -Os md5.c -DTEST  -DSYSUNIX -DV_FULL=1  -o md5

o/1.x: /dev/shm/shttps/o/1.x
	ln -s /dev/shm/shttps/o .
	ln -s /dev/shm/shttps/o64 .
	ln -s /dev/shm/shttps/wo .
	ln -s /dev/shm/shttps/arm .

/dev/shm/shttps/o/1.x :
	mkdir -p /dev/shm/shttps/o ; mkdir -p /dev/shm/shttps/o64 ; mkdir -p /dev/shm/shttps/wo ; mkdir -p /dev/shm/shttps/arm ; echo 1 >$@
	mkdir -p  o/dist o/adist/ o64/dist o64/distu 
	mkdir -p  o/dist/langpacks/en o/adist/langpacks/en o64/dist/langpacks/en o64/distu/langpacks/en 
	mkdir -p  o/dist/langpacks/ru o/adist/langpacks/ru o64/dist/langpacks/ru o64/distu/langpacks/ru 


o/httpd.exe: $(OOBJS)
	$(GCC)  $(G)  $(OOBJS) -o $@  $(LOPT) $(LIB)  

tclean:
	rm $(OOBJS)


o64/httpd.exe: $(OOBJS64)
	$(GCC64)  $(G)  $(OOBJS64) -o $@  $(LOPT64) $(LIB64)   

o64/httpdu.exe: $(OOBJS64)
	$(GCC64)  $(G)  $(OOBJS64) -o $@  $(LOPT64) $(LIB64u)   

wo/http.exe: $(WINOOBJS)
	$(WINEGCC) -s  $(WINOOBJS) -o $@ $(WINELFLG)

wo/httpg.exe: $(WINOOBJS)
	$(WINEGCC) -g  $(WINOOBJS) -o $@ $(WINELFLG)

#   -nostartfiles	 -Telfi386.x  -nodefaultlibs
   
#	$(GCC) $(G)   /usr/lib/i386-linux-gnu/crt1.o /usr/lib/i386-linux-gnu/crti.o $(OOBJS) -o $@ $(LIB) $(LOPT)  /usr/lib/i386-linux-gnu/crtn.o -nostartfiles  -Telfi386.x -nodefaultlibs

#	   $(GCC) $(G)   $(OOBJS) -o $@ $(LIB) $(LOPT)   -nostartfiles -Telfi386.x -nodefaultlibs

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


wo/stpdta.o: wo/stpdta.s wo/uninst.bin wo/http.bin wo/ind1.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/lic.bin
	 cd wo ;  $(WINEAS)  stpdta.s -o stpdta.o  

wo/stpdtar.o: wo/stpdtar.s wo/uninst.bin wo/http.bin wo/ind1r.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/licr.bin
	 cd wo ;  $(WINEAS)  stpdtar.s -o stpdtar.o  

wo/stpdtai.o: wo/stpdtai.s wo/uninst.bin wo/http.bin wo/ind1.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/lic.bin wo/ipbase.s
	 cd wo ;  $(WINEAS)  stpdtai.s -o stpdtai.o  

wo/stpdtari.o: wo/stpdtari.s wo/uninst.bin wo/http.bin wo/ind1r.bin wo/eshs_lang.bin wo/shs_lang.bin wo/lnotes.bin wo/licr.bin wo/ipbase.s
	 cd wo ;  $(WINEAS)  stpdtari.s -o stpdtari.o  
  
wo/stpdta.s: stpdta.s
	cp   $< $@

wo/stpdtar.s: stpdtar.s
	cp   $< $@
  
wo/stpdtai.s: stpdtai.s
	cp   $< $@

wo/stpdtari.s: stpdtari.s
	cp   $< $@

wo/%.bin:  wo/%.exe 
	$(WINEBIN2S)  $< $@

# wo/%.bin:  %.htm
# 	$(WINEBIN2S)  $< $@

wo/ind1.bin:  desc.htm
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


o/t2icfg.o: t2icfg.cpp g4s1.hh




t2icfg.cpp o/s1.hh: icfg.cpp
	rm -f o/s1.hh
	./getstr -f o/s1.hh -y -n t2 icfg.cpp
	./def4str.pl S1 <o/s1.hh >g4s1.hh


t2icfghtm.cpp o/strhtm.hh g4strhtm.hh: icfghtm.cpp
	rm -f o/strhtm.hh
	./getstr -f o/strhtm.hh -y -n t2 icfghtm.cpp
	./def4str.pl S5 <o/strhtm.hh >g4strhtm.hh



g4s1.hh: o/s1.hh
	./def4str.pl S1 < $< > $@


g4strc.h S2_lf.hh: strc.h  
	./def4str.pl S2 < $< > g4strc.h
	echo "#define G4STRING_CONST_H\n\n" >> g4strc.h

g4strcwm.h S3_lf.hh:   strcwm.h
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


o/dist/libsec.so: o/libsec.so
	cp $^ $@ 

o/dist/libsecgnutls.so: o/libsecgnutls.so
	cp $^ $@ 

o64/dist/libsec.so: o64/libsec.so
	cp $^ $@ 

o64/dist/libsecgnutls.so: o64/libsecgnutls.so
	cp $^ $@ 

o64/distu/libsec.so: o64/libsec.so
	cp $^ $@ 

o64/distu/libsecgnutls.so: o64/libsecgnutls.so
	cp $^ $@ 

o/dist/descu.htm o/adist/descu.htm o64/dist/descu.htm o64/distu/descu.htm: descu.htm
	cp $^ $@ 

o/dist/license06.txt o/adist/license06.txt o64/dist/license06.txt o64/distu/license06.txt: license06.txt
	cp $^ $@ 

en/shs_lang.cfg : lS1_lf.cfg
	./add_to_langcfg.pl lS1_lf.cfg $@

ru/shs_lang.cfg : lS1_lf.cfg
	./add_to_langcfg.pl lS1_lf.cfg $@


dist:   o/dist/libsec.so o/dist/license06.txt o/adist/license06.txt o/dist/libsecgnutls.so o/dist/langpacks/ru/shs_lang.cfg o/dist/langpacks/en/shs_lang.cfg o/dist/descu.htm o/adist/descu.htm dist64
	cp o/httpd.exe o/dist/
	cd o/dist/ ; strip httpd.exe ; chown -R root:root  * ; chmod -R 0444 *  ; chmod -R a+X * ; chmod  0700 httpd.exe libsec.so libsecgnutls.so ; chmod  0755 sndmsg ; chmod  0600 httpd.cfg ;  rm -f shttplnx.tgz ; tar -czf shttplnx.tgz * ; chmod a+rw shttplnx.tgz
	cp at/httpd.exe o/adist/
	cd o/adist/ ; arm-linux-gnueabi-strip httpd.exe ;  chown -R root:root  * ; chmod -R 0444 *  ; chmod -R a+X * ; chmod  0700 httpd.exe libsec.so ; chmod  0755 sndmsg ; chmod  0600 httpd.cfg ;  rm -f shttparmlnx.tgz ; tar -czf shttparmlnx.tgz * ; chmod a+rw shttparmlnx.tgz


dist64: o64/dist/libsec.so o64/dist/libsecgnutls.so o64/distu/libsec.so o64/distu/libsecgnutls.so o64/dist/langpacks/ru/shs_lang.cfg o64/dist/langpacks/en/shs_lang.cfg o64/dist/descu.htm o64/distu/descu.htm  o64/dist/license06.txt o64/distu/license06.txt
	cp o64/httpd.exe o64/dist/
	cd o64/dist/ ; strip httpd.exe ; chown -R root:root  * ; chmod -R 0444 *  ; chmod -R a+X * ; chmod  0700 httpd.exe libsec.so libsecgnutls.so ; chmod  0755 sndmsg ; chmod  0600 httpd.cfg ;  rm -f shttplnx64.tgz ; tar -czf shttplnx64.tgz * ; chmod a+rw shttplnx64.tgz
	cp o64/httpdu.exe o64/distu/httpd.exe
	cd o64/distu/ ; strip httpd.exe ;  chown -R root:root  * ; chmod -R 0444 *  ; chmod -R a+X * ; chmod  0700 httpd.exe libsec.so libsecgnutls.so ; chmod  0755 sndmsg ; chmod  0600 httpd.cfg ;  rm -f shttplnx64u.tgz ; tar -czf shttplnx64u.tgz * ; chmod a+rw shttplnx64u.tgz


sinst:  o/dist/httpd.exe o/dist/shttplnx.tgz o/adist/shttparmlnx.tgz o64/dist/shttplnx64.tgz  o64/distu/shttplnx64u.tgz
	chmod 0666 wo/shttps_mg.exe wo/shttpsr_mg.exe
	for i in wo/shttps_mg.exe wo/shttpsr_mg.exe o/dist/shttplnx.tgz o/adist/shttparmlnx.tgz o64/dist/shttplnx64.tgz o64/distu/shttplnx64u.tgz ; do cp $$i /mnt/d/var/www/pre/ ; done
	mv /mnt/d/shttps/httpd.exe /mnt/d/shttps/oldhttpd.exe
	cp o/dist/httpd.exe /mnt/d/shttps/

sinsta: sinst wo/shttps_mgi.exe wo/shttpsr_mgi.exe
	chmod 0666 wo/shttps_mgi.exe wo/shttpsr_mgi.exe
	for i in wo/shttps_mgi.exe wo/shttpsr_mgi.exe ; do cp $$i /mnt/d/var/www/pre/ ; done


o/dist/langpacks/en/shs_lang.cfg: en/shs_lang.cfg
	cp $< $@

o/dist/langpacks/ru/shs_lang.cfg: ru/shs_lang.cfg
	cp $< $@

o64/dist/langpacks/en/shs_lang.cfg: en/shs_lang.cfg
	cp $< $@

o64/dist/langpacks/ru/shs_lang.cfg: ru/shs_lang.cfg
	cp $< $@

o64/distu/langpacks/en/shs_lang.cfg: en/shs_lang.cfg
	cp $< $@

o64/distu/langpacks/ru/shs_lang.cfg: ru/shs_lang.cfg
	cp $< $@

o/libsec.so: runssl.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $< -Lfakelibs -lssl 

o64/libsec.so: runssl.cpp
	 $(GCC64) -s -o $@  -fPIC  $(OPT64) -shared -O2  $<  -nodefaultlibs -Lfakelibs64 -lssl  -Lfakelibs64 -lc

o/libsecgnutls.so: rungnutls.cpp
	 $(GCC) -s -o $@  -fPIC  $(OPT) -shared -O2  $<  -nodefaultlibs -Lfakelibs -lgnutls  -Lfakelibs -lc 

o64/libsecgnutls.so: rungnutls.cpp
	 $(GCC64) -s -o $@   -fPIC $(OPT64) -shared -O2  $<  -nodefaultlibs -Lfakelibs64 -lgnutls  -Lfakelibs64 -lc 

 

SSLIDIR=z:\\home\\adv\\openssl\\include 

wo/libsec.o: wo/runssl.o
	$(WINEGCC)  $(G)  $< -c -o $@ -I$(MGDIR)\\include -fno-implicit-templates -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -I$(SSLIDIR) -nodefaultlibs -nostartfiles 
	

SSLLIBDIR=z:\\home\\adv\\openssl\\lib


wo/libsec.dll: wo/libsec.o
	$(WINEGCC)  -s  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(SSLLIBDIR) -lssl -lcrypto -L$(MGDIR)\\lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -lmsvcrt_fix -lgcc -Wl,--entry=_DllMainCRTStartup  -Wl,--heap -Wl,0x20000000 -Wl,--stack -Wl,0x800000 -Wl,-Map -Wl,wo/dllmap -Wl,--subsystem,windows

wo/libsec.dll: /home/adv/openssl/lib/libssl.a /home/adv/openssl/lib/libcrypto.a


GNUTSLIDIR=z:\\home\\adv\\gnutls\\include


wo/libsecgnutls.o: rungnutls.cpp
	$(WINEGCC)  $(G)  $< -c -o $@ -I$(MGDIR)\\include -fno-implicit-templates -DSERVICE -DFREE_VER  -DFREEVER -DV_FULL=1 -DUSE_IPV6 -DMINGW -I$(GNUTSLIDIR) -nodefaultlibs -nostartfiles 
	

GNUTSLLIBDIR=z:\\home\\adv\\gnutls\\lib


wo/libsecgnutls.dll: wo/libsecgnutls.o
	$(WINEGCC)  -s  $< -o $@  -shared  -nodefaultlibs -nostartfiles -L$(GNUTSLLIBDIR) -lgnutls -L$(MGDIR)\\lib -luser32 -lkernel32 -lws2_32 -lgdi32 -lshell32 -lcomdlg32  -ladvapi32 -Wl,--entry=_DllMainCRTStartup  -Wl,--heap -Wl,0x20000000 -Wl,--stack -Wl,0x800000 -Wl,-Map -Wl,wo/dllmap -Wl,--subsystem,windows


clean:
	rm -f $(OOBJS) $(OOBJS64) $(ROBJS) $(MIPSOBJ) $(WINOOBJS) httpd.exe r/httpd.exe mips/httpd.exe $(ARMOBJ)



o/srv0a.o o64/srv0a.o wo/srv0a.o r/srv0a.o lpc/srv0a.o at/srv0a.o : g4strc.h srv0a.cpp slloop.cpp srvars.cpp
o/adminr.o o64/adminr.o r/adminr.o wo/adminr.o at/adminr.o : g4strc.h adminr.cpp t2icfghtm.cpp g4strhtm.hh
o/stat.o o64/stat.o wo/stat.o r/stat.o at/stat.o mips/stat.o : g4strc.h stat.cpp cstat.cpp statusr.cpp
o/smptps.o o64/smptps.o r/smptps.o mips/smptps.o at/smptps.o : g4strc.h smptps.cpp pop3d.cpp
wo/fwnd.o: wndcfg.cpp 
o/conf.o o64/conf.o o/t2icfg.o o64/t2icfg.o wo/conf.o wo/t2icfg.o at/t2icfg.o at/conf.o: g4strc.h g4s1.hh g4strcwm.h S2_lf.hh
o/wmbx.o o64/wmbx.o o/t2icfg.o wo/wmbx.o at/t2icfg.o at/wmbx.o: g4strc.h g4s1.hh g4strcwm.h
o/srv_ssi.o o64/srv_ssi.o wo/srv_ssi.o r/srv_ssi.o lpc/srv_ssi.o at/srv_ssi.o : g4strc.h regular.cpp



NIOBJS=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o adminr.o restart.o proxy.o musers.o conf.o icfg.o stat.o icfgjs.o reghtm.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintf.o msprintfchk.o

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
-fno-verbose-asm -fno-implicit-templates  -DUSE_FUTEX -DUSE_POOL -DFREEVER -DWITHMD5 -DTELNET \
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


arm/libsec.so:    arm/runssl.o
	 export PATH=$(ARMPATH) ; arm-linux-gcc $(G) -s $^  -o $@  -shared $(ARMLOPT) $(LIB)


LIOBJS=to_linux.o srv0a.o srv_ssi.o srv_cgi.o req.o accept.o adminr.o restart.o proxy.o musers.o conf.o t2icfg.o stat.o icfgjs.o ftpd.o smptps.o smtpcl.o mailip.o dnsd.o tlsm.o gz.o dhcpd.o wmbx.o bvprintf.o telnet.o md5.o msprintfchk.o

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
ATLIB=/usr/src/at/musl-1.0.4/lib
ATINCLUDE=/usr/src/at/musl-1.0.4/include
ATFLAGS= -mlittle-endian -marm  -mcpu=arm926ej-s -mthumb-interwork  -msoft-float -Uarm  -nostdinc -I$(ATINCLUDE)  -I/usr/src/uC/uClinux-dist/lib/zlib -I/usr/local/arm-linux/lib/gcc/arm-linux/4.4.2/include -Wno-deprecated-declarations -Wno-conversion  -Wno-write-strings  -fno-access-control  -fno-nonansi-builtins -fno-elide-constructors -fno-enforce-eh-specs  -fno-for-scope  -fno-implicit-templates -fno-implicit-inline-templates -fnothrow-opt  -fno-pretty-templates  -fno-rtti  -fno-weak -nostdinc++  -Wnoexcept   -fno-exceptions -Wno-format -fpermissive -DARM -DLINUX  -DSYSUNIX -DNOTINTEL -DAT_ARM -DFREEVER -DTELNET -DUSE_IPV6 -DV_FULL=1 -DUSE_FUTEX -DUSE_POOL -DWITHMD5 -DFIX_EXCEPT


ATOBJS=$(addprefix at/,$(LIOBJS))

at/%.o : %.cpp
	  $(ATGPP) -c $(S) $(ATFLAGS) -o $@  $<


at/%.o : %.c
	  $(ATGPP) -c $(S) $(ATFLAGS) -o $@  $<

	  
at/atobjdir:
	mkdir -p /dev/shm/shttps/o/at
	[ -d at ] || ln -s /dev/shm/shttps/o/at .
	echo   >$@


at/httpd.exe: $(ATOBJS)
	$(ATGPP)  $(S) -o $@ $^  -mlittle-endian  -msoft-float -march=armv5te -mtune=arm9tdmi -L$(ATLIB)/  -L/usr/lib/gcc/arm-linux-gnueabi/4.7  -nostartfiles  $(ATLIB)/crt1.o $(ATLIB)/crti.o $(ATLIB)/crtn.o -static -lc -lutil -lpthread

	

	 
ANDRARMDIR=/usr/local/android-ndk-r4/build/platforms/android-4/arch-arm/usr/
ANDRARMPATH=/usr/local/android-ndk-r4/build/prebuilt/linux-x86/arm-eabi-4.4.0/bin:$(PATH)
ANDRARMOBJ=$(addprefix andrarm/,$(NIOBJS))

ANDRARMOPT= -pipe -falign-loops=0 -falign-jumps=0 -falign-functions=0 -fno-enforce-eh-specs \
-fno-verbose-asm -fno-implicit-templates   -Wno-deprecated  \
 -DUSE_IPV6  -fno-exceptions -fno-nonansi-builtins  -fno-rtti  -fno-ms-extensions -fno-builtin  -fno-access-control   \
 -I/usr/local/android-ndk-r4/build/platforms/android-4/arch-arm/usr/include

# -fwritable-strings

ANDRARMLOPT= -L/usr/local/android-ndk-r4/build/platforms/android-4/arch-arm/usr/lib -Xlinker -Map -Xlinker flxmap


andrarm/%.o :  %.cpp
	export PATH=$(ANDRARMPATH) ; arm-eabi-gcc -c -s -DSYSUNIX  -DARM -DANDROID -DNOTINTEL -DV_FULL=1 $(G) $(ANDRARMOPT) $< -o $@

andrarm/httpd.exe :  $(ANDRARMOBJ)
	cd andrarm ; export PATH=$(ANDRARMPATH) ; arm-eabi-gcc $(G) -s  $(NIOBJS) -o httpd.exe  $(ANDRARMLOPT) $(LIB)


andrarm/runssl.o :  runssl.cpp
	export PATH=$(ANDRARMPATH) ; arm-eabi-g++ -c -s -DSYSUNIX  -DARM  -DANDROID -DNOTINTEL -DV_FULL=1 $(G) $(ANDRARMOPT) $< -o $@ -I/usr/include


andrarm/libsec.so:    arm/runssl.o
	 export PATH=$(ANDRARMPATH) ; arm-eabi-gcc $(G) -s $^  -o $@  -shared $(ANDRARMLOPT) $(LIB)

