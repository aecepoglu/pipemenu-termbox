EXE=pmenu-tb
out:
	gcc -o ${EXE} main.c streamReader.c -ltermbox -lcsv
clean:	
	rm ${EXE}
install:
	install ${EXE} "${DESTDIR}/rssproc" 
