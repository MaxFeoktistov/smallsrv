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


#ifndef SLIB_H
#define SLIB_H

extern "C" {


uint atoui(const char *a);
uint atouix(const char *a);
uint atouix2(const char *a);
uint atouisc(const char *&a);
inline uint atouis(char *&a){return atouisc( (const char *&) a); };

unsigned long  strtoul(const char *s, char **e, int base);

#if (!defined(_STDLIB_H)) &&  (!defined(_STDLIB_H_))
char * malloc(int l);
char * realloc(void *,int l);
void free(void *);

#endif

}


#endif
