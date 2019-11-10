all:
	make th-cmd

src:
	cd src && make all

th-cmd:
	gcc -o th-init th-init.c src/fileHandler.c
	gcc -o th-commit th-commit.c src/fileHandler.c	
	gcc -o th-log th-log.c
	gcc -o th-checkout th-checkout.c