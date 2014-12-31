/**
 *
 */
/***************************************************************************
 * File Name  : decmplzma.cpp
 *
 * Description: This program decompress the lzma section in a bootloader rom
 *              Seems that Broadcom uses a custom LZMA format to compress the
 *              second stage of CFE, this program decompresses that.
 *
 *              In order to compile this program launch (from the root of the
 *              project)
 *
 *                make -C hostTools decmplzma EXTRA=-D__DECMPLZMA__
 *
 * Updates    : 31/12/2014  gipi.  Created.
 *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



#include "7z.h"


void usage(char* progName) {
    printf("decompress LZMA broadcom specific second stage CFE\n\n");
    printf("usage: %s compressedfile\n", progName);
    exit(-1);
}

int main(int argc, char* argv[]) {
    struct stat StatBuf;

    if (argc < 2) {
        usage(argv[0]);
    }

    char* inputFilePath = strdup(argv[1]);

    FILE* inputFile = fopen(inputFilePath, "rb");
    stat(inputFilePath, &StatBuf);

    unsigned char* srcBuffer = (unsigned char*) malloc(StatBuf.st_size);

    if (fread(srcBuffer, sizeof(char), StatBuf.st_size, inputFile) != StatBuf.st_size) {
        printf( "Error read input file %s.\n\n", inputFile);
        return -1;
    }

    unsigned char* dstBuffer = (unsigned char*) malloc(1024*1024*24);

    int err = decompress_lzma_7z(
        srcBuffer,
        StatBuf.st_size,
        dstBuffer,
        23*1024*1024);

    if (err != 0) {
        fprintf(stderr, "fatal: LZMA error %d", err);
        exit(-1);
    }

    fwrite(dstBuffer, sizeof(char), 1024*1024, stdout);

    free(srcBuffer);
    free(dstBuffer);

    return 0;
}
