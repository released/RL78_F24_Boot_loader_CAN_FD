# CAN FD verification gates

### Source publication / portable generation

- [x] Three MTPJ generator records use local names/relative output folder;
  iodefine command comments omit machine locations. License/SFR definitions
  and complete non-comment C text unchanged;compiler installation settings kept.
- [x] Shared helper synthetic tests,boot/app/transfer preparation CheckOnly
  and relocated sibling source checks PASS. App/Transfer README dependency
  on same-parent FD Boot retained. Required DefaultBuild deliveries present.
- [x] Publisher nine-repository/eleven-delivery catalog and19 isolated Git
  integration cases PASS. Actual preview refuses missing FD .git before cleanup.
  Tool EXE/16firmware artifact hashes unchanged;MD/web/PDF synchronized.
- [ ] No real Git init/index/commit/push or product rebuild in publication work.
  Manual CS+ GUI/SC regeneration and compiler rebuild NOT RUN for this task.

### Compact FD Boot/App interrupt baseline

- [x] RXMB0/64byte +8-entry queue,manual no-vect ISR/AA55 route/App4C slot;
  hardware IRQ enabled,driver vect ISR disabled;no FIFO/main-loop polling.
- [x] Boot TX fixed8/control route;all64BR opcode/ranges and existing metadata/
  version/CRC/Flash guards preserved. RFD RAM chain/DI/EI/ACK pacing unchanged.
- [x] Boot1.2.0/App1.0.4/build20261020 clean builds0errors,2/5existing warnings.
  Boot end1DBF/spare576;App4756/CRC9819B975. Actual delivery/route/RETI checked.
- [x]9artifact/negative route/RETI tests and strict C90 Boot/App config,
  512captures/queue/full/order/maskedIRQ/overwrite/flags/saturation/stop,
  three-frame RAM/CRC and64slot mutation tests PASS. PC FD fault models PASS.
- [x] Transfer model/trace0/1/PP unchanged;516protected hashes unchanged.
  MD/Web/CheckOnly/310refs/content/33safe-clean/syntax and13-page PDF/4diagrams,
  every page visually inspected. No protected product rebuilt.
- [x] User112409 standalone normal IRQ baseline verified/archived,not Flash evidence.
- [x] User130843 initial FD update and133221 App->ENTER->update:75FD64 blocks,
  END CRC/APP_READY/live image/CAN_STOP and UART V1 counter pass.133221 uses
  Tool0097 quiet wait and one health probe;all seven Host error deltas zero.
- [ ] Long repeated-session,queue-full,power/stress/fault acceptance: NOT RUN.

### Standalone RXMB0 interrupt / RAM queue

- [x] Transfer-only actual vendor ISR/vector4C and InHandler ->8-entry queue;
  main-loop owns protocol/UART,no polling fallback. TX/timing/wire unchanged.
- [x]512native8/64-byte ISR/API captures,wrap/full/drop-newest/order,masked IRQ,
  overwrite/errors/canaries/saturation/stop/reinit/window restore,no FIFO pop.
  PC/C protocol trace0/1 and actual config guards PASS.
- [x] Clean standalone0errors/2license warnings;HEX/MOT range/banner/ISR4C=00FC,
  timer2E=01B9,linked InHandler/no polling RXMB/RXFIFO,vendor SHA PASS.
 9artifact/compact HAL/metadata/CRC/block models PASS;no protected image rebuilt.
- [x] Boot/App backend/config PP unchanged;517protected hashes unchanged.
  MD/Web/CheckOnly/310refs/content/26safe-clean;11-page PDF/3diagrams/all pages PASS.
- [x] User112409 interrupt image:Discover+4full tests,79RX/TX/IRQ/queue,
  zero drops/overwrite/API errors. Not queue-full or sustained-burst evidence.
- [ ] Physical queue-full/stress/fault acceptance NOT RUN.

### Prior polling RXMB0 verification record

- [x] Transfer-only RXMB0/64-byte/exact0x710 polling API,no RX FIFO/interrupt;
 busy deferred,overwrite dropped/counted. Timing/TX/wire unchanged.
- [x] Actual vendor RXMB C90 body/table/types:128mixed8/64-byte reads,busy/
 overwrite/empty/error/length/flags/index/null/buffer/saturation/no FIFO write
 PASS. Actual config/timing and PC/protocol trace0/1 models PASS.
- [x] Clean standalone0errors/2license warnings;HEX matches audited MOT
 delivery range,RXMB banner and linked receive API verified.9artifact/
 compact HAL/metadata/block models PASS. Boot/App preprocessed config/backend
 unchanged;517protected hashes identical. No protected image rebuilt.
- [x] MD/web/CheckOnly/310refs/content/26safe-clean and9-page PDF/all-page
 visual review PASS.
- [ ] Physical RXMB0 continuous-session/root-cause acceptance: NOT RUN.

### Prior FIFO implementation verification record

- [x] Transfer-only official R_CAN_ReadRXFIFO adapter:actual vendor body/types/
 tables in strict C90 register tests,128 modeled consumes,eight16-entry wraps,
 exact8/64-byte copies,empty/lost/error/DLC/flags/length/no-extra-pop PASS.
 Non-Transfer preprocessing identical to task entry;Boot/App not rebuilt.
- [x] Three standalone clean builds0errors/2license warnings; final startup banner
 identifies the backend. Production PC protocol trace0/1 and9artifact/compact
 HAL/metadata/block models PASS.
- [x]517task-entry protected hashes unchanged;actual HEX banner/vendor symbol
 verified. MD/web/CheckOnly,9-page PDF/all-page visual review,310refs/content/
26safe-clean scripts and PowerShell syntax PASS.
- [ ] Physical official-receiver continuous-session acceptance: NOT RUN.

- [x] Transfer-only runtime snapshots are read-only;pop-write/last TX status
  C90 HAL model PASS. Non-Transfer backend preprocessing and Boot/App hashes unchanged.
- [ ] Physical Transfer diagnostic/root-cause capture after16requests: NOT RUN.
- [x] Explicit standalone -Kind transfer uses FD helpers/projection and builds
  only Transfer; default all remains Boot/App. Transfer CheckOnly PASS.
- [x] FD Transfer actual C90 protocol/production PC client trace0/1 PASS;
  Host and FD Boot/App delivery hashes unchanged during this helper extension.
- [x] Independent FD Boot/App paths,CS+ pre/post hooks and outputs.
- [x] Clean Boot/App,8KiB Boot,spare522>=512 and App2000/code2200.
- [x] Full physical vectors/AA55/App-local ISR and RAM-only RFD checks.
- [x] FD metadata mode2 / nominalmask4 / datamask2 / format6 / protocol5.
- [x] Production FD HAL64-byte RX/TX,DLC,flags,RTR,IDE and exact raw filter.
- [x] Actual BIN CRC/metadata,malformed staging,block duplicate/conflict/CRC/missing/padding and version policy models.
- [x] Nine PowerShell artifact/negative checks; production Tool parser.
- [x] Native end-to-end FD PC/host codec/device RAM model,retry/replay and disconnect.
- [ ] Physical500K/2M/BRS/FD64 update and post-reset App evidence: NOT RUN.
- [ ] Physical faults,power interruption,IRQ timing and throughput: NOT RUN.

Run build.ps1 -Kind all,then test.ps1.
Optional test.ps1 -ToolProject <Tool checkout> requires MSVC developer tools.
No model result establishes physical CAN or Flash acceptance.
