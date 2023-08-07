#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkfile(fs, "hello.txt");
	FsPut(fs, "hello.txt", "hello\n");
	FsMkdir(fs, "world");
	FsMkfile(fs, "world/hello.txt");
	FsTree(fs, NULL);
	printf("---\n");
	FsCat(fs, "world/hello.txt");
	printf("---\n");
	char *src[] = { "hello.txt", NULL };
	FsCp(fs, false, src, "world");
	FsTree(fs, NULL);
	printf("---\n");
	FsCat(fs, "world/hello.txt");

    FsFree(fs);
}