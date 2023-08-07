#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	FsMkfile(fs, "/tmp");
	FsMkfile(fs, "tmp");
	FsMkfile(fs, "./tmp");

    FsFree(fs);
}