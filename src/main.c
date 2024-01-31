#include <pspsdk.h>
#include <pspkernel.h>
#include <systemctrl.h>
#include <systemctrl_se.h>

PSP_HEAP_SIZE_MAX();
PSP_MODULE_INFO("noumd_prx", 0x1000, 1, 0);

static void redirectFunction(int *a, void *f)
{
	*a = 0x08000000 | (((int)f >> 2) & 0x03FFFFFF);
	*(a + 1) = 0x00000000;
}

static int sceGpioPortReadPatched(void)
{
	return *((int *)0xBE240004) & 0xFBFFFFFF;
}

static int sceUmdRegisterUMDCallBackPatched(int cbid)
{
	int k1 = pspSdkSetK1(0);
	int res = sceKernelNotifyCallback(cbid, 0x01);
	pspSdkSetK1(k1);
	return res;
}

static void umd_thread(SceSize args, void *argp)
{
	int umdPresent = sceKernelFindModuleByName("sceNp9660_driver") ||
					 sceKernelFindModuleByName("sceIsofs_driver") ||
					 sceKernelFindModuleByName("PRO_Inferno_Driver");
	if (!umdPresent)
	{
		void *sceUmdUser = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0xAEE7404D);
		if (sceUmdUser)
			redirectFunction(sceUmdUser, sceUmdRegisterUMDCallBackPatched);

		void *CheckMedium = sctrlHENFindFunction("sceUmd_driver", "sceUmdUser", 0x46EBB729);
		if (CheckMedium)
		{
			*(int *)CheckMedium = 0x03E00008;
			*((int *)CheckMedium + 1) = 0x24020000;
		}

		void (*IoDelDrv)(const char *) = sctrlHENFindFunction("sceIOFileManager", "IoFileMgrForKernel", 0xC7F35804);
		if (IoDelDrv)
			IoDelDrv("umd");
	}
	void *sceGpio_driver = sctrlHENFindFunction("sceLowIO_Driver", "sceGpio_driver", 0x4250D44A);
	if (sceGpio_driver)
		redirectFunction(sceGpio_driver, sceGpioPortReadPatched);
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("NoUMD", umd_thread, 0x18, 0x1000, 0, NULL);
	if (thid >= 0)
		sceKernelStartThread(thid, args, argp);
	return 0;
}

int module_stop(SceSize args, void *argp)
{
	return 0;
}
