#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkdir(fs, "home");
	FsMkdir(fs, "home/jas");
	FsMkdir(fs, "home/jas/Documents");
	FsCd(fs, "home/jas/Documents");
	FsPwd(fs);
	FsCd(fs, "..");
	FsPwd(fs);
	FsCd(fs, "..");
	FsPwd(fs);
	FsCd(fs, "..");
	FsPwd(fs);
	FsCd(fs, "..");
	FsPwd(fs);

    FsFree(fs);
}