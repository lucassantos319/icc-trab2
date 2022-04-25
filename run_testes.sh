
for N in 10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096
do
	likwid-perfctr -c 3 -g BRANCH ./p2/newtonPC < ./in/$N.txt > ./p2/tempos/$N.txt
done

