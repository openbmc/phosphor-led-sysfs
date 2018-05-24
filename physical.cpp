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

    auto trigger = led.getTrigger();
    if (trigger == "timer")
    {
        // LED is blinking. Get the delay_on and delay_off and compute
        // DutyCycle. sfsfs values are in strings. Need to convert 'em over to
        // integer.
        auto delayOn = led.getDelayOn();
        auto delayOff = led.getDelayOff();

        // Calculate frequency and then percentage ON
        frequency = delayOn + delayOff;
        auto factor = frequency / 100;
        auto dutyOn = delayOn / factor;

        // Update.
        this->dutyOn(dutyOn);
    }
    else
    {
        // This is hardcoded for now. This will be changed to this->frequency()
        // when frequency is implemented.
        // TODO
        frequency = 1000;

        // LED is either ON or OFF
        auto brightness = led.getBrightness();
        if (brightness == ASSERT)
        {
            // LED is in Solid ON
            sdbusplus::xyz::openbmc_project::Led::server ::Physical::state(
                Action::On);
        }
        else
        {
            // LED is in OFF state
            sdbusplus::xyz::openbmc_project::Led::server ::Physical::state(
                Action::Off);
        }
    }
    return;
}

/** @brief Overloaded State Property Setter function */
auto Physical::state(Action value) -> Action
{
    // Obtain current operation
    auto current =
        sdbusplus::xyz::openbmc_project::Led::server ::Physical::state();

    // Update requested operation into base class
    auto requested =
        sdbusplus::xyz::openbmc_project::Led::server ::Physical::state(value);

    // Apply the action.
    driveLED(current, requested);

    return value;
}

/** @brief apply action on the LED */
void Physical::driveLED(Action current, Action request)
{
    if (current == request)
    {
        // Best we can do here is ignore.
        return;
    }

    // Transition TO Blinking state
    if (request == Action::Blink)
    {
        return blinkOperation();
    }

    // Transition TO Stable states.
    if (request == Action::On || request == Action::Off)
    {
        return stableStateOperation(request);
    }
    return;
}

/** @brief Either TurnON -or- TurnOFF */
void Physical::stableStateOperation(Action action)
{
    auto value = (action == Action::On) ? ASSERT : DEASSERT;

    // Write "none" to trigger to clear any previous action
    led.setTrigger("none");

    // And write the current command
    led.setBrightness(value);
    return;
}

/** @brief BLINK the LED */
void Physical::blinkOperation()
{
    // Get the latest dutyOn that the user requested
    auto dutyOn = this->dutyOn();

    // Write "timer" to "trigger" file
    led.setTrigger("timer");

    // Write DutyON. Value in percentage 1_millisecond.
    // so 50% input becomes 500. Driver wants string input
    auto percentage = frequency / 100;
    led.setDelayOn(dutyOn * percentage);

    // Write DutyOFF. Value in milli seconds so 50% input becomes 500.
    led.setDelayOff((100 - dutyOn) * percentage);
    return;
}

} // namespace led
} // namespace phosphor
