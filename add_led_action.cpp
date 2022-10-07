#include "argument.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>

static constexpr auto busName = "xyz.openbmc_project.LED.Controller";
static constexpr auto rootPath = "/xyz/openbmc_project/led";
static constexpr auto devPath = "/sys/class/leds/";
static constexpr auto interface = "xyz.openbmc_project.Led.Sysfs.Internal";
static constexpr auto ledAddMethod = "AddLED";

std::string rootPathVerify(std::string path)
{
    if (!path.starts_with(devPath))
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
        auto method = bus.new_method_call(busName, rootPath, interface,
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
