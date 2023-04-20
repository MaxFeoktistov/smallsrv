
  Small HTTP server
  =================

  Copyright (C) 1999-2023 Maksim Feoktistov.

  Build requirments:
  ------------------
  To build this project required Perl, gcc, g++, binutils, make
  For SSL/TLS support also required OpenSSL 1.0/1.1.1 or GnuTLS libraries.



  Build Linux version:
  --------------------

  32-bit:
  ```
  ./Configure --target=i32  --withoutfakelibs
  make
  make o/libsec111.so
  make o/libsecgnutls.so
  ```

  64-bit:
  ```
  ./Configure --target=i64  --withoutfakelibs
  make
  make o/libsec111.so
  make o/libsecgnutls.so
  ```
 libsec111.so -- for OpenSSL 1.1.1
 libsecgnutls.so -- for GnuTLS
 required one of them.

 If you just run
 ```
  ./Configure
```
 without any keys, it try to create Makefile for all avilable targets.

 If you planing copy the binary executable to another PC with another  Linux don't use --withoutfakelibs key.


  Build Linux version for ARM CPU:
  --------------------------------

  ```
  ./Configure --target=arm  --armgcc="arm-linux-gnueabi-gcc"  \
              --arminclude="/usr/src/crossarm/include" \
              --armlib="/usr/src/crossarm/lib"
  make
  ```
  Direct your pathes instead /usr/src/crossarm/...


  Build Windows version under Linux:
  ----------------------------------

  Required i686-w64-mingw32 or x86_64-w64-mingw32

  ```
  ./Configure --target=win --winsslinclude="/usr/src/openssl/include"  --winssllib="/usr/src/openssl/lib" \
              --wingnutlsinclude="/usr/src/gnutls/include"    --wingnutlslib="/usr/src/gnutls/lib"
  make
  make wo/libsec111.so
  make wo/libsecgnutls.so
  ```

 Direct your pathes to Windows libraries instead /usr/src/...
 libsec.so -- for OpenSSL
 libsecgnutls.so -- for GnuTLS
 required one of them.

 Build Windows version under Windows or withhelp Wine:
 ------------------------------------------------------

 Required Mingw and Perl for Windows

  ```
  ./Configure    --wingcc="c:\\MINGW\\bin\\gcc.exe" \
                 --windir="c:\\MINGW" \
                 --target=win --winsslinclude="c:\\openssl\\include"  --winssllib="c:\\openssl\\lib" \
                 --wingnutlsinclude="c:\\gnutls\\include"    --wingnutlslib="c:\\gnutls\\lib"
  make
  make wo/libsec111.so
  make wo/libsecgnutls.so
  ```

 Direct your pathes instead c:\\...

 Build Windows version with DJGPP GCC/G++
 ----------------------------------------

 It is tested and optimised for DJGPP 2.95 only. Also required Windows libraries that can be generated withhelp [Utilites](http://smallsrv.com/mkpe/)
 If your DJGGP ready to build Windows PE you may just use makefile.dj.  Fix the pathes in this file and then run:
 ```
 make -f makefile.dj
 ```
 Otherwise you may edit files 'ya', 'yc' for your pathes and try to run
 ```
 c1.bat
 ```
 This DGPP build will be using precompiled binary make_pe.exe, bin2c.exe, copyres.exe. Sources of them present here, oyu may rebuild it yourself.
