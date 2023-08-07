#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Fs.h"

int main(void) {
	Fs fs = FsNew();
	
	char cwd[PATH_MAX + 1];
	FsGetCwd(fs, cwd);
	printf("cwd: %s\n", cwd);
	
	FsMkdir(fs, "tmp"); // see Stage 1 for FsCd and FsMkdir
	FsCd(fs, "tmp");
	FsGetCwd(fs, cwd);
	printf("cwd: %s\n", cwd);
	
	FsMkdir(fs, "abc");
	FsCd(fs, "abc");
	FsGetCwd(fs, cwd);
	printf("cwd: %s\n", cwd);

    FsFree(fs);
}