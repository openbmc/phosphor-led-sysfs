project(
    'phosphor-ledcontroller', 'cpp',
    version : '1.0.0',
    meson_version: '>=0.57.0',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++20',
        'buildtype=debugoptimized'
    ]
)

conf_data = configuration_data()
conf_data.set_quoted('BUSNAME', 'xyz.openbmc_project.LED.Controller')
conf_data.set_quoted('OBJPATH', '/xyz/openbmc_project/led/physical')
conf_data.set_quoted('DEVPATH', '/sys/class/leds/')

configure_file(output: 'config.h',
    configuration: conf_data
)

sdbusplus_dep = dependency('sdbusplus', required: false)
if not sdbusplus_dep.found()
    sdbusplus_proj = subproject('sdbusplus', required: true)
    sdbusplus_dep = sdbusplus_proj.get_variable('sdbusplus_dep')
endif
phosphor_dbus_interfaces_dep = dependency(
    'phosphor-dbus-interfaces',
    fallback: [
        'phosphor-dbus-interfaces',
        'phosphor_dbus_interfaces_dep'
    ],
)
systemd = dependency('systemd')

deps = [
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
    systemd,
]

sources = [
    'controller.cpp',
	'argument.cpp',
	'physical.cpp',
	'sysfs.cpp',
]

executable(
    'phosphor-ledcontroller',
    sources,
    implicit_include_directories: true,
    dependencies: deps,
	link_args : [
      '-lstdc++fs',
    ],
    install: true,
    install_dir: get_option('bindir')
)

build_tests = get_option('tests')
if not build_tests.disabled()
  subdir('test')
endif