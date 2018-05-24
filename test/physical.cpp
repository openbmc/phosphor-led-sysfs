#include "physical.hpp"

#include <sdbusplus/bus.hpp>

#include <gtest/gtest.h>

constexpr auto LED_OBJ = "/foo/bar/led";
constexpr auto LED_SYSFS = "/sys/class/leds/test";

using Action = sdbusplus::xyz::openbmc_project::Led::server::Physical::Action;
namespace fs = std::experimental::filesystem;

TEST(Physical, ctor)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::SysfsLed led{fs::path(LED_SYSFS)};
    phosphor::led::Physical phy(bus, LED_OBJ, led);
}

TEST(Physical, off)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::SysfsLed led{fs::path(LED_SYSFS)};
    phosphor::led::Physical phy(bus, LED_OBJ, led);
    phy.state(Action::Off);
}

TEST(Physical, on)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::SysfsLed led{fs::path(LED_SYSFS)};
    phosphor::led::Physical phy(bus, LED_OBJ, led);
    phy.state(Action::On);
}

TEST(Physical, blink)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::SysfsLed led{fs::path(LED_SYSFS)};
    phosphor::led::Physical phy(bus, LED_OBJ, led);
    phy.state(Action::Blink);
}
