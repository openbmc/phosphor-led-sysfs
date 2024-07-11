project(
    'phosphor-ledcontroller', 'cpp',
    version : '1.0.0',
    meson_version: '>=1.1.1',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++23',
        'buildtype=debugoptimized'
    ]
)

sdbusplus_dep = dependency('sdbusplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
boost = dependency('boost', include_type: 'system')
phosphor_logging_dep = dependency('phosphor-logging')

cxx = meson.get_compiler('cpp')
if cxx.has_header('CLI/CLI.hpp')
    cli11_dep = declare_dependency()
else
    cli11_dep = dependency('CLI11')
endif

deps = [
    cli11_dep,
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
    boost,
    phosphor_logging_dep,
]

udevdir = dependency('udev').get_variable('udevdir')
install_data(['udev' / 'rules.d' / '70-leds.rules'], install_dir : udevdir / 'rules.d')

systemd = dependency('systemd')
install_data(['systemd' / 'system' / 'xyz.openbmc_project.led.controller@.service'],
             install_dir: systemd.get_variable('systemdsystemunitdir')
)

sources = [
    'controller.cpp',
    'physical.cpp',
    'sysfs.cpp',
]

executable(
    'phosphor-ledcontroller',
    sources,
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: '/usr/libexec/phosphor-led-sysfs'
)

if get_option('tests').allowed()
  subdir('test')
endif
