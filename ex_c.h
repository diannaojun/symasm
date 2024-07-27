#ifndef __SYMLANG_EX_C_H__
#define __SYMLANG_EX_C_H__

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned __int128 u128;
typedef u128 uintrg;

union allu{
  u8 n8; u16 n16; u32 n32;
  u64 n64; u128 n128;
  uintrg nmax;
};

void strlwr (char* s){
  while(*s != '\0'){
    if(isupper(*s)){
      *s = tolower(*s);
    }
    s++;
  }
}

void* fgetln (char* s, size_t len, FILE* fp){
  static size_t i;
  -- len;
  for(i=0; i < len; ++ i){
    if(feof(fp)){
      fprintf(stderr, "EOF\n");
      if(i == len - 1){
        if(ungetc(s[i], fp) == EOF){
          fprintf(stderr, "ERROR::FIO::Output_File_Read_Error\n");
          exit(ERROR__FIO__Output_File_Read_Error);
        }
      }
      s[i] = '\n';
      break;
    }else{
      s[i] = fgetc(fp);
      if(s[i] == '\n'){
        break;
      }
      if(s[i] == EOF){
        s[i] = '\n';
        break;
      }
    }
  }
  s[i + 1] = '\0';
  return s;
}

void fputd (uintrg i, int siz, FILE* fp){
  union allu an;
  an.nmax = i;
  switch(siz){
    case 0: fwrite(&an.nmax, sizeof(an.nmax), 1, fp); break;
    case 1: fwrite(&an.n8, sizeof(an.n8), 1, fp); break;
    case 2: fwrite(&an.n16, sizeof(an.n16), 1, fp); break;
    case 3: fwrite(&an.n32, sizeof(an.n32), 1, fp); break;
    case 4: fwrite(&an.n64, sizeof(an.n64), 1, fp); break;
    case 5: fwrite(&an.n128, sizeof(an.n128), 1, fp); break;
    default:;
  }
}
#endif
