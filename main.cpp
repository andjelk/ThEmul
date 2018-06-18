/*
 * Thontelix Emulator - /main.cpp
 */
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<malloc.h>
#include<cctype>
#include<getopt.h>
#include"global.h"
#include"storage.h"
#include"interpret.h"
#include"mkst/mkstore.h"
using namespace std;
int emulate();
char choice;
FILE* storage_file;
unsigned long long conv_num(char* str)
{
	unsigned long long ret;
	size_t len=strlen(str);
	char *c=optarg+len-1;
	if(isalpha(*c))
	{
		char vl=*c;
		*c=0;
		ret=atoi(str);
		switch(tolower(vl))
		{
		case 'g':ret*=1024;
		case 'm':ret*=1024;
		case 'k':ret*=1024;
		break;
		}
	} else { ret=atoi(optarg); }
	return ret;
}
struct option option_table[]=
{
		{"blksize", required_argument, 0, 0},
		{"blkcount",required_argument, 0, 0},
		{"storage", required_argument, 0, 's'},
		{"memsize", required_argument, 0, 'm'},
		{"mkstore", required_argument, 0, 'f'},
		{"memdump", optional_argument, 0, 'd'},
		{"help", no_argument, 0, 'h'},
		{"gtstore", required_argument, 0, 'g'},
		{NULL, NULL, NULL, NULL}
};
char usage_str[]="Usage: %s [s storage_file] [m memory_size] [d dump_file]\n";
char vstring[]="0.0.1";
FILE* usage_prompt_io=stderr;
bool mkstore=false;
int option_index;
int main(int argc, char *argv[])
{
	_magic_sf_t.cv[0]='T';
	_magic_sf_t.cv[1]='h';
	_magic_sf_t.cv[2]='E';
	_magic_sf_t.cv[3]='m';
	_magic_sf_t.cv[4]='u';
	_magic_sf_t.cv[5]='l';
	_magic_sf_t.cv[6]='S';
	_magic_sf_t.cv[7]='F';
	int opt;
	opt=getopt_long(argc, argv, "s:m:f:d::hg:y",option_table, &option_index);
	do
	{
		switch(opt)
		{
		case 's':
		{
			storage_file =
					fopen(
	staddr=strcpy(reinterpret_cast<char*>(malloc(strlen(optarg))),optarg), "rb+"
				);
			if(storage_file)
			{
				fread(st_f, sizeof(unsigned long long), 3, storage_file);
				if(magic_sf==magic_sf_t)stbuflen=blksize*blkcount;
				else
				{
					fprintf(stderr, "%s : Not a storage file (INVALID_MAGIC_NUMBER --> (content : %llx) != %llx)\n", optarg,magic_sf,magic_sf_t);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				fprintf(stderr, "Could not open file %s.\n", optarg);
			}
			break;
		}
		case 'm':
		{
			memsize=conv_num(optarg);
			break;
		}
		case 'd':
		{
			dumpaddr = strcpy(reinterpret_cast<char*>(malloc(strlen(optarg))), optarg);
			break;
		}
		case 'f':
		{ //mkstore
			mkaddr = strcpy(reinterpret_cast<char*>(malloc(strlen(optarg))), optarg);
			mkstore = true;
			break;
		}
		case 'g':
		{ //gtstore
			break;
		}
		case 0:
		{
			switch(option_index)
			{
			case 0:
				set_mk_store_blksize(conv_num(optarg));
			case 1:
				set_mk_store_blkcount(conv_num(optarg));
			}
			break;
		}
		case 'y':
		case 'n':
			choice=opt;
			break;
		case 'h':
			usage_prompt_io=stdout;
			printf("Thontelix Emulator v. %s\n", vstring);
		default:
			fprintf(usage_prompt_io, usage_str, argv[0]);
			exit((usage_prompt_io==stdout)?EXIT_SUCCESS:EXIT_FAILURE);
		}
	}while((opt=getopt_long(argc, argv, "s:m:f:d::hg:",option_table, &option_index)) != -1);
	FILE* ret_mk=(FILE*)0;
	if(mkstore)
	{
		ret_mk=run_mkstore();
		if(!ret_mk)
		{
			fprintf(stderr,"mkstore:Failed reading %s.", mkaddr);
		}
	}
	if(!stbuflen&&ret_mk)
	{
		printf("Use %s as storage (Y/N)? ", mkaddr);
		if(!choice)choice=getchar();
		do{
			switch(toupper(choice))
			{
			case 'Y':
				blkcount=mk_stp[1];
				blksize=mk_stp[2];
				stbuflen=blkcount*blksize;
				storage_file=fopen(mkaddr, "rb+");
				staddr=mkaddr;
				puts("");
			case 'N':
				puts("");
				goto escp;
			default:
				fprintf(stderr, "Invalid input '%c'. Use 'Y' or 'N' (case-insensetive).", choice);
			}
		}while((choice=getchar()));
	}
	escp:
	if(!(memsize&&stbuflen))
	{
		fprintf(stderr, usage_str, argv[0]);
		exit(EXIT_FAILURE);
	}
	printf("Storage file : %s (%llu %llu-byte blocks)\n", staddr, blkcount, blksize);
	stbuf=reinterpret_cast<unsigned char*>(malloc(stbuflen));
	fread(stbuf, stbuflen, 1,storage_file);
	puts("Emulated ramdisk of storage file initialization done.\n");
	membuf=reinterpret_cast<unsigned char*>(calloc(1,memsize));
	printf("Allocated %llu bytes for emulated dynamic memory\n", memsize);
	exit(emulate());
}
int emulate()
{
	power=true;
	EPAR=0;
	ESAR=0;
	ETAR=1;
	read_sec();
	EIP=0;
	while(power)
	{
		interpret_at_ip();
	}
	puts("Emulation ended. Registers after execution:\n");
	for(unsigned i=0;i<reg_count;i++)
	{
		printf("%s : 0x%#08x (%u)\n", reg_name[i],
				(reinterpret_cast<reg*>(&regs))[i],
				(reinterpret_cast<reg*>(&regs))[i]);
	}
	fclose(storage_file);
	//fclose(dump_file);
	free(membuf);
	free(stbuf);
	return EXIT_SUCCESS;
}
