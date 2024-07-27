#ifndef __SYMLANG_INS_H__
#define __SYMLANG_INS_H__

#define EXEH char* token[], int tokenCnt, FILE* fp
#define PRERR(str, code) {printf("ERROR::MOV::Dest_is_Immediate\n");return code;}
#define ELERR(str, code) else PRERR(str, code)

#include <stdio.h>
#include <ctype.h>
#include "ex_c.h"

enum ins {
  pas, end_i, end_r, mov_rr, mov_ri, mov_rm
};

// 检查是否为寄存器
int isRegister (char* str){
  return strlen(str) == 2 && strncmp(str, "r", 1) == 0 
  && str[1] >= '0' && str[1] <= '3';
}
// 检查是否为立即数
int isImmediate(char *str) {
    return (str[0] >= '0' && str[0] <= '9') || (str[0] == '-' || str[0] == '+');
}
// 解析立即数
uintrg parseImmediate(char *str) {
    static uintrg ret;
    static char sng;
    ret = 0, sng = 0;
    while(!isdigit(*str)){
      if(*str == '-') sng = ~sng;
      ++ str;
    }
    while(isdigit(*str)){
      ret = (ret << 3) + (ret << 1) + (*str - '0');
      ++ str;
    }
    return (sng ? ret : -ret);
}
// 解析寄存器
int parseRegister(char *str) {
    return str[1] - '0';
}

int executeMOV (EXEH){
  int dest, src;
  if(tokenCnt == 3){
    // printf("%s %s\n",token[1],token[2]);
    if(isRegister(token[1])){
      dest = parseRegister(token[1]);
      if(isImmediate(token[2])){
        src = parseImmediate(token[2]);
        fputd(mov_ri, 1, fp);
        fputd(dest, 1, fp);
        fputd(src, 0, fp);
      }else if(isRegister(token[2])){
        src = parseRegister(token[2]);
        fputd(mov_rr, 1, fp);
        fputd(dest, 1, fp);
        fputd(src, 1, fp);
      }else{
        // 假设src是内存地址
        src = parseImmediate(token[2]);
        fputd(mov_rm, 1, fp);
        fputd(dest, 1, fp);
        fputd(src, 0, fp);
      }
    }else if(isImmediate(token[1]))
      PRERR("ERROR::MOV::Dest_is_Immediate\n", 1)
    else{
      dest = parseRegister(token[1]);
      if(isRegister(token[2])){
        src = parseRegister(token[2]);
        printf("mov [%d], r%d\n", dest, src);
      }ELERR("ERROR::MOV::xx_of_M-xx_isnot_Register\n", 1)
    }
  }ELERR("ERROR::MOV::ALE\n",1)
  return 0;
}

int executePAS(EXEH){
  static int tim, i;
  if(tokenCnt == 1){
    tim = 1;
  }else if(tokenCnt == 2){
    if(isImmediate(token[1])){
      tim = parseImmediate(token[1]);
      for(i = 0; i < tim; ++ i){
        fputd(pas, 1, fp);
      }
    }ELERR("ERROR::PAS::Times_Number_Must_be_an_Immediate\n",1)
  }ELERR("ERROR::PAS::ALE\n",1)
  return 0;
}

int executeEND(EXEH){
  static uintrg ret;
  if(tokenCnt == 1){
    fputd(end_i, 1, fp);
    fputd(0, 0, fp);
  }else if(tokenCnt == 2){
    if(isImmediate(token[1])){
      ret = parseImmediate(token[1]);
      fputd(end_i, 1, fp);
      fputd(ret, 0, fp);
    }else if(isRegister(token[1])){
      ret = parseRegister(token[1]);
      fputd(end_r, 1, fp);
      fputd(ret, 1, fp);
    }
    ELERR("ERROR::END::Times_Number_Mustnot_be_an_Address\n",1)
  }ELERR("ERROR::END::ALE\n",1)
  return 0;
}

#endif
