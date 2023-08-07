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
	FsMkfile(fs, "hello/a.txt");
	FsMkdir(fs, "world");
	FsTree(fs, NULL);
	printf("---\n");
	char *src[] = { "hello", NULL };
	FsCp(fs, true, src, "world");
	FsTree(fs, NULL);

    FsFree(fs);
}