#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	
	// this is equivalent to FsMkdir(fs, "tmp"); because initially,
	// the current working directory is the root directory
	FsMkdir(fs, "/tmp");
	
	FsMkdir(fs, "/tmp/tmp.123");
	FsMkdir(fs, "/usr");
	FsMkdir(fs, "/bin");
	
	// see the section for FsTree for details
	FsTree(fs, NULL);

    FsFree(fs);
}