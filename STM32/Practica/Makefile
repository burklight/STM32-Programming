# STM32v2 Basic Makefile 
# Version 1.5 (9/6/2015)

# This Makefile by default uses precompiled objects
# If any configuration file is modified o a change is made
# inside the ChibiOS folder, the objects will need to be recompiled

# This Makefile can be used for local and network compilation
# the only requirement is that this file should be in the same
# folder where the source .c and .h files are

# The Makefile also supports RAM compilation so that the code
# can be debugged without the need to rewrite the flash memory
# RAM compilation requires a special stack configuration
# on the reset handler inside the file crt0.c in the ChibiOS/RT 
# folder \os\ports\GCC\ARMCMx 

############ PROJECT SOURCE FILES ####################

# The source .c files that make the proyect can be added
# between Base.c amd main.c
# The character "\" at the end of all but the last line is important
PCSRC = Base.c \
		accel.c \
		analog.c \
		comms.c \
		encoder.c \
		int.c \
        lcd.c \
        process.c \
        mutexes.c \
        main.c

# COMPILATION OPTIONS ####################################################

# OBJECT FILE REBUILD #####################################@@@############

# The REBUILD variable indicates if the object files for
# ChibiOS/RT should be rebuilt o are taken fron the CHOBJDIR
# folder.
#
# When using local compilation on c: or other local drive
# rebuild can be left enabled without a great impact on
# compilation time.
#
# When using network compilation, the file checks can greatly
# increase compilation time is rebuild is enabled.  
# 
# To generate the object files the OS must be built using
# the rebuild option both for Flash and RAM options
# The object ".o" files should be copied to the proper
# folder from \build\obj:
#
#    C:\STM32v2\Objects\Ram       For RAM compilation
#    C:\STM32v2\Objects\Flash     For Flash compilation    
#
# Only ChibiOS objects need to be copied
# object files like Base.o or main.o don't need to be copied
# 
# The need to have two sets of object file is due to the fact that
# the interrupt vectors have positions that depends on the program
# location as is defined on the ChibiOS folder:
# 
# os\ports\GCC\ARMCMx\chcore_v7m.c

# Uncomment this option so that no rebuild is done and
# object files in CHOBJDIR are used.
# This option is recomended for compilation in network drives.
#
REBUILD = no

# Uncoment this option so that the rebuild is done.
# This option is recomended for compilation in local drives.
#
# REBUILD = yes

# RAM COMPILATION AND EXECUTION #########################################

# Uncoment this line in order to execute the code from RAM
# This option is the default one as it reduces the Flash
# memory degradation 
#
RAM_COMPILE = yes

# No comenteu aquesta linia per executar sobre Flash
# Aquesta opció només s'ha de fer servir quan els requisits del projecte
# verifiquin una d'aquestes condicions:
#        * Programa que excedeix la mida disponible de la RAM (64k per codi)
#        * Programa que ha de poder arrancar sense el Debugger
#
# Uncoment this line in order to execute from Flash
# This option should be used when:
#   * The program does not fit in the available RAM (64kB code)
#   * The program should start standalone (without using the debugger) 
#
#RAM_COMPILE = no

# FLOAT POINT OPERATIONS ################################################

# The use of the FPU is configured by default
# That increases the speed when operating using float32_t values
# This, however, increases the context size as the float point registers
# need to be saved.
# 
# This value needs to be the same configured at the end of chconf.h
#
USE_FPU= yes

# FOLDER LOCATIONS ###########################################################          

# This section defines some folder locations that depend on the
# current configuration of the computer

# ChibiOS/RT root folder
# Leave uncomented the proper folder location
# 
# CHIBIOS = ../..
CHIBIOS = C:/STM32v2/ChibiOS_2.6.2

# Folder where the precompiled ChibiOS/RT files are located
# Those folders are not used when rebuilding the OS
#
ifeq ($(RAM_COMPILE),yes)
# Folder for RAM compilation
CHOBJDIR = C:/STM32v2/Objects/Ram
else
# Folder for Flash compilation
CHOBJDIR = C:/STM32v2/Objects/Flash
endif

##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here
#
# By default we don't use compiler optimizations
# That increases the execution time but enables us to
# run the program step by step on the debugger.
# In order to enable the optimizations comment the line
# that includes the -O0 option and uncomment the line
# that includes the -O2 option.
ifeq ($(USE_OPT),)
  #USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
  USE_OPT = -O0 -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Enables the use of FPU on Cortex-M4.
# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FPU),)
  USE_FPU = yes
endif

# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = ch

include $(CHIBIOS)/boards/ST_STM32F4_DISCOVERY/board.mk
include $(CHIBIOS)/os/hal/platforms/STM32F4xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F4xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
#include $(CHIBIOS)/test/test.mk

# Define linker script file here 
# The linker file depens on the final location of the code
# A different linker file will be used for RAM and Flash compilation
# The vector positions also depend on the code location
ifeq ($(RAM_COMPILE),yes)
#LDSCRIPT= $(PORTLD)/STM32F407xG_CCM_RAM.ld
LDSCRIPT= $(PORTLD)/STM32F407xG_RAM.ld
#LDSCRIPT= $(PORTLD)/STM32F407xG_RAM2.ld
CORTEX_VTOR_INIT = 0x20000000
#CORTEX_VTOR_INIT = 0x10000000
else
LDSCRIPT= $(PORTLD)/STM32F407xG.ld
CORTEX_VTOR_INIT = 0x00000000
endif

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
BCSRC = $(PORTSRC) \
        $(KERNSRC) \
        $(HALSRC) \
        $(PLATFORMSRC) \
        $(BOARDSRC)

# Files excluded from standard ChibiOS
# $(TESTSRC) \
# $(CHIBIOS)/os/various/devices_lib/accel/lis302dl.c \
# $(CHIBIOS)/os/various/chprintf.c \

# Conditional compilation of ChibiOS/RT files
ifeq ($(REBUILD),yes)
 MESSAGE = Chibios/RT is rebuilt from sources 
 # Files to build from .c sources
 CSRC = $(BCSRC) $(PCSRC)
 # Files to get from CHOBJDIR
 CCSRC = 
else
 MESSAGE = Chibios/RT is linked from precompiled objects
 # Files to build from .c sources
 CSRC = $(PCSRC)
 # Files to get from CHOBJDIR
 CCSRC = $(BCSRC)
endif

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC =

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCPPSRC =

# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various/devices_lib/accel \
         $(CHIBIOS)/os/various

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m4

#TRGT = arm-elf-
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
# This line is added so that the processor vectors are configured
DDEFS = -DCORTEX_VTOR_INIT=$(CORTEX_VTOR_INIT)

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =-lm

#
# End of default section
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

#
# End of user defines
##############################################################################

# FPU needs to be configured using the hard option
ifeq ($(USE_FPU),yes)
# USE_OPT += -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -fsingle-precision-constant
  USE_OPT += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant
  DDEFS += -DCORTEX_USE_FPU=TRUE
else
  DDEFS += -DCORTEX_USE_FPU=FALSE
endif

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stm32lib/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

#include $(CHIBIOS)/os/ports/GCC/ARMCMx/rules.mk
######################### RULES ##############################################

# Object files we need
# They will be located in the same place as the source files
CHOBJECTS=$(SOURCES:.c=.o)

# ARM Cortex-Mx common makefile scripts and rules.

# Output directory and files
ifeq ($(BUILDDIR),)
  BUILDDIR = build
endif
ifeq ($(BUILDDIR),.)
  BUILDDIR = build
endif
OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).hex \
           $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).dmp

# Automatic compiler options
OPT = $(USE_OPT)
COPT = $(USE_COPT)
CPPOPT = $(USE_CPPOPT)
ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections -fno-common
endif

# Source files groups and paths
ifeq ($(USE_THUMB),yes)
  TCSRC += $(CSRC)
  TCPPSRC += $(CPPSRC)
else
  ACSRC += $(CSRC)
  ACPPSRC += $(CPPSRC)
endif
ASRC	  = $(ACSRC)$(ACPPSRC)
TSRC	  = $(TCSRC)$(TCPPSRC)
SRCPATHS  = $(sort $(dir $(ASMSRC)) $(dir $(ASRC)) $(dir $(TSRC)))

# Various directories
OBJDIR    = $(BUILDDIR)/obj
LSTDIR    = $(BUILDDIR)/lst

# Object files groups
ACOBJS    = $(addprefix $(OBJDIR)/, $(notdir $(ACSRC:.c=.o)))
ACPPOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(ACPPSRC:.cpp=.o)))
TCOBJS    = $(addprefix $(OBJDIR)/, $(notdir $(TCSRC:.c=.o)))
TCPPOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(TCPPSRC:.cpp=.o)))
ASMOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(ASMSRC:.s=.o)))
OBJS	  = $(ASMOBJS) $(ACOBJS) $(TCOBJS) $(ACPPOBJS) $(TCPPOBJS)

