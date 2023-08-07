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
	FsCd(fs, "tmp");
	FsCd(fs, NULL);
	FsMkfile(fs, "hello.txt");
	FsTree(fs, NULL);

    FsFree(fs);
}