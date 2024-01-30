#include <pspsdk.h>
#include <pspkernel.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#define JR_RA 0x03E00008
#define LI_V0(n) ((0x24020000) | ((n) & 0xFFFF))

PSP_MODULE_INFO("noumd_prx", 0x1000, 1, 0);
PSP_HEAP_SIZE_MAX();

static void redirectFunction(int *a, void *f)
{
	*a = 0x08000000 | (((int)f >> 2) & 0x03FFFFFF);
	*(a + 1) = 0;
}

static void makeDummyFunctionReturnZero(int *a)
{
	*a = JR_RA;
	*(a + 1) = LI_V0(0);
}

static int sceUmdRegisterUMDCallBackPatched(int cbid)
{
	int k1 = pspSdkSetK1(0);
	int res = sceKernelNotifyCallback(cbid, 0x01);
	pspSdkSetK1(k1);
	return res;
}

static int sceGpioPortReadPatched(void)
{
	return *((int *)0xBE240004) & 0xFBFFFFFF;
}

static int main_thread(SceSize args, void *argp)
{
	if (!sceKernelFindModuleByName("sceNp9660_driver") && !sceKernelFindModuleByName("sceIsofs_driver") && !sceKernelFindModuleByName("PRO_Inferno_Driver"))
	{
		void *sceUmdUser = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0xAEE7404D);
		if (sceUmdUser)
			redirectFunction((int)sceUmdUser, sceUmdRegisterUMDCallBackPatched);

		void *CheckMedium = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0x46EBB729);
		if (CheckMedium)
			makeDummyFunctionReturnZero((int)CheckMedium);

		int (*IoDelDrv)(const char *) = sctrlHENFindFunction("sceIOFileManager", "IoFileMgrForKernel", 0xC7F35804);
		if (IoDelDrv)
			IoDelDrv("umd");
	}

	void *sceGpio_driver = sctrlHENFindFunction("sceLowIO_Driver", "sceGpio_driver", 0x4250D44A);
	if (sceGpio_driver)
		redirectFunction((int)sceGpio_driver, sceGpioPortReadPatched);
	return 0;
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("NoUMD", main_thread, 0x18, 0x1000, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, args, argp);
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}