CFLAGS = -fdump-rtl-expand -Wall -g -O -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-variable -Wno-maybe-uninitialized -I ../../../SPOOLES.2.2 -DARCH="Linux" -DSPOOLES -DARPACK -DMATRIXSTORAGE -DNETWORKOUT #指定SPOOLES头文件的目录，定义宏SPOOLES和ARPACK表示使用这两个库，-g调试。
FFLAGS = -fdump-rtl-expand -Wall -g -O -Wno-unused-label -Wno-maybe-uninitialized -Wno-unused-dummy-argument -Wno-unused-variable -std=legacy #设置-std=legacy会使得gfortran8及以上的编译器在编译老代码中不再被支持的特性时，不会报错。

CC = gcc
FC = gfortran

%o : %c
	$(CC) $(CFLAGS) -c $<
%o : %f
	$(FC) $(FFLAGS) -c $<

include Makefile.inc #其中定义了三个变量，分别为一堆文件名SCCXF包含940个.f源文件，SCCXC包含176个.c源文件，SCCXCXX包含一个umat_dl.cpp文件。第一个字母表示source，中间的CCX表示Calculi下，最后一个F表示Fortran

SCCXMAIN = ccx_2.22.c #主文件，main函数所在位置

#分别进行后缀名替换，创建对应的目标文件集合的变量名
OCCXF = $(SCCXF:.f=.o)
OCCXC = $(SCCXC:.c=.o)
OCCXMAIN = $(SCCXMAIN:.c=.o)

DIR = ../../../SPOOLES.2.2

#设置要链接的库名和位置
LIBS = $(DIR)/spooles.a ../../../ARPACK/libarpack_INTEL.a -lpthread -lm -lc

ccx_2.22: $(OCCXMAIN) ccx_2.22.a  $(LIBS)
	#./date.pl; #修改三个重要文件中的时间标志，可以定期单独在shell中执行该命令
	$(CC) $(CFLAGS) -c ccx_2.22.c; #单独编译主文件
	$(FC) $(FFLAGS) -o $@ $(OCCXMAIN) ccx_2.22.a $(LIBS) -fopenmp #使用gfortran进行链接。
#打包所有的目标文件到静态库文件

ccx_2.22.a: $(OCCXF) $(OCCXC)
	ar vr $@ $?

clean:
	rm -f *.o */*.o *.a ccx_2.22
	rm -f *.expand */*.expand *.png *.pdf callgrind.out.*
	rm -f input.* spooles.out #并不需要了，因为已经添加了修改工作目录的代码
	rm -f .vscode-ctags .ctags tags

test:
	rm -f callgrind.out.*
	valgrind --tool=callgrind --compress-strings=no --compress-pos=no --collect-jumps=yes ./ccx_2.22 -i ../test/beamp
	kcachegrind callgrind.out.*