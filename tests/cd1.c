#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkdir(fs, "/home");
	FsCd(fs, "home");
	FsMkdir(fs, "jas");
	FsCd(fs, "jas");
	FsMkdir(fs, "cs2521");
	FsCd(fs, "cs2521");
	FsMkdir(fs, "lectures");
	FsMkdir(fs, "tutes");
	FsMkdir(fs, "labs");
	FsTree(fs, NULL);

    FsFree(fs);
}