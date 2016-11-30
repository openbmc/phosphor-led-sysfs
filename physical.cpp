/**
 * Copyright © 2016 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <sdbusplus/message.hpp>
#include "physical.hpp"
#include "config.h"

namespace phosphor
{
namespace led
{

/** @brief Overloaded State Property Setter function */
auto Physical::state(Action value) -> Action
{
    iv_action = value;
    // Set the base class's state to actuals since the getter
    // operation is handled there.
    return sdbusplus::xyz::openbmc_project::Led::server::
                           Physical::state(driveLED());
}

/** @brief Overloaded DutyOn Property Setter function */
auto Physical::dutyOn(uint8_t value) -> uint8_t
{
    // Accepted values are in 0..100 range.
    if (value < 0 || value > 100)
    {
        throw std::runtime_error(value + " out of 0..100 range");
    }
    iv_dutyOn = value;

    // Set the base class's state to actuals since the getter
    // operation is handled there.
    return sdbusplus::xyz::openbmc_project::Led::server::
                           Physical::dutyOn(iv_dutyOn);
}

/** @brief Overloaded Color Property Setter function */
auto Physical::color(Palette value) -> Physical::Palette
{
    iv_color = value;
    // Set the base class's state to actuals since the getter
    // operation is handled there.
    return sdbusplus::xyz::openbmc_project::Led::server::
                           Physical::color(iv_color);
}

/** @brief apply action on the LED */
Physical::Action Physical::driveLED()
{
    if (iv_action == Action::On ||
        iv_action == Action::Off)
    {
        return stableStateOperation();
    }
    else if (iv_action == Action::Blink)
    {
        return blinkOperation();
    }
    else
    {
        throw std::runtime_error("Invalid LED operation requested");
    }
}

/** @brief Either TurnON -or- TurnOFF */
Physical::Action Physical::stableStateOperation()
{
    auto value = (iv_action == Action::On) ? "255" : "0";
    auto brightFile = iv_path + iv_name + BRIGHTNESS;
    auto blinkFile = iv_path + iv_name + BLINKCTRL;

    // Write "none" to trigger to clear any previous action
    write(blinkFile, "none");

    // And write the current command
    write(brightFile, value);
    return iv_action;
}

/** @brief BLINK the LED */
Physical::Action Physical::blinkOperation()
{
    auto blinkFile = iv_path + iv_name + BLINKCTRL;
    auto dutyOnFile = iv_path + iv_name + DELAYON;
    auto dutyOffFile = iv_path + iv_name + DELAYOFF;

    // Write "timer" to "trigger" file
    write(blinkFile, "timer");

    // Write DutyON. Value in percentage 1_millisecond.
    // so 50% input becomes 500. Driver wants string input
    write(dutyOnFile, std::to_string(iv_dutyOn * 10));

    // Write DutyOFF. Value in milli seconds so 50% input becomes 500.
    write(dutyOffFile, std::to_string((100 - iv_dutyOn) * 10));
    return iv_action;
}

/** @brief Initialize the bus and announce services */
Physical::Physical(
    sdbusplus::bus::bus &&bus,
    const char* busName,
    const char* objPath,
    std::string ledName,
    std::string ledPath) :

    sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Physical>(
        bus, objPath),
    iv_bus(std::move(bus)),
    iv_ObjManager(sdbusplus::server::manager::manager(iv_bus, objPath)),

    iv_name(ledName),
    iv_path(ledPath),
    iv_action(Action::Off),
    iv_color(Palette::Unknown),
    iv_dutyOn(50)
{
    /** @brief Claim the bus */
    iv_bus.request_name(busName);
    run();
}

/** @brief Wait for client requests */
void Physical::run() noexcept
{
    while(true)
    {
        try
        {
            iv_bus.process_discard();
            iv_bus.wait();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

} // namespace led
} // namespace phosphor
