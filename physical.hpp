#pragma once

#include <map>
#include <set>
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
    auto state(Action value) -> Action override;

private:
    /** @brief Name of the LED, like "EnclosureFault" */
    std::string iv_name;

    /** @brief File system location where this LED is exposed
     *   Typically /sys/class/leds/
     */
    std::string iv_path;

    /** @brief User intended action */
    Action iv_action;

    /** @brief Applies the user triggered action on the LED
     *   by writing to sysfs
     *
     *  @return Status or exception thrown
     */
    Action driveLED(void);

    /** @brief Generic file reader.
     *    This is mostly used to read the 'brightness' file for a given LED
     *    to know what is the current state of LED.
     *
     *  @param[in] fileName - Name of file to be read
     *  @return             - File content
     */
};

} // namespace led
} // namespace phosphor
