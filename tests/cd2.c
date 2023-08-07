#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsCd(fs, "."); // does nothing
	FsCd(fs, ".."); // does nothing, since the parent of the root directory is itself
	FsCd(fs, "./.././../."); // also does nothing
	FsMkdir(fs, "tmp");
	FsCd(fs, "tmp");
	FsMkfile(fs, "random.txt");
	FsMkdir(fs, "../bin");
	FsMkdir(fs, "./../home");
	FsTree(fs, NULL);

    FsFree(fs);
}