set architecture armv7e-m
target remote 127.0.0.1:3334
set $cfsr = *(unsigned int *) 0xe000ed28
set $hfsr = *(unsigned int *) 0xe000ed2c
set $bfar = *(unsigned int *) 0xe000ed38
set $abfsr = *(unsigned int *) 0xe000efa8
printf "Fault registers: CFSR=0x%08x HFSR=0x%08x BFAR=0x%08x ABFSR=0x%08x\n", $cfsr, $hfsr, $bfar, $abfsr
if $cfsr & 0x00000400
  printf "Imprecise BusFault: exception PC is asynchronous; offending instruction may be earlier.\n"
end
if $cfsr & 0x00008000
  printf "BFAR valid: 0x%08x\n", $bfar
end
bt
