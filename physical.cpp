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

/** @brief Populates key parameters */
void Physical::setInitialState()
{
    // 1. read /sys/class/leds/name/trigger
    // 2. If its 'timer', then its blinking.
    //    2.1: On blink, use delay_on and delay_off into dutyOn
    // 3. If its 'none', then read brightness. 255 means, its ON, else OFF.
    // Implementation in the next patchset.
}

/** @brief Overloaded State Property Setter function */
auto Physical::state(Action value) -> Action
{
    // Set the base class's state to actuals since the getter
    // operation is handled there.
    auto action = this->state(value);

    // Apply the action.
    driveLED();

    return action;
}

/** @brief apply action on the LED */
void Physical::driveLED()
{
    // Replace with actual code.
    std::cout << " Drive LED  STUB :: \n";
    return;
}

} // namespace led
} // namespace phosphor
