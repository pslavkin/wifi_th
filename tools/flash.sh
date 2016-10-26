echo "00000-00100 ...... versiones de produccion"
echo "10000-10100 ...... versiones de debug"
echo "10000       ...... Test de consumo: Estrategia sleep"
echo "10000       ...... Test de consumo: Estrategia sleep"
echo "Ingrese opcion: 00000-99999"
read NAME
if [ -f ../exe/app$NAME.bin ];
 then 
	echo "Ok, inicio programacion"
	cp ../exe/app$NAME.bin ../exe/app.bin
	/mnt/local_mass/opt/ti/uniflashv3_3/uniflash_3.3/uniflashCLI.sh  -config ./cc3200.usf -setOptions com=1 -operation program
 else
	echo "Nop, esta version no esta disponible"
 fi
