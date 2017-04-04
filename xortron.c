#include <stdio.h>
#include <string.h>

void xor_encrypt(char *key, char *string, int n)
{
    int i;
    int keyLength = strlen(key);
    for( i = 0 ; i < n ; i++ )
    {
        string[i]=string[i]^key[i%keyLength];
    }
}

int main(void) {
  char plain[] = "This is plain text";
  char key[] = "Abcdabcdabciabcdabcd";
  int n = strlen(plain);
  // encrypt:
  xor_encrypt(key, plain, n);
  printf("encrypted string: \n");
  for(int ii = 0; ii < n; ii++) {
    if(plain[ii] > 0x32 && plain[ii] < 0x7F ) printf("%c", plain[ii]);
   else printf(" 0x%02x ", plain[ii]);
  }
  printf("\n");
  // **** if you include this next line, things go wrong!
  // n = strlen(plain);
  xor_encrypt(key, plain, n);
  printf("after round trip, plain string is '%s'\n", plain);
}
