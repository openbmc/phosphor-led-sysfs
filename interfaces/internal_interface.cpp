/**
 * Copyright © 2020 IBM Corporation
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

#include "internal_interface.hpp"

namespace phosphor
{
namespace led
{
namespace sysfs
{
namespace interface
{

InternalInterface::InternalInterface(sdbusplus::bus_t& bus, const char* path) :
    bus(bus), serverInterface(bus, path, interface, vtable, this)
{}

void InternalInterface::addLed(std::string name)
{
    createLEDPath(name);
}

void InternalInterface::removeLed(std::string name)
{
    createLEDPath(name);
}

int InternalInterface::addLedConfigure(sd_bus_message* msg, void* context,
                                       sd_bus_error* error)
{
    if (msg == nullptr && context == nullptr)
    {
        lg2::error("Unable to configure addLed");
        return -EINVAL;
    }

    try
    {
        auto message = sdbusplus::message_t(msg);

        auto reply = message.new_method_return();
        reply.method_return();
    }
    catch (const sdbusplus::exception_t& e)
    {
        return sd_bus_error_set(error, e.name(), e.description());
    }

    return 1;
}

int InternalInterface::removeLedConfigure(sd_bus_message* msg, void* context,
                                          sd_bus_error* error)
{
    if (msg == nullptr && context == nullptr)
    {
        lg2::error("Unable to configure removeLed");
        return -EINVAL;
    }

    try
    {
        auto message = sdbusplus::message_t(msg);

        auto reply = message.new_method_return();
        reply.method_return();
    }
    catch (const sdbusplus::exception_t& e)
    {
        return sd_bus_error_set(error, e.name(), e.description());
    }

    return 1;
}

const sdbusplus::vtable::vtable_t InternalInterface::vtable[] = {
    sdbusplus::vtable::start(),
    // AddLed method takes a string parameter and returns void
    sdbusplus::vtable::method("AddLed", "s", "", addLedConfigure),
    // RemoveLed method takes a string parameter and returns void
    sdbusplus::vtable::method("RemoveLed", "s", "", removeLedConfigure),
    sdbusplus::vtable::end()};

} // namespace interface
} // namespace sysfs
} // namespace led
} // namespace phosphor
