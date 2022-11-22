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

#include "interfaces/internal_interface.hpp"
#include "physical.hpp"
#include "sysfs.hpp"

#include <boost/algorithm/string.hpp>
#include <phosphor-logging/lg2.hpp>

#include <algorithm>
#include <iostream>
#include <string>

static constexpr auto busName = "xyz.openbmc_project.LED.Controller";
static constexpr auto objectPath = "/xyz/openbmc_project/led/physical";
static constexpr auto rootPath = "/xyz/openbmc_project/led";
static constexpr auto devPath = "/sys/class/leds/";

std::unordered_map<std::string, std::shared_ptr<phosphor::led::Physical>> leds;

struct LedDescr
{
    std::string devicename;
    std::string color;
    std::string function;
};

/** @brief parse LED name in sysfs
 *  Parse sysfs LED name in format "devicename:colour:function"
 *  or "devicename:colour" or "devicename" and sets corresponding
 *  fields in LedDescr struct.
 *
 *  @param[in] name      - LED name in sysfs
 *  @param[out] ledDescr - LED description
 */
void getLedDescr(const std::string& name, LedDescr& ledDescr)
{
    std::vector<std::string> words;
    boost::split(words, name, boost::is_any_of(":"));
    try
    {
        ledDescr.devicename = words.at(0);
        ledDescr.color = words.at(1);
        ledDescr.function = words.at(2);
    }
    catch (const std::out_of_range&)
    {
        return;
    }
}

/** @brief generates LED DBus name from LED description
 *
 *  @param[in] name      - LED description
 *  @return              - DBus LED name
 */
std::string getDbusName(const LedDescr& ledDescr)
{
    std::vector<std::string> words;
    words.emplace_back(ledDescr.devicename);
    if (!ledDescr.function.empty())
        words.emplace_back(ledDescr.function);
    if (!ledDescr.color.empty())
        words.emplace_back(ledDescr.color);
    return boost::join(words, "_");
}

namespace phosphor::led::sysfs::interface
{
void InternalInterface::createLEDPath(const std::string& ledName)
{
    std::string name = ledName;

    std::string path = devPath + name;

    if (!std::filesystem::exists(fs::path(path)))
    {
        lg2::error("No such directory {PATH}", "PATH", path);
        return;
    }

    // Convert LED name in sysfs into DBus name
    LedDescr ledDescr;
    getLedDescr(name, ledDescr);
    name = getDbusName(ledDescr);

    // Unique path name representing a single LED.
    sdbusplus::message::object_path objPath = std::string(objectPath);
    objPath /= name;

    auto sled = std::make_unique<phosphor::led::SysfsLed>(fs::path(path));
    auto& physical = leds[objPath];
    physical = std::make_unique<phosphor::led::Physical>(
        bus, objPath, std::move(sled), ledDescr.color);
}
} // namespace phosphor::led::sysfs::interface

int main()
{
    // Get a handle to system dbus
    auto bus = sdbusplus::bus::new_default();

    // Add the ObjectManager interface
    sdbusplus::server::manager::manager objManager(bus, rootPath);

    // Create an led controller object
    phosphor::led::sysfs::interface::InternalInterface internal(bus, rootPath);

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
