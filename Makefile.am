sbin_PROGRAMS = phosphor-ledcontroller

phosphor_ledcontroller_SOURCES = \
				controller.cpp  \
				argument.cpp 	\
				physical.cpp

phosphor_ledcontroller_LDFLAGS = $(SYSTEMD_LIBS) $(SDBUSPLUS_LIBS) $(PHOSPHOR_DBUS_INTERFACES_LIBS)
phosphor_ledcontroller_CFLAGS = $(SYSTEMD_CFLAGS) $(SDBUSPLUS_CFLAGS) $(PHOSPHOR_DBUS_INTERFACES_CFLAGS)
