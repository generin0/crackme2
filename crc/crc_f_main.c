#include <stdio.h>
#include <windows.h>
#include <stdint.h>
#include <psapi.h>
#include "G:\PY\crackme v3\headers\h_func.h"
#include "G:\PY\crackme v3\crc\crc_f.h"

static pe_crc_info_t crc_info;
static CRITICAL_SECTION crc_critical_section;
static volatile int monitoring_active = 1;

typedef void (*xor_ptr)(const char*, char*);
typedef void (__stdcall* Sleep_ptr)(DWORD);
typedef int (__cdecl* printf_ptr)(const char*, ...);

xor_ptr sub3;
Sleep_ptr sleep;
printf_ptr print;

static void p_init(void) {
    sub3 = (xor_ptr)sub_401003;
    sleep = (Sleep_ptr)Sleep;
    print = (printf_ptr)printf;
}

static void generate_crc32_table(uint32_t* table) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
            }
            else {
                crc = crc >> 1;
            }
        }
        table[i] = crc;
    }
}

uint32_t calculate_pe_crc(uint8_t* base_address, size_t image_size) {
    static uint32_t table[256];
    static int table_generated = 0;

    if (!table_generated) {
        generate_crc32_table(table);
        table_generated = 1;
    }

    uint32_t crc = 0xFFFFFFFF;

    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)base_address;
    IMAGE_NT_HEADERS* nt_headers = (IMAGE_NT_HEADERS*)(base_address + dos_header->e_lfanew);

    IMAGE_SECTION_HEADER* section_header = IMAGE_FIRST_SECTION(nt_headers);

    for (int i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
        if (section_header[i].Characteristics & IMAGE_SCN_CNT_CODE) {
            uint8_t* section_start = base_address + section_header[i].VirtualAddress;
            size_t section_size = section_header[i].Misc.VirtualSize;

            for (size_t j = 0; j < section_size; j++) {
                uint8_t byte = section_start[j];
                uint8_t index = (crc ^ byte) & 0xFF;
                crc = (crc >> 8) ^ table[index];
            }
        }
    }

    return crc ^ 0xFFFFFFFF;
}

static int get_module_info(HMODULE module, uint8_t** base_address, size_t* image_size) {
    MODULEINFO module_info;

    if (!GetModuleInformation(GetCurrentProcess(), module, &module_info, sizeof(module_info))) {
        return 0;
    }

    *base_address = (uint8_t*)module_info.lpBaseOfDll;
    *image_size = module_info.SizeOfImage;
    return 1;
}

unsigned int __declspec(noinline) sub_10004(void) {
    InitializeCriticalSection(&crc_critical_section);

    crc_info.module_handle = GetModuleHandle(NULL);
    if (!crc_info.module_handle) {
        return 0;
    }

    if (!get_module_info(crc_info.module_handle, &crc_info.image_base, &crc_info.image_size)) {
        return 0;
    }

    crc_info.original_crc = calculate_pe_crc(crc_info.image_base, crc_info.image_size);

    HANDLE thread = CreateRemoteThread(
        GetCurrentProcess(),
        NULL,
        0,
        pe_crc_monitor_thread,
        NULL,
        0,
        NULL
    );
    
    if (!thread) {
        return 0;
    }

    CloseHandle(thread);
    return 1;
}

DWORD static WINAPI pe_crc_monitor_thread(LPVOID lpParam) {
    p_init();
    while (monitoring_active) {
        sleep(CHECK_INTERVAL_MS);

        EnterCriticalSection(&crc_critical_section);

        uint8_t* current_base;
        size_t current_size;
        if (get_module_info(crc_info.module_handle, &current_base, &current_size)) {
            if (current_base != crc_info.image_base || current_size != crc_info.image_size) {
                char output[] = "pWM\\^KPM@ OPVUXMPVW ]\\M\\ZM\\]";
                char temp_output[64];
                sub3(output, temp_output);
                print("%s.\n", temp_output);
                pe_crc_integrity_violation();
            }
            else {
                uint32_t current_crc = calculate_pe_crc(crc_info.image_base, crc_info.image_size);
                if (current_crc != crc_info.original_crc) {
                    char output1[] = "pWM\\^KPM@ OPVUXMPVW ]\\M\\ZM\\]";
                    char temp_output1[64];
                    sub3(output1, temp_output1);
                    print("%s!\n", temp_output1);
                    pe_crc_integrity_violation();
                }
            }
        }
        LeaveCriticalSection(&crc_critical_section);
    }
    return 0;
}

void pe_crc_integrity_violation(void) {
    char output2[] = "zUVJPW^ XIIUPZXMPVW ]L\\ MV PWM\\^KPM@ OPVUXMPVW";
    char temp_output2[64];
    sub3(output2, temp_output2);
    print("%s.\n", temp_output2);
    ExitProcess(0xDEADBEEF);
}

void sub_10005(void) {
    monitoring_active = 0;
    DeleteCriticalSection(&crc_critical_section);
}