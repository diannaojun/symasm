#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_REGISTERS 4
#define MAX_MEMORY 256
#define MAX_MACROS 10
#define MAX_INSTRUCTION_LENGTH 20
#define MAX_TOKEN_LENGTH 10

// 寄存器和内存
int registers[MAX_REGISTERS];
int memory[MAX_MEMORY];

// 宏定义
struct Macro {
    char name[MAX_TOKEN_LENGTH];
    int args;
    char replacement[MAX_INSTRUCTION_LENGTH][MAX_TOKEN_LENGTH];
};
struct Macro macros[MAX_MACROS];
int macroCount = 0;

// 检查是否为寄存器
int isRegister(char *str) {
    return strncmp(str, "R", 1) == 0 && strlen(str) == 2 && str[1] >= '0' && str[1] <= '3';
}

// 检查是否为立即数
int isImmediate(char *str) {
    return str[0] >= '0' && str[0] <= '9';
}

// 解析立即数
int parseImmediate(char *str) {
    return atoi(str);
}

// 解析寄存器
int parseRegister(char *str) {
    return str[1] - '0';
}

// 执行MOV指令
void executeMOV(char *dest, char *src) {
    if (isRegister(dest)) {
        int reg = parseRegister(dest);
        if (isImmediate(src)) {
            registers[reg] = parseImmediate(src);
        } else if (isRegister(src)) {
            registers[reg] = registers[parseRegister(src)];
        } else {
            // 假设src是内存地址
            registers[reg] = memory[atoi(src)];
        }
    } else {
        // 假设dest是内存地址
        int value = parseRegister(src);
        memory[atoi(dest)] = value;
    }
}

// 执行ADD指令
void executeADD(char *dest, char *src) {
    int reg1 = parseRegister(dest);
    int reg2 = parseRegister(src);
    registers[reg1] += reg2;
}

// 简化的词法分析
void tokenize(char *input, char *tokens[], int maxTokens) {
    int tokenCount = 0;
    char *tokenStart = input;
    while (*input != '\0' && tokenCount < maxTokens) {
        while (*input == ' ') input++; // 跳过空格
        tokenStart = input;
        while (*input != ' ' && *input != '\0') input++;
        size_t tokenLength = input - tokenStart;
        strncpy(tokens[tokenCount], tokenStart, tokenLength);
        tokens[tokenCount][tokenLength] = '\0';
        tokenCount++;
    }
    printf("tokenized %d\n",tokenCount);
}

int main() {
    int inputCount = 0;
    char input[MAX_INSTRUCTION_LENGTH];
    char tokens[MAX_TOKEN_LENGTH][MAX_INSTRUCTION_LENGTH];

    // 定义一个宏
    strcpy(macros[0].name, "ADD_R0_R1");
    macros[0].args = 0;
    strcpy(macros[0].replacement[0], "ADD R0 R1");

    // 编译器主循环
    while (1) {
        printf("asm > ");
        fgets(input,sizeof(input),stdin);
        printf("CCC");
        if (strncmp(input, "MACRO", 5) == 0) {
            // 处理宏定义
        } else {
            tokenize(input, tokens, 2);
            if (strcmp(tokens[0], "MOV") == 0) {
                executeMOV(tokens[1], tokens[2]);
            } else if (strcmp(tokens[0], "ADD") == 0) {
                executeADD(tokens[1], tokens[2]);
            } else if (strncmp(input, "END", 3) == 0) {
                break;
            } else {
              printf("ERROR!\n");
              break;
            }
        }
    }

    return 0;
}
