#pragma once

#include <map>
#include <set>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdbusplus/server/object.hpp>
#include "xyz/openbmc_project/Led/Physical/server.hpp"
namespace phosphor
{
namespace led
{

/** @class Physical
 *  @brief Responsible for applying actions on a particular physical LED
 */
class Physical : public sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Led::server::Physical>
{
    public:
        Physical() = delete;
        ~Physical() = default;
        Physical(const Physical&) = delete;
        Physical& operator=(const Physical&) = delete;
        Physical(Physical&&) = default;
        Physical& operator=(Physical&&) = default;

        /** @brief Constructs LED object
         *
         * @param[in] bus       - system dbus handler
         * @param[in] busName   - The Dbus name to own
         * @param[in] objPath   - The Dbus path that hosts physical LED
         * @param[in] ledName   - The name of the LED like "EnclosureFault"
         * @param[in] ledPath   - sysfs path where this LED is exported
         */
        Physical(sdbusplus::bus::bus& bus,
                const char* busName,
                const char* objPath,
                const std::string& ledName,
                const std::string& ledPath);

    private:
        /** @brief Name of the LED, like "EnclosureFault" */
        std::string name;

        /** @brief File system location where this LED is exposed
         *   Typically /sys/class/leds/
         */
        std::string path;
};

} // namespace led
} // namespace phosphor
