#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/uia_2_01_00_01/packages;/home/craig/lib/ti/ccsv7/ccs_base;/home/craig/Dropbox/OC/src/LoRaBug_TXRX/.config
override XDCROOT = /home/craig/lib/ti/xdctools_3_32_01_22_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/tidrivers_cc13xx_cc26xx_2_21_00_04/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/bios_6_46_01_37/packages;/home/craig/ti/tirtos_cc13xx_cc26xx_2_21_00_06/products/uia_2_01_00_01/packages;/home/craig/lib/ti/ccsv7/ccs_base;/home/craig/Dropbox/OC/src/LoRaBug_TXRX/.config;/home/craig/lib/ti/xdctools_3_32_01_22_core/packages;..
HOSTOS = Linux
endif
