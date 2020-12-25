#!/usr/bin/perl
#
#  Copyright (C) Maksim Feoktistov (max@smallsrv.com)
# 
#  This script is free for commercial and non-commercial use.
#


 $cflag=0;
 $cmd = 'Command line:
 mkfakelib.pl  libxx.so  [TARGET_DIR]  [-c]
 ';

  die($cmd)   if($#ARGV<0 || $ARGV[0] !~ /(.*)\.so/ );

  $bname = $1 ;
  $bname =~ s/.*\///g;
  $soname = $ARGV[0] ;
  $soname =~ s/.*\///g;

  $data= "\n\n.data\n\n";
  
  $pref='';
  
  if($#ARGV>0)
  {
   $pref=$ARGV[1]  ; 
   $pref .= "./"  if($pref !~ /\/$/);
   if($#ARGV>1 && $ARGV[2] eq '-c' )
   {
     $cflag=1 ;
     $data= "\n";
   };
  }
 
  
  print "src: $ARGV[0] base:$bname target: $pref$bname.c\n" ;
  
 
 
  
#  open FILEC, ">$pref$bname.c" || die("Can't open $pref$bname.c");
  $tsrcname=($cflag) ? "$pref$bname.c" : "$pref$bname.s" ;
  open FILEC,">$tsrcname"  || die("Can't open $pref$bname.s");
  print FILEC "\n\n";
  print FILEC "\n\n.text\n\n";
  
  @str = `readelf  --wide --dyn-syms $ARGV[0] `;  
#  while(`readelf  --dyn-syms $ARGV[0] `)
  foreach $_  (@str)
  {
  
#  FUNC    WEAK   DEFAULT   13 mktemp@@GLIBC_2.0
#     print $_;
    if(/FUNC[ \t]+.*[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+(.+)\@\@/)
    {
      $n=$1;
 #     print FILEC "void $n(){}; \n"; 
       print FILEC ($cflag)? "void $n(){}; \n" : 
                            " .globl $n\n .type $n, \@function\n$n:\n .cfi_startproc\n ret\n .cfi_endproc\n .size   $n, .-$n\n"; 
#       print FILEC ".globl $n\n.type $n, \@function\n$n: .cfi_startproc\n ret\n .cfi_endproc\n"; 
 
    }
#  1398: 001b3dbc     4 OBJECT  GLOBAL DEFAULT   34 __environ@@GLIBC_2.0
    elsif(/[0-9]+:[ \t]+([0-9a-f]+)[ \t]+[0-9]+[ \t]+OBJECT[ \t]+(GLOBAL|WEAK)[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+(.+)\@\@/)
    {
      ($offs,$n)=($1,$3);
      if($offs =~ /[1-9a-f]/ )
      {
#         print FILEC "void * $n;\n"; 
#         print FILEC "\nint $n;\n"; 
#         $data .= ".globl $n\n$n: .long 0\n"
#        $data .= "\n .comm  $n,4,4\n"
         $data .= ($cflag) ? "void * $n;\n" : 
                            "\n  .align 4\n .globl $n\n .type  $n, \@object\n .size   $n, 4\n$n: \n  .long  0\n" ;

      }
      
    }
  
  }
  
#  print FILEC  (cflag)?$data  : "\n\n.data\n\n$data\n";
  print FILEC  $data ;

  print FILEC "\n\n\n";
  close FILEC ;
  
#  $r = `gcc -m32 -o $pref$bname.so  -fno-builtin -fPIC -shared -O2 $pref$bname.c  -Wl,-soname=$soname` ;
#  $r = `gcc -m32 -o $pref$bname.so  -fno-builtin  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -shared -O2 $pref$bname.c  -Wl,-soname=$soname` ;
  $r = ($cflag)?
     `gcc -m32 -o $pref$bname.so -fno-builtin  -fPIC -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -shared -O2 $tsrcname  -Wl,-soname=$soname`
     : `gcc -m32 -o $pref$bname.so  -fno-builtin -fPIC -shared -O2 $tsrcname -Wl,-soname=$soname` ;
  
  print $r ;
  
  exit(0);
  
  