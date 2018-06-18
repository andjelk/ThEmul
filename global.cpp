/*
 * Thontelix Emulator - /global.cpp
 */
#include"global.h"
unsigned long long st_f[3];
unsigned char *stbuf, *membuf;
char *dumpaddr, *staddr;
unsigned long long stbuflen;
unsigned long long memsize;
struct registers regs;
const unsigned reg_count=sizeof(regs)/sizeof(reg);
const char* reg_name[reg_count]=
{
	"EAX", "EBX", "ECX", "EDX",
	"EIP", "EPAR", "ESAR", "ETAR",
	"EFLAGS"
};
bool power;
void *regs_ptr[]=
{
	&EAX, &EBX, &ECX, &EDX,
	&EIP, &EPAR, &ESAR, &ETAR, &EFLAGS,
	&AXL, &BXL, &CXL, &DXL,
	&AXH, &BXH, &CXH, &DXH,
	&ALL, &BLL, &CLL, &DLL,
	&AHL, &BHL, &CHL, &DHL,
	&ALH, &BLH, &CLH, &DLH,
	&AHH, &BHH, &CHH, &DHH,
};
const unsigned v_reg_count=sizeof(regs_ptr)/sizeof(regs_ptr[0]);
const char *reg_f_nm[v_reg_count]=
{
	"AE   ", "BE   ", "CE   ", "DE   ",
	"I    ", "P    ", "S    ", "T    ", "F    ",
	"AD   ", "BD   ", "CD   ", "DD   ",
	"AD H ", "BD H ", "CD H ", "DD H ",
	"AS   ", "BS   ", "CS   ", "DS   ",
	"AS  H", "BS  H", "CS  H", "DS  H",
	"AS H ", "BS H ", "CS H ", "DS H ",
	"AS HH", "BS HH", "CS HH", "DS HH",
};
