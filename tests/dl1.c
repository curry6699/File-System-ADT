#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkdir(fs, "hello");
	FsMkfile(fs, "hello/world.txt");
	FsMkfile(fs, "abc.txt");
	FsTree(fs, NULL);
	printf("---\n"); // marker to separate output
	FsDl(fs, true, "abc.txt");
	FsTree(fs, NULL);
	printf("---\n"); // marker to separate output
	FsDl(fs, true, "hello");
	FsTree(fs, NULL);

    FsFree(fs);
}