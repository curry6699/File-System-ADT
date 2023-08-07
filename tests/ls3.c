#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkdir(fs, "tmp");
	FsMkfile(fs, "tmp/hello.txt");
	FsMkfile(fs, "tmp/world.txt");
	FsCd(fs, "tmp");
	FsLs(fs, NULL);

    FsFree(fs);
}