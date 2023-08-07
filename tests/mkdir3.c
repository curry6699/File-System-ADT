#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	
	// see the section for FsMkfile for details
	FsMkfile(fs, "hello.txt");
	
	FsTree(fs, NULL);
	FsMkdir(fs, "hello.txt/world");
	FsMkdir(fs, "html");
	FsMkfile(fs, "html/index.html");
	FsMkdir(fs, "html/index.html/hi");
	FsTree(fs, NULL);

    FsFree(fs);
}