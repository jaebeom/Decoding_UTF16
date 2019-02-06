IDIR =../include
CC=gcc
CFLAGS= -g -Wall

all: decode

decode: decodeUTF16.c
	$(CC) $(CFLAGG) decodeUTF16.c -o decodeUTF16

testCheck:
	./decodeUTF16 utf16-val-7a-be tmp
	cmp tmp utf32-val-7a-be
	./decodeUTF16 utf16-val-7a-le tmp
	cmp tmp utf32-val-7a-le

	./decodeUTF16 utf16-val-760-be tmp
	cmp tmp utf32-val-760-be
	./decodeUTF16 utf16-val-760-le tmp
	cmp tmp utf32-val-760-le

	./decodeUTF16 utf16-val-a999-be tmp
	cmp tmp utf32-val-a999-be
	./decodeUTF16 utf16-val-a999-le tmp
	cmp tmp utf32-val-a999-le

	./decodeUTF16 utf16-val-12345-be tmp
	cmp tmp utf32-val-12345-be
	./decodeUTF16 utf16-val-12345-le tmp
	cmp tmp utf32-val-12345-le

	./decodeUTF16 utf16-100-vals-be tmp
	cmp utf32-100-vals-be tmp
	./decodeUTF16 utf16-100-vals-le tmp
	cmp utf32-100-vals-le tmp

	./decodeUTF16 utf16-val-12345-be-test tmp
	cmp tmp utf32-val-12345-be-test

ta: testErr

testErr:
	od -tx1 utf16-err4
	./decodeUTF16 utf16-err4 tmp

currentTest:
	./decodeUTF16 utf16-val-12345-le tmp

	od -tx1 utf16-val-12345-le
	od -tx1 utf32-val-12345-le
	od -tx1 tmp

	od -tx1 utf16-val-12345-be
	od -tx1 utf32-val-12345-be

	cmp tmp utf32-val-12345-le

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
