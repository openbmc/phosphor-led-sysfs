#pragma once

#include <boost/container/flat_map.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>

using BasicVariantType =
    std::variant<std::vector<std::string>, std::string, int64_t, uint64_t,
                 double, int32_t, uint32_t, int16_t, uint16_t, uint8_t, bool>;

using LedConfigMap = boost::container::flat_map<std::string, BasicVariantType>;

using LedData = boost::container::flat_map<std::string, LedConfigMap>;

using ManagedObjectType =
    boost::container::flat_map<sdbusplus::message::object_path, LedData>;

static constexpr auto busName = "xyz.openbmc_project.LED.Controller";
static constexpr auto objectPath = "/xyz/openbmc_project/led/physical";
static constexpr auto devPath = "/sys/class/leds/";

static constexpr const char* interface =
    "xyz.openbmc_project.Configuration.SysfsLed";
static constexpr const char* entityService =
    "xyz.openbmc_project.EntityManager";

namespace phosphor
{
namespace led
{

class Controller
{
  public:
    Controller() = delete;
    ~Controller() = default;

    Controller(sdbusplus::bus_t& bus) : bus(bus)
    {
        interfacesAdded(bus);
    }

  private:
    sdbusplus::bus_t& bus;

    std::vector<std::unique_ptr<sdbusplus::bus::match_t>> matches;

    void interfacesAdded(sdbusplus::bus_t& bus);

    void getManagedObjects(sdbusplus::message_t& message);

    void createLEDPath(std::string& ledName);
};

} // namespace led
} // namespace phosphor
