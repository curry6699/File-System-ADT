#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	printf("---\n"); // marker to separate output
	FsLs(fs, "/");
	printf("---\n");
	FsMkfile(fs, "hello.txt");
	FsMkdir(fs, "tmp");
	FsLs(fs, "/");

    FsFree(fs);
}