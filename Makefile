# Number of Compilation Threads
OPT=-j8

PYTHON = $(shell which python2)
ARKROOT ?= $(CURDIR)

export DEBUG ARKROOT

SUBDIRS = libs \
	contrib/PC/btcnf \
	contrib/PC/prxencrypter \
	loader/rebootex \
	core/systemctrl \
	core/compat/psp \
	core/compat/vita \
	core/compat/vitapops \
	core/inferno \
	core/stargate \
	core/popcorn \
	core/vshctrl \
	loader/usploit/linkless_payload \
	loader/usploit/signed_eboot \
	loader/kxploit/kernel_loader \
	loader/kxploit/compat \
	loader/kxploit/kram_dumper \
	loader/kxploit/vita_flash_dumper \
	loader/pro_updater \
	extras/menus/arkMenu \
	extras/menus/recovery \
	extras/menus/xMenu \
	extras/menus/vshmenu

.PHONY: subdirs $(SUBDIRS) cleanobj clean cleanobj copy-bin mkdir-dist encrypt-prx

all: subdirs kxploits mkdir-dist encrypt-prx copy-bin
	@echo "Build Done"

copy-bin:
#	Common installation
	$(Q)cp loader/usploit/signed_eboot/EBOOT.PBP dist/ARK_Live/EBOOT.PBP # Signed EBOOT
	$(Q)cp loader/kxploit/crossfw/psp660/K.BIN dist/ARK_Live/K.BIN # Kernel exploit for PSP
	$(Q)cp loader/usploit/vitabubble/PBOOT.PBP dist/VitaBubble/ # Vita 3.60 PBOOT.PBP bubble
	$(Q)cp loader/usploit/vitabubble/H.BIN dist/VitaBubble/ # Kernel exploit for Vita 3.60
	$(Q)cp loader/kxploit/crossfw/vita360/K.BIN dist/VitaBubble/K.BIN # Kernel exploit for Vita 3.60+
	$(Q)cp loader/pro_updater/EBOOT.PBP dist/ARK_PRO_Updater/EBOOT.PBP # PRO Updater
	$(Q)cp -r contrib/PSP/SAVEDATA/ARK_01234/ dist/ # ARK Savedata installation
	$(Q)cp loader/kxploit/compat/ARK.BIN dist/ARK_01234/ARK.BIN # ARK-2 chainloader
	$(Q)cp loader/kxploit/kernel_loader/ARK4.BIN dist/ARK_01234/ARK4.BIN # ARK-4 loader
	$(Q)cp loader/kxploit/crossfw/dummy/K.BIN dist/ARK_01234/K.BIN # Dummy Kernel exploit
	$(Q)cp loader/rebootex/reboot.bin.gz dist/ARK_01234/REBOOT.BIN # Kernel exploit for PS Vita
	$(Q)cp extras/menus/recovery/EBOOT.PBP dist/ARK_01234/RECOVERY.PBP # Default recovery menu
	$(Q)cp extras/menus/arkMenu/EBOOT.PBP dist/ARK_01234/MENU.PBP # Default launcher
	$(Q)cp extras/menus/xMenu/EBOOT.PBP dist/ARK_01234/XMENU.PBP # PS1 launcher
	$(Q)cp extras/menus/arkMenu/themes/classic/DATA.PKG dist/ARK_01234/DATA.PKG # Launcher and Recovery resources
	$(Q)cp extras/menus/vshmenu/satelite.prx dist/ARK_01234/VSHMENU.PRX # Default vsh menu
	$(Q)cp loader/usploit/linkless_payload/H.BIN dist/ARK_01234/H.BIN # game exploit loader
	$(Q)cp core/compat/psp/pspcompat.prx dist/ARK_01234/PSPCOMP.PRX # PSP Compat layer
	$(Q)mv dist/FLASH0.ARK dist/ARK_01234/ # flash0 package
	
