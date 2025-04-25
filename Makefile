all: ARPACK SPOOLES.2.2 CalculiX
	cd ARPACK; make -j lib
	cd SPOOLES.2.2; make -j lib
	cd CalculiX/ccx_2.22/src; make -j
install:
	sudo ln -sf /home/zj/CCX/CalculiX/ccx_2.22/src/ccx_2.22 /usr/bin/ccx
clean:
	cd ARPACK; make clean
	cd SPOOLES.2.2; make clean
	cd CalculiX/ccx_2.22/src; make clean
	cd CalculiX/ccx_2.22/test; make clean