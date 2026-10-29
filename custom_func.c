/* Project/API reading notes: Project-owned initialization and foreground work.
 * The selected main/startup adapter calls hardware_init() once and loop() repeatedly.
 * Keep application changes here so generated entry sources remain replaceable; lower-level APIs
 * live in this project's hal directories.
 */
#include "platform.h"
#include "device_board.h"
#include "can_boot_transport.h"
#include "user_def.h"
#include "sw_crc32.h"
#include "device_app_vectors.h"

/* Check the complete installed App before handoff: metadata/range, CRC trailer and the fixed App
 * jump-table destinations must all pass.
 */
unsigned char CAN_BootInstalledValid(void)
{
    unsigned long size;
    if (ImageMetadataValidate((const unsigned long __far *)APP_MANIFEST_ADDRESS) !=
        IMAGE_METADATA_OK)
    {
        return 0U;
    }
    size = *(const unsigned long __far *)(APP_MANIFEST_ADDRESS + IMAGE_MANIFEST_SIZE_OFFSET);
    if (caculate_crc32_checksum(APP_FLASH_START, size - 4UL) !=
        *(const unsigned long __far *)(APP_FLASH_START + size - 4UL))
    {
        return 0U;
    }
    return Device_AppVectorsValid((const unsigned long __far *)APP_FLASH_START,
                                  APP_FLASH_START + 0x200UL, APP_FLASH_START + size - 4UL);
}

static unsigned char g_custom_flash_status, g_custom_timed;

/* Initialize the role selected by this project before foreground processing.
 * Preserve clock, image/handoff and peripheral/IRQ enable order when adding application
 * initialization.
 */
void hardware_init(void)
{
    unsigned long request;
    unsigned char valid;
    request = *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS + 12UL);
    *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS + 12UL) = 0UL;
    g_custom_timed = (unsigned char)(request == BOOT_APP_RESET_TO_BOOT_SIGNATURE);
    if (!Device_BoardInit())
    {
        for (;;)
        {
            BSP_NOP();
        }
    }
#if CAN_BOOT_DEBUG_UART
    Device_Log("F24 CAN FD DEVICE BOOT: X1=4M PLL80 CPU/CAN40 FD500K/2M\r\n");
#endif
    valid = CAN_BootInstalledValid();
    if (valid && !g_custom_timed && (P13 & 0x80U))
    {
        Device_Jump();
    }
    if (!CAN_BootInit())
    {
#if CAN_BOOT_DEBUG_UART
        Device_Log("CAN init failed; no Flash operation\r\n");
#endif
        for (;;)
        {
            Device_Heartbeat();
        }
    }
    g_custom_flash_status = Device_FlashInit();
    P6 &= 0xBFU;
    g_custom_timed = (unsigned char)(valid && g_custom_timed);
}

/* Foreground service point called repeatedly by the selected main adapter.
 * Keep protocol/application work and text logging bounded so pending traffic and deferred actions
 * can continue.
 */
void loop(void)
{
    signed long result;

    result = CAN_BootReceive(APP_FLASH_START, g_custom_flash_status, g_custom_timed);
    if (result > 0L)
    {
#if CAN_BOOT_DEBUG_UART
        Device_Log("END CRC OK; resetting to revalidate app\r\n");
#endif
        *(volatile unsigned long __far *)(BOOT_APP_SHARED_RAM_ADDRESS + 8UL) =
            BOOT_APP_UPDATE_COMPLETE_SIGNATURE;
        Device_Reset();
    }
    if (result == CAN_BOOT_RESULT_TIMEOUT && CAN_BootInstalledValid())
    {
        Device_Jump();
    }
    g_custom_timed = 0U;
#if CAN_BOOT_DEBUG_UART
    Device_Log("Boot session stopped; recovery mode, result=");
    Device_LogHex((unsigned long)result);
    Device_Log("\r\n");
#endif
}
