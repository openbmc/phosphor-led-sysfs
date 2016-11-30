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
#include <memory>
#include <string>
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
    // For creating a bus name
    using namespace std::string_literals;

    // Read arguments.
    auto options = phosphor::led::ArgumentParser(argc, argv);

    // Parse out Name argument.
    auto name = std::move((options)["name"]);
    if (name == phosphor::led::ArgumentParser::empty_string)
    {
        ExitWithError("Name not specified.", argv);
    }

    // Parse out Name argument.
    auto path = std::move((options)["path"]);
    if (path == phosphor::led::ArgumentParser::empty_string)
    {
        ExitWithError("Path not specified.", argv);
    }

    // Unique bus name representing a single LED.
    auto busName =  BUSNAME + "."s + name;
    auto objPath =  OBJPATH + "/"s + name;

    // Get a handle to system dbus.
    auto bus = std::move(sdbusplus::bus::new_system());

    // Add systemd object manager.
    sdbusplus::server::manager::manager(bus, objPath.c_str());

    // Create the Physical LED objects for directing actions.
    // Need to save this else sdbusplus destructor will wipe this off.
    auto obj = phosphor::led::Physical(bus, busName,
                                       objPath, name, path);
    /** @brief Claim the bus */
    bus.request_name(busName.c_str());

    /** @brief Wait for client requests */
    while(true)
    {
        try
        {
            // Handle dbus message / signals discarding unhandled
            bus.process_discard();
            bus.wait();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    return 0;
}
