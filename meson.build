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
sdeventplus_dep = dependency('sdeventplus')

deps = [
    sdbusplus_dep,
    phosphor_dbus_interfaces_dep,
    sdeventplus_dep,
]

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
    install_dir: '/usr/sbin'
)

build_tests = get_option('tests')
if build_tests.enabled()
  subdir('test')
endif
