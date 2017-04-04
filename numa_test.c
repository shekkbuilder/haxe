#include <stdlib.h>
#include <stdio.h>

int main() {
	char *blah[65535];
	int i;
	FILE *fp;
	char buf[4096];

	printf("Allocate...\n");
	for (i=0;i<65535;i++) {
		blah[i] = malloc((size_t) 32768);
		if (blah[i] == NULL) {
			printf("Nop.\n");
		} else {
			blah[i][0] = 'X';
		}
	}
	
	fp = fopen("/proc/self/numa_maps","r");
	while (fgets(buf,4096,fp) != NULL)
		printf(buf);
	fclose(fp);
	printf("Free willy!\n");
	for (i=0;i<65535;i++) {
		if (blah[i] != NULL) {
			free(blah[i]);
		}
	}
	return 0;
}
