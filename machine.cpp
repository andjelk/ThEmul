/*
 * Thontelix Emulator - /machine.cpp
 */
#include"machine.h"
mach_info execMachine={32, 0x0FFF};
struct mach_info getMachineInfo(void)
{
	return execMachine;
}
