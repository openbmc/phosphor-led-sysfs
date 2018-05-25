/**
 * Copyright © 2016,2018 IBM Corporation
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

#include "physical.hpp"

#include <cassert>
#include <iostream>
#include <string>
namespace phosphor
{
namespace led
{

/** @brief Populates key parameters */
void Physical::setInitialState()
{
    auto trigger = led.getTrigger();
    if (trigger == "timer")
    {
        // LED is blinking. Get the on and off delays and derive percent duty
        auto delayOn = led.getDelayOn();
        periodMs = delayOn + led.getDelayOff();
        auto percentScale = periodMs / 100;
        this->dutyOn(delayOn / percentScale);
    }
    else
    {
        // TODO: Periodicity is hardcoded for now. This will be changed to
        // this->period() when configurable periodicity is implemented.
        periodMs = 1000;

        // Cache current LED state
        auto brightness = led.getBrightness();
        if (brightness == ASSERT)
        {
            sdbusplus::xyz::openbmc_project::Led::server::Physical::state(
                Action::On);
        }
        else
        {
            sdbusplus::xyz::openbmc_project::Led::server::Physical::state(
                Action::Off);
        }
    }
    return;
}

auto Physical::state(Action value) -> Action
{
    auto current =
        sdbusplus::xyz::openbmc_project::Led::server::Physical::state();

    auto requested =
        sdbusplus::xyz::openbmc_project::Led::server::Physical::state(value);

    driveLED(current, requested);

    return value;
}

void Physical::driveLED(Action current, Action request)
{
    if (current == request)
    {
        return;
    }

    if (request == Action::On || request == Action::Off)
    {
        return stableStateOperation(request);
    }

    assert(request == Action::Blink);
    blinkOperation();
}

void Physical::stableStateOperation(Action action)
{
    auto value = (action == Action::On) ? ASSERT : DEASSERT;

    led.setTrigger("none");
    led.setBrightness(value);
    return;
}

void Physical::blinkOperation()
{
    auto dutyOn = this->dutyOn();

    // Convert percent duty to milliseconds for sysfs interface
    auto factor = periodMs / 100;
    led.setDelayOn(dutyOn * factor);
    led.setDelayOff((100 - dutyOn) * factor);

    led.setTrigger("timer");
    return;
}

} // namespace led
} // namespace phosphor
