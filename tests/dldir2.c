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
	FsMkdir(fs, "hello/world");
	FsDldir(fs, "hello");
	FsTree(fs, NULL);

    FsFree(fs);
}