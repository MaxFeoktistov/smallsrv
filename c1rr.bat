as stpdtar.s -o stpdtar.o
gcc @sc0 -DRUS -DRICON
gcc @sar -DRICON
make_pe.exe -o shttp3r.exe a.out
cpres.exe copyres.exe shttp3r.exe
