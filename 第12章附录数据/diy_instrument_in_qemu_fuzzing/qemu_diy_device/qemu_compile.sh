cd ~/Desktop/vm_qemu/qemu_fuzzer/instrument
cp ./sanitize_converage.o ../../qemu-5.0.0/
cd ~/Desktop/vm_qemu/qemu-5.0.0/
./configure --cc="/usr/bin/clang-sp" --extra-cflags="-fsanitize-coverage=trace-pc-guard -g3 -fsanitize=address -fPIE"
make -j14 CFLAGS="/home/ubuntu/Desktop/vm_qemu/qemu_fuzzer/instrument/sanitize_converage.o -g3 -Wall -Wno-unused-command-line-argument"
