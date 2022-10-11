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

sdbusplus_dep = dependency('sdbusplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
phosphor_logging_dep = dependency('phosphor-logging')

deps = [
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
]

udevdir = dependency('udev').get_variable(pkgconfig: 'udevdir')
install_data(['70-leds.rules'], install_dir : udevdir / 'rules.d')

systemd = dependency('systemd')
install_data(
  ['xyz.openbmc_project.led.controller@.service', 'sysfs-led@.service'],
  install_dir: systemd.get_variable(pkgconfig: 'systemdsystemunitdir')
)

sources = [
    'argument.cpp',
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

executable(
    'add-led-action',
    'argument.cpp',
    'add_led_action.cpp',
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: '/usr/libexec/phosphor-led-sysfs'
)

build_tests = get_option('tests')
if build_tests.enabled()
  subdir('test')
endif
