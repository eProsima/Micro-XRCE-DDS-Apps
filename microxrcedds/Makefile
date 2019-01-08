-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

MICRO_XRCE_DIR = $(APPDIR)/microxrcedds/Micro-XRCE-DDS-Client
MICRO_XRCE_INSTALL_DIR = $(MICRO_XRCE_DIR)/build/install

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
else
  BIN = ../libapps$(LIBEXT)
endif
endif

# Build type flag
ifeq ($(CONFIG_DEBUG_SYMBOLS),y)
	BUILD_TYPE = Debug
else
	BUILD_TYPE = Release
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

# Read Access profile flag
ifeq ($(CONFIG_UXR_PROFILE_READ_ACCESS),y)
	PROFILE_READ_ACCESS = TRUE
else
	PROFILE_READ_ACCESS = FALSE
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

# Serialization endianness flag
ifeq ($(CONFIG_ENDIAN_BIG),y)
	CONFIG_SERIALIZATION_ENDIANNESS = 1
else
	CONFIG_SERIALIZATION_ENDIANNESS = 0
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
	$(call DELDIR, $(MICRO_XRCE_DIR)/build/extracted)
	$(Q) mkdir $(MICRO_XRCE_DIR)/build/extracted
	$(Q) cd $(MICRO_XRCE_DIR)/build/extracted; \
		$(ARCROSSDEV)ar -x $(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a; \
		$(ARCROSSDEV)ar -x $(MICRO_XRCE_INSTALL_DIR)/lib/libmicrocdr.a;
	$(Q) $(AR) $(BIN) $(MICRO_XRCE_DIR)/build/extracted/*.obj
	$(Q) touch .built

cmakecompile: $(MICRO_XRCE_DIR) $(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a

$(MICRO_XRCE_DIR):
	@echo "Cloning eProsima/Micro-XRCE-DDS-Client"
	$(Q) git clone -c remote.origin.fetch='+refs/pull/*:refs/remotes/origin/pull/*' --recursive https://github.com/eProsima/Micro-XRCE-DDS-Client && \
		cd Micro-XRCE-DDS-Client && \
		git fetch && \
		git checkout $(CONFIG_UXR_BRANCH) 

$(MICRO_XRCE_INSTALL_DIR)/lib/libmicroxrcedds_client.a: $(MICRO_XRCE_DIR)/build/CMakeCache.txt
	$(Q) cd $(MICRO_XRCE_DIR)/build; \
		 make install

$(MICRO_XRCE_DIR)/build/CMakeCache.txt: $(TOPDIR)/.config
	$(Q) echo 'PROFILE_CREATE_ENTITIES_REF=$(PROFILE_CREATE_ENTITIES_REF)' > $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_CREATE_ENTITIES_XML=$(PROFILE_CREATE_ENTITIES_XML)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_WRITE_ACCESS=TRUE' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_READ_ACCESS=$(PROFILE_READ_ACCESS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_SERIAL_TRANSPORT=$(PROFILE_SERIAL_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_UDP_TRANSPORT=$(PROFILE_UDP_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'PROFILE_TCP_TRANSPORT=$(PROFILE_TCP_TRANSPORT)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_SERIAL_TRANSPORT_MTU=$(CONFIG_UXR_SERIAL_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_UDP_TRANSPORT_MTU=$(CONFIG_UXR_UDP_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_TCP_TRANSPORT_MTU=$(CONFIG_UXR_TCP_TRANSPORT_MTU)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MAX_OUTPUT_BEST_EFFORT_STREAMS=$(CONFIG_UXR_MAX_OUTPUT_BEST_EFFORT_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MAX_OUTPUT_RELIABLE_STREAMS=$(CONFIG_UXR_MAX_OUTPUT_RELIABLE_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MAX_INPUT_BEST_EFFORT_STREAMS=$(CONFIG_UXR_MAX_INPUT_BEST_EFFORT_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MAX_INPUT_RELIABLE_STREAMS=$(CONFIG_UXR_MAX_INPUT_RELIABLE_STREAMS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MAX_SESSION_CONNECTION_ATTEMPTS=$(CONFIG_UXR_MAX_SESSION_CONNECTION_ATTEMPTS)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MIN_SESSION_CONNECTION_INTERVAL=$(CONFIG_UXR_MIN_SESSION_CONNECTION_INTERVAL)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_MIN_HEARTBEAT_TIME_INTERVAL=$(CONFIG_UXR_MIN_HEARTBEAT_TIME_INTERVAL)' >> $(MICRO_XRCE_DIR)/client.config; \
         echo 'CONFIG_SERIALIZATION_ENDIANNESS=$(CONFIG_SERIALIZATION_ENDIANNESS)' >> $(MICRO_XRCE_DIR)/client.config
	$(Q) mkdir -p $(MICRO_XRCE_DIR)/build
	$(Q) cd $(MICRO_XRCE_DIR)/build; \
		 cmake -DCMAKE_TOOLCHAIN_FILE=$(MICRO_XRCE_DIR)/toolchains/nuttx_toolchain.cmake \
			-DCMAKE_SYSROOT=$(TOPDIR) \
			-DCROSSDEV=$(CROSSDEV) \
			-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
			-DVERBOSE_SERIALIZATION=$(VERBOSE_OPT) \
			-DVERBOSE_MESSAGE=$(VERBOSE_OPT) \
			-DCHECK_ENDIANNESS=OFF \
			-DTHIRDPARTY=ON \
			-DARCH_CPU_FLAGS="$(ARCHCPUFLAGS)" \
			-DARCH_OPT_FLAGS="$(ARCHOPTIMIZATION)" \
			-DCMAKE_INSTALL_PREFIX=$(MICRO_XRCE_INSTALL_DIR) ..; \

install: 
	$(Q) cd $(MICRO_XRCE_DIR)/build/; \
		 make install

context: cmakecompile 

.depend:

depend: .depend

clean:
	$(call DELDIR, $(MICRO_XRCE_DIR)/build)
	$(call DELFILE, $(MICRO_XRCE_DIR)/.built)

distclean: clean
	$(call DELDIR, $(MICRO_XRCE_DIR))

preconfig:

-include Make.dep
