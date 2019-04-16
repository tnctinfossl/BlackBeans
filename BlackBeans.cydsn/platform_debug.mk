# Macros required for the generic driver for toolchain and/or operating system.

DESIGN= BlackBeans
CONFIG= debug
TOOLCHAIN_DIR ?= C:/Program Files (x86)/Cypress/PSoC Creator/4.2/PSoC Creator/import/gnu/arm/5.4.1/bin

CC_BASE= arm-none-eabi-gcc
CC_PATH= $(shell which $(CC_BASE) 2>/dev/null)
CC= $(if $(CC_PATH),$(CC_PATH),$(TOOLCHAIN_DIR)/$(CC_BASE))

OUT_DIR_CortexM3= output/$(CONFIG)/CortexM3
CFLAGS_CortexM3= -mcpu=cortex-m3 -mthumb -I. -IGenerated_Source/PSoC5 -Wa,-alh=$(OUT_DIR_CortexM3)/$(basename $(<F)).lst -funsigned-char -g -D DEBUG -D CY_CORE_ID=0 -Wall -ffunction-sections -ffat-lto-objects -finline-functions -Og

CDEPGEN_CortexM3= -MM $< -MF $(OUT_DIR_CortexM3)/$(<F).d -MT $(OUT_DIR_CortexM3)/$(@F)  $(CFLAGS_CortexM3)

LD_BASE= arm-none-eabi-gcc
LD_PATH= $(shell which $(LD_BASE) 2>/dev/null)
LD= $(if $(LD_PATH),$(LD_PATH),$(TOOLCHAIN_DIR)/$(LD_BASE))

LDFLAGS_CortexM3= -mcpu=cortex-m3 -mthumb -L Generated_Source/PSoC5 -Wl,-Map,$(OUT_DIR_CortexM3)/$(DESIGN).map -T Generated_Source/PSoC5/cm3gcc.ld -specs=nano.specs -Wl,--gc-sections -g -ffunction-sections -finline-functions -Og -ffat-lto-objects

AS_BASE= arm-none-eabi-as
AS_PATH= $(shell which $(AS_BASE) 2>/dev/null)
AS= $(if $(AS_PATH),$(AS_PATH),$(TOOLCHAIN_DIR)/$(AS_BASE))

ASFLAGS_CortexM3= -mcpu=cortex-m3 -mthumb -I. -IGenerated_Source/PSoC5 -alh=$(OUT_DIR_CortexM3)/$(basename $(<F)).lst -g -W

AR_BASE= arm-none-eabi-ar
AR_PATH= $(shell which $(AR_BASE) 2>/dev/null)
AR= $(if $(AR_PATH),$(AR_PATH),$(TOOLCHAIN_DIR)/$(AR_BASE))
ARFLAGS= -rs

RM= rm
RMFLAGS= -rf
