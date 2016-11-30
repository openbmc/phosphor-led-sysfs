#pragma once

#include <string>
#include <sdbusplus/bus.hpp>
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
        Physical(Physical&&) = delete;
        Physical& operator=(Physical&&) = delete;

        /** @brief Constructs LED object
         *
         * @param[in] bus       - system dbus handler
         * @param[in] objPath   - The Dbus path that hosts physical LED
         * @param[in] ledPath   - sysfs path where this LED is exported
         */
        Physical(sdbusplus::bus::bus& bus,
                const std::string& objPath,
                const std::string& ledPath) :

            sdbusplus::server::object::object<
                sdbusplus::xyz::openbmc_project::Led::server::Physical>(
                        bus, objPath.c_str()),
            path(ledPath)
        {
                // Nothing to do here
        }

    private:
        /** @brief File system location where this LED is exposed
         *   Typically /sys/class/leds/<Led-Name>
         */
        std::string path;
};

} // namespace led
} // namespace phosphor
