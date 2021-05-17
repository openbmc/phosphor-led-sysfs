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

sdbusplus_dep = dependency(
    'sdbusplus',
    fallback: [
        'sdbusplus',
        'sdbusplus_dep'
    ],
)
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
    install_dir: get_option('bindir')
)

build_tests = get_option('tests')
if not build_tests.disabled()
  subdir('test')
endif