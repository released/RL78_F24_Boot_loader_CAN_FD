# F24 MCU CAN FD device handoff

## Linear CRC Flash traversal (2026-10-22)

sw_crc32.c matches F13/Classical/FD:integer-address increment before far cast
preserves carry across64KiB. CRC/FD/RXMB/IRQ/RFD/Flash contracts unchanged.
Boot1.2.1/build20261022 clean0errors/2existing warnings,end1DC1/spare574;
App clean0/5,4756bytes/CRC2244D36A. App identity unchanged;generated minimum
Boot follows the paired release. Combined installation images refreshed.
Native109CRC checks,strict-C90 target carry and rejected unsafe-pointer
mutation PASS;9artifact checks,compact512capture HAL/IRQ/vector tests and
actual Tool/parser/RAM models PASS. No standalone Transfer/host/Tool runtime
rebuild or physical Flash/COM test. Timing BINs unchanged;MD/web/PDF sync
recorded in Tool HANDOFF. Earlier hardware evidence is a separate baseline.

## Normal hardware acceptance / source publication (2026-10-20)

User130843 confirms initial Boot-mode update;133221 confirms Discover App->
ENTER->Boot->update->App using Tool0097,Host1.7.0/115200,FD500K/2M/BRS.
75FD64 blocks/4756bytes/CRC9819B975,END ACK,APP_READY/live1.0.4/20261020,
CAN_STOP ACK and UART completed/RXMB IRQ/V1 counter pass.133221 transfer
2402ms,total5543ms,1000ms restart wait,one health probe. All seven Host error
deltas zero,no raw error/state events. Earlier initial run had warning/passive
+1;do not replace its result with this later clean run. Pre-update manual Host
reset had one bootstrap PING retry,not an update-phase retry.

Current publication work preserves the validated Boot/App/Transfer images.
MTPJ generator file names/output folder are relative;the iodefine generator
comment omits machine locations without changing SFR code/license. Shared
tools/public_project_paths.ps1 and preparation checks preserve this on normal
generation. README keeps same-parent Boot/App/Transfer dependencies explicit.
Hardware stress/queue-full,other rates and power/fault recovery remain NOT RUN.
No Git init/commit/push,COM/Flash/reset or product rebuild in publication work.

Publication verification complete:19 isolated Git cases/four workspace path
tests PASS;portable comment/XML tests and all three prepare CheckOnly gates
PASS,including a relocated sibling source copy. Comment-free iodefine C text
and Tool EXE/16firmware artifact hashes unchanged. MD/web CheckOnly,310refs,
18-file public-content/33safe-clean checks PASS. CAN FD PDF13pages/4diagrams,
all pages visually reviewed. Actual preview refuses missing FD .git before any
cleanup/index/commit/remote changes. All three required deliveries remain in
the user's trimmed DefaultBuild folders;remaining intermediates untouched.
Manual CS+ GUI/SC regeneration and compiler rebuild NOT RUN for this task.

## FD Boot/App compact RXMB interrupt (2026-10-20)

User112409 logs verify normal standalone IRQ request/reply:Discover+4full
tests,79RX/TX/IRQ/queue,zero drops/overwrite/errors. Benchmark logs/HEX are
preserved in Tool ignored tmp/internal/benchmarks/fd_transfer_rxmb_irq_20261020_112409.
Do not infer FD Flash or stress acceptance from that evidence.

Boot/App now independently link compact Device_CanRxIsr with RXMB0/64bytes
and8-entry SPSC queue. No FIFO or main-loop hardware polling. Driver vect
ISR is disabled;RMIEC1/CPU source are enabled explicitly. Boot4C routes under
ownerAA to its ISR;owner55 routes App slot2098 to its registered ISR.
Boot replies are fixed configured standard ID/FD+BRS/eight bytes;App/Transfer
generic TX remains. Flash gates/DI/RAM-only chain/COMMIT ACK pacing unchanged.
New common/device_app_vectors.h validates all64 BR words after metadata/CRC.
Discard count saturates255;sticky queue/overwrite/header bits reset at CAN init.
App prints receiver/nonzero RX faults/drops and retains V1 counter.

Changed:common/device_can.c,device_board.h,f24_device_config.h,
device_app_vectors.h;boot_main.c,can_boot_transport.c,image_metadata_config.h;
tools/prepare_projects.ps1,package_images.ps1,test_device.ps1,
tests/fd_pair_can_tests.c,tests/transfer_mock/platform.h,test.ps1;
App app_main.c,app_irq_handlers.h,app_image_config.h and generated project/
vector/metadata/image outputs. README/agent/checklists and Tool MD/Web/PDF sync.

