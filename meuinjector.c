#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "meuheader.h"
#include <windows.h>

BOOL DllMain(HINSTANCE janela, DWORD flag, LPVOID mem_reservada){
    switch(flag){
        case DLL_PROCESS_ATTACH:

            FILE *meu_aquivo;

            meu_aquivo = fopen("C:\\Windows\\Temp\\past_tem.txt", "w");

            if (meu_aquivo == NULL){
                printf("houve um erro \n");
                
            }else{
                printf("criado \n");
                MessageBoxA(NULL, "DLL carregada!", "Injetado", MB_OK);
                break;
                fclose(meu_aquivo);
            }

        break;

        case DLL_THREAD_ATTACH:

        break;

        case DLL_THREAD_DETACH:

        break;

        case DLL_PROCESS_DETACH:

        break;

    }

    if (mem_reservada != NULL){
            return FALSE;
    }

    return TRUE;
}