#include<stdio.h>
#include"encrypter.h"

int encrypter(char *Text){
  int Val;
  while(*Text != '\0'){
    if(*Text >= 65 && *Text <= 90) Val = (((*Text)-'A')/5) + 1;
    else{if(*Text >= 97 && *Text <= 122) Val = (((*Text)-'a')/5) + 30;}

    switch (Val) {
      
    }
    (*Text) += 5;
    Text++;
  }
  return 0;
}