encrypt-prx: \
	dist/SYSCTRL.BIN dist/VSHCTRL.BIN dist/INFERNO.BIN dist/STARGATE.BIN dist/POPCORN.BIN dist/VITACOMP.BIN dist/VITAPOPS.BIN
	$(Q)cp contrib/PC/btcnf/psvbtinf.bin dist/PSVBTINF.BIN
	$(Q)cp contrib/PC/btcnf/psvbtcnf.bin dist/PSVBTCNF.BIN
	$(Q)cp contrib/PC/btcnf/psvbtxnf.bin dist/PSVBTXNF.BIN
	$(Q)$(PYTHON) contrib/PC/pack/pack.py -p dist/FLASH0.ARK contrib/PC/pack/packlist.txt


kxploits:
	$(Q)$(MAKE) $@ K=dummy -C loader/kxploit
	$(Q)$(MAKE) $@ K=psp660 -C loader/kxploit
	$(Q)$(MAKE) $@ K=vita320 -C loader/kxploit
	$(Q)$(MAKE) $@ K=vita360 -C loader/kxploit

# Only clean non-library code
cleanobj:
	$(Q)$(MAKE) clean CLEANOBJ=1

clean:
	$(Q)$(MAKE) $@ -C libs
	$(Q)$(MAKE) $@ -C loader/usploit/linkless_payload
	$(Q)$(MAKE) $@ -C loader/usploit/signed_eboot
	$(Q)$(MAKE) $@ -C loader/pro_updater
	$(Q)$(MAKE) $@ -C loader/kxploit/kernel_loader
	$(Q)$(MAKE) $@ -C loader/kxploit/compat
	$(Q)$(MAKE) $@ -C loader/kxploit/kram_dumper
	$(Q)$(MAKE) $@ -C loader/kxploit/vita_flash_dumper
	$(Q)$(MAKE) $@ -C loader/rebootex
	$(Q)$(MAKE) $@ -C core/systemctrl
	$(Q)$(MAKE) $@ -C core/vshctrl
	$(Q)$(MAKE) $@ -C core/stargate
	$(Q)$(MAKE) $@ -C core/popcorn
	$(Q)$(MAKE) $@ -C core/inferno
	$(Q)$(MAKE) $@ -C core/compat/psp
	$(Q)$(MAKE) $@ -C core/compat/vita
	$(Q)$(MAKE) $@ -C core/compat/vitapops
	$(Q)$(MAKE) $@ -C extras/menus/recovery
	$(Q)$(MAKE) $@ -C extras/menus/arkMenu
	$(Q)$(MAKE) $@ -C extras/menus/vshmenu
	$(Q)$(MAKE) $@ -C extras/menus/provsh
	$(Q)$(MAKE) $@ K=dummy -C loader/kxploit
	$(Q)$(MAKE) $@ K=psp660 -C loader/kxploit
	$(Q)$(MAKE) $@ K=vita320 -C loader/kxploit
	$(Q)$(MAKE) $@ K=vita360 -C loader/kxploit
	$(Q)$(MAKE) $@ -C contrib/PC/btcnf/
	$(Q)-rm -rf dist *~ | true
	$(Q)$(PYTHON) cleandeps.py

subdirs: $(SUBDIRS)

$(filter-out libs, $(SUBDIRS)): libs
	$(Q)$(MAKE) $(OPT) -C $@

libs:
	$(Q)$(MAKE) $(OPT) -C $@

arkmenu: libs
	$(Q)$(MAKE) $@ -C extras/menus/arkMenu

xmenu: libs
	$(Q)$(MAKE) $@ -C extras/menus/xMenu

recovery: libs
	$(Q)$(MAKE) $@ -C extras/menus/recovery

mkdir-dist:
	$(Q)mkdir dist | true
	$(Q)mkdir dist/VitaBubble | true
	$(Q)mkdir dist/ARK_Live | true
	$(Q)mkdir dist/ARK_PRO_Updater | true

-include $(ARKROOT)/.config
include $(ARKROOT)/common/make/quiet.mak
include $(ARKROOT)/common/make/mod_enc.mak
