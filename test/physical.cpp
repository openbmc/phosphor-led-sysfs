#include <gtest/gtest.h>
#include <sdbusplus/bus.hpp>

#include "physical.hpp"

constexpr auto LED_OBJ = "/foo/bar/led";
constexpr auto LED_SYSFS = "/sys/class/leds/test";

using Action = sdbusplus::xyz::openbmc_project::Led::server::Physical::Action;

TEST(Physical, ctor)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::Physical led(bus, LED_OBJ, LED_SYSFS);
}

TEST(Physical, off)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::Physical led(bus, LED_OBJ, LED_SYSFS);
    led.state(Action::Off);
}

TEST(Physical, on)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::Physical led(bus, LED_OBJ, LED_SYSFS);
    led.state(Action::On);
}

TEST(Physical, blink)
{
    sdbusplus::bus::bus bus = sdbusplus::bus::new_default();
    phosphor::led::Physical led(bus, LED_OBJ, LED_SYSFS);
    led.state(Action::Blink);
}
