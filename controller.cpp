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
#include <algorithm>
#include "argument.hpp"
#include "physical.hpp"
#include "config.h"

static void ExitWithError(const char* err, char** argv)
{
    phosphor::led::ArgumentParser::usage(argv);
    std::cerr << std::endl;
    std::cerr << "ERROR: " << err << std::endl;
    exit(-1);
}

int main(int argc, char** argv)
{
    // Read arguments.
    auto options = phosphor::led::ArgumentParser(argc, argv);

    // Parse out Path argument.
    auto path = std::move((options)["path"]);
    if (path == phosphor::led::ArgumentParser::empty_string)
    {
        ExitWithError("Path not specified.", argv);
    }

    // If the LED has a hyphen in the name like: "one-two", then it gets passed
    // as /one/two/ as opposed to /one-two to the service file. There is a
    // change needed in systemd to solve this issue and hence putting in this
    // work-around.

    // Since this application always gets invoked as part of a udev rule,
    // it is always guaranteed to get /sys/class/leds/one/two
    // and we can go beyond leds/ to get the actual led name.
    // Refer: systemd/systemd#5072

    // On an error, this throws an exception and terminates.
    auto name = path.substr(strlen(DEVPATH));

    // LED names may have a hyphen and that would be an issue for
    // dbus paths and hence need to convert them to underscores.
    std::replace(name.begin(), name.end(), '/', '-');
    path = std::move(DEVPATH + name);

    // Convert to lowercase just in case some are not and that
    // we follow lowercase all over
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // LED names may have a hyphen and that would be an issue for
    // dbus paths and hence need to convert them to underscores.
    std::replace(name.begin(), name.end(), '-', '_');

    // Unique bus name representing a single LED.
    auto busName =  std::string(BUSNAME) + '.' + name;
    auto objPath =  std::string(OBJPATH) + '/' + name;

    // Get a handle to system dbus.
    auto bus = std::move(sdbusplus::bus::new_default());

    // Add systemd object manager.
    sdbusplus::server::manager::manager(bus, objPath.c_str());

    // Create the Physical LED objects for directing actions.
    // Need to save this else sdbusplus destructor will wipe this off.
    phosphor::led::Physical led(bus, objPath, path);

    /** @brief Claim the bus */
    bus.request_name(busName.c_str());

    /** @brief Wait for client requests */
    while(true)
    {
        // Handle dbus message / signals discarding unhandled
        bus.process_discard();
        bus.wait();
    }
    return 0;
}
