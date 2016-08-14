#
# The base directory.
#
ROOT=../..

TARGET=${BINDIR}/libsimplelink_nonos.a
OBJDIR=../obj_nonos

#
# Include the common make definitions.
#
include ${ROOT}/tools/makedefs


CFLAGS+=-DSL_FULL
	

#
# Where to find source files that do not live in this directory.
#
VPATH=..
VPATH+=../source

#
# Where to find header files that do not live in the source directory.
#
IPATH=..
IPATH+=../..
IPATH+=../include
IPATH+=../source
IPATH+=../../oslib
IPATH+=../../driverlib
IPATH+=../../inc

#
# The default rule, which causes the driver library to be built.
#
all: ${OBJDIR} ${BINDIR}
all: ${TARGET}



#
# The rule to clean out all the build products.
#
clean:
	@rm -f obj_nonos ${wildcard *~}
	@rm -f obj_os    ${wildcard *~}
	@rm -f ${BINDIR} ${wildcard *~}



#
# Rules for building the Simplelink library with os support.
#
${TARGET}: ${OBJDIR}/cc_pal.o
${TARGET}: ${OBJDIR}/device.o
${TARGET}: ${OBJDIR}/driver.o
${TARGET}: ${OBJDIR}/flowcont.o
${TARGET}: ${OBJDIR}/netapp.o
${TARGET}: ${OBJDIR}/netcfg.o
${TARGET}: ${OBJDIR}/nonos.o
${TARGET}: ${OBJDIR}/fs.o
${TARGET}: ${OBJDIR}/socket.o
${TARGET}: ${OBJDIR}/spawn.o
${TARGET}: ${OBJDIR}/wlan.o


#
# Include the automatically generated dependency files.
#
#ifneq (${MAKECMDGOALS},clean)
#-include ${wildcard ${OBJDIR}/*.d} __dummy__
#endif
