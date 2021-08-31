-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

CONFIG_UXR_BRANCH = develop

MICRO_XRCE_DIR = $(APPDIR)/microxrcedds/Micro-XRCE-DDS-Client
MICRO_XRCE_INSTALL_DIR = $(MICRO_XRCE_DIR)/build/install

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
  INC = ..\include
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
  INC = ..\\include
else
  BIN = ../libapps$(LIBEXT)
  INC = ../include
endif
endif

# Build type flag
ifeq ($(CONFIG_DEBUG_SYMBOLS),y)
	BUILD_TYPE = Debug
else
	BUILD_TYPE = Release
endif

# Shared memory flag
ifeq ($(CONFIG_UXR_SHARED_MEMORY),y)
	PROFILE_SHARED_MEMORY = TRUE
else
	PROFILE_SHARED_MEMORY = FALSE
endif

# Create Entities by REF profile flag
ifeq ($(CONFIG_UXR_PROFILE_CREATE_ENTITIES_REF),y)
	PROFILE_CREATE_ENTITIES_REF = TRUE
else
	PROFILE_CREATE_ENTITIES_REF = FALSE
endif

# Create Entities by XML profile flag
ifeq ($(CONFIG_UXR_PROFILE_CREATE_ENTITIES_XML),y)
	PROFILE_CREATE_ENTITIES_XML = TRUE
else
	PROFILE_CREATE_ENTITIES_XML = FALSE
endif

# Serial Transport profile flag
ifeq ($(CONFIG_UXR_PROFILE_SERIAL_TRANSPORT),y)
	PROFILE_SERIAL_TRANSPORT = TRUE
else
	PROFILE_SERIAL_TRANSPORT = FALSE
endif

# UDP Transport profile flag
ifeq ($(CONFIG_UXR_PROFILE_UDP_TRANSPORT),y)
	PROFILE_UDP_TRANSPORT = TRUE
else
	PROFILE_UDP_TRANSPORT = FALSE
endif

# TCP Transport profile flag
ifeq ($(CONFIG_UXR_PROFILE_TCP_TRANSPORT),y)
	PROFILE_TCP_TRANSPORT = TRUE
else
	PROFILE_TCP_TRANSPORT = FALSE
endif

# Discovery profile flag
ifeq ($(CONFIG_UXR_PROFILE_DISCOVERY),y)
	PROFILE_DISCOVERY = TRUE
else
	PROFILE_DISCOVERY = FALSE
endif

# Serialization endianness flag
ifeq ($(CONFIG_ENDIAN_BIG),y)
	BIG_SERIALIZATION_ENDIANNESS = TRUE
else
	BIG_SERIALIZATION_ENDIANNESS = FALSE
endif

# Verbose flag
ifeq ($(CONFIG_UXR_VERBOSE_OUTPUT),y)
	VERBOSE_OPT = ON
else
	VERBOSE_OPT = OFF
endif

all: .built
  .PHONY: clean depend distclean preconfig
  .PRECIOUS: ../../libapps$(LIBEXT)

.built: cmakecompile


