
output: sds_gen.o elipt_cur.o hash_lib.o
	gcc sds_gen.o elipt_cur.o hash_lib.o -o sds_gen -lgmp

sds_gen.o: sds_gen.c elipt_cur.h hash_lib.h
	gcc -c sds_gen.c -lgmp

elipt_cur.o: elipt_cur.c
	gcc -c elipt_cur.c -lgmp

hash_lib.o: hash_lib.c
	gcc -c hash_lib.c -lgmp

clean: 
	del *.o output