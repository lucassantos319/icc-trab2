#!/bin/bash

sizes=(10)
groups=("L3" "L2CACHE" "FLOPS_DP")

for folderCount in 2
do
	echo "-- Inicio do processo para p$folderCount --"
	for i in ${!groups[@]};
	do
		if [[ ${groups[$i]} == "L3" ]]; then
			echo "=== Inicio do processo de L3 ==="
			for N in $sizes
			do
				echo "[+] Inicialiazando $N.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/l3/$N.txt
				echo "[+] Finalizando $N.txt"
			done
		fi
		if [[ ${groups[$i]} == "L2CACHE" ]]; then
			echo "=== Inicio do processo L2CACHE ==="
			for N in $sizes;
			do
				echo "[+] Inicializando $N.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/l2cache/$N.txt
				echo "[+] Finalizando $N.txt"
			done
		fi
		if [[ ${groups[$i]} == "FLOPS_DP" ]]; then
			echo "=== Inici do processo FLOPS_DP ==="
			for N in $sizes
			do
				echo "[+] Inicializando $N.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/flops/$N.txt
				echo "[+] Finalizando $N.txt"
			done
		fi	
				
	done
done

