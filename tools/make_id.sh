#!/bin/bash
echo "compilar desde:"
read From
echo "compilar hasta:"
read To


cat > make_id_aux.sh <<_EOF_
#!/bin/bash
for i in {$From..$To}; do  
 	echo "compilando ID=\$i"
 	echo "#define SERIAL_ID \"justbody\$i\"" > ../h/id.h
 	cd ..
	make clean
	make  
 	cd ./tools
	cp ../exe/app.bin ./app\$i.bin
	cp ../exe/app.bin ../exe/app\$i.bin
 	done ; 
_EOF_
./make_id_aux.sh

tar -cvzf ./app.tar.gz ./app*.bin
#rm ./*.bin

