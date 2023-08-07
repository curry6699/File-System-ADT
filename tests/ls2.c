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
	FsMkdir(fs, "tmp");
	FsMkfile(fs, "tmp/world.txt");
	FsLs(fs, "hello.txt");
	FsLs(fs, "tmp/world.txt");
	FsLs(fs, "tmp/.././hello.txt");

    FsFree(fs);
}