gcc @yc  -DRICON
gcc @ya  -DRICON
make_pe.exe  -p 0x3C00000 -o http.exe a.out
cpres.exe copyres.exe http.exe
bin2c.exe http.exe http.bin
as stpdta.s -o stpdta.o
gcc @sc0 -DRICON
gcc @sa -DRICON
make_pe.exe -o shttp3.exe a.out
cpres.exe copyres.exe shttp3.exe