Boot1.2.0/App1.0.4/build20261020;App minimum Boot1.2.0. Final clean builds:
Boot0errors/2license warnings,end1DBF/spare576;App0errors/5existing license/
option-byte warnings,size4756,CRC9819B975. HEX/MOT equality,actual dispatch/
RETI/App table,all image/RFD guards PASS. Boot SHA256:
CA52B280B10864726C4D562B9566B3B98A336B396F920370FC033BAE7F49D9D9.
App update BIN SHA256:
8D0A7DB3146D6DA84FECC49A0D897DDF734FDA3E60960E06203DD1FB7478BBB7.
test.ps1 -CModels PASS9artifact/config/C90 profiles,512captures,queue full/
order/overwrite/flags/saturation/stop,three-frame RAM/CRC boundaries,64-slot
range/opcode tests. Tool test_can_fd.ps1 PC/codec/RAM retry/replay/padding/
lost-ACK/disconnect PASS. Transfer tests trace0/1 and production preprocessing
unchanged;516protected hashes unchanged. No Classical/Host/Tool/Transfer build.
MD/Web/CheckOnly/310refs/content/33safe-clean/syntax PASS;13-page PDF/4diagrams,
all pages visually reviewed. Physical FD update/fault/power NOT RUN.
No COM/Flash/reset/Git publication. Next:program Boot HEX,Tool CAN Upgrade/
CAN FD/500K/2M with FD App update BIN,verify END/App/UART then App->ENTER->
repeat. Preserve report/Tool log/UART. Full file/results record:Tool HANDOFF.md.

## Standalone Transfer RXMB0 interrupt (2026-10-20)

Current standalone backend:unchanged vendor RXMB ISR/vector4C -> callback
R_CAN_ReadRXMBInHandler ->8-entry SPSC RAM queue -> main-loop validation,
protocol/logging. TX polling,clock/rates/IDs/wire unchanged. No RX FIFO and no
main-loop hardware polling fallback. Queue full drops newest;overwritten and
API errors counted separately. Snapshot masks/restores only RXMB CPU source
for coherent counters,never acknowledges flags/consumes queue/resets CAN.
Producer publishes after copy;consumer releases after copy. All counters saturate.
Changed files:common/device_can.c,f24_device_config.h,device_board.c(comment only);
standalone transfer_main.c,
transfer_diagnostics.h,can_transfer_protocol.c,README and delivery HEX;
tools/tests/fd_can_tests.c,prepare_rxmb_model.ps1,transfer_mock/platform.h.
Tool native test script/fixture and canonical MD/Web/PDF are synchronized.

Clean standalone0errors/2existing license warnings.512 actual vendor ISR/API
modeled receives,8/64-byte/wrap/full/order/masked IRQ/no fallback/overwrite/API
errors/saturation/stop/reinit/window restore PASS. PC/C protocol trace0/1,
9artifact/compact HAL/metadata/CRC/block models PASS. Boot/App effective config
and production backend PP identical to task entry;517protected hashes unchanged.
Actual HEX matches audited MOT range;4C=00FC ISR,2E=01B9 timer,InHandler linked,
no polling RXMB/RXFIFO API linked;vendor driver SHA unchanged. HEX SHA256:
B37E9BCE4F360DD392AECF78540F0D419920BE19D4E7C00B0219A0B199F8CFAA.
MD/Web CheckOnly/310refs/content/26safe-clean PASS;11-page PDF/3diagrams/all-page
visual QA PASS. No other image rebuilt;Tool remains0096.

User-supplied polling RXMB logs verified Discover+4full tests/79RX/TX. Polling
HEX/logs preserved in Tool ignored tmp/internal/benchmarks. IRQ hardware
acceptance NOT RUN. No agent COM/Flash/reset/power or Git publication.
Next:program only standalone HEX,confirm interrupt/queue startup ID,Discover
once,repeat4full Transfer Tests without reconnect,retain both traces. Require
12stats/FD64 per test,counters progress,drops/overwritten/api_errors=0 and
depth0/queued=dequeued after processing. FIFO root cause remains unproven.

## Prior standalone Transfer polling RXMB0 record (2026-10-20)

F24_DEVICE_TRANSFER alone configures one64-byte RXMB0,no RX FIFO/interrupt,
and routes exact0x710/mask7FF to it. Main-loop R_CAN_ReadRXMB manages RMND and
overwrite checks;busy is deferred,overwrite dropped/counted. Runtime snapshots
are read-only RMNB/RMND/reads/overwritten plus TX state;startup identifies RXMB.
Clock/rates/IDs/TX/wire unchanged. Boot/App effective configuration and adapter
preprocessing are identical to task entry;neither image rebuilt.

Actual vendor body/table/types C90 model128mixed8/64-byte receives,busy/
overwrite/empty/error/length/flags/index/null/buffer guards/saturation/no FIFO
write PASS. Effective Transfer config/timing and PC/protocol trace0/1 PASS.
Clean standalone0errors/2existing license warnings;9artifact/compact HAL/
metadata/block models PASS.517protected hashes unchanged. HEX matches audited
MOT delivery range,contains RXMB startup banner,links RXMB not RXFIFO.
HEX SHA256:7F751524F18F5C3A8BFCFCB5D97A4DBABD27D7F3D0EC1B48A137ED4BF79D1FF0.
MD/web/CheckOnly/310refs/public-content/26safe-clean PASS;9-page PDF/two
diagrams rendered and all pages visually inspected. Tool remains0096.
No physical RXMB test,agent COM/Flash/reset/power or Git publication.
Next:program only Transfer HEX,capture RXMB banner/RMNB0701,Discover once and
four complete Transfer Tests without disconnecting;retain UART/Tool traces.
The earlier FIFO implementation record below is historical.

