#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsPwd(fs);
	FsMkdir(fs, "home");
	FsCd(fs, "home");
	FsPwd(fs);
	FsMkdir(fs, "jas");
	FsCd(fs, "jas");
	FsPwd(fs);

    FsFree(fs);
}