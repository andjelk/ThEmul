/*
 * Thontelix Emulator - /mkst/mkstore.h
 */
#include<cstdio>
#pragma once
#define DEFAULT_STORAGE_BLOCK_SIZE 512
#define DEFAULT_STORAGE_BLOCK_COUNT 128
void set_mk_store_blksize(unsigned long long v);
void set_mk_store_blkcount(unsigned long long v);
FILE* run_mkstore();
extern unsigned long long mk_stp[3];
extern char* mkaddr;

