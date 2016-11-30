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
#include "physical.hpp"
namespace phosphor
{
namespace led
{

/** @brief Overloaded State Property Setter function */
auto Physical::state(Action value) -> Action
{
    action = value;
    // Set the base class's state to actuals since the getter
    // operation is handled there.
    return sdbusplus::xyz::openbmc_project::Led::server
                         ::Physical::state(driveLED());
}

/** @brief Run through the map and apply action on the LEDs */
auto Physical::driveLED() -> Action
{
    // Replace with actual code.
    std::cout << " Drive LED STUB :: " << std::endl;
    return action;
}

/** @brief Initialize the bus and announce services */
Physical::Physical(
    sdbusplus::bus::bus& bus,
    const std::string& objPath,
    const std::string& ledName,
    const std::string& ledPath) :

    sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Physical>(
        bus, objPath.c_str()),
    name(ledName),
    path(ledPath),
    action(sdbusplus::xyz::openbmc_project::Led::server
           ::Physical::state())
{
    // Nothing to do here
}

} // namespace led
} // namespace phosphor
