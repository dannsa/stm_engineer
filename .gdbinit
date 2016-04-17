#OpenOCD GDB server, enable semihosting, program the flash, and wait for command
define reload
kill
monitor jtag_reset
load
end

target extended localhost:3333
monitor arm semihosting enable
monitor reset halt
load
monitor reset init
