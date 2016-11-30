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
#include <string>
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

/** @brief apply action on the LED */
auto Physical::driveLED() -> Action
{
    if (action == Action::On ||
        action == Action::Off)
    {
        return stableStateOperation();
    }
    else if (action == Action::Blink)
    {
        return blinkOperation();
    }
    else
    {
        throw std::runtime_error("Invalid LED operation requested");
    }
}

/** @brief Either TurnON -or- TurnOFF */
auto Physical::stableStateOperation() -> Action
{
    using namespace std::string_literals;

    auto value = (action == Action::On) ? ASSERT : DEASSERT;
    auto brightFile = path + "/"s + BRIGHTNESS;
    auto blinkFile =  path + "/"s + BLINKCTRL;

    // Write "none" to trigger to clear any previous action
    write(blinkFile, "none");

    // And write the current command
    write(brightFile, value);
    return action;
}

/** @brief BLINK the LED */
auto Physical::blinkOperation() -> Action
{
    using namespace std::string_literals;

    auto blinkFile = path + "/"s + BLINKCTRL;
    auto dutyOnFile = path + "/"s + DELAYON;
    auto dutyOffFile = path + "/"s + DELAYOFF;

    // Get the latest dutyOn that the user requested
    auto dutyOn = this->dutyOn();

    // Write "timer" to "trigger" file
    write(blinkFile, "timer");

    // Write DutyON. Value in percentage 1_millisecond.
    // so 50% input becomes 500. Driver wants string input
    write(dutyOnFile, std::to_string(dutyOn * 10));

    // Write DutyOFF. Value in milli seconds so 50% input becomes 500.
    write(dutyOffFile, std::to_string((100 - dutyOn) * 10));
    return action;
}

} // namespace led
} // namespace phosphor
