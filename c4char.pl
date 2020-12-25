#!/usr/bin/perl

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



sub upc
{
 my ($t)=@_;

 @a = unpack("cccc",$t);
 $t=sprintf '0x%2.2X%2.2X%2.2X%2.2X x4CHAR("%s") ',$a[3],$a[2],$a[1],$a[0],$t;
 $t ;
}

  while(<>)
  {
#   s/C4CHAR\(\"(.+)\"\)/sprintf('0x%X ', unpack("cccc",$1)) /eg ;
    while( s/C4CHAR\(\"(.+)\"\)/upc($1) /eg ){ print STDERR "found $1\n" ;};
    print $_;
  }

