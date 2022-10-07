#include "argument.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>

constexpr auto ledConnection = "xyz.openbmc_project.LED.Controller";
constexpr auto ledPath = "/xyz/openbmc_project/led";
constexpr auto ledPresenceInterface = "xyz.openbmc_project.Led.Sysfs.Internal";
constexpr auto ledAddMethod = "AddLed";
constexpr auto ledClassRoot = "/sys/class/leds/";

std::string rootPathVerify(std::string path)
{
    int pos = 0;
    int index = path.find(ledClassRoot, pos);

    if (index != 0)
    {
        lg2::error("Invalid sys path {PATH}", "PATH", path);
        exit(-1);
    }
    std::string led = path.substr(strlen(ledClassRoot));
    return led;
}

void addLed(std::string ledName)
{
    try
    {
        auto bus = sdbusplus::bus::new_default();

        auto method = bus.new_method_call(ledConnection, ledPath,
                                          ledPresenceInterface, ledAddMethod);

        method.append(ledName);

        bus.call(method);
    }
    catch (const std::exception& e)
    {
        lg2::error("Unable to add LED name {LEDNAME}", "LEDNAME", ledName);
        exit(-1);
    }
}

/* Each LED udev event will trigger systemd service (sysfs-led@.service)
 * Systemd service will invoke the binary (add-led-action) by passing LED
 * name as argument.
 *
 * Usage: /usr/libexec/phosphor-led-sysfs/add-led-action [options]
 * Options:
 *  --help           Print this menu
 *  --path=<path>    absolute path of LED in sysfs; like /sys/class/leds/<name>
 *
 */

int main(int argc, char* argv[])
{
    // Read arguments.
    auto options = phosphor::led::ArgumentParser(argc, argv);

    // Parse out Path argument.
    auto path = std::move((options)["path"]);
    if (path == phosphor::led::ArgumentParser::empty_string)
    {
        phosphor::led::ArgumentParser::usage(argv);
        lg2::error("Argument parser error : Path not specified");
        exit(-1);
    }

    std::string name = rootPathVerify(path);

    addLed(name);

    return 0;
}
