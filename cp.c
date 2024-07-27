#ifndef __SYMLANG_CP_C__
#define __SYMLANG_CP_C__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_REGISTERS 4
#define MAX_MEMORY 256
#define MAX_MACROS 10
#define MAX_INSTRUCTION_LENGTH 32
#define MAX_TOKEN_LENGTH 10
#define MAX_FILENAME_LENGTH 1024

#define ERROR__FIO__Input_File_Cannot_Open 0x00010001
#define ERROR__FIO__Output_File_Cannot_Open 0x00010002
#define ERROR__FIO__Output_File_Read_Error 0x00010003
#define ERROR__CODE__Instruction_Error 0x00020001
#define ERROR__CODE__Grammar_Error 0x00021000

#include "ins.h"
#include "ex_c.h"

int getToken (char*, char*[]);

int main (int argc, char* argv[]){
  int error, tokenCnt, line;
  char input[MAX_INSTRUCTION_LENGTH];
  char cpinput[sizeof(input)];
  char* token[MAX_TOKEN_LENGTH];
  char infn[MAX_FILENAME_LENGTH]="a.sym";
  char outfn[MAX_FILENAME_LENGTH]="a.sya";
  FILE* fpo = stdout;
  
  line = 0;
  if(argc > 1){
    strlcpy(infn, argv[1], sizeof(infn));
  }
  if(argc > 2){
    strlcpy(outfn, argv[2], sizeof(infn));
  }
  if(freopen(infn, "r", stdin) == NULL){
    fprintf(stderr, "ERROR::FIO::Input_File_Cannot_Open\n");
    fprintf(stderr, "At : %s\n", infn);
    exit(ERROR__FIO__Input_File_Cannot_Open);
  }
  if(freopen(outfn, "wb", fpo) == NULL){
    fprintf(stderr, "ERROR::FIO::Output_File_Cannot_Open\n");
    fprintf(stderr, "At : %s\n", outfn);
    exit(ERROR__FIO__Output_File_Cannot_Open);
  }
  
  while(!feof(stdin)){
    ++ line, error = 0;
    fgetln(input, sizeof(input), stdin);
    strlcpy(cpinput, input, sizeof(cpinput));
    strlwr(cpinput);
    tokenCnt = getToken(cpinput, token);
    //fprintf(fpo, "GET %d\n", tokenCnt);
    if(!strcmp(token[0], "mov")){
      error = executeMOV(token, tokenCnt, fpo);
    }else if(!strcmp(token[0], "pas")){
      error = executePAS(token, tokenCnt, fpo);
    }else if(tokenCnt == 0){
      continue;
    }else{
      fprintf(stderr, "ERROR::CODE::Instruction_Error\n");
      fprintf(stderr, "At Line %d : %s\n", line, input);
      exit(ERROR__CODE__Instruction_Error);
      break;
    }
    if(error != 0){
      fprintf(stderr, "ERROR::CODE::Grammar_Error\n");
      fprintf(stderr, "At Line %d : %s\n", line, input);
      exit(ERROR__CODE__Grammar_Error | error);
    }
  }
  return 0;
}
// 简化的词法分析
int getToken (char* input, char* token[]){
  static int tokenCnt, inLen;
  static char *tokenBgn, *tokenFill;
  tokenCnt = 0;
  tokenBgn = NULL;
  while(isspace(*input) && *input != '\0'){
    ++ input;
  }
  while(1){
    tokenBgn = input;
    while(!isspace(*input) && *input != '\0'){
      ++ input;
    }
    *input = '\0';
    if(input == tokenBgn){
      break;
    }
    token[tokenCnt] = tokenBgn;
    ++ tokenCnt, ++ input;
    while(isspace(*input) && *input != '\0'){
      ++ input;
    }
  }
  return tokenCnt;
}
#endif
