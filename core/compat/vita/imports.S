	.set noreorder

#include "pspimport.s"

    IMPORT_START "SystemCtrlPrivate",0x00090000
	IMPORT_FUNC  "SystemCtrlPrivate",0x5D7817CF,msstorCacheInit
	IMPORT_FUNC  "SystemCtrlPrivate",0xD31A2704,loadSettings
	IMPORT_FUNC  "SystemCtrlPrivate",0xBC7554DF,strcasecmp
	IMPORT_FUNC  "SystemCtrlPrivate",0x6EAFC03D,sctrlHENSetArkConfig

	IMPORT_START "SystemCtrlForKernel",0x00090000
	IMPORT_FUNC  "SystemCtrlForKernel",0x745286D1,sctrlHENSetMemory
