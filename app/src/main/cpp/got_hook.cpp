//
// Created by Lenovo on 2025/2/26.
//
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include<stdint.h>
#include<string.h>
#include<inttypes.h>
#include<elf.h>
#include "logger.h"
#include "got_hook.h"

#define PAGE_SIZE 4096
#define PAGE_START(addr) (addr >> 12 << 12)
#define PAGE_END(addr) (PAGE_START + PAGE_SIZE)

void *my_malloc(size_t size){                        //my test function
    LOG_d("The malloc is hooked");
    return malloc(size);
}

uintptr_t get_offset(const char* filename){         //get rel offset
    Elf64_Ehdr eheader;
    Elf64_Phdr pheader;
    Elf64_Off dy_offset;
    Elf64_Xword dy_size;
    Elf64_Dyn Dyn;
    Elf64_Xword rel_offset;
    Elf64_Xword rel_size;
    Elf64_Rela rel_table;
    Elf64_Xword rel_info;
    Elf64_Xword str_offset;
    Elf64_Xword str_size;
    Elf64_Xword str_index = 0;
    uintptr_t ret_addr;
    const char* target_str = "malloc";

    FILE* fp = fopen(filename,"rb");
    if(fp == NULL){
        LOG_d("The %s open failed\n",filename);
        exit(0);
    }
    fread(&eheader,sizeof(eheader),1,fp);
    if(!eheader.e_phoff){
        LOG_d("The file cannt be load");
        exit(0);
    }
    //printf("%s\n",eheader.e_ident);

    for(int i = 0;i<eheader.e_phnum;i++){                  //get .dynamic offset and size
        fread(&pheader,sizeof(pheader),1,fp);
        if(pheader.p_type == PT_DYNAMIC){
            dy_offset = pheader.p_offset;
            dy_size = pheader.p_filesz;
            break;
        }
    }
    fseek(fp,dy_offset,SEEK_SET);
    for(int i = 0;i<dy_size/sizeof(Elf64_Dyn);i++){
        fread(&Dyn,sizeof(Dyn),1,fp);
        switch (Dyn.d_tag) {                         //get strtable reltable address and size
            case DT_JMPREL:
                rel_offset = Dyn.d_un.d_ptr;
                break;
            case DT_PLTRELSZ:
                rel_size = Dyn.d_un.d_val;
                break;
            case DT_STRTAB:
                str_offset = Dyn.d_un.d_ptr;
                break;
            case DT_STRSZ:
                str_size = Dyn.d_un.d_val;
                break;
            default:
                break;
        }
    }

    // //printf("here");
    fseek(fp,str_offset,SEEK_SET);
    char *data = (char*)malloc(str_size);
    fread(data,str_size,1,fp);
    for(int i = 0;i<str_size;i++){                                           //ananize the string table
        if(data[i] == '\0'){
            //printf("%s\n",&data[i+1]);
            if(strncmp(target_str,&data[i+1],strlen(target_str)) == 0){
                //printf("The index is %x\n",str_index);
                break;
            }
            str_index ++;
        }
        if(i == str_size-1){
            printf("Error ! can not find the target function");
            exit(0);
        }
    }

    fseek(fp,rel_offset,SEEK_SET);
    for(int i = 0;i<rel_size/sizeof(Elf64_Rela);i++){                       //get the tartget function offset
        fread(&rel_table,sizeof(Elf64_Rela),1,fp);
        rel_info = rel_table.r_info;
        Elf64_Xword index = rel_info >> 32;
        if(index == str_index){
            ret_addr = rel_table.r_offset;
            break;
        }
    }
    if(ret_addr){
        LOG_d("The offset founded : 0x%x",ret_addr);
        return ret_addr;
    }else{
        LOG_d("Error! Relocation failed");
        exit(0);
    }


}

extern "C" JNIEXPORT void JNICALL Java_com_example_got_1hook_MainActivity_hook(JNIEnv *env, jobject thiz,jstring file){
    const char* filename = env->GetStringUTFChars(file, nullptr);
    uintptr_t base_addr;
    uintptr_t addr;
    uintptr_t offset;
    FILE* fp = fopen("/proc/self/maps","r");
    char line[255];
    if(fp == NULL){
        LOG_d("open maps erro!");
        exit(0);
    }
    while (fgets(line, sizeof(line), fp)) {
        //printf("%s\n",line);
        if((strstr(line,filename) != NULL)&&sscanf(line,"%" PRIxPTR,&base_addr)==1){
            LOG_d("The libhello.so base : 0x%x\n",base_addr);
            break;
        }
    }
    offset = get_offset(filename);
    addr = base_addr + offset;
    mprotect((void*)PAGE_START(addr),PAGE_SIZE,PROT_READ|PROT_WRITE);
    *(void **)addr = (void *)my_malloc;                                                     //write my funcion address
}
