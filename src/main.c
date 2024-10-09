#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/display.h>
#include <psp2/ctrl.h>

#include "debugScreen.h"

#include "system.h"
#include "kernel.h"

#define VERSION "v0.0.1b"
#define printf(...) psvDebugScreenPrintf(__VA_ARGS__)

char* menuItems[] =  {
    "Dump full system information",
    "Quit VitaForge",
	"Credits"
};

int menuItemsLength = sizeof(menuItems) / sizeof(menuItems[0]);

void clearScreen() {
    psvDebugScreenClear(0xFF000000);
}

void print_menu(int selectedIndex) {
    printf("VitaForge %s\n\n", VERSION);

    for (int i = 0; i < menuItemsLength; i++) {  // Changed <= to <
        if (i == selectedIndex)
            printf(" > %s\n", menuItems[i]);
        else
            printf("   %s\n", menuItems[i]);
    }
}

int main(int argc, char* argv[]) {
    psvDebugScreenInit();

    SceCtrlData pad;
    int selectedIndex = 0;

    clearScreen();  // Clear the screen before printing the menu
    print_menu(selectedIndex);

    while (1) {
		reset:
        sceCtrlPeekBufferPositive(0, &pad, 1);

        if (pad.buttons == SCE_CTRL_UP) {
            if (selectedIndex > 0) selectedIndex--;  // Changed != 0 to > 0
            clearScreen();
            print_menu(selectedIndex);
        }

        if (pad.buttons == SCE_CTRL_DOWN) {
            if (selectedIndex < menuItemsLength - 1) selectedIndex++;  // Changed != 3 to < menuItemsLength - 1
            clearScreen();
            print_menu(selectedIndex);
        }

		if (pad.buttons == SCE_CTRL_CROSS) {
			if (strcmp(menuItems[selectedIndex], "Dump full system information") == 0) {
				clearScreen();

				printf("VitaForge %s\n\n", VERSION);

				printf("* Firmware version: %s\n", Kernel_GetFirmwareVersion(SCE_FALSE));
				printf("* System version: %s\n", Kernel_GetFirmwareVersion(SCE_TRUE));
				printf("* PS Vita unit: %s\n", Kernel_GetDeviceUnit());
				printf("* Motherboard: %s\n", Kernel_GetDeviceBoard());
				printf("* PS Vita CID: %s\n", Kernel_GetCID());
				printf("* PSID: %s\n", Kernel_GetPSID());

				while (1) {
					sceCtrlPeekBufferPositive(0, &pad, 1);
					if (pad.buttons == SCE_CTRL_CIRCLE) break;
					sceKernelDelayThread(100 * 1000);
				}

				clearScreen();
				print_menu(selectedIndex);
				goto reset;
			}
			if (strcmp(menuItems[selectedIndex], "Quit VitaForge") == 0) break;
			if (strcmp(menuItems[selectedIndex], "Credits") == 0) {
				clearScreen();

				printf("Credits\n\nDeveloper: $pectr4\nGithub: https://github.com/SpectrTech\n");

				while (1) {
					sceCtrlPeekBufferPositive(0, &pad, 1);
					if (pad.buttons == SCE_CTRL_CIRCLE) break;
					sceKernelDelayThread(100 * 1000);
				}

				clearScreen();
				print_menu(selectedIndex);
				goto reset;
			}
		}

        sceKernelDelayThread(100 * 1000);  // Adjusted to 100 ms for responsiveness
    }

    sceKernelExitProcess(0);
    return 0;
}
