
CLANG = clang-sp
CLANGPP = clang++

obj-m += kvm_hypercall.o

all:
	${CLANG}  -g -fsanitize-coverage=trace-pc-guard sanitize_converage.c -c -fPIE
	${CLANG}  -g example1.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example1
	${CLANG}  -g example2.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example2
	${CLANG}  -g example3.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example3
	${CLANG}  -g example4.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example4
	${CLANG}  -g example5.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example5
	${CLANG}  -g example6.c sanitize_converage.o -fsanitize-coverage=trace-pc-guard -o example6

clean:
	rm -rf sanitize_converage.o
	rm -rf example1
	rm -rf example2
	rm -rf example3
	rm -rf example4
	rm -rf example5
	rm -rf example6
	rm -rf temp_*

