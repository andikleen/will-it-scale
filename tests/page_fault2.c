#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#define MEMSIZE (128 * 1024 * 1024)

char *testcase_description = "Separate file private mapping page fault";

void testcase(unsigned long long *iterations)
{
	char tmpfile[] = "/tmp/willitscale.XXXXXX";
	int fd = mkstemp(tmpfile);
	unsigned long pgsize = getpagesize();

	assert(fd >= 0);
	assert(ftruncate(fd, MEMSIZE) == 0);
	unlink(tmpfile);

	while (1) {
		unsigned long i;

		char *c = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE,
			       MAP_PRIVATE, fd, 0);
		assert(c != MAP_FAILED);

		for (i = 0; i < MEMSIZE; i += pgsize) {
			c[i] = 0;
			(*iterations)++;
		}

		munmap(c, MEMSIZE);
	}
}
