#pragma once

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/interface.hpp>
#include <sdbusplus/vtable.hpp>

namespace phosphor
{
namespace led
{
namespace sysfs
{
namespace interface
{

class InternalInterface
{
  public:
    InternalInterface() = delete;
    InternalInterface(const InternalInterface&) = delete;
    InternalInterface& operator=(const InternalInterface&) = delete;
    InternalInterface(InternalInterface&&) = delete;
    InternalInterface& operator=(InternalInterface&&) = delete;
    virtual ~InternalInterface() = default;

    /**
     *  @brief Construct a class to put object onto bus at a dbus path.
     *
     *  @param[in] bus  - D-Bus object.
     *  @param[in] path - D-Bus Path.
     */

    InternalInterface(sdbusplus::bus_t& bus, const char* path);

    /**
     *  @brief Implementation for the AddLed method to add
     *  the LED name to dbus path.
     *
     *  @param[in] name - LED name to add.
     */

    void addLed(std::string name);

    /**
     *  @brief Implementation for the RemoveLed method to remove
     *  the LED name to dbus path.
     *
     *  @param[in] name - LED name to remove.
     */

    void removeLed(std::string name);

    /**
     *  @brief Dbus interface's name
     */

    static constexpr auto interface = "xyz.openbmc_project.Led.Sysfs.Internal";

  private:
    /**
     *  @brief Systemd bus callback for the AddLed method.
     */

    static int addLedConfigure(sd_bus_message* msg, void* context,
                               sd_bus_error* error);

    /**
     *  @brief Systemd bus callback for the RemoveLed method.
     */

    static int removeLedConfigure(sd_bus_message* msg, void* context,
                                  sd_bus_error* error);

    /**
     *  @brief Systemd vtable structure that contains all the
     *  methods, signals, and properties of this interface with their
     *  respective systemd attributes
     */

    static const sdbusplus::vtable::vtable_t vtable[];

    /**
     *  @brief Support for the dbus based instance of this interface.
     */

    sdbusplus::server::interface::interface serverInterface;
};

} // namespace interface
} // namespace sysfs
} // namespace led
} // namespace phosphor