cmakecompile: $(MICRO_XRCE_DIR) $(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a
	@echo "Installing Micro-XRCE-DDS"
	$(call DELDIR, $(MICRO_XRCE_DIR)/build/extracted)
	$(Q) mkdir $(MICRO_XRCE_DIR)/build/extracted
	$(Q) cd $(MICRO_XRCE_DIR)/build/extracted; \
		$(ARCROSSDEV)ar -x $(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a; \
		$(ARCROSSDEV)ar -x $(MICRO_XRCE_INSTALL_DIR)/lib/libmicrocdr.a;
	$(Q) $(AR) $(BIN) $(MICRO_XRCE_DIR)/build/extracted/*.obj
	$(Q) cp -R $(MICRO_XRCE_INSTALL_DIR)/include/* $(APPDIR)/include
	$(Q) touch .built

$(MICRO_XRCE_DIR):
	@echo "Cloning eProsima/Micro-XRCE-DDS-Client"
	$(Q) git clone https://github.com/eProsima/Micro-XRCE-DDS-Client && \
		cd Micro-XRCE-DDS-Client && \
		git fetch && \
		git checkout $(CONFIG_UXR_BRANCH)

$(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a: $(MICRO_XRCE_DIR)/build/CMakeCache.txt
	$(Q) cd $(MICRO_XRCE_DIR)/build; \
		 make && make install

$(MICRO_XRCE_DIR)/build/CMakeCache.txt: $(MICRO_XRCE_DIR) $(TOPDIR)/.config
	$(Q) echo 'PROFILE_CREATE_ENTITIES_REF=$(PROFILE_CREATE_ENTITIES_REF)' > $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_CREATE_ENTITIES_XML=$(PROFILE_CREATE_ENTITIES_XML)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_PROFILE_SERIAL=$(PROFILE_SERIAL_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_PROFILE_UDP=$(PROFILE_UDP_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_PROFILE_TCP=$(PROFILE_TCP_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_PROFILE_DISCOVERY=$(PROFILE_DISCOVERY)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_SERIAL_TRANSPORT_MTU=$(CONFIG_UXR_SERIAL_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_UDP_TRANSPORT_MTU=$(CONFIG_UXR_UDP_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_TCP_TRANSPORT_MTU=$(CONFIG_UXR_TCP_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MAX_OUTPUT_BEST_EFFORT_STREAMS=$(CONFIG_UXR_MAX_OUTPUT_BEST_EFFORT_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MAX_OUTPUT_RELIABLE_STREAMS=$(CONFIG_UXR_MAX_OUTPUT_RELIABLE_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MAX_INPUT_BEST_EFFORT_STREAMS=$(CONFIG_UXR_MAX_INPUT_BEST_EFFORT_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MAX_INPUT_RELIABLE_STREAMS=$(CONFIG_UXR_MAX_INPUT_RELIABLE_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MAX_SESSION_CONNECTION_ATTEMPTS=$(CONFIG_UXR_MAX_SESSION_CONNECTION_ATTEMPTS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MIN_SESSION_CONNECTION_INTERVAL=$(CONFIG_UXR_MIN_SESSION_CONNECTION_INTERVAL)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_MIN_HEARTBEAT_TIME_INTERVAL=$(CONFIG_UXR_MIN_HEARTBEAT_TIME_INTERVAL)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'UCLIENT_BIG_ENDIANNESS=$(BIG_SERIALIZATION_ENDIANNESS)' >> $(MICRO_XRCE_DIR)/client.config; \
		 echo 'UCLIENT_PROFILE_SHARED_MEMORY=$(PROFILE_SHARED_MEMORY)' >> $(MICRO_XRCE_DIR)/client.config; \
		 echo 'UCLIENT_SHARED_MEMORY_MAX_ENTITIES=$(CONFIG_UXR_SHARED_MEMORY_MAX_ENTITIES)' >> $(MICRO_XRCE_DIR)/client.config; \
		 echo 'UCLIENT_ISOLATED_INSTALL=OFF' >> $(MICRO_XRCE_DIR)/client.config
	$(Q) mkdir -p $(MICRO_XRCE_DIR)/toolchains/build
	$(Q) cd $(MICRO_XRCE_DIR)/toolchains/build; \
		cmake .. -DCROSSDEV=$(CROSSDEV) \
			-DARCH_CPU_FLAGS="$(ARCHCPUFLAGS)" \
			-DARCH_OPT_FLAGS="$(ARCHOPTIMIZATION);"
	$(Q) mkdir -p $(MICRO_XRCE_DIR)/build
	$(Q) cd $(MICRO_XRCE_DIR)/build; \
		 cmake -DCMAKE_TOOLCHAIN_FILE=$(MICRO_XRCE_DIR)/toolchains/build/nuttx_toolchain.cmake \
			-DCMAKE_SYSROOT=$(TOPDIR) \
			-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
			-DUCLIENT_VERBOSE_SERIALIZATION=$(VERBOSE_OPT) \
			-DUCLIENT_VERBOSE_MESSAGE=$(VERBOSE_OPT) \
			-DCMAKE_INSTALL_PREFIX=$(MICRO_XRCE_INSTALL_DIR) ..; \

install:
	$(Q) cd $(MICRO_XRCE_DIR)/build/; \
		 make install

context: cmakecompile

.depend:

depend: .depend

register:

clean:
	$(call DELDIR, $(MICRO_XRCE_DIR)/build)
	$(call DELDIR, $(APPDIR)/include/uxr)
	$(call DELDIR, $(APPDIR)/include/ucdr)
	$(call DELFILE, $(MICRO_XRCE_DIR)/.built)

distclean: clean
	$(call DELDIR, $(MICRO_XRCE_DIR))

preconfig:

-include Make.dep

