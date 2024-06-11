	.set noreorder
#include "pspstub.s"
	STUB_START "ThreadManForUser",0x40090000,0x00010005
	STUB_FUNC  0xC11BA8C4,sceKernelNotifyCallback
	STUB_END