/*
 * Thontelix Emulator - \storage.cpp
 */
/*
 * EPAR = Destination (offset from 0 in bytes)
 * ESAR = Source (in blocks)
 * ETAR = Count (in blocks)
 */
#include<cstring>
#include"global.h"
void read_sec()
{
	memcpy(
			reinterpret_cast<void*>(stbuf+ESAR*blksize),
			reinterpret_cast<void*>(membuf+EPAR),
			ETAR*blksize);
}
