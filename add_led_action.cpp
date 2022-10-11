#include "argument.hpp"
#include "interfaces/internal_interface.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>

std::string rootPathVerify(std::string path)
{
    int pos = 0;
    unsigned long index = path.find(devPath, pos);

    if (index != 0UL)
    {
        lg2::error("Invalid sys path - {PATH}", "PATH", path);
        throw std::invalid_argument("Invalid argument");
    }

    std::string led = path.substr(strlen(devPath));
    return led;
}

void addLed(std::string ledName)
{
    try
    {
        auto bus = sdbusplus::bus::new_default();
        auto method = bus.new_method_call(busName, rootPath, internalInterface,
                                          ledAddMethod);

        method.append(ledName);
        bus.call(method);
    }
    catch (const std::exception& e)
    {
        lg2::error("Unable to add LED name - {LEDNAME}", "LEDNAME", ledName);
        throw e;
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
    const auto& path = options["path"];

    if (path.empty())
    {
        phosphor::led::ArgumentParser::usage(argv);

        lg2::error("Argument parser error : Path not specified");
        throw std::invalid_argument("Invalid argument");
    }

    std::string name = rootPathVerify(path);

    addLed(name);

    return 0;
}
