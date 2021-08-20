#include <stdio.h>
#include <stdlib.h>

unsigned char buff[100];
int main(){
  FILE *fp = fopen("./test.ch8","rb");
  if(!fp)
  {
    perror("Error opening file");
    exit(1);
  }
  int n  = 0;
  int tot = 0;
  /* while( (n=fread(buff+n, sizeof(unsigned int), 100 ,fp)) != EOF) { tot+=n; puts("reading"); } */
  printf("%d\n",);
  for(int i =0;i<n;i++)
    printf("%x ",buff[i]);
  fclose(fp);
  return 0;
}
