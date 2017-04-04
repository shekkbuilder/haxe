#include <stdio.h>
#include <stdlib.h>
int globalvar = 0;
void testypie() { return; }
int main () {
    char *heap = malloc(10);
    int stackvar = 0;
    printf ("Globals: %p, Text %p, Stack %p\nHeap %p, testpie %p, printf %p\n",
            &globalvar, main, &stackvar, heap, &testypie, &printf);
}
