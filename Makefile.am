sbin_PROGRAMS = phosphor-ledcontroller

phosphor_ledcontroller_SOURCES = \
				controller.cpp  \
				argument.cpp 	\
				physical.cpp

phosphor_ledcontroller_LDFLAGS = $(SDBUSPLUS_LIBS) \
                                 $(PHOSPHOR_DBUS_INTERFACES_LIBS)
phosphor_ledcontroller_CFLAGS =  $(SDBUSPLUS_CFLAGS) \
                                 $(PHOSPHOR_DBUS_INTERFACES_CFLAGS)
