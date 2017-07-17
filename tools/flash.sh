fcho "00000-00100 ...... versiones de produccion"
echo "10000-10100 ...... versiones de debug"
echo "10000       ...... Test de consumo: Estrategia sleep"
echo "10000       ...... Test de consumo: Estrategia sleep"
echo "Ingrese opcion: 00000-99999"
read NAME
if [ -f ../exe/app$NAME.bin ];
 then 
	echo "Ok, inicio programacion"
	cp ../exe/app$NAME.bin ../exe/app.bin
	/opt/ti/uniflash_3.4/uniflashCLI.sh  -config ./cc3200.usf -setOptions com=167 -operation program
 else
	echo "Nop, esta version no esta disponible"
 fi
