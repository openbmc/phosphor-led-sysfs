#pragma once

#include <string>
#include <fstream>
#include <sdbusplus/bus.hpp>
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

        /** @brief Frequency range that the LED can operate on.
         *  Will be removed when frequency is put into interface
         */
        uint32_t frequency;

        /** @brief Brightness described above */
        std::string brightCtrl;

        /** @brief BlinkCtrl described above */
        std::string blinkCtrl;

        /** @brief delay_on described above */
        std::string delayOnCtrl;

        /** @brief delay_ff described above */
        std::string delayOffCtrl;

        /** @brief reads sysfs and then setsup the parameteres accordingly
         *
         *  @return None
         */
        void setInitialState();

        /** @brief Applies the user triggered action on the LED
         *   by writing to sysfs
         *
         *  @param [in] current - Current state of LED
         *  @param [in] request - Requested state
         *
         *  @return None
         */
        void driveLED(Action current, Action request);

        /** @brief Sets the LED to either ON or OFF state
         *
         *  @param [in] action - Requested action. Could be OFF or ON
         *  @return None
         */
        void stableStateOperation(Action action);

        /** @brief Sets the LED to BLINKING
         *
         *  @return None
         */
        void blinkOperation();

        /** @brief Generic file writer.
         *   There are files like "brightness", "trigger" , "delay_on" and
         *   "delay_off" that will tell what the LED driver needs to do.
         *
         *  @param[in] filename - Name of file to be written
         *  @param[in] data     - Data to be written to
         *  @return             - None
         */
        template <typename T>
        auto write(const std::string& fileName, T&& data)
        {
            if(std::ifstream(fileName))
            {
                std::ofstream file(fileName, std::ios::out);
                file << data;
                file.close();
            }
            return;
        }

        /** @brief Generic file reader.
         *   There are files like "brightness", "trigger" , "delay_on" and
         *   "delay_off" that will tell what the LED driver needs to do.
         *
         *  @param[in] filename - Name of file to be read
         *  @return             - File content
         */
        template <typename T>
        T read(const std::string& fileName)
        {
            T data = T();
            if(std::ifstream(fileName))
            {
                std::ifstream file(fileName, std::ios::in);
                file >> data;
                file.close();
            }
            return data;
        }
};

} // namespace led
}
