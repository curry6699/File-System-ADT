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
	FsPut(fs, "hello.txt", "hello\n");
	FsCat(fs, "hello.txt");
	FsPut(fs, "./hello.txt", "world\n"); // overwrites existing content
	FsCat(fs, "/hello.txt");

    FsFree(fs);
}