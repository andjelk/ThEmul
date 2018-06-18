/*
 * Thontelix Emulator - /mkst/mkstore.cpp
 */
#include"mkstore.h"
#include"./../global.h"
#include<cstdio>
#include<cstdlib>
unsigned long long mk_stp[3];
char* mkaddr;
void set_mk_store_blksize(unsigned long long v)
{
	mk_stp[2]=v;
}
void set_mk_store_blkcount(unsigned long long v)
{
	mk_stp[1]=v;
}
FILE* run_mkstore()
{
	if(!mk_stp[1])
	{
		mk_stp[1]=DEFAULT_STORAGE_BLOCK_COUNT;
		printf("mkstore:blkcount not specified; set to %d.\n",DEFAULT_STORAGE_BLOCK_COUNT);
	}
	if(!mk_stp[2])
	{
		mk_stp[2]=DEFAULT_STORAGE_BLOCK_SIZE;
		printf("mkstore:blksize not specified; set to %d.\n",DEFAULT_STORAGE_BLOCK_SIZE);
	}
	FILE* store_mk=fopen(mkaddr, "wb");
	if(store_mk)
	{
		mk_stp[0]=magic_sf_t;
		fwrite(mk_stp,sizeof(unsigned long long),3,store_mk);
		void *dn = calloc(sizeof(unsigned char), 512);
		for(unsigned i=0;i<mk_stp[1];i++)fwrite(dn, 1, mk_stp[2], store_mk);
		free(dn);
	}
	return store_mk;
}
