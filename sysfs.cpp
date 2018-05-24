#include <fstream>
#include <iostream>
#include <string>
#include "sysfs.hpp"

namespace fs = std::experimental::filesystem;

namespace phosphor
{
namespace led
{

constexpr char SysfsLed::BRIGHTNESS[];
constexpr char SysfsLed::MAX_BRIGHTNESS[];
constexpr char SysfsLed::TRIGGER[];
constexpr char SysfsLed::DELAY_ON[];
constexpr char SysfsLed::DELAY_OFF[];

template <typename T> T get_sysfs_attr(fs::path&& path);

template <> std::string get_sysfs_attr(fs::path&& path)
{
    std::string content;
    std::ifstream file(path, std::ios::in);
    file >> content;
    return content;
}

template <> unsigned long get_sysfs_attr(fs::path&& path)
{
    std::string content = get_sysfs_attr<std::string>(std::move(path));
    return std::strtoul(content.c_str(), nullptr, 0);
}

template <typename T> void set_sysfs_attr(fs::path&& path, T& value)
{
    std::ofstream file(path, std::ios::out);
    file << value;
}

unsigned long SysfsLed::getBrightness()
{
    return get_sysfs_attr<unsigned long>(root / BRIGHTNESS);
}

void SysfsLed::setBrightness(unsigned long brightness)
{
    set_sysfs_attr<unsigned long>(root / BRIGHTNESS, brightness);
}

unsigned long SysfsLed::getMaxBrightness()
{
    return get_sysfs_attr<unsigned long>(root / MAX_BRIGHTNESS);
}

std::string SysfsLed::getTrigger()
{
    return get_sysfs_attr<std::string>(root / TRIGGER);
}

void SysfsLed::setTrigger(std::string trigger)
{
    set_sysfs_attr<std::string>(root / TRIGGER, trigger);
}

unsigned long SysfsLed::getDelayOn()
{
    return get_sysfs_attr<unsigned long>(root / DELAY_ON);
}

void SysfsLed::setDelayOn(unsigned long ms)
{
    set_sysfs_attr<unsigned long>(root / DELAY_ON, ms);
}

unsigned long SysfsLed::getDelayOff()
{
    return get_sysfs_attr<unsigned long>(root / DELAY_OFF);
}

void SysfsLed::setDelayOff(unsigned long ms)
{
    set_sysfs_attr<unsigned long>(root / DELAY_OFF, ms);
}
}
}
