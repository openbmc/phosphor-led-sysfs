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
#include "argument.hpp"
#include "physical.hpp"
#include "config.h"

static void ExitWithError(const char* err, char** argv)
{
    ArgumentParser::usage(argv);
    std::cerr << std::endl;
    std::cerr << "ERROR: " << err << std::endl;
    exit(-1);
}

int main(int argc, char** argv)
{
    // Read arguments.
    auto options = std::make_unique<ArgumentParser>(argc, argv);

    // Parse out Name argument.
    auto name = (*options)["name"];
    if (name == ArgumentParser::empty_string)
    {
        ExitWithError("Name not specified.", argv);
    }

    // Parse out Name argument.
    auto path = (*options)["path"];
    if (path == ArgumentParser::empty_string)
    {
        ExitWithError("Path not specified.", argv);
    }

    // Finished getting options out, so release the parser.
    options.release();

    // Unique bus name representing a single LED.
    auto busName = BUSNAME + std::string(".") + name;
    auto objPath = OBJPATH + std::string("/") + name;

    phosphor::led::Physical(sdbusplus::bus::new_system(),
                            busName.c_str(), objPath.c_str(),
                            name, path);
    return 0;
}
