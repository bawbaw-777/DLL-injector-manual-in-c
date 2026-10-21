#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meuheader.h"
#include <windows.h>
#include <tlhelp32.h>

DWORD processo_thread(LPVOID long_ptr_param){
    LPTHREAD_START_ROUTINE ponteiro_thread = (LPTHREAD_START_ROUTINE)long_ptr_param;

    printf("acho que comecamos aqui \n");

    return 0;
}

MEUMACRO int main(void){
    DWORD dados_32b = PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ;
    WINBOOL windows_bool = FALSE;
    DWORD flags = TH32CS_SNAPPROCESS;
    char path[] = "C:\\dll_injection_manual\\output\\meuinjector.dll";
    DWORD pid = 0;
    PROCESSENTRY32W struct_proc;
    DWORD flag_alloc = MEM_COMMIT | MEM_RESERVE;
    DWORD protecao = PAGE_READWRITE;
    LPCVOID diretorio = path;
    LPCSTR  ptr_str = "meuinjector.dll";

    void *inicializando = NULL;
    inicializando = memset(&struct_proc, 0, sizeof(PROCESSENTRY32W));
    struct_proc.dwSize = sizeof(PROCESSENTRY32W);

        HANDLE snap_shot = CreateToolhelp32Snapshot(flags, pid);

        if (snap_shot == INVALID_HANDLE_VALUE){
            printf("Erro com o createtoolhelp: %lu \n", GetLastError());
        }else{

            BOOL first_proc = Process32FirstW(snap_shot, &struct_proc);

            if (first_proc == FALSE){
                printf("Nao foi para o buffer %lu \n", GetLastError());

                return 0;
            }else{
                printf("Funcao comeca aqui \n");
                for (DWORD init_pid = pid; init_pid>=struct_proc.th32ProcessID; init_pid++){
                    WINBOOL proximo = Process32NextW(snap_shot, &struct_proc); // a função deve estar dentro do loop
                    
                    if (proximo == TRUE){
                        init_pid = struct_proc.th32ProcessID;

                        printf("Atual PID: %lu -- nome dele: %ls \n", struct_proc.th32ProcessID, struct_proc.szExeFile);
                        
                    }else{
                        printf("Acabou os PIDs %lu ultimo nome: %ls \n ", struct_proc.th32ProcessID, struct_proc.szExeFile);
                        break;
                        
                    }
                }
                HANDLE abertura_proc = OpenProcess(dados_32b, windows_bool, struct_proc.th32ProcessID);

                if (abertura_proc == NULL){
                    printf("Algum erro com a abertura %lu \n", GetLastError());
                }else{
                    printf("Abertura feita \n");

                    LPVOID alocando_mem = VirtualAllocEx(abertura_proc, NULL, sizeof(path), flag_alloc, protecao);
                    
                    if(alocando_mem == NULL){
                        printf("deu erro %lu \n", GetLastError());
                    }else{
                        printf("Ta bom \n");

                        WINBOOL caminho_mem = WriteProcessMemory(abertura_proc, alocando_mem, diretorio, sizeof(path), NULL);
                        
                        if (caminho_mem == 0){
                            printf("Error, veja qual foi: %lu \n", GetLastError());
                        }else{
                            printf("Foi hein \n");

                            DWORD flags_thread = STACK_SIZE_PARAM_IS_A_RESERVATION;
                            LPDWORD long_ptr_dword = NULL;
                            
                            HANDLE criarthread = CreateRemoteThreadEx(abertura_proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, alocando_mem, flags_thread, NULL, NULL);

                            if (criarthread == NULL){
                                printf("Houve um erro com o createremotethread %lu \n", GetLastError());
                            }else{
                                printf("passado\n");
                                
                                DWORD curupira = WaitForSingleObject(criarthread, 50000);

                                if (curupira == WAIT_FAILED){
                                    printf("Deu rui %lu \n", GetLastError());
                                }else{
                                    printf("Suave \n");
                                }

                                
                            }
                            
                        }
                    }
                }

            }

        }

    return 0;
}