Prepare : $(BUILD_DIR)/RtErrorlogElink.h

$(BUILD_DIR)/RtErrorlogElink.h : $(BUILD_DIR)/token.mak
	$(ECHO) \
"#define MEM_ERROR_INIT_FUNCTION_LIST $(MemErrorInitFunctions)$(EOL)\
#define PCI_ERROR_INIT_FUNCTION_LIST $(PciErrorInitFunctions)$(EOL)\
#define MEM_ERROR_HANDLER_FUNCTION_LIST $(MemErrorHandlers)$(EOL)\
#define CORE_ERROR_HANDLER_FUNCTION_LIST $(CoreErrorHandlers)$(EOL)\
#define QPI_ERROR_HANDLER_FUNCTION_LIST $(QpiErrorHandlers)$(EOL)\
#define CBO_ERROR_HANDLER_FUNCTION_LIST $(CboErrorHandlers)$(EOL)\
#define IOHQPI_ERROR_HANDLER_FUNCTION_LIST $(IohQpiErrorHandlers)$(EOL)\
#define IOHCORE_ERROR_HANDLER_FUNCTION_LIST $(IohCoreErrorHandlers)$(EOL)\
#define IOHVTD_ERROR_HANDLER_FUNCTION_LIST $(IohVtdErrorHandlers)$(EOL)\
#define PCI_ERROR_HANDLER_FUNCTION_LIST $(PciErrorHandlers)$(EOL)\
$(EOL)"\
> $@