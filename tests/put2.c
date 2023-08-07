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
	FsPut(fs, "hello", "random-message\n");
	FsPut(fs, ".", "random-message\n");
	FsPut(fs, "/", "random-message\n");

    FsFree(fs);
}