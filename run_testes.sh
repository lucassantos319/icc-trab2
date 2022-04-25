sizes=(10)
groups=("L3" "L2CACHE" "FLOPS_DP")

for folderCount in seq 2
do
	for G in $groups
	do
		if [$G == "L3"]; then
			for N in $sizes
			do
				likwid-perfctr -c 3 -g $G ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/l3/$N.txt
			done
		else
			if [$G == "L2CACHE"]; then
				for N in $sizes
				do
					likwid-perfctr -c 3 -g $G ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/l2cache/$N.txt
				done
			else
				if [$G == "FLOPS_DP"]; then
					for N in $sizes
					do
						likwid-perfctr -c 3 -g $G ./p$folderCount/newtonPC < ./in/$N.txt > ./p$folderCount/flops/$N.txt
					done
				fi
			fi
		fi


		
	done
done

