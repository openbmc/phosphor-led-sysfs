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

#include "controller.hpp"

#include "physical.hpp"
#include "sysfs.hpp"

#include <boost/algorithm/string.hpp>
#include <sdeventplus/event.hpp>

#include <algorithm>
#include <iostream>
#include <string>

boost::container::flat_map<std::string,
                           std::unique_ptr<phosphor::led::Physical>>
    leds;

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
    if (!ledDescr.function.empty())
        words.emplace_back(ledDescr.function);
    if (!ledDescr.color.empty())
        words.emplace_back(ledDescr.color);
    if (words.empty())
        words.emplace_back(ledDescr.devicename);
    return boost::join(words, "_");
}

namespace phosphor
{
namespace led
{

void Controller::interfacesAdded(sdbusplus::bus_t& bus)
{
    auto eventHandler = [&](sdbusplus::message_t& message) {
        if (message.is_method_error())
        {
            std::cerr << "callback method error\n";
            return;
        }
        getManagedObjects(message);
    };

    auto match = std::make_unique<sdbusplus::bus::match_t>(
        static_cast<sdbusplus::bus_t&>(bus),
        sdbusplus::bus::match::rules::interfacesAdded() +
            sdbusplus::bus::match::rules::sender(entityService),
        eventHandler);
    matches.emplace_back(std::move(match));
}

void Controller::getManagedObjects(sdbusplus::message_t& message)
{
    sdbusplus::message::object_path path;
    LedData interfaces;
    message.read(path, interfaces);

    if (!interfaces.contains(interface))
    {
        return;
    }

    const auto& entry = interfaces.at(interface);

    auto findName = entry.find("Name");
    std::string function = std::get<std::string>(findName->second);

    std::string deviceName = function;
    auto findClass = entry.find("Class");
    if (findClass != entry.end())
    {
        deviceName = std::get<std::string>(findClass->second);
    }

    std::string color;
    auto findColor = entry.find("Name1");
    if (findColor != entry.end())
    {
        color = std::get<std::string>(findColor->second);
    }

    std::string sysfsName = deviceName + ":" + color + ":" + function;
    createLEDPath(sysfsName);
}

void Controller::createLEDPath(std::string& ledName)
{
    std::string name = ledName;

    // LED names may have a hyphen and that would be an issue for
    // dbus paths and hence need to convert them to underscores.
    std::replace(name.begin(), name.end(), '/', '-');
    std::string path = devPath + name;

    // Convert to lowercase just in case some are not and that
    // we follow lowercase all over
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // LED names may have a hyphen and that would be an issue for
    // dbus paths and hence need to convert them to underscores.
    std::replace(name.begin(), name.end(), '-', '_');

    // Convert LED name in sysfs into DBus name
    LedDescr ledDescr;
    getLedDescr(name, ledDescr);
    name = getDbusName(ledDescr);

    // Unique path name representing a single LED.
    std::string objPath =
        std::string(objectPath) + '/' + ledDescr.devicename + '/' + name;

    // Check whether the path is present in hardware.
    if (!std::filesystem::exists(fs::path(path)))
    {
        std::cerr << "No such directory " << path << "\n";
        return;
    }

    // Create the Physical LED objects for directing actions.
    // Need to save this else sdbusplus destructor will wipe this off.
    auto& physical = leds[objPath];
    physical = std::make_unique<phosphor::led::Physical>(
        bus, objPath, fs::path(path), ledDescr.color);
}
} // namespace led
} // namespace phosphor

int main()
{
    // Get a default event loop
    auto event = sdeventplus::Event::get_default();

    // Get a handle to system dbus
    auto bus = sdbusplus::bus::new_default();

    // Add the ObjectManager interface
    sdbusplus::server::manager::manager objManager(bus, "/");

    // Create an led controller object
    phosphor::led::Controller controller(bus);

    // Request service bus name
    bus.request_name(busName);

    // Attach the bus to sd_event to service user requests
    bus.attach_event(event.get(), SD_EVENT_PRIORITY_NORMAL);
    event.loop();

    return 0;
}
