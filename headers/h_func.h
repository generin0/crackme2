#ifndef F_CALLS_H
#define F_CALLS_H

#include <windows.h>

extern FARPROC MessageBoxA_addr;

typedef struct {
    char input_pass[64];
} user_data_t;

typedef struct {
    unsigned int (*init_ptr)(void);
    void (*stop_ptr)(void);
    void (*xor_ptr)(const char*, char*);
    void (*Addr_ptr)(void);
    void (*clean_ptr)(char*, size_t);
    int (*printf_ptr)(const char*, ...);
    char* (*fgets_ptr)(char*, int, FILE*);
    void (*Sleep_ptr)(DWORD);
    void (*t_ptr)(void);
    BOOL (*debug_ptr)(void);
    int (*verif_ptr)(const char*);
} FUNCTION_TABLE;
FUNCTION_TABLE funcs;

typedef struct {
    char output1[32];
    char output2[32];
    char output3[32];
    char output4[32];
    char output5[32];
    char output6[32];
    char output7[32];
    char output8[32];
} ENCODED_STRINGS;

void sub_401003(const char* input, char* output);
void sub_401006(void);
void sub_401007(char* buffer, size_t size);
void sub_401001(void);
int sub_401002(const char* crypted_pass);
BOOL sub_401008(void);

#endif // F_CALLS_H