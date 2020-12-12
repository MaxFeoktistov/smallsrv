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

;//as crt0pe.s -o crt0pe.o
.text

ImageBase:

ImportRva:
;// Import Directory Entry
.long KernelTbl-ImageBase+0x1000,0,0xFFFFFFFF  ;//Import LookUp,Time/Date Stamp,Forward Chain
.long sKernel32-ImageBase+0x1000,KernelDTA-ImageBase+0x1000     ;//Name RVA,Addres Table RVA
;// Import Directory Entry
.long UserTbl-ImageBase+0x1000,0,0xFFFFFFFF  ;//Import LookUp,Time/Date Stamp,Forward Chain
.long sUser32-ImageBase+0x1000,UserDTA-ImageBase+0x1000     ;//Name RVA,Addres Table RVA

.long 0,0,0,0,0,0,0,0,0


//EndImport:

//--------------------
 KernelTbl:
xExitProcess:
 .long               sExitProcess-ImageBase+0x1000
xGetModuleHandleA:
 .long               sGetModuleHandleA-ImageBase+0x1000
xGetProcAddress:
 .long               sGetProcAddress       -ImageBase+0x1000
xLoadLibraryA:
 .long               sLoadLibraryA         -ImageBase+0x1000
 .long 0
 .long 0
 .long 0

 UserTbl:
xMessageBoxA:
 .long                sMessageBoxA        -ImageBase+0x1000
 .long 0
 .long 0
 .long 0






sKernel32:
.ascii "KERNEL32.DLL\0"
sUser32:
.ascii "USER32.DLL\0"
.long 0,0

sExitProcess:
.word 0x7
.asciz "ExitProcess"
.align 1,0
sGetModuleHandleA:
.word 0x7fff
.asciz "GetModuleHandleA"
.align 1,0
sGetProcAddress:
.word 0x17
.asciz "GetProcAddress"
.align 1,0
sLoadLibraryA:
.word 0x1C
.asciz "LoadLibraryA"
.align 4,0
.long 0,0,0



sMessageBoxA:
.word 0x9
.asciz "MessageBoxA"
.align 4,0

//KernelTbl:
KernelDTA:
_PExitProcess:
ExitProcess:
.long               sExitProcess          -ImageBase+0x1000
_PGetModuleHandleA:
GetModuleHandleA:
.long               sGetModuleHandleA   -ImageBase+0x1000
_PGetProcAddress:
GetProcAddress:
.long               sGetProcAddress       -ImageBase+0x1000
_PLoadLibraryA:
LoadLibraryA:
.long               sLoadLibraryA         -ImageBase+0x1000
.long 0
.long 0
.long 0
;//--------------

//UserTbl:
UserDTA:
_PMessageBoxA:
MessageBoxA:
.long                sMessageBoxA        -ImageBase+0x1000
.long 0
.long 0
.long 0

.global _PExitProcess
.global _PGetProcAddress
.global _PLoadLibraryA
.global _PMessageBoxA

//;;=================================================================
.macro  Pub_label   lbl
.globl   \lbl
\lbl:
.endm
.macro  Debug  lng1,lng2,lng3
        pushl \lng1
        pushl \lng2
        pushl \lng3
        call dbg_fnc
.endm


//;;=================================================================

.global PEStart
PEStart:
        movl %esp,_beg_esp
     //   movl 4(%esp),%eax
        movl  $first_bss,%edi
        xorl  %eax,%eax
        movl   $_end ,%ecx
        subl   %edi,%ecx
        cld
        rep;  stosb

        finit
        pushl %eax
        fstcw (%esp);
        fwait
        orb $0x3f,(%esp);
        fldcw (%esp);
        popl %eax

//        pushl $0
//        pushl $msg_name
//        pushl $msg_text
//        pushl $0
//        call  *_PMessageBoxA


//     jmp  _WinMain

        mov   $djgpp_first_ctor,%esi
        mov   $djgpp_last_ctor, %edi
 constr_call_loop:
        cmp   %esi,%edi
        jbe   1f
        cld
        lodsl
        call  *%eax
        jmp   constr_call_loop
  1:
        pushl $1
        call  _GetCommandLineA
        pushl %eax
        pushl $0
        pushl $0
        call  *_PGetModuleHandleA
        movl  %eax,__proc_handle
        pushl %eax

        call  _WinMain

