#!/bin/bash
for i in {00000..00000}; do  
 	echo "compilando ID=$i"
 	echo "#define SERIAL_ID \"justbody$i\"" > ../h/id.h
 	cd ..
	make clean
	make  
 	cd ./tools
	cp ../exe/app.bin ./app$i.bin
	cp ../exe/app.bin ../exe/app$i.bin
 	done ; 
