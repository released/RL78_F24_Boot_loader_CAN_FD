# RL78/F24 MCU CAN FD device Boot

Independent CAN FD Boot project for the 4MHz RL78/F24 target board.
Boot, App and Transfer each contain the sources and build tools used by that project.

## Board and interface

| Setting | Value |
| --- | --- |
| MCU | R7F124FPJ5xFB,256KiB Flash,24KiB RAM |
| Clock | X1 4MHz,PLL80MHz,CPU/CAN40MHz |
| CAN | P10 TX,P11 RX,P12 STB LOW; nominal500K,data2M,BRS |
| Control / response / block | Standard IDs0x700 /0x701 /0x720 |
| Control / block payload | FD0..8bytes / FD64bytes (wire DLC15) |
| App UART | P15 TX,P16 RX,115200; V1/V2 counter |
| Recovery | SW1 P137 LOW |
| IRQ owner | Boot0xAA / App0x55 |
| Image | TargetF24B,IRQ ABI F2V1,CAN protocol5,image format6 |

Use a CAN FD-capable transceiver and analyzer configured for500K/2M.
The actual transceiver supply, standby and wiring follow the fitted hardware.

## Flash and image contract

Full-image CRC reads use 32-bit byte-address arithmetic across 64 KiB Flash
boundaries. See the [CRC address reference](https://released.github.io/slide_boot_loader_for_RL78/#image-crc-linear-address)
for image ranges, trailer addresses and porting checks.

| Address | Purpose |
| --- | --- |
| 0x0000..0x1FFF | Boot,physical vectors and options |
| 0x2000..0x20FF | Complete64-slot App branch table |
| 0x2100..0x217F |128-byte metadata core |
| 0x2180..0x21FF |128-byte metadata reservedFF |
| 0x2200 onward | App startup/code/constants |
| App start + image size -4 | CRC32 tail,excluded from its own CRC |
| 0x3FC00..0x3FFFF | Protected final1KiB |
| RAM0xFF710..0xFF71F | No-init owner/handoff |

`image_metadata_config.h` owns geometry,format,policy and Boot identity.
`can_boot_config.h` owns IDs and compiled rate masks.
`hal/device/f24_device_config.h` owns actual FD timing/RXMB overrides.
Metadata requires mode mask2,nominal mask4,data mask2; Classical images do not match.
The parser is independently compiled `image_metadata.c/.h` with a project profile.

## Reception and IRQ ownership

RXMB0 captures standard control0x700 and block0x720 with64-byte capacity.
`Device_CanRxIsr` acknowledges RMND,checks header/length and captures into an
eight-entry RAM queue; the main loop owns metadata/CRC/Flash and bounded TX polling.
The driver vect ISR is disabled; Boot owns physical0x4C through its complete
AA/55 dispatcher. App registers its own no-vect RETI handler at0x4C.
Full queue drops newest; overwrite/invalid reads are discarded. Sticky faults
and saturating discarded-frame observations are available through
`Device_CanRxFaults()` / `Device_CanRxDrops()` and debugger map symbols.
Flash busy periods execute the checked RAM-only RFD chain with interrupts
disabled; the host waits for COMMIT ACK before the next block. Boot replies
use the configured standard response ID,FD+BRS and eight payload bytes.
See [FD receive/Flash flow](https://released.github.io/slide_boot_loader_for_RL78/#can-fd-device-rx).

## Build and checks

Use Windows PowerShell5.1/.NET and the project's CS+ CC-RL.
From this Boot directory:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .uild.ps1
```

Native software models are maintainer checks; normal MCU builds use CS+ and PowerShell.
Normal MCU builds do not require Python.
Pre/post-build hooks reconcile the selected project and audit linked outputs.
Packaging enforces8KiB Boot,at least512bytes spare,complete vectors,metadata,CRC,Flash bounds and RAM-only RFD.
App-only builds do not rewrite Boot.
Preparation keeps generator bookkeeping project-relative and removes machine
paths only from the iodefine command comment,without changing SFR declarations
or toolchain installation settings. Use the same build entry after SC generation.

## Programmer and Tool files

| Artifact | Use |
| --- | --- |
| `DefaultBuild/boot0000_1FFF.hex` | Programmer installation of Boot |
| App `DefaultBuild/flash_app_variable_CRC.hex` | Programmer installation of packaged App |
| App `boot_app.hex` | Combined Boot/App programmer image |
| App `DefaultBuild/RL78_F24_Boot_loader_CAN_FD_app_update.bin` | Tool CAN FD App update |
| App `DefaultBuild/app_build_report.html` | Actual linked metadata,CRC/hash and IRQ report |

Do not load a raw App MOT or combined binary as a Tool update image.
Install this Boot using a programmer,then use its matching FD App.
Update the MCU host App through Misc to obtain FD burst capability bit13; Host Boot is unchanged.
Select CAN FD /500K /2M in Tool,Discover,Enter Boot if App is running,load the matching BIN and Start Device Update.
ENTER inactivity is30seconds before START while a valid App exists; Discover again after expiry.
Completion requires END CRC and post-reset App confirmation,not a progress value alone.

## Editing and references

App inputs are `app_image_config.h` and `app_irq_handlers.h`.
Generated INC,identity and vector files are outputs.
See [metadata/vector workshop](https://released.github.io/slide_boot_loader_for_RL78/#device-metadata-vector-workshop),
[IRQ ownership](https://released.github.io/slide_boot_loader_for_RL78/#device-irq-ownership) and
[CAN FD guide](https://released.github.io/slide_simple_renesas_isp_tool/#can-fd-update).

## Verification boundary

Clean builds and software models check metadata,IRQ/Flash bounds,FD packing,CRC,retry/replay and malformed input.
Physical FD bus/RFD/IRQ/power acceptance requires saved board evidence.
Normal500K/2M/BRS reception and App update have saved board evidence:standalone
Transfer tests,initial Boot programming and App->ENTER->update->APP_READY/live
identity confirmation. The checked update has zero Host error-counter increases
and a running UART counter. Long stress,queue-full and power/fault recovery
remain separate acceptance items.
See HANDOFF.md and SELF_CHECK.md for measured local results.

## User code entry

`custom_func.c` provides `hardware_init()` for initialization and `loop()` for
foreground service. Add application initialization and foreground work through
these callbacks, retaining the existing communication service calls.
The selected main/startup adapter owns entry and handoff. Generated peripheral
files remain under `src/`; retain the project's MTPJ source selection when using
Code Generator or Smart Configurator, then rebuild to validate the image and vectors.

## Project layout and standalone build

This project includes its own source files, configuration headers and build tools.
A sibling Boot, App or Transfer checkout is not required to compile it.
Keep the project directory name aligned with its `.mtpj` filename; remove a ZIP download suffix when necessary.
Build with CS+ Rebuild, or run the local PowerShell entry:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
```

| Location | Responsibility |
| --- | --- |
| `custom_func.c` | Project initialization in `hardware_init()` and recurring work in `loop()`; runtime orchestration stays here |
| `hal/can` | CAN service, controller driver and CAN adapter, when used by this project |
| `hal/device` | Device, clock, peripheral and CPU handoff APIs |
| `hal/image` | Metadata parser/profile, image validation and software CRC |
| `hal/flash` | Flash access/geometry APIs, when required by the selected image |
| `hal/isp` | Update/Transfer protocol and transport processing APIs |
| Root configuration headers | Local MCU/Flash, handoff and communication contract |
| `tools` and root PowerShell helpers | Local metadata/IRQ generation and linked-image checks |
| `src` | Code Generator or Smart Configurator peripheral sources |

Only the sources registered in this project's CS+ configuration are linked.
Generated main/vector implementations must not become a second active entry.
Retain the `custom_func.c` callbacks and the selected startup adapter after code generation.

Boot and App each carry a local contract. Keep their MCU, Flash layout, image format,
IRQ ABI, handoff and communication settings compatible when installing a pair.
App releases are edited in `app_image_config.h` and `app_irq_handlers.h` in the App project.
Rebuild the selected App to regenerate its metadata, vector table, update BIN and timing BINs.
The App build does not read or rewrite a Boot checkout.
