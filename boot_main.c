#include "platform.h"
#include "device_board.h"
#include "can_boot_transport.h"
#include "user_def.h"
#include "sw_crc32.h"
#include "device_app_vectors.h"

unsigned char CAN_BootInstalledValid(void)
{
    unsigned long size;
    if (ImageMetadataValidate((const unsigned long __far *)APP_MANIFEST_ADDRESS) != IMAGE_METADATA_OK) return 0U;
    size = *(const unsigned long __far *)(APP_MANIFEST_ADDRESS + IMAGE_MANIFEST_SIZE_OFFSET);
    if (caculate_crc32_checksum(APP_FLASH_START, size-4UL) !=
        *(const unsigned long __far *)(APP_FLASH_START+size-4UL)) return 0U;
    return Device_AppVectorsValid((const unsigned long __far *)APP_FLASH_START,
        APP_FLASH_START+0x200UL, APP_FLASH_START+size-4UL);
}
void main(void)
{
    unsigned long request;
    unsigned char valid, flash_status, timed;
    signed long result;
    request = *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS+12UL);
    *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS+12UL) = 0UL;
    if (!Device_BoardInit()) for (;;) { BSP_NOP(); }
#if CAN_BOOT_DEBUG_UART
    Device_Log("F24 CAN FD DEVICE BOOT: X1=4M PLL80 CPU/CAN40 FD500K/2M\r\n");
#endif
    valid = CAN_BootInstalledValid();
    if (valid && request != BOOT_APP_RESET_TO_BOOT_SIGNATURE && (P13 & 0x80U)) Device_Jump();
    if (!CAN_BootInit()) {
#if CAN_BOOT_DEBUG_UART
        Device_Log("CAN init failed; no Flash operation\r\n");
#endif
        for (;;) { Device_Heartbeat(); }
    }
    flash_status = Device_FlashInit();
    P6 &= 0xBFU;
    timed = (unsigned char)(valid && request == BOOT_APP_RESET_TO_BOOT_SIGNATURE);
    for (;;) {
        result = CAN_BootReceive(APP_FLASH_START, flash_status, timed);
        if (result > 0L) {
#if CAN_BOOT_DEBUG_UART
            Device_Log("END CRC OK; resetting to revalidate app\r\n");
#endif
            *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS+8UL) = BOOT_APP_UPDATE_COMPLETE_SIGNATURE;
            Device_Reset();
        }
        if (result == CAN_BOOT_RESULT_TIMEOUT && CAN_BootInstalledValid()) Device_Jump();
        timed = 0U;
#if CAN_BOOT_DEBUG_UART
        Device_Log("Boot session stopped; recovery mode, result=");
        Device_LogHex((unsigned long)result); Device_Log("\r\n");
#endif
    }
}
