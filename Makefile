#
# The base directory.
#
ROOT=.

#
# Include the common make definitions.
#
include ${ROOT}/tools/makedefs

#
# Where to find source files that do not live in this directory.
#
VPATH=.
VPATH+=./drivers
VPATH+=./common
VPATH+=./c

#
# Additional Compiler Flags
#
CFLAGS+=-DUSER_INPUT_ENABLE 

#
# Where to find header files that do not live in the source directory.
#
IPATH=./h
IPATH+=./common
IPATH+=./inc
IPATH+=./oslib
IPATH+=./driverlib
IPATH+=./simplelink
IPATH+=./simplelink/source
IPATH+=./simplelink/include

#
# The default rule, which causes the driver library to be built.
#
all: ${OBJDIR} ${BINDIR}
all: ${BINDIR}/app.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -f ${OBJDIR}/*
	@rm -f ${BINDIR}/*


#
#
# Rules for building the app
#
${BINDIR}/app.axf: ${OBJDIR}/main.o
${BINDIR}/app.axf: ${OBJDIR}/uart_if.o
${BINDIR}/app.axf: ${OBJDIR}/wdt_if.o
${BINDIR}/app.axf: ${OBJDIR}/gpio_if.o
${BINDIR}/app.axf: ${OBJDIR}/udma_if.o
${BINDIR}/app.axf: ${OBJDIR}/timer_if.o
${BINDIR}/app.axf: ${OBJDIR}/utils_if.o

${BINDIR}/app.axf: ${OBJDIR}/schedule.o
${BINDIR}/app.axf: ${OBJDIR}/checksum.o

${BINDIR}/app.axf: ${OBJDIR}/one_wire_phisical.o
${BINDIR}/app.axf: ${OBJDIR}/one_wire_network.o
${BINDIR}/app.axf: ${OBJDIR}/one_wire_transport.o

${BINDIR}/app.axf: ${OBJDIR}/wifi_socket.o
${BINDIR}/app.axf: ${OBJDIR}/wifi_network.o
${BINDIR}/app.axf: ${OBJDIR}/wifi_session.o
${BINDIR}/app.axf: ${OBJDIR}/wifi_phisical.o
${BINDIR}/app.axf: ${OBJDIR}/wdog.o
${BINDIR}/app.axf: ${OBJDIR}/flash.o
${BINDIR}/app.axf: ${OBJDIR}/rti.o
${BINDIR}/app.axf: ${OBJDIR}/events.o
${BINDIR}/app.axf: ${OBJDIR}/type_conversion.o
${BINDIR}/app.axf: ${OBJDIR}/leds_session.o
${BINDIR}/app.axf: ${OBJDIR}/buttons.o
${BINDIR}/app.axf: ${OBJDIR}/state_machine.o
${BINDIR}/app.axf: ${OBJDIR}/everythings.o
${BINDIR}/app.axf: ${OBJDIR}/string.o


${BINDIR}/app.axf: ${OBJDIR}/startup_${COMPILER}.o
${BINDIR}/app.axf: ${ROOT}/simplelink/${COMPILER}/${BINDIR}/libsimplelink_nonos.a
${BINDIR}/app.axf: ${ROOT}/driverlib/${COMPILER}/${BINDIR}/libdriver.a
SCATTERapp=app.ld
ENTRY_app=ResetISR

#
# Include the automatically generated dependency files.

#ifneq (${MAKECMDGOALS},clean)
#-include ${wildcard ${COMPILER}/*.d} __dummy__
#endif
