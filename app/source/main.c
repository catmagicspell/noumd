#include <pspsdk.h>
#include <pspkernel.h>
#include <systemctrl.h>

PSP_HEAP_SIZE_KB(-1);
PSP_MAIN_THREAD_ATTR(0);
PSP_MODULE_INFO("noumd_prx", 0x1000, 1, 1);

void redirectFunction(u32 addr, void (*func)())
{
	*(vu32 *)addr = 0x08000000 | (((u32)(func) >> 2) & 0x03FFFFFF);
	*((vu32 *)(addr + 4)) = 0x00000000;
}

void makeDummyFunction(u32 addr, int val)
{
	*(vu32 *)addr = 0x03E00008;
	if (val == 0)
		*((vu32 *)(addr + 4)) = 0x00001021;
	else if (val == 1)
		*((vu32 *)(addr + 4)) = 0x24020001;
	else
		*((vu32 *)(addr + 4)) = 0x00000000;
}

int sceGpioPortReadPatched(void)
{
	vu32 *port = (vu32 *)0xBE240004;
	return *port & 0xFBFFFFFF;
}

int sceUmdRegisterUMDCallBackPatched(int cbid)
{
	int k1 = pspSdkSetK1(0);
	int res = sceKernelNotifyCallback(cbid, 0x01);
	pspSdkSetK1(k1);
	return res;
}

void main_thread(SceSize args, void *argp)
{
	int umdPresent = sceKernelFindModuleByName("sceNp9660_driver") ||
					 sceKernelFindModuleByName("sceIsofs_driver") ||
					 sceKernelFindModuleByName("PRO_Inferno_Driver");
	if (!umdPresent)
	{
		void *sceUmdCheckMedium = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0x46EBB729);
		void *sceUmdRegisterUMDCallBack = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0xAEE7404D);
		if (sceUmdCheckMedium)
			makeDummyFunction(sceUmdCheckMedium, 0);
		if (sceUmdRegisterUMDCallBack)
			redirectFunction(sceUmdRegisterUMDCallBack, sceUmdRegisterUMDCallBackPatched);
	}
	void *sceGpioPortRead = sctrlHENFindFunction("sceLowIO_Driver", "sceGpio_driver", 0x4250D44A);
	if (sceGpioPortRead)
		redirectFunction(sceGpioPortRead, sceGpioPortReadPatched);
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("noumd_thread", main_thread, 0x18, 0x10000, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, args, argp);
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}