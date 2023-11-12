GIT_VER := "$(shell git describe --abbrev=4 --dirty --always --tags)"
CC	    := gcc
CFLAGS  := -Wall -Wextra \
		   -O3 -march=native \
		   -I"include" \
		   #-lm \
		   #-rdynamic -pg \
		   #-fsanitize=address
LDFLAGS := #-rdynamic -pg \
		   #-fsanitize=address
LDLIBS  := -lm

BIN_DIR := bin
OBJ_DIR := obj
CSUFFIX := .c
.phony  :  all clean list

####### LINKING FUNCTION : $(call link,LDFLAGS,EXECUTABLE,PATH/TO/FILE.o PATH/TO/ANOTHERFILE.o)
# arguments
# 1: [IN]  linker options (LDFLAGS)
# 2: [IN]  linker libraries (LDLIBS)
# 3: [OUT] executable output
# 4: [IN]  path/to/file.o path/to/anotherfile.o
define link
	@echo link	: $@ $^
	@$(CC) $1 -o $3 $4 $2
endef

####### COMPILATION FUNCTION : $(call compile,CFLAGS,OBJ_DIR,PATH/FILE.o)
# arguments
# 1: [IN]  compile options (CFLAGS)
# 2: [OUT] object directory
# 3: [IN]  src/folder
define compile_job
$2/%.o: $3/%.c Makefile | $2
	@echo compile : $$<
	@$$(CC) $1 -DVERSION=\"$$(GIT_VER)\" -c -MMD -MP -o $$(addprefix $2/,$$(notdir $$(basename $$<)).o) $$<
endef

####### CREATE BINARY FUNCTION : $(call CFLAGS,LDFLAGS,OBJ_DIR,BIN_PATH,PATH/TO/FILE.c PATH/TO/ANOTHERFILE.c)
# arguments
# 1: [IN]  compile options
# 2: [IN]  linker options
# 3: [IN]  linker libraries
# 4: [IN]  object directory
# 5: [OUT] binary path
# 6: [IN]  source files
define binary
BIN_DIR := $$(patsubst %/,%,$$(dir $5))
OBJ_DIR := $4
C_FILES := $6
C_DIRS  := $$(patsubst %/,%,$$(dir $$(C_FILES)))
O_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .o,$$(notdir $$(basename $$(C_FILES)))))
D_FILES := $$(addprefix $$(OBJ_DIR)/,$$(addsuffix .d,$$(notdir $$(basename $$(C_FILES)))))

-include $$(D_FILES)

ifeq ($$(RULE_$$(BIN_DIR)),)
RULE_$$(BIN_DIR) := 1
$$(BIN_DIR):
	mkdir -p $$(BIN_DIR)
endif

ifeq ($$(RULE_$$(OBJ_DIR)),)
RULE_$$(OBJ_DIR) := 1
$$(OBJ_DIR):
	mkdir -p $$(OBJ_DIR)
endif

$$(foreach SRC,$$(C_DIRS),$$(eval $$(call compile_job,$1,$$(OBJ_DIR),$$(SRC))))

$5$$(XSUFFIX): $$(O_FILES) | $$(BIN_DIR)
	$$(call link,$2,$3,$$@,$$^)

endef

####### Random Windows stuff 
ifeq ($(OS),Windows_NT)
ifeq ($(XSUFFIX),)
XSUFFIX := .exe
endif
endif

################################################################################
### Targets ####################################################################
################################################################################
JOBS    := bin/main bin/test
TARGETS := $(addsuffix $(XSUFFIX),$(JOBS))
.phony  :  list
all     :  $(TARGETS)

$(eval $(call binary,$(CFLAGS),$(LDFLAGS),$(LDLIBS),$(OBJ_DIR),bin/main,src/main.c src/string_to_float.c))
$(eval $(call binary,$(CFLAGS) -DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=1e-15,$(LDFLAGS),$(LDLIBS),$(OBJ_DIR),bin/test,src/test.c src/unity.c src/string_to_float.c))

list:
	@echo possible binaries
	@$(foreach TARGET,$(TARGETS),echo "- $(TARGET)";)

################################################################################
### Cleaning ###################################################################
################################################################################
ifeq ($(OS),Windows_NT)
# Cleaning rules for Windows OS, idk if this works :)
clean:
	@del /q $(OBJ_DIR), $(TARGETS), $(BIN_DIR)
	@rmdir $(OBJ_DIR)
	@rmdir $(BIN_DIR)
else
# Cleaning rules for Unix-based OS (no clue if this works)
clean:
	@rm -rf $(OBJ_DIR) $(TARGETS) $(BIN_DIR)
endif

