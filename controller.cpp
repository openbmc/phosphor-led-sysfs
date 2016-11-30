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
    auto options = std::make_unique<
        phosphor::led::ArgumentParser>(argc, argv);

    // Parse out Name argument.
    auto name = std::move((*options)["name"]);
    if (name == phosphor::led::ArgumentParser::empty_string)
    {
        ExitWithError("Name not specified.", argv);
    }

    // Parse out Name argument.
    auto path = std::move((*options)["path"]);
    if (path == phosphor::led::ArgumentParser::empty_string)
    {
        ExitWithError("Path not specified.", argv);
    }

    // Finished getting options out, so release the parser.
    options.release();

    return 0;
}
