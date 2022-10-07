#include "argument.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/server.hpp>

#include <iostream>

constexpr auto busName = "xyz.openbmc_project.LED.Controller";
constexpr auto objectPath = "/xyz/openbmc_project/led";
constexpr auto interface = "xyz.openbmc_project.Led.Internal";
constexpr auto methodName = "AddLed";
constexpr auto DEVPATH = "/sys/class/leds/";

static void ExitWithError(const char* err, char** argv)
{
    phosphor::led::ArgumentParser::usage(argv);
    lg2::error("Argument parser error {ERROR}", "ERROR", err);
    exit(-1);
}

void methodCall(std::string ledName)
{
    try
    {
        auto bus = sdbusplus::bus::new_default();

        auto method =
            bus.new_method_call(busName, objectPath, interface, methodName);

        method.append(ledName);

        bus.call(method);
    }
    catch (const std::exception& e)
    {
        lg2::error("Unable to add LED name {LEDNAME}", "LEDNAME", ledName);
    }
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

    auto name = path.substr(strlen(DEVPATH));

    methodCall(name);

    return 0;
}
