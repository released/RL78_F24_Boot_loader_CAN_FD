# F24 MCU CAN FD device projects

- Read README.md, HANDOFF.md and SELF_CHECK.md first. This is the 4MHz target
  board with its own Transfer/Boot/App projects.
- image_metadata_config.h owns IMAGE_* geometry/metadata/policy/Boot identity;
  can_boot_config.h owns CAN, boot_app_handoff.h owns RAM/owner. Generators
  read these three explicit sources; do not copy values or use CAN aliases.
  Keep 8KiB Boot/App2000 and
  at least 512 bytes spare; mandatory packaging guards may not be bypassed.
- Do not edit generated smc_gen or vendor RFD. Use common overrides/projections.
- Handwritten embedded C90, block-start declarations, English comments.
- Preserve complete normal IRQ forwarding, App-local registered ISR mappings,
  256-byte metadata/CRC, fail-closed range checks and RAM-only RFD call chain.
- F13/F24 Classical CAN device Boot/App are immutable golden samples for this task.
  This FD pair uses500K/2M/BRS and one64-byte raw data frame on0x720.
  Never build or regenerate its sibling Classical projects.
  Align common metadata/vector/build behavior before introducing another MCU.
  Both devices take priority over F24 host; keep host changes out of this work.
- Independently compile the common image_metadata.c/.h plus the project
  image_metadata_profile.c in Boot/App. Keep local copies identical to Tool
  firmware/common/device_metadata (sync_device_metadata.ps1). Parser has no CAN/
  Flash API; preserve all validation with detailed/compact status options,
  C90/32-bit/endian/alignment guards and existing CRC/launch decisions.
- App users edit app_image_config.h and app_irq_handlers.h. Generate
  metadata INC/wrapper, identity and vector table in App pre-build; reject
  stale linked metadata before packaging. No JSON or manual INC fallback.
- tools/app_config.ps1 is an identical local copy of the golden C-header reader;
  preserve common syntax and strict validation in both independent Boot repos.
- Windows PowerShell5.1/.NET only for prepare/package/test; no Python runtime.
  Preserve Boot ownerAA/App55 and verify compiled dispatch.
- tools/public_project_paths.ps1 keeps generator bookkeeping project-relative
  and normalizes only the iodefine command comment. Preserve SFR declarations,
  licenses and SDK installation options. Preparation and -CheckOnly share
  these rules; keep App/Transfer sibling dependencies explicit in README.
- Device magic is CAN plus NUL (0x004E4143). Policy3 uses metadata0x74 for
  APP_IMAGE_VERSION_CONTROL, the sixth App define. A valid installed App
  controls ordinary START; enabled means same/newer only. FORCE_START0E skips
  only ordering for that update. Preserve other gates and no persistent unlock
  or stored version floor; invalid-App recovery is unrestricted by version.
- Boot/App releases are independent; generated minimum Boot is a lower bound,
  not an equality comparison with the App version or build date.
- tools/app_build_report.ps1 matches the common golden report helper.
  Emit App HTML only after linked-image checks; use actual BIN/CRC/hash/vector
  data and distinguish build evidence from live MCU/hardware evidence.
- build.ps1 clean rebuild + post-build audit; test.ps1 for safe offline checks.
- Default all builds Boot/App only; explicit Kind transfer builds the sibling
  RL78_F24_CAN_FD_Transfer without touching Boot/App outputs or Classical projects.
  Transfer owns custom_transfer_fd.h,mirrored from Tool firmware/common and
  checked by test_fd_transfer.ps1; keep FD64 capability/length/CRC/replay aligned.
  Runtime observation code in common/device_can.c is F24_DEVICE_TRANSFER-only.
  Transfer uses the selected project's unchanged RXMB ISR at vector0x4C and
  R_CAN_ReadRXMBInHandler callback with one64-byte RXMB0,exact0x710,no FIFO.
  ISR only captures into an8-entry SPSC RAM queue;main-loop owns protocol/logging
  with no polling fallback. TX stays polled. Queue full drops newest;
  overwrite/API errors drop that read. Boot/App use their compact no-vect
  Device_CanRxIsr with RXMB0 and an8-entry queue. Keep the Transfer receiver
  preprocessed implementation and delivered HEX unchanged for Boot/App work.
  Driver vect ISR generation stays off in Boot/App;hardware RMIEC bit0 and
  CPU source enable are independent. Boot dispatcher owns4C for AA;App-local
  mapping owns4C for55. Preserve post-build compiled routing/RETI/no-double-vector
  checks. Boot TX is compile-time standard response ID/FD+BRS/eight bytes.
  Common device_app_vectors.h validates all64 BR words after metadata/CRC.
  Preserve all destinations/ranges;do not strip checks to meet ROM budget.
  Extract vendor RX types/table/ISR/InHandler for prepare_rxmb_model.ps1 tests;
  software read/overwrite counts do not establish physical bus acceptance.
  Snapshots briefly mask/restore only RXMB IRQ for coherent16-bit counters,
  never ack flags/pop queues/reset CAN. Diagnostic-only work must not rewrite
  Boot/App products or add recovery. Runtime Boot/App changes require clean
  rebuilds plus physical acceptance distinct from Transfer communication.
  FD-specific HAL tests use mock registers;
  physical CAN/Flash/power checks require saved board evidence.
  No COM/flashing/power tests without scope/authorization. Report NOT RUN.
- Sync related Tool MD/web/PDF when changing behavior. Preserve unrelated
  project outputs; App-only configuration must not rewrite Boot images.
- CRC traversal uses integer-address carry across 64 KiB, not far-pointer
  increments. Keep the three device CRC sources identical and run the Tool
  test_device_crc_boundary.ps1 native/CC-RL assembly checks after CRC edits.
