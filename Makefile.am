sbin_PROGRAMS = phosphor-ledcontroller

phosphor_ledcontroller_SOURCES = \
				controller.cpp  \
				argument.cpp 	\
				physical.cpp	\
				xyz.openbmc_project.Led.Physical.cpp

phosphor_ledcontroller_LDFLAGS = $(SDBUSPLUS_LIBS)
phosphor_ledcontroller_CFLAGS = $(SDBUSPLUS_CFLAGS)
