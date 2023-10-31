
  Small HTTP server
  =================

  Copyright (C) 1999-2023 Maksim Feoktistov.  

  It is not only a web server, it is also Mail servers, DNS, FTP, Proxy, DHCP, VPN ... All in one!  
  Detail description: [descu.htm](descu.htm)  
  Detail description of version for Windows: [desc.htm](desc.htm)  

  Build requirments:
  ------------------
  To build this project required Perl, gcc, g++, binutils, make, standart C/C++ headers and libraries.  
  For SSL/TLS support also required OpenSSL 1.0/1.1.1 or GnuTLS libraries.

  Build Linux version:
  --------------------
  For Debian-style distribution, you may prepare the system with the command:
  ```
  apt install  gcc g++ make libc6-dev libc6-dev-i386 gcc-multilib g++-multilib gnutls-dev libssl-dev
  ```
  For another cases install the same package with way recomendet for your distribution.  
  libc6-dev-i386 required to build 32-bit version under 64-bit system  
  For OpenSSL 1.1.1 required openssl-1.1.1-dev  

  
  Now you need to run the `./configure` script with the required parameters. You can disable some features. For example, to exclude the TLS VPN server, use the --withoutvpn switch.  
  If you plan to install the program, pay attention to the CONFIG_BASE variable. The default value is "/usr/local/", the program will be installed in /usr/local/lib/smallsrv/ and links will be created in /usr/local/bin/.  
  Use `./configure --help` for all options.  
  
  32-bit:  
  ```
  ./configure --target=i32  --withoutfakelibs
  make
  make o/libsec111.so
  make o/libsecgnutls.so
  ```
  Result will be o/httpd.exe o/libsec111.so o/libsecgnutls.so o/sndmsg  
  In the httpd.exe configuration you can select the security library. You can also build binaries with a pre-selected security library:
  ```
  make o/httpd.exopenssl
  make o/httpd.exgnutls
  ```
  To install the program use:  
  ```
  make install
  ```
  
  64-bit:
  ```
  ./configure --target=i64  --withoutfakelibs
  make
  make o64/libsec111.so
  make o64/libsecgnutls.so
  ```
 Result will be o64/httpd.exe o64/libsec111.so o64/libsecgnutls.so o64/sndmsg  
 libsec111.so -- for OpenSSL 1.1.1  
 libsecgnutls.so -- for GnuTLS  
 required one of them.  
 You can also build binaries with a pre-selected security library:
  ```
  make o64/httpd.exopenssl
  make o64/httpd.exgnutls
  ```
  To install the program use:  
  ```
  make install
  ```
  

 If you just run  
 ```
  ./configure
 ```
 without any keys, it will try to detect the current architecture and create a Makefile for it or, if detection fails, for all available targets.  
   
 If you plan to copy the binary executable to another PC with a different Linux, use the --withfakelibs key.  
 Fake libraries are very useful for distributing binaries that will run on any Linux, but on some systems the `dlopen` function does not work with executables that are built with it. This function is required to load the library specified in the configuration. `httpd.exopenssl` and `httpd.exgnutls` were created for this case.
 
 Even if you are configure a Makefile without fake libraries, you can build them with:
 ```
 make i32f
 make i64f
 ```
 Binaries without dynamic object versions will be in `o/of/`  or `o64/of`  


  Cross build Linux version for ARM CPU with specifed libs:
  ----------------------------------------------------------

  ```
  ./configure --target=arm  --armgcc="arm-linux-gnueabi-gcc"  \
              --arminclude="/usr/src/crossarm/include" \
              --armlib="/usr/src/crossarm/lib"
  make
  ```
  Direct your pathes instead /usr/src/crossarm/...  


  Cross build Linux version for ARM64 CPU:
  ----------------------------------------

  ```
  ./configure --target=arm64"
  make
  ```

  Cross build Linux version for architecture that not defined in Makefile:
  ------------------------------------------------------------------------

  You can try to build the program for any system that not defined before.
  This script and Makefile understands the ARCH, CROSS_COMPILE, INSTALL_ROOT environment variables.  

  ```
  export INSTALL_ROOT=/dev/shm/arm64_root/
  export ARCH=arm64
  export CROSS_COMPILE=aarch64-linux-gnu-
  ./configure
  make
  make install
  ```

  Build Windows version under Linux:
  ----------------------------------

  Required i686-w64-mingw32 or x86_64-w64-mingw32
  For Debian style distribution you may prepare the system with the command:  
  ```
  apt install i686-w64-mingw32
  ```
  For TLS/SSL also required headers for openssl 1.1.1 or for GnuTLS libararies.  
  For GnuTLS you may try to download binaies .dll and headers
  from [www.gnupg.org](https://www.gnupg.org/ftp/gcrypt/gnutls/w32/).  
  For openssl -- required to build its from sources, to get .a libraries. 
  The sources available at [openssl.org](https://www.openssl.org/)  

  Run the commands:
  ```
  ./configure --target=win \
              --winsslinclude="/usr/src/openssl/include" --winssllib="/usr/src/openssl/lib" \
              --wingnutlsinclude="/usr/src/gnutls/include" --wingnutlslib="/usr/src/gnutls/lib"
  make
  make wo/libsec111.dll
  make wo/libsecgnutls.dll
  ```

 Direct your pathes to Windows libraries instead /usr/src/...  
 libsec111.dll -- for OpenSSL  
 libsecgnutls.dll -- for GnuTLS  
 required one of them.  

 Build Windows version under Windows or withhelp Wine:
 ------------------------------------------------------

 Required MinGW and Perl for Windows

  ```
  ./configure    --wingcc="c:\\MINGW\\bin\\gcc.exe" \
                 --windir="c:\\MINGW" \
                 --target=win \
                 --winsslinclude="c:\\openssl\\include"  --winssllib="c:\\openssl\\lib" \
                 --wingnutlsinclude="c:\\gnutls\\include" --wingnutlslib="c:\\gnutls\\lib"
  make
  make wo/libsec111.dll
  make wo/libsecgnutls.dll
  ```
 Direct your pathes instead c:\\...  

 Build Windows version with DJGPP GCC/G++
 ----------------------------------------

 It is tested and optimised for DJGPP 2.95 only. Also required Windows libraries that can be generated withhelp [Utilites](https://smallsrv.com/mkpe/)  
 If your DJGGP ready to build Windows PE you may just use makefile.dj.  Fix the pathes in this file and then run:  
 ```
 make -f makefile.dj
 ```
 Otherwise you may edit files 'ya', 'yc' for your pathes and try to run
 ```
 c1.bat
 ```
 This DGPP build will be using precompiled binary make_pe.exe, bin2c.exe, copyres.exe. Sources of them present here, you may rebuild it yourself.  
 
 
