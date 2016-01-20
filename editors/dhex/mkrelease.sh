mkdir dhex_0.68
for I in `echo "
Makefile 
buffers.c 
buffers.h 
config.h 
configfile.c 
configfile.h 
correlation.c 
correlation.h 
datatypes.h 
gpl.c 
gpl.h 
gpl.txt 
hexcalc.c 
hexcalc.h 
input.c 
input.h 
machine_type.c 
machine_type.h 
main.c 
markers.c 
markers.h 
menu.c 
menu.h 
output.c 
output.h 
search.c 
search.h 
ui.c 
ui.h 
README.txt 
todo.txt 
dhex.1 
dhexrc.5 
dhex_markers.5 
dhex_searchlog.5 
mkrelease.sh"  | sort -f`
do
	cp $I dhex_0.68/
done
( cd dhex_0.68/ && make && ./dhex -v && make clean && cd .. && \
tar cvfz dhex_0.68.tar.gz dhex_0.68  )
cp dhex_0.68.tar.gz dhex_latest.tar.gz
md5sum dhex_0.68.tar.gz
md5sum dhex_latest.tar.gz

