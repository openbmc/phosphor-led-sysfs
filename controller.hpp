#pragma once

#include "interfaces/internal_interface.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>

namespace phosphor::led::sysfs
{

class Controller :
    public sdbusplus::server::object_t<
        phosphor::led::sysfs::interface::InternalInterface>
{
  public:
    Controller() = delete;
    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller& operator=(Controller&&) = delete;
    ~Controller() = default;

    Controller(sdbusplus::bus_t& bus, const char* path) :
        sdbusplus::server::object_t<
            phosphor::led::sysfs::interface::InternalInterface>(bus, path),
        bus(bus)
    {}

    void addLed(std::string name) override;

    void removeLed(std::string name) override;

  private:
    sdbusplus::bus_t& bus;
};
} // namespace phosphor::led::sysfs
