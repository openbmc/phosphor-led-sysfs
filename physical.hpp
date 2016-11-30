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
    Physical(sdbusplus::bus::bus&& bus,
             const char* busName,
             const char* objPath,
             std::string ledName,
             std::string ledPath);

    /** @brief Dbus constructs used by Physical LED controller */
    sdbusplus::bus::bus iv_bus;

    /** @brief sd_bus object manager */
    sdbusplus::server::manager::manager iv_ObjManager;

    /** @brief Waits on the client request and processes them */
    void run() noexcept;

    /** @brief Overloaded State Property Setter function
     *
     *  @param[in] value   -  One of OFF / ON / BLINK
     *  @return            -  Success or exception thrown
     */
    virtual auto state(Action value) -> Action override;

    /** @brief Overloaded DutyOn Property Setter function
     *
     *  @param[in] value   -  A number within 100
     *  @return            -  Success or exception thrown
     */
    auto dutyOn(uint8_t value) -> uint8_t override;

    /** @brief Overloaded Color Property Setter function
     *
     *  @param[in] value   -  One of possible colors
     *  @return            -  Success or exception thrown
     */
    auto color(Palette value) -> Physical::Palette override;

private:
    /** @brief Name of the LED, like "EnclosureFault" */
    std::string iv_name;

    /** @brief File system location where this LED is exposed
     *   Typically /sys/class/leds/
     */
    std::string iv_path;

    /** @brief User intended action */
    Action iv_action;

    /** @brief User action */
    Palette iv_color;

    /** @brief default duty cycle is 50:50 */
    uint8_t iv_dutyOn;

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
     * @param[in] filename - Name of file to be written
     * @param[in] data     - Data to be written to
     * @return             - 0 or exception.
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
