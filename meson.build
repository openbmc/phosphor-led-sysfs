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

sdbusplusplus_prog = find_program('sdbus++', native: true)
sdbuspp_gen_meson_prog = find_program('sdbus++-gen-meson', native: true)
sdbusplusplus_depfiles = files()
if sdbusplus_dep.type_name() == 'internal'
    sdbusplusplus_depfiles = subproject('sdbusplus').get_variable('sdbusplusplus_depfiles')
endif

generated_sources = []
generated_others = []
subdir('gen')
subdir('gen/xyz')

deps = [
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
]

udevdir = dependency('udev').get_variable(pkgconfig: 'udevdir')
install_data(['70-leds.rules'], install_dir : udevdir / 'rules.d')

systemd = dependency('systemd')
install_data(
  ['xyz.openbmc_project.led.controller@.service'],
  install_dir: systemd.get_variable(pkgconfig: 'systemdsystemunitdir')
)

sources = [
    generated_sources,
    files(
        'argument.cpp',
        'controller.cpp',
        'physical.cpp',
        'sysfs.cpp',
    )
]

executable(
    'phosphor-ledcontroller',
    sources,
    include_directories: include_directories('gen'),
    implicit_include_directories: true,
    dependencies: deps,
    install: true,
    install_dir: '/usr/libexec/phosphor-led-sysfs'
)

build_tests = get_option('tests')
if build_tests.enabled()
  subdir('test')
endif
