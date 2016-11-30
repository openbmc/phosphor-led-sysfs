#pragma once

#include <map>
#include <set>
#include <fstream>
#include <sdbusplus/server/object.hpp>
#include "xyz/openbmc_project/Led/Physical/server.hpp"

namespace phosphor
{
namespace led
{
/** @brief Acts as input and output file for the current LED power state.
 *   Write "0" to trigger a LED OFF operation.
 *   Write "255" to trigger a LED ON operation.
 *   To know the current power state of the LED, a read on this
 *   would give either 0 or 255 indicating if the LED is currently
 *   Off / On AT THAT moment.
 *   Example: /sys/class/leds/myled/brightness
 */
constexpr auto BRIGHTNESS = "/brightness";

/** @brief Assert LED by writing 255 */
constexpr auto ASSERT = "255";

/** @brief De-assert by writing "0" */
constexpr auto DEASSERT = "0";

/** @brief Write "timer" to this file telling the driver that
 *   the intended operation is BLINK. When the value "timer" is written to
 *   the file, 2 more files get auto generated and are named "delay_on" and
 *   "delay_off"
 *   To move away from blinking, write "none"
 *   Example:  /sys/class/leds/myled/trigger
 */
constexpr auto BLINKCTRL = "/trigger";

/** @brief write number of milliseconds that the LED needs to be ON
 *   while blinking. Default is 500 by the driver.
 *   Example:  /sys/class/leds/myled/delay_on
 */
constexpr auto DELAYON = "/delay_on";

/** @brief Write number of milliseconds that the LED needs to be OFF
 *   while blinking. Default is 500 by the driver.
 *   Example:  /sys/class/leds/myled/delay_off
 */
constexpr auto DELAYOFF = "/delay_off";

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
                 const std::string& busName,
                 const std::string& objPath,
                 const std::string& ledName,
                 const std::string& ledPath);

        /** @brief Overloaded State Property Setter function
         *
         *  @param[in] value   -  One of OFF / ON / BLINK
         *  @return            -  Success or exception thrown
         */
        Action state(Action value) override;

    private:
        /** @brief Name of the LED, like "EnclosureFault" */
        std::string name;

        /** @brief File system location where this LED is exposed
         *   Typically /sys/class/leds/
         */
        std::string path;

        /** @brief User intended action */
        Action action;

        /** @brief Applies the user triggered action on the LED
         *   by writing to sysfs
         *
         *  @return Status or exception thrown
         */
        Action driveLED(void);

        /** @brief Sets the LED to either ON or OFF state
         *
         *  @return Current action or exception thrown
         */
        Action stableStateOperation();

        /** @brief Sets the LED to BLINKING
         *
         *  @return Current Action or exception thrown
         */
        Action blinkOperation();

        /** @brief Generic file writer.
         *   There are files like "brightness", "trigger" , "delay_on" and
         *   "delay_off" that will tell what the LED driver needs to do.
         *
         *  @param[in] filename - Name of file to be written
         *  @param[in] data     - Data to be written to
         *  @return             - 0 or exception.
         */
        template <typename T>
        auto write(const std::string& fileName, T&& data)
        {
            if(std::ifstream(fileName))
            {
                std::ofstream file(fileName, std::ios::out);
                file << data;
                file.close();
                return 0;
            }
            else
            {
                throw std::runtime_error(fileName  + " Not Present");
            }
        }
};

} // namespace led
} // namespace phosphor
