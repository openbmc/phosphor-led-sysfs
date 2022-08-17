#pragma once

#include <boost/container/flat_map.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>

#include <iostream>

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
    "xyz.openbmc_project.Configuration.Led";
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

    explicit Controller(sdbusplus::bus_t& bus) :
        bus(bus), match(bus,
                        sdbusplus::bus::match::rules::interfacesAdded() +
                            sdbusplus::bus::match::rules::sender(entityService),
                        std::bind(&Controller::interfacesAddedHandler, this,
                                  std::placeholders::_1))
    {}

  private:
    sdbusplus::bus_t& bus;

    sdbusplus::bus::match_t match;

    void interfacesAddedHandler(sdbusplus::message_t& message);

    void createLEDPath(const std::string& ledName);
};

} // namespace led
} // namespace phosphor