.global __exit,__abort,_abort,_exit
__exit:
_exit:
        mov   $djgpp_first_dtor,%esi
        mov   $djgpp_last_dtor ,%edi
 destr_call_loop:
        cmp   %esi,%edi
        jae   2f
        cld
        lodsl
        call  *%eax
        jmp   destr_call_loop

__abort:
_abort:
       pushl $0
       call *ExitProcess
  2:
       movl _beg_esp,%esp
       ret

.globl  _beg_esp
_beg_esp:
.long  0x40FFFF
//msg_name:
//.asciz "Please.."
//msg_text:
//.asciz "Hello world !"
.global __proc_handle
__proc_handle:
.long   0

;//===========================================
MsgNoDLL: .asciz "**Error. Can't load dll library"
MsgNoEntry: .asciz "**Error. Can't find function in dll library"

.global _OptFunc
_OptFunc:
   movl   (%edx),%eax
   orl  %eax,%eax
   jnz gf1
   pushl  %edx
//   pushl  4(%edx)
   addl  $4,%edx
   pushl  %edx
   call  *_PGetModuleHandleA
   popl %edx
   orl  %eax,%eax
   jnz gf2


   pushl %edx
//   pushl 4(%edx)
   addl  $4,%edx
   pushl  %edx
   call  *_PLoadLibraryA
   popl  %edx
   orl   %eax,%eax
   jnz gf2

   pushl $0
//   pushl 4(%edx)
   addl  $4,%edx
   pushl %edx
   pushl $MsgNoDLL
   pushl $0
   call  *_PMessageBoxA
   jmp   _abort

gf2:
   movl  %eax,(%edx)
gf1:
  movl  %eax,%edx

  movl  (%esp),%eax
  cmpw  $0,2(%eax)
  jne    5f
  movl (%eax),%eax
5:
  pushl %eax
  pushl %edx
  call  *_PGetProcAddress
  popl  %ecx
  orl   %eax,%eax
  jz    _NoService
//  subl  $get_func,%eax
//  addl  %eax,-4(%ecx)
  movl  %eax,%edx
  subl  %ecx,%eax
  movl  %eax,-4(%ecx)
  movb  $0xE9,-5(%ecx)
//  Debug  %eax,_PMessageBoxA,-5(%ecx)
  jmp   *%edx
//  subl  $5,%ecx
//  jmp   *%ecx

_NoService:
   pushl $0
   pushl %ecx
   pushl $MsgNoEntry
   pushl $0
   call  *_PMessageBoxA
   jmp   _abort


;//===========================================
/*
dbg_fnc:
        pushal
        pushl %ds
        popl  %es

        pushal

        movl  $reg_nma,%esi
        movl  $dbg_dta,%edi

        movb  $8,%bl

 DBLP0:

        popl  %edx
        movsl
        call  bin2hex
        movb  $0xA,(%edi)
        inc  %edi

        dec   %bl
        jnz   DBLP0


        movl 32(%esp),%edx
        movsl
        call  bin2hex
        movb  $0xA,(%edi)
        inc  %edi

        movl 36(%esp),%edx
        call bin2hex
        movb  $',',(%edi)
        inc  %edi

        movl  40(%esp),%edx
        call bin2hex
        movb  $',',(%edi)
        inc  %edi

        movl  44(%esp),%edx
        call bin2hex
        movb  $',',(%edi)
        inc  %edi

        pushl $3
        pushl $dbg_name
        pushl $dbg_dta
        pushl $0
        call  *MessageBoxA
        cmpl  $2,%eax
        je    _abort

        popal
        ret  $12
bin2hex:
        cld
        movl  $8,%ecx
DBLP1:
        roll  $4,%edx
        movb  %dl,%al
        andb  $0xF,%al
        addb  $'0',%al
        cmpb  $'9',%al
        jbe   DBL1
        addb  $('A'-'0'-10),%al
DBL1:
        stosb
        loop  DBLP1
        ret
dbg_dta:
.long   0,0,0,0,0,0,0,0
.long   0,0,0,0,0,0,0,0
.long   0,0,0,0,0,0,0,0
.long   0,0,0,0,0,0,0,0
.long   0,0,0,0,0,0,0,0
dbg_name: .asciz "Debug"
reg_nma:  .asciz "edi esi ebp esp ebx edx ecx eax eip "
*/
;//===========================================

