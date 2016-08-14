break _main
#target remote 127.0.0.1:3333
target remote | openocd  -f cc3200.cfg -c "gdb_port pipe; log_output openocd.log"
monitor reset halt
set disassemble-next-line on
set logging on
load

#*****************************************************************************
# Initialize the SP and PC values from the application's
# vector table
#*****************************************************************************
set $sp = g_pfnVectors[0]
set $pc = g_pfnVectors[1]

#*****************************************************************************
# Set break point at main and run to main
#*****************************************************************************
break main
continue