# Nueva regla para objetos ya proporcionados
COBJS     = $(addprefix $(CHOBJDIR)/, $(notdir $(CCSRC:.c=.o)))

# Paths
IINCDIR   = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR   = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

# Macros
DEFS      = $(DDEFS) $(UDEFS)
ADEFS 	  = $(DADEFS) $(UADEFS)

# Libs
LIBS      = $(DLIBS) $(ULIBS)

# Various settings
MCFLAGS   = -mcpu=$(MCU)
ODFLAGS	  = -x --syms
ASFLAGS   = $(MCFLAGS) -Wa,-amhls=$(LSTDIR)/$(notdir $(<:.s=.lst)) $(ADEFS)
CFLAGS    = $(MCFLAGS) $(OPT) $(COPT) $(CWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.c=.lst)) $(DEFS)
CPPFLAGS  = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.cpp=.lst)) $(DEFS)
ifeq ($(USE_LINK_GC),yes)
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch,--gc-sections $(LLIBDIR)
else
  LDFLAGS = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch $(LLIBDIR)
endif

# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CFLAGS   += -DTHUMB_PRESENT
  CPPFLAGS += -DTHUMB_PRESENT
  ASFLAGS  += -DTHUMB_PRESENT
  ifneq ($(ASRC),)
    # Mixed ARM and THUMB mode.
    CFLAGS   += -mthumb-interwork
    CPPFLAGS += -mthumb-interwork
    ASFLAGS  += -mthumb-interwork
    LDFLAGS  += -mthumb-interwork
  else
    # Pure THUMB mode, THUMB C code cannot be called by ARM asm code directly.
    CFLAGS   += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    CPPFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    ASFLAGS  += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING -mthumb
    LDFLAGS  += -mno-thumb-interwork -mthumb
  endif
else
  # Pure ARM mode
  CFLAGS   += -mno-thumb-interwork
  CPPFLAGS += -mno-thumb-interwork
  ASFLAGS  += -mno-thumb-interwork
  LDFLAGS  += -mno-thumb-interwork
endif

# Generate dependency information
CFLAGS   += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

# Paths where to search for sources
VPATH     = $(SRCPATHS)

#
# Makefile rules
#

all: $(OBJS) $(OUTFILES) MAKE_ALL_RULE_HOOK

MAKE_ALL_RULE_HOOK:

$(OBJS): | $(BUILDDIR)

$(BUILDDIR) $(OBJDIR) $(LSTDIR):
ifneq ($(USE_VERBOSE_COMPILE),yes)
	@echo Compiler Options
	@echo $(CC) -c $(CFLAGS) -I. $(IINCDIR) main.c -o main.o
	@echo
endif
	mkdir -p $(OBJDIR)
	mkdir -p $(LSTDIR)

$(ACPPOBJS) : $(OBJDIR)/%.o : %.cpp Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $<
	@$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

$(TCPPOBJS) : $(OBJDIR)/%.o : %.cpp Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $<
	@$(CPPC) -c $(CPPFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
endif

$(ACOBJS) : $(OBJDIR)/%.o : %.c Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(CFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $<
	@$(CC) -c $(CFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

$(TCOBJS) : $(OBJDIR)/%.o : %.c Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(CFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $<
	@$(CC) -c $(CFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
endif

$(ASMOBJS) : $(OBJDIR)/%.o : %.s Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $<
	@$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
endif

%.elf: $(OBJS) $(COBJS) $(LDSCRIPT)
	@echo $(MESSAGE)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(LD) $(OBJS) $(COBJS) $(LDFLAGS) $(LIBS) -o $@
else
	@echo Linking $@
	@$(LD) $(OBJS) $(COBJS) $(LDFLAGS) $(LIBS) -o $@
endif

%.hex: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(HEX) $< $@
else
	@echo Creating $@
	@$(HEX) $< $@
endif

%.bin: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(BIN) $< $@
else
	@echo Creating $@
	@$(BIN) $< $@
endif

%.dmp: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(OD) $(ODFLAGS) $< > $@
else
	@echo Creating $@
	@$(OD) $(ODFLAGS) $< > $@
	@echo Done
endif

clean:
	@echo Cleaning
	-rm -fR .dep $(BUILDDIR)
	@echo Done

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***



