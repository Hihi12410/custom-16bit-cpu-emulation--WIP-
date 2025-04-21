gcc -ggdb -o ../test/test_bin/cpu_test ../cpu/cpu.c ../render/render.c ../utils/helper.c -lSDL2
gcc -o ../test/testcode_gen ../test/test_code.c
../test/testcode_gen ../test/test_bin/prog.bin
fpath="../dump/memdump/dump-$(date +%y-%m-%d_%H-%M-%S).dump"
../test/test_bin/cpu_test ../test/test_bin/prog.bin $fpath
cat $fpath > ../dump/memdump/latest.bin