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
	char str[] = "hello\n";
	FsPut(fs, "hello.txt", str);
	strcpy(str, "world\n");
	FsCat(fs, "hello.txt"); // see section on FsCat for details

    FsFree(fs);
}