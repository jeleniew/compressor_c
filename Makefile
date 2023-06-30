compress: main.o tree_files/tree_compression.o tree_files/tree_decompression.o compression_files/compress.o compression_files/decompress.o coding_files/code.o
	cc -o compress main.o tree_files/tree_compression.o tree_files/tree_decompression.o compression_files/compress.o compression_files/decompress.o coding_files/code.o

main.o: main.c
	cc -c main.c

tree_files/tree_compression.o: tree_files/tree_compression.c tree_files/tree.h
	cc -o tree_files/tree_compression.o -c tree_files/tree_compression.c

tree_files/tree_decompression.o: tree_files/tree_decompression.c tree_files/tree.h
	cc -o tree_files/tree_decompression.o -c tree_files/tree_decompression.c

compression_files/compress.o: compression_files/compress.c compression_files/compress.h tree_files/tree.h coding_files/code.h
	cc -o compression_files/compress.o -c compression_files/compress.c

compression_files/decompress.o: compression_files/decompress.c compression_files/compress.h tree_files/tree.h coding_files/code.h
	cc -o compression_files/decompress.o -c compression_files/decompress.c

coding_files/code.o: coding_files/code.c coding_files/code.h tree_files/tree.h
	cc -o coding_files/code.o -c coding_files/code.c

.PHONY: clean
clean:
	-rm *.o compress
	-rm */*.o