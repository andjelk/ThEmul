/*
 * Thontelix Emulator - /interpret.cpp
 */
#include"interpret.h"
#include"global.h"
#include"machine.h"
#include<cstring>
#include<cstdio>
#define nullptr 0
struct operand_opcode
{
	enum operand_type
	{
		Addr=1,
		Val=2
	}dst_t, src_t;
	bool destHasMnem;
	bool srcHasMnem;
	enum operand_len
	{
		Op32bit=4,
		Op16bit=2,
		Op8bit=1,
		Op64bit=8
	}sz;
#define IsTypeMnem(v) (v>2)
#define HasTypeMnem(v) IsTypeMnem(v)
	unsigned char reg_d;
	unsigned char reg_s;
	unsigned long long MnemDst, MnemSrc;
}_opc;
#define O_D_MNEM 0b010000000000
#define O_S_MNEM 0b100000000000
#define O_SZ_8BIT 0b0000
#define O_SZ_16BIT 0b0100
#define O_SZ_32BIT 0b1000
#define O_SZ_64BIT 0b1100
#define O_SZ 0b1100
#define O_PRESENT 32768 //Last bit
#define O_D_REG 1
#define O_S_REG 2
#define O_S_NULL_REG 16384
#define O_D_N_REGPTR 4096
#define O_S_N_REGPTR 8192
#define O_DST_A_REG 0
#define O_DST_B_REG 1
#define O_DST_C_REG 2
#define O_DST_D_REG 3
#define O_DST_EIP 4
#define O_DST_PAR 8
#define O_DST_SAR 12
#define O_DST_TAR 9
#define O_DST_F_REG 10
#define O_SRC_A_REG 0
#define O_SRC_B_REG 1
#define O_SRC_C_REG 2
#define O_SRC_D_REG 3
#define O_SRC_EIP 4
#define O_SRC_PAR 8
#define O_SRC_SAR 12
#define O_SRC_TAR 9
#define O_SRC_F_REG 10
#define O_DST_64I 16
#define O_DST_32I 32
#define O_DST_16I 64
#define O_SRC_64I 128
#define O_SRC_32I 256
#define O_SRC_16I 512
int err_c;
int find_reg(char *st)
{
	int i=0;
	for(;i<v_reg_count;i++)
	{
		if(!strcmp(reg_f_nm[i], st))break;
	}
	return i;
}
operand_opcode *parse_op_oc(unsigned int *op)
{
	if(!((*(op))&O_PRESENT))
	{
		err_c=1;
		return nullptr;
	}
	_opc.MnemDst=0;
	_opc.MnemSrc=0;
	if((*(op))&O_D_REG)
	{
		_opc.dst_t=operand_opcode::Val;
	}
	else
	{
		_opc.dst_t=operand_opcode::Addr;
		if((*(op))&O_D_N_REGPTR)
			_opc.destHasMnem=true;
		else
			_opc.destHasMnem=false;
	}
	if((*(op))&O_S_REG)
	{
		_opc.src_t=operand_opcode::Val;
		if((*(op))&O_S_NULL_REG)
			_opc.srcHasMnem=true;
		else
			_opc.srcHasMnem=false;
	}
	else
	{
		_opc.src_t=operand_opcode::Addr;
		if((*(op))&O_S_N_REGPTR)
			_opc.srcHasMnem=true;
		else
			_opc.srcHasMnem=false;
	}
	switch((*(op))&O_SZ)
	{
	case O_SZ_8BIT:
		_opc.sz=operand_opcode::Op8bit;break;
	case O_SZ_16BIT:
		_opc.sz=operand_opcode::Op16bit;break;
	case O_SZ_32BIT:
		_opc.sz=operand_opcode::Op32bit;break;
	case O_SZ_64BIT:
		if(getMachineInfo().busLen<64)
		{
			//puts("[Error] : 64-bit registers not supported in this mode!\n");
			err_c=2;
			return nullptr;
		}
		_opc.sz=operand_opcode::Op64bit;break;
	}
	EIP+=2;
	bool dCond=!IsTypeMnem(_opc.dst_t);
	bool sCond=!IsTypeMnem(_opc.src_t);
	if(dCond||sCond)
	{
		char f_str_d[6]={' '};
		char f_str_s[6]={' '};
		EIP++;
		unsigned char types = *(reinterpret_cast<unsigned char*>(op)+2);
		if(dCond)
		{
			bool r_t_d=true;
			switch(types&0x0F)//Destination
			{
				case O_DST_A_REG:f_str_d[0]='A';break;
				case O_DST_B_REG:f_str_d[0]='B';break;
				case O_DST_C_REG:f_str_d[0]='C';break;
				case O_DST_D_REG:f_str_d[0]='D';break;
				case O_DST_EIP:f_str_d[0]='I';r_t_d=false;break;
				case O_DST_PAR:f_str_d[0]='P';r_t_d=false;break;
				case O_DST_SAR:f_str_d[0]='S';r_t_d=false;break;
				case O_DST_TAR:f_str_d[0]='T';r_t_d=false;break;
				case O_DST_F_REG:f_str_d[0]='F';r_t_d=false;break;
			}
			if(r_t_d)
			{
				switch(_opc.sz)
				{
					case operand_opcode::Op8bit:
					{
						f_str_d[1]='S';break;
					}
					case operand_opcode::Op16bit:
					{
						f_str_d[1]='D';break;
					}
					case operand_opcode::Op32bit:
					{
						f_str_d[1]='E';break;
					}
					case operand_opcode::Op64bit:
					{
						puts("[Error] : 64-bit registers unimplemented.");
						return nullptr;
					}
				}
				if((*(op))&O_DST_64I)f_str_d[2]='H';
				if((*(op))&O_DST_32I)f_str_d[3]='H';
				if((*(op))&O_DST_16I)f_str_d[4]='H';
			}
			else
			{
				_opc.sz=operand_opcode::Op32bit;
			}
		}
		if(sCond)
		{
			bool r_t_s=true;
			switch(types&0xF0)//Source
			{
				case O_SRC_A_REG:f_str_s[0]='A';break;
				case O_SRC_B_REG:f_str_s[0]='B';break;
				case O_SRC_C_REG:f_str_s[0]='C';break;
				case O_SRC_D_REG:f_str_s[0]='D';break;
				case O_SRC_EIP:f_str_s[0]='I';r_t_s=false;break;
				case O_SRC_PAR:f_str_s[0]='P';r_t_s=false;break;
				case O_SRC_SAR:f_str_s[0]='S';r_t_s=false;break;
				case O_SRC_TAR:f_str_s[0]='T';r_t_s=false;break;
				case O_SRC_F_REG:f_str_s[0]='F';r_t_s=false;break;
			}
			if(r_t_s)
			{
				switch(_opc.sz)
				{
					case operand_opcode::Op8bit:
					{
						f_str_s[1]='S';break;
					}
					case operand_opcode::Op16bit:
					{
						f_str_s[1]='D';break;
					}
					case operand_opcode::Op32bit:
					{
						f_str_s[1]='E';break;
					}
					case operand_opcode::Op64bit:
					{
						puts("[Error] : Unimplemented.");
						return nullptr;
					}
				}
				if((*(op))&O_SRC_64I)f_str_s[2]='H';
				if((*(op))&O_SRC_32I)f_str_s[3]='H';
				if((*(op))&O_SRC_16I)f_str_s[4]='H';
			}
			else
			{
				_opc.sz=operand_opcode::Op32bit;
			}
		}
		bool sStr=true;
		if(dCond)
		{
			if((_opc.reg_d=find_reg(f_str_d))==v_reg_count)
			{
				//puts("[Error] : Undefined destination register.");
				err_c=3;
				return nullptr;
			}
			else sStr=strcmp(f_str_d, f_str_s);
		}
		if(sCond)
		{
			if(sStr==false)_opc.reg_s=_opc.reg_d;
			else if((_opc.reg_s=find_reg(f_str_d))==v_reg_count)
			{
				//puts("[Error] : Undefined source register.");
				err_c=4;
				return nullptr;
			}
		}
	}
	unsigned char offset=0;
	if(!dCond)
	{
		//Destination is a mnemonic
		switch(_opc.sz)
		{
		case operand_opcode::Op8bit:
		{
			_opc.MnemDst=*(reinterpret_cast<unsigned char*>(reinterpret_cast<char*>((op)+3+offset)));
			offset=sizeof(unsigned char);
			break;
		}
		case operand_opcode::Op16bit:
		{
			_opc.MnemDst=*(reinterpret_cast<unsigned short*>(reinterpret_cast<char*>((op)+3+offset)));
			offset=sizeof(unsigned short);
			break;
		}
		case operand_opcode::Op32bit:
		{
			_opc.MnemDst=*(reinterpret_cast<unsigned int*>(reinterpret_cast<char*>((op)+3+offset)));
			offset=sizeof(unsigned int);
			break;
		}
		case operand_opcode::Op64bit:
		{
			_opc.MnemDst=*(reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>((op)+3+offset)));
			offset=sizeof(unsigned long long);
			break;
		}
		}
	}
	if(!sCond)
	{
		//Source is a mnemonic
		switch(_opc.sz)
		{
		case operand_opcode::Op8bit:
		{
			_opc.MnemSrc=*(reinterpret_cast<unsigned char*>(reinterpret_cast<char*>((op)+3+offset)));
			break;
		}
		case operand_opcode::Op16bit:
		{
			_opc.MnemSrc=*(reinterpret_cast<unsigned short*>(reinterpret_cast<char*>((op)+3+offset)));
			break;
		}
		case operand_opcode::Op32bit:
		{
			_opc.MnemSrc=*(reinterpret_cast<unsigned int*>(reinterpret_cast<char*>((op)+3+offset)));
			break;
		}
		case operand_opcode::Op64bit:
		{
			_opc.MnemSrc=*(reinterpret_cast<unsigned long long*>(reinterpret_cast<char*>((op)+3+offset)));
			break;
		}
		}
	}
	return &_opc;
}
struct type
{
	unsigned size;
	unsigned long long val;
	void type::setType(void *ptr)
	{
		switch(size)
		{
		case sizeof(unsigned char):
			*(reinterpret_cast<unsigned char *>(ptr))=val;break;
		case sizeof(unsigned short):
			*(reinterpret_cast<unsigned short *>(ptr))=val;break;
		case sizeof(unsigned int):
			*(reinterpret_cast<unsigned int *>(ptr))=val;break;
		case sizeof(unsigned long long):
			*(reinterpret_cast<unsigned long long *>(ptr))=val;break;
		default:
			fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
		}
	}
};
void halt(void *, type)
{
	power=false;
	return;
}
void mov(void * dst, type src)
{
	src.setType(dst);
}
void __and(void * dst, type src)
{
	switch(src.size)
	{
	case sizeof(unsigned char):
		*(reinterpret_cast<unsigned char *>(dst))&=src.val;break;
	case sizeof(unsigned short):
		*(reinterpret_cast<unsigned short *>(dst))&=src.val;break;
	case sizeof(unsigned int):
		*(reinterpret_cast<unsigned int *>(dst))&=src.val;break;
	case sizeof(unsigned long long):
		*(reinterpret_cast<unsigned long long *>(dst))&=src.val;break;
	default:
		fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
	}
}
void __xor(void * dst, type src)
{
	switch(src.size)
	{
	case sizeof(unsigned char):
		*(reinterpret_cast<unsigned char *>(dst))^=src.val;break;
	case sizeof(unsigned short):
		*(reinterpret_cast<unsigned short *>(dst))^=src.val;break;
	case sizeof(unsigned int):
		*(reinterpret_cast<unsigned int *>(dst))^=src.val;break;
	case sizeof(unsigned long long):
		*(reinterpret_cast<unsigned long long *>(dst))^=src.val;break;
	default:
		fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
	}
}
void __or(void * dst, type src)
{
	switch(src.size)
	{
	case sizeof(unsigned char):
		*(reinterpret_cast<unsigned char *>(dst))|=src.val;break;
	case sizeof(unsigned short):
		*(reinterpret_cast<unsigned short *>(dst))|=src.val;break;
	case sizeof(unsigned int):
		*(reinterpret_cast<unsigned int *>(dst))|=src.val;break;
	case sizeof(unsigned long long):
		*(reinterpret_cast<unsigned long long *>(dst))|=src.val;break;
	default:
		fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
	}
}
void __not(void * dst, type src)
{
	//TODO
	printf("NOT instruction unimplemented. Skipping...");
}
void __add(void * dst, type src)
{
	switch(src.size)
	{
	case sizeof(unsigned char):
		*(reinterpret_cast<unsigned char *>(dst))+=src.val;break;
	case sizeof(unsigned short):
		*(reinterpret_cast<unsigned short *>(dst))+=src.val;break;
	case sizeof(unsigned int):
		*(reinterpret_cast<unsigned int *>(dst))+=src.val;break;
	case sizeof(unsigned long long):
		*(reinterpret_cast<unsigned long long *>(dst))+=src.val;break;
	default:
		fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
	}
}
void __sub(void * dst, type src)
{
	switch(src.size)
	{
	case sizeof(unsigned char):
		*(reinterpret_cast<unsigned char *>(dst))-=src.val;break;
	case sizeof(unsigned short):
		*(reinterpret_cast<unsigned short *>(dst))-=src.val;break;
	case sizeof(unsigned int):
		*(reinterpret_cast<unsigned int *>(dst))-=src.val;break;
	case sizeof(unsigned long long):
		*(reinterpret_cast<unsigned long long *>(dst))-=src.val;break;
	default:
		fprintf(stderr, "type::setType(void*); >>> Invalid type size!");
	}
}
void (*instructions[])(void * dst, type src)=
{
	halt, mov, __and, __xor, __or, __not, __add, __sub
};
const unsigned int inst_impl_c=sizeof(instructions)/sizeof(instructions[0]);
bool inst_has_operands[inst_impl_c]=
{
	false, true, true, true, true, true, true, true
};
void interpret_at_ip()
{
	unsigned char Instruc=*membuf+EIP;
	EIP++;
	unsigned char extInstruc=*membuf+EIP;
	EIP++;
	if(inst_impl_c<=Instruc)
	{
		printf("[Error] : Undefined/Unimplemented instruction (OPCODE : %#08x)");
		power=false;
		return;
	}
	if(inst_has_operands[Instruc])
	{
		operand_opcode *instop = parse_op_oc((unsigned int*)(membuf+EIP));
		if(err_c)
		{
			printf("[Error] : Invalid opcode (ERR : %d)", err_c);
			power=false;
			return;
		}
		if(instop->dst_t==operand_opcode::Addr&&instop->src_t==operand_opcode::Addr)
		{
			printf("[Error] : Invalid opcode (Both source and destination are pointers)");
		}
		static type typ;
		void *ptr=instop->MnemDst+regs_ptr[instop->reg_d];
		typ.size=instop->sz;
		typ.val=instop->MnemSrc+regs_ptr[instop->reg_s];
		instructions[Instruc](typ);
	}
	else instructions[Instruc](0, 0);

}
