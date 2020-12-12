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

//as ndp.s -o ndp.o

.text
.globl _id_heap
_id_heap:
id_heap: .long 0;

heap_cnt: .long 0

.globl ___builtin_vec_new
.globl ___init_vec_new
.globl malloc
.globl _malloc
.globl ___builtin_new


___init_vec_new:
           call _GetProcessHeap
           mov  %eax,id_heap
           ret

_malloc:
malloc:
___builtin_new:
___builtin_vec_new:
           pushl  4(%esp)
           pushl  $8
           pushl  id_heap
           call  _HeapAlloc
           ret $4
   2: ;//Error
           xorl %eax,%eax
           ret $4

.globl ___builtin_delete
.globl ___builtin_vec_delete
.globl _free
.globl free
free:
_free:
___builtin_delete:
___builtin_vec_delete:
           pushl  4(%esp)
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
