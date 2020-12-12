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

//as ndpdbg.s -o ndp.o

.text
.globl _id_heap
_id_heap:
id_heap: .long 0;

heap_cnt: .long 0

.globl ___builtin_vec_new
.globl ___init_vec_new
___init_vec_new:
           call _GetProcessHeap
           mov  %eax,id_heap
           ret

___builtin_vec_new:
           addl   $8,4(%esp)
           pushl  4(%esp)
           pushl  $8
           pushl  id_heap
           call  _HeapAlloc
           movl   -4(%esp),%ecx
           movl   %ecx,(%eax)
           movl   $0x12345678,-8(%eax,%ecx,1)
           addl  $4,%eax
           ret $4
   2: ;//Error
           xorl %eax,%eax
           ret $4

.globl ___builtin_delete
___builtin_delete:
           movl  4(%esp),%eax
           subl  $4,%eax
           pushl %eax
           movl  (%eax),%ecx
           cmpl   $0x12345678,-8(%eax,%ecx,1)
           je    2f
           pushl  %ecx
           pushl  %eax
           call   _DbgMemError
     2:

           pushl  $0
           pushl  id_heap
           call  _HeapFree
;//           incl   heap_cnt
;//           andl   $0xF,heap_cnt
;//           jnz    1f
           pushl  $0
           pushl  id_heap
           call   _HeapCompact
;//      1:
           ret $4

.globl _Get_nd_Func
_Get_nd_Func:  movl $l1,%edx
               jmp _OptFunc;
l1: .long 0
    .asciz "kernel32.dll"

.globl _GetProcessHeap
_GetProcessHeap: call _Get_nd_Func
.asciz "GetProcessHeap"
.globl _HeapAlloc
_HeapAlloc: call _Get_nd_Func
.asciz "HeapAlloc"
.globl _HeapFree
_HeapFree: call _Get_nd_Func
.asciz "HeapFree"
.globl _HeapCompact
_HeapCompact: call _Get_nd_Func
.asciz "HeapCompact"

.section .ctor0
.long  ___init_vec_new
