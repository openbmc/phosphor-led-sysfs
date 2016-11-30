sbin_PROGRAMS = phosphor-ledcontroller
AM_CPPFLAGS = -I$(top_srcdir)/../sdbusplus

phosphor_ledcontroller_SOURCES = controller.cpp argument.cpp

phosphor_ledcontroller_LDFLAGS = $(SYSTEMD_LIBS)
phosphor_ledcontroller_CFLAGS = $(SYSTEMD_CFLAGS)
