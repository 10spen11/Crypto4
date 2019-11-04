ALL: MAC
	
MAC: mac.c MD5
	gcc -m32 -o mac mac.c
	

MD5: md5.h
	
CLEAN:
	rm -f mac
	make ALL