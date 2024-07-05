#include<stdio.h>
#include"encrypter.h"

int main(){
  char Text[10] = "HelloTech\0";
  printf("%s\n", Text);
  encrypter(Text);
  printf("%s\n", Text);
}
