	.set noreorder

#include "pspstub.s"

	STUB_START	"ThreadManForKernel",0x40010000,0x00060005
	STUB_FUNC	0xC11BA8C4,sceKernelNotifyCallback
	STUB_END