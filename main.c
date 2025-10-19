#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "headers\h_func.h"
#include "G:\PY\crackme v3\crc\crc_f.h"

// correct password DEADBEAFDEADBEEFDEADBEEFDEADBEEF

int main(void) {
    // time start for GetTickCount64
    DWORD start_time, end_time;
    start_time = GetTickCount64();

    typedef unsigned int (*pe_crc_init_ptr)(void);
    typedef void (*pe_crc_stop_ptr)(void);
    typedef void (*xor_ptr)(const char*, char*);
    typedef void (*GetMessageBoxAddr_ptr)(void);
    typedef void (*secure_clean_ptr)(char*, size_t);
    typedef int (*printf_ptr)(const char*, ...);
    typedef char* (*fgets_ptr)(char*, int, FILE*);
    typedef void (*Sleep_ptr)(DWORD);
    typedef int (*sub_401002_ptr)(const char*);
    typedef void (*t_ptr)(void);
    typedef BOOL (WINAPI* CheckDebug_Ptr)(void);
    typedef int (WINAPI* MessageBoxA_ptr)(HWND, LPCSTR, LPCSTR, UINT);

    funcs.init_ptr = sub_10004;
    funcs.stop_ptr = sub_10005;
    funcs.xor_ptr = sub_401003;
    funcs.Addr_ptr = sub_401006;
    funcs.clean_ptr = sub_401007;
    funcs.printf_ptr = printf;
    funcs.fgets_ptr = fgets;
    funcs.Sleep_ptr = Sleep;
    funcs.verif_ptr = sub_401002;
    funcs.debug_ptr = sub_401008;
    funcs.t_ptr = sub_401001;

    ENCODED_STRINGS encoded = {
    "XPU\\] MV PWPM zkz",           // "Failed to init CRC"
    "r\\@",                         // "Key" 
    "~K\\XM SV[",                   // "Great job!"
    "N\\UZVT\\ MV MQ\\ IKV^KXT",    // "Welcome to the program!"
    "xIIUPZXMPVW _PWPJQ\\]",        // "Application finished."
    "mK@ [\\MM\\K",                 // "Try better!"
    "PWZVKK\\ZM IXJJNVK]",          // "incorrect password!"
    "|KKVK K\\X]PW^ PWILM"          // Unknown
    };

    funcs.debug_ptr();

    while (1) {
        // trash call
        funcs.t_ptr();

        if (funcs.init_ptr() != 1) {
            char temp_output1[32];
            funcs.xor_ptr(encoded.output1, temp_output1);
            funcs.printf_ptr("%s\n", temp_output1);
            funcs.clean_ptr(temp_output1, sizeof(temp_output1));
            return 1;
        }

        //time end for anti-debugging
        end_time = GetTickCount64();

        // trash call
        funcs.t_ptr();

        if (end_time - start_time > 100) {
            return 1;
        }

        char temp_output2[32];
        funcs.xor_ptr(encoded.output2, temp_output2);
        funcs.printf_ptr("%s: ", temp_output2);
        funcs.clean_ptr(temp_output2, sizeof(temp_output2));

        user_data_t user_data = {0};

        if (!funcs.fgets_ptr(user_data.input_pass, sizeof(user_data.input_pass), stdin)) {
            char temp_output8[32];
            funcs.xor_ptr(encoded.output8, temp_output8);
            funcs.printf_ptr("%s\n", temp_output8);
            funcs.clean_ptr(user_data.input_pass, sizeof(user_data.input_pass));
        }

        // trash call
        funcs.t_ptr();

        // getting the mBox address
        funcs.Addr_ptr();
        MessageBoxA_ptr mBox = (MessageBoxA_ptr)MessageBoxA_addr;

        // input validation (length must be 32 chars)
        size_t len = strlen(user_data.input_pass);

        if (len > 0 && user_data.input_pass[len - 1] == '\n') {
            user_data.input_pass[len - 1] = '\0';
            len--;
        }

        if (len != 32) {
            char temp_output6[32];
            char temp_output7[32];
            funcs.xor_ptr(encoded.output7, temp_output7);
            funcs.xor_ptr(encoded.output6, temp_output6);
            mBox(NULL, temp_output7, temp_output6, MB_OK | MB_ICONERROR);

            funcs.clean_ptr(temp_output6, sizeof(temp_output6));
            funcs.clean_ptr(temp_output7, sizeof(temp_output7));
            return 1;
        }

        // trash call
        funcs.t_ptr();

        int match = 0;

        // main verif logic
        if (funcs.verif_ptr(user_data.input_pass) == 1) {
            match = 1;
        }
        else {
            match = 0;
        }

        // trash
        funcs.t_ptr();

        if (match == 1) {
            char temp_output3[32];
            funcs.xor_ptr(encoded.output3, temp_output3);           // Encoded "Great job"
            char temp_output4[32];
            funcs.xor_ptr(encoded.output4, temp_output4);           // Encoded "welcome to the program!"
            mBox(NULL, temp_output3, temp_output4, MB_OK | MB_ICONINFORMATION);

            funcs.clean_ptr(user_data.input_pass, sizeof(user_data.input_pass));
            funcs.clean_ptr(temp_output3, sizeof(temp_output3));
            funcs.clean_ptr(temp_output4, sizeof(temp_output4));

            char temp_output5[32];
            funcs.xor_ptr(encoded.output5, temp_output5);
            funcs.printf_ptr("%s.\n", temp_output5);                // Encoded "Application finished."

            funcs.clean_ptr(temp_output5, sizeof(temp_output5));
            funcs.Sleep_ptr(1000);
            funcs.stop_ptr();
            return 0;
        }
        else {
            char temp_output6[32];
            funcs.xor_ptr(encoded.output6, temp_output6);
            char temp_output7[32];
            funcs.xor_ptr(encoded.output7, temp_output7);
            mBox(NULL, temp_output7, temp_output7, MB_OK | MB_ICONERROR);

            funcs.clean_ptr(user_data.input_pass, sizeof(user_data.input_pass));
            funcs.clean_ptr(temp_output6, sizeof(temp_output6));
            funcs.clean_ptr(temp_output7, sizeof(temp_output7));
            return 1;
        }
    }
    funcs.Sleep_ptr(5000);
    return 0;
}