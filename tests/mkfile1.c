#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	
	// this is equivalent to FsMkfile(fs, "hello.c"); because initially,
	// the current working directory is the root directory
	FsMkfile(fs, "/hello.c");
	
	FsMkfile(fs, "world.c");
	FsMkdir(fs, "/bin");
	FsMkfile(fs, "bin/mkdir");
	FsMkfile(fs, "bin/mkfile");
	
	// see the section for FsTree for details
	FsTree(fs, NULL);

    FsFree(fs);
}