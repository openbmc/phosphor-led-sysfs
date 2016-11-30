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
        Physical(Physical&&) = default;
        Physical& operator=(Physical&&) = default;

        /** @brief Constructs LED object. Argument 'true' says that we hold off
         *   from sending the signals since we need to do some house keeping and
         *   only when we finish that, we are considered active and can then
         *   broadcast the signal.
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
                        bus, objPath.c_str(), true),
            path(ledPath)
        {
            // Suppose this is getting launched as part of BMC reboot, then we
            // need to save what the micro-controller currently has.
            setInitialState();

            // We are now ready.
            emit_object_added();
        }

        /** @brief Overloaded State Property Setter function
         *
         *  @param[in] value   -  One of OFF / ON / BLINK
         *  @return            -  Success or exception thrown
         */
        Action state(Action value) override;

    private:
        /** @brief File system location where this LED is exposed
         *   Typically /sys/class/leds/<Led-Name>
         */
        std::string path;

        /** @brief Applies the user triggered action on the LED
         *   by writing to sysfs
         *
         *  @return None
         */
        void driveLED(void);

        /** @brief reads sysfs and then setsup the parameteres accordingly
         *
         *  @return Status or exception thrown
         */
        void setInitialState(void);
};

} // namespace led
}
