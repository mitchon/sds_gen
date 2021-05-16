.PHONY: any clean install uninstall

any: output

output: sds_gen.o elipt_cur.o hash_lib.o
	gcc sds_gen.o elipt_cur.o hash_lib.o -o sds_gen -lgmp

sds_gen.o: sds_gen.c elipt_cur.h hash_lib.h
	gcc -c sds_gen.c -lgmp

elipt_cur.o: elipt_cur.c
	gcc -c elipt_cur.c -lgmp

hash_lib.o: hash_lib.c
	gcc -c hash_lib.c -lgmp

clean: 
	rm *.o sds_gen

install:
	install ./sds_gen /usr/local/bin
	install -d /usr/local/etc/sds
	install ./ds_params /usr/local/etc/sds
	install ./accounts /usr/local/etc/sds
	install ./public_accounts /usr/local/etc/sds

uninstall:
	rm -rf /usr/local/bin/sds_gen
	rm -rf /usr/local/etc/sds
	