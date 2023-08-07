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
	FsMkfile(fs, "world.txt");
	FsPut(fs, "world.txt", "world\n");
	FsCat(fs, "world.txt");
	printf("---\n");
	char *src[] = { "hello.txt", NULL };
	FsCp(fs, false, src, "world.txt");
	FsCat(fs, "world.txt");
	printf("---\n");
	FsTree(fs, NULL);

    FsFree(fs);
}