/*
 * Thontelix Emulator - /machine.h
 */
#pragma once

struct mach_info
{
	unsigned short busLen;
	unsigned short architecture;
};
struct mach_info getMachineInfo(void);
