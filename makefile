all:
	make -f makecli
	make -f makesrv

clean:
	make -f makecli clean
	make -f makesrv clean


