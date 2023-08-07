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
	FsPut(fs, "world/hello.txt", "world\n");
	FsTree(fs, NULL);
	printf("---\n");
	FsCat(fs, "hello.txt");
	FsCat(fs, "world/hello.txt");
	printf("---\n");
	char *src[] = { "hello.txt", NULL };
	FsMv(fs, src, "world");
	FsTree(fs, NULL);
	printf("---\n");
	FsCat(fs, "world/hello.txt");

    FsFree(fs);
}