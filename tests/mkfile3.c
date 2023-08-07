#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkfile(fs, "hello");
	FsTree(fs, NULL);
	FsMkfile(fs, "hello/world");
	FsMkdir(fs, "html");
	FsMkfile(fs, "html/index.html");
	FsMkfile(fs, "html/index.html/hi");
	FsTree(fs, NULL);

    FsFree(fs);
}