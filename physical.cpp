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
#include <cstring>
#include <unistd.h>
#include <algorithm>
#include <sdbusplus/message.hpp>
#include "physical.hpp"
#include "config.h"
namespace phosphor
{
namespace led
{

/** @brief Initialize the bus and announce services */
Physical::Physical(
    sdbusplus::bus::bus& bus,
    const char* busName,
    const char* objPath,
    const std::string& ledName,
    const std::string& ledPath) :

    sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Physical>(
        bus, objPath),
    name(ledName),
    path(ledPath)
{
    // Nothing to do here
}

} // namespace led
} // namespace phosphor