## Standalone Transfer vendor FIFO receiver (2026-10-20)

F24_DEVICE_TRANSFER alone delegates reception to R_CAN_ReadRXFIFO; the vendor
owns RAM window/lost flag/dequeue. Standard FD+BRS/DLC/length validation stays
in the adapter,which never pops again. Boot/App preprocessing matches entry;
neither image is rebuilt. FIFO16x64,500K/2M,TX and protocol stay unchanged.
The standalone startup banner identifies the receiver. Real vendor body,
tables and RX types are extracted for C90 native models:128 dequeues/eight
modeled wraps,no extra adapter pop,empty/lost/error/length/flag guards PASS.
Trace0/1 PC protocol models and9artifact/compact HAL/metadata/block models PASS.
Three clean standalone builds0errors/2existing license warnings PASS.
517task-entry protected hashes (512Classical plus Tool/Host/FD deliveries)
unchanged;linked vendor symbol and HEX startup identifier verified.
MD/web/projection checks and9-page PDF/all-page visual review PASS;310refs/
public-content/26safe-clean scripts/PowerShell syntax PASS. Tool remains0096.
Transfer HEX SHA256:11056982D797FAE3D39CF31A158CB1CDABBA5B42730ACA00DC1DA3F766503536.
Physical acceptance after16/32/64 receives and root cause remain NOT RUN.

## Transfer runtime observations (2026-10-19)

common/device_can.c adds F24_DEVICE_TRANSFER-only snapshots and observational
pop-write/last-status recording for the sibling FD Transfer diagnostics header.
No extra FIFO pop,flag clear,controller reset or timeout-policy change.
Non-Transfer preprocessing is identical to task entry;Boot/App were not rebuilt,
and both delivery hashes remain unchanged. Extended mock HAL test covers read-only
snapshots,128software pop writes and pre-clear TX status. Existing9artifact/
actual HAL/metadata/block models and native Tool regressions PASS.
Standalone Transfer clean build0errors/2existing license warnings;Tool0096.
User traces verify FD discovery/base/64-byte echo but fail after16requests,
GET_STATS index3. Root cause/new diagnostic hardware capture remains pending.
No agent COM/Flash/reset/power operation or Git publication performed.

## Standalone FD Transfer helper (2026-10-19)

build.ps1 and tools/prepare_projects.ps1 now accept explicit -Kind transfer
for sibling RL78_F24_CAN_FD_Transfer. Default all still builds only Boot/App.
Transfer owns its SC projection,defines F24_DEVICE_TRANSFER,and uses this
project's FD preinclude/common driver with explicit FD pre/post hooks.
No Boot/App regeneration/build occurred during this change; both delivery
hashes match task entry. FD Transfer clean builds and actual C90 protocol /
production PC client models trace0/1 PASS. Physical FD testing NOT RUN.
Tool0095 and the standalone Transfer HEX are the paired test deliveries;
Transfer uses generic Host21,not the update-only FD burst contract.

## CAN FD profile (2026-10-19)

Independent Boot/App projects use500K nominal /2M data,BRS,FD64 data.
Build helpers,CS+ includes/hooks and combined-image packaging resolve only the FD pair.
Classical F13/F24 golden projects are not build targets.

The compact HAL rejects Classical/non-BRS/RTR/IDE and unsupported DLC.
FIFO payload is64bytes; control is0..8bytes,raw block data is exactly64bytes on0x720.
Final logical block is FF-padded; padding,block CRC and staged metadata are checked before Flash.
Duplicate-identical data is accepted; conflicting duplicates poison the block.
Only the last committed block may replay without writing again.
Image format6,protocol5,256-byte metadata,AA/55 IRQ forwarding and all launch gates remain.
FD metadata requires mode2,nominalmask4,datamask2.

Clean Boot0errors/2existing license warnings,end1DF5,spare522.
Clean App0errors/5existing license/device-file warnings,4284bytes,CRC087ADA53.
Nine PowerShell artifact/negative guards,production C90 FD HAL and production metadata/CRC/block models PASS.
Tool's explicit test_can_fd.ps1 passes actual FD BIN through production PC upload/host codec/device RAM with loss/retry/replay/CRC/conflict/missing/padding/mode/disconnect cases.

Host App needs FD burst bit13/command22 version2; Host Boot is unchanged.
Tool source/build and public instructions are maintained in the Tool checkout.

Hardware FD CAN/RFD/reset/IRQ/fault/power tests NOT RUN.
No COM writes,programmer activity,Git commit/push or website deployment.
