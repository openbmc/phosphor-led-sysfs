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

#include "physical.hpp"
#include "sysfs.hpp"

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iostream>
#include <string>

static constexpr auto busName = "xyz.openbmc_project.LED.Controller";
static constexpr auto rootPath = "/xyz/openbmc_project/led";

int main()
{
    // Get a handle to system dbus
    auto bus = sdbusplus::bus::new_default();

    // Add the ObjectManager interface
    sdbusplus::server::manager_t objManager(bus, rootPath);

    // Request service bus name
    bus.request_name(busName);

    while (true)
    {
        // Handle dbus message / signals discarding unhandled
        bus.process_discard();
        bus.wait();
    }
    return 0;
}
