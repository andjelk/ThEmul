/*
 * Thontelix Emulator - /global.h
 */
#include<cctype>
typedef unsigned int reg;
struct registers
{
	union reg_32
	{
		reg re;
		struct b_reg_16
		{
			struct reg_16
			{
				unsigned char _l;
				unsigned char _h;
			}l,h;
		}r;
	}a, b, c, d;
	reg eip, epar, esar, etar, eflags;
};
extern const unsigned reg_count;
extern const unsigned v_reg_count;
extern const char* reg_name[];
extern const char *reg_f_nm[];
extern void *regs_ptr[];
#pragma once
union
{
	unsigned long long lv;
	char cv[8];
}_magic_sf_t;
#define magic_sf_t _magic_sf_t.lv
#define magic_sf st_f[0]
#define blkcount st_f[1]
#define blksize st_f[2]
//32-bit regs
#define EAX regs.a.re
#define EBX regs.b.re
#define ECX regs.c.re
#define EDX regs.d.re
#define EIP regs.eip
#define EPAR regs.epar
#define ESAR regs.esar
#define ETAR regs.etar
#define EFLAGS regs.eflags
//16-bit regs
//(LOW)
#define AXL regs.a.r.l
#define BXL regs.b.r.l
#define CXL regs.c.r.l
#define DXL regs.d.r.l
//(HIGH)
#define AXH regs.a.r.h
#define BXH regs.b.r.h
#define CXH regs.c.r.h
#define DXH regs.d.r.h
//8-bit regs
//(LOW PARENT, LOW)
#define ALL regs.a.r.l._l
#define BLL regs.b.r.l._l
#define CLL regs.c.r.l._l
#define DLL regs.d.r.l._l
//(LOW PARENT, HIGH)
#define AHL regs.a.r.l._h
#define BHL regs.b.r.l._h
#define CHL regs.c.r.l._h
#define DHL regs.d.r.l._h
//(HIGH PARENT, LOW)
#define ALH regs.a.r.h._l
#define BLH regs.b.r.h._l
#define CLH regs.c.r.h._l
#define DLH regs.d.r.h._l
//(HIGH PARENT, HIGH)
#define AHH regs.a.r.h._h
#define BHH regs.b.r.h._h
#define CHH regs.c.r.h._h
#define DHH regs.d.r.h._h
extern bool power;
extern unsigned long long st_f[3];
extern unsigned char *stbuf, *membuf;
extern char *dumpaddr, *staddr;
extern unsigned long long stbuflen;
extern unsigned long long memsize;
extern struct registers regs;
