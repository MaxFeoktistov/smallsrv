#!/usr/bin/perl
#
#  Copyright (C) Maksim Feoktistov (max@smallsrv.com)
#
#  This script is free for commercial and non-commercial use.
#


 $cflag=0;
 $cmd = "Command line:
$0 libxx.so prefix_gcc [TARGET_DIR] [-c]
";

  die($cmd)   if($#ARGV<1 || $ARGV[0] !~ /(.*)\.so/ );

  $bname = $1 ;
  $bname =~ s/.*\///g;
  $soname = $ARGV[0] ;
  $soname =~ s/.*\///g;
  $prefix_gcc = $ARGV[1];

  $data= "\n\n.data\n\n";

  $pref='';

  if($#ARGV>1)
  {
   $pref=$ARGV[2]  ;
   $pref .= "./"  if($pref !~ /\/$/);
   if($#ARGV>1 && $ARGV[3] eq '-c' )
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
  print FILEC "\n\n.text\n\n" if(!$cflag);
  @str = `readelf  --wide --dyn-syms $ARGV[0] `;
#  while(`readelf  --dyn-syms $ARGV[0] `)
  $all=' ';

  foreach $_  (@str)
  {

#  FUNC    WEAK   DEFAULT   13 mktemp@@GLIBC_2.0
#    if(/FUNC[ \t]+.*[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+(.+)\@\@/)
    if(/FUNC[ \t]+.*[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+([^@ \t\n\r]+)/)
    {
      $n=$1;
#     print "($n)$_";
      if( $all =~ / $n /)
      {
        print "function $n ( $_ ) already exist\n";
      }
      else
      {
 #     print FILEC "void $n(){}; \n";
       print FILEC ($cflag)? "void $n(){}; \n" :
                            " .globl $n\n .type $n, \@function\n$n:\n .cfi_startproc\n ret\n .cfi_endproc\n .size   $n, .-$n\n";
#       print FILEC ".globl $n\n.type $n, \@function\n$n: .cfi_startproc\n ret\n .cfi_endproc\n";
        $all .= "$n ";
      }

    }
#  1398: 001b3dbc     4 OBJECT  GLOBAL DEFAULT   34 __environ@@GLIBC_2.0
#    elsif(/[0-9]+:[ \t]+([0-9a-f]+)[ \t]+[0-9]+[ \t]+OBJECT[ \t]+(GLOBAL|WEAK)[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+(.+)\@\@/)
    elsif(/[0-9]+:[ \t]+([0-9a-f]+)[ \t]+[0-9]+[ \t]+OBJECT[ \t]+(GLOBAL|WEAK)[ \t]+DEFAULT[ \t]+[0-9]+[ \t]+([^@ \t\n\r]+)/)
    {
      ($offs,$n)=($1,$3);
      if($offs =~ /[1-9a-f]/ )
      {
#         print FILEC "void * $n;\n";
#         print FILEC "\nint $n;\n";
#         $data .= ".globl $n\n$n: .long 0\n"
#        $data .= "\n .comm  $n,4,4\n"
         $data .= ($cflag) ? "void * $n;\n" :
                            "\n  .align 8\n .globl $n\n .type  $n, \@object\n .size   $n, 4\n$n: \n  .long  0\n" ;

      }

    }

  }

#  print FILEC  (cflag)?$data  : "\n\n.data\n\n$data\n";
  print FILEC  $data ;

  print FILEC "\n\n\n";
  close FILEC ;

#  $r = `gcc -m32 -o $pref$bname.so  -fno-builtin -fPIC -shared -O2 $pref$bname.c  -Wl,-soname=$soname` ;
#  $r = `gcc -m32 -o $pref$bname.so  -fno-builtin  -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -shared -O2 $pref$bname.c  -Wl,-soname=$soname` ;


#   $r = ($cflag)?
#      `gcc -m64 -o $pref$bname.so -fno-builtin -fno-rtti -fno-threadsafe-statics -fno-access-control -fno-nonansi-builtins -fno-elide-constructors -shared -O2 $tsrcname  -Wl,-soname=$soname`
#      : `gcc -m64 -o $pref$bname.so  -fno-builtin  -shared -O2 $tsrcname -Wl,-soname=$soname` ;
#

  $cmd = ($cflag)?
     "${prefix_gcc}gcc -pipe -o $pref$bname.so -fno-builtin -fPIC -nostartfiles -nodefaultlibs -shared -O2 $tsrcname -Wl,-soname=$soname"
   : "${prefix_gcc}gcc -pipe -o $pref$bname.so -fno-builtin -fPIC -nostartfiles -nodefaultlibs -shared -O2 $tsrcname -Wl,-soname=$soname" ;

  print  "$cmd\n" ;
  $r = `$cmd` ;

  print $r ;

  exit(0);


