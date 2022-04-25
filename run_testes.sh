#!/bin/bash

sizes=(10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)
groups=("L3" "L2CACHE" "FLOPS_DP")

for folderCount in 2
do
	echo "-- Inicio do processo para p$folderCount --"
	for i in ${!groups[@]};
	do
		if [[ ${groups[$i]} == "L3" ]]; then
			echo "=== Inicio do processo de L3 ==="
			for N in ${!sizes[@]};
			do
				echo "[+] Inicialiazando ${sizes[$N]}.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/${sizes[$N]}.txt > ./p$folderCount/l3/${sizes[$N]}-l3.txt
				echo "[+] Finalizando ${sizes[$N]}.txt"
			done
		fi
		if [[ ${groups[$i]} == "L2CACHE" ]]; then
			echo "=== Inicio do processo L2CACHE ==="
			for N in ${!sizes[@]};
			do
				echo "[+] Inicializando ${sizes[$N]}.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/${sizes[$N]}.txt > ./p$folderCount/l2cache/${sizes[$N]}-l2cache.txt
				echo "[+] Finalizando ${sizes[$N]}.txt"
			done
		fi
		if [[ ${groups[$i]} == "FLOPS_DP" ]]; then
			echo "=== Inici do processo FLOPS_DP ==="
			for N in ${!sizes[@]}
			do
				echo "[+] Inicializando ${sizes[$N]}.txt"
				likwid-perfctr -c 3 -g ${groups[$i]} ./p$folderCount/newtonPC < ./in/${sizes[$N]}.txt > ./p$folderCount/flops/${sizes[$N]}-flops.txt
				echo "[+] Finalizando ${sizes[$N]}.txt"
			done
		fi	
				
	done
done

