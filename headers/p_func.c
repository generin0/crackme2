#include <windows.h>
#include <stdio.h>
#include <intrin.h>
#include <tlhelp32.h>

// LQQXZ66dfqqxzGKPRWfmo8afhmmtvCMR - hash of password

FARPROC MessageBoxA_addr = NULL;

typedef int (*strcmp_ptr)(const char*, const char*);
typedef void (*xor_ptr)(const char*, char*);

// XOR func
__forceinline void sub_401003(const char* input, char* output) {
    const unsigned char key = 0x39;
    size_t size = strlen(input);

    for (size_t i = 0; i < size; i++) {
        if (input[i] == ' ') {
            output[i] = ' ';
        }
        else {
            output[i] = input[i] ^ key;
        }
    }
    output[size] = '\0';
}

xor_ptr crypt_x = sub_401003;
strcmp_ptr cmp = strcmp;

// getting mBox addr to "remove" mBox from IAT
__declspec(noinline) void sub_401006() {
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    if (!hUser32) {
        hUser32 = LoadLibraryA("user32.dll");
    }
    if (hUser32) {
        MessageBoxA_addr = GetProcAddress(hUser32, "MessageBoxA");
    }
}

// clear func to preven memory leaks
void sub_401007(char* buffer, size_t size) {
    if (buffer) {
        for (size_t i = 0; i < size; i++) {
            buffer[i] = 0;
        }
    }
}

// trash instructions
void sub_401001() {
    char buffer[64] = {0};
    int counter = 0;
    
    for (int i = 0; i < 64; i++) {
        buffer[i] = (char)(i * 7 + 3);
        counter += buffer[i];
    }
    
    if (counter > 1000) {
        counter = counter % 100;
    } else {
        counter = counter * 2;
    }
    
    if (counter == 32) {
        counter ^= 0x10;
    } else {
        counter += 0x20;
    }

    while (counter > 256) {
        counter -= 256;
    }

    (void)counter;
}

// main verif logic
int sub_401002(const char* input_pass) {
    const char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char mixed[33] = {0};
    char hash[33] = {0};

    for (size_t i = 0; i < 32; i++) {
        mixed[i] = alphabet[(i * 3) % 62];
    }
    mixed[32] = '\0';

    for (size_t i = 0; i < 32; i++) {
        hash[i] = input_pass[i] + mixed[i];
    }

    char hex_hash[33] = { 0 };

    for (size_t i = 0; i < 32; i++) {
        unsigned char val = (hash[i] + i) & 0x3F;
        hex_hash[i] = alphabet[val];
    }
    hex_hash[32] = '\0';

    char temp[33] = "YDDMO66qsddmoR^EGBsxz8ts}xxacVXG";
    char decrypted_temp[33] = { 0 };

    for (int i = 0; i < strlen(temp); i++) {
        if (temp[i] == '8' || temp[i] == '6') {
            decrypted_temp[i] = temp[i];
        }
        else {
            decrypted_temp[i] = temp[i] ^ 0x15;
        }
    }
    
    if (cmp(hex_hash, decrypted_temp) == 0) {
        return 1;
    }

    return 0;
}

/*ANTI - DEBUG*/
// remove main debugger thread - ThreadHideFromDebugger
__declspec(noinline) BOOL sub_401008() {
    typedef enum _THREAD_INFORMATION_CLASS {
        ThreadHideFromDebugger = 0x11
    } THREAD_INFORMATION_CLASS;

    typedef NTSTATUS(NTAPI* ZW_SET_INFORMATION_THREAD)(
        IN HANDLE ThreadHandle,
        IN THREAD_INFORMATION_CLASS ThreadInformationClass,
        IN PVOID ThreadInformation,
        IN ULONG ThreadInformationLength
        );

    HMODULE hNtdll = LoadLibrary(L"ntdll.dll");
    if (!hNtdll) {
        return FALSE;
    }

    ZW_SET_INFORMATION_THREAD ZwSetInformationThread =
        (ZW_SET_INFORMATION_THREAD)GetProcAddress(hNtdll, "ZwSetInformationThread");

    if (!ZwSetInformationThread) {
        FreeLibrary(hNtdll);
        return FALSE;
    }

    ZwSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, 0);

    FreeLibrary(hNtdll);
    return TRUE;
}