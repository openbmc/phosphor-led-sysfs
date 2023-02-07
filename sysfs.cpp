/**
 * Copyright © 2019 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sysfs.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

namespace phosphor
{
namespace led
{
template <typename T>
T getSysfsAttr(const fs::path& path);

template <>
std::string getSysfsAttr(const fs::path& path)
{
    std::string content;
    std::ifstream file(path);
    file >> content;
    return content;
}

template <>
unsigned long getSysfsAttr(const fs::path& path)
{
    std::string content = getSysfsAttr<std::string>(std::move(path));
    return std::strtoul(content.c_str(), nullptr, 0);
}

template <typename T>
void setSysfsAttr(const fs::path& path, const T& value)
{
    std::ofstream file(path);
    file << value;
}

unsigned long SysfsLed::getBrightness()
{
    return getSysfsAttr<unsigned long>(root / BRIGHTNESS);
}

void SysfsLed::setBrightness(unsigned long brightness)
{
    setSysfsAttr<unsigned long>(root / BRIGHTNESS, brightness);
}

unsigned long SysfsLed::getMaxBrightness()
{
    return getSysfsAttr<unsigned long>(root / MAX_BRIGHTNESS);
}

std::string SysfsLed::getTrigger()
{
    return getSysfsAttr<std::string>(root / TRIGGER);
}

void SysfsLed::setTrigger(const std::string& trigger)
{
    setSysfsAttr<std::string>(root / TRIGGER, trigger);
}

unsigned long SysfsLed::getDelayOn()
{
    return getSysfsAttr<unsigned long>(root / DELAY_ON);
}

void SysfsLed::setDelayOn(unsigned long ms)
{
    setSysfsAttr<unsigned long>(root / DELAY_ON, ms);
}

unsigned long SysfsLed::getDelayOff()
{
    return getSysfsAttr<unsigned long>(root / DELAY_OFF);
}

void SysfsLed::setDelayOff(unsigned long ms)
{
    setSysfsAttr<unsigned long>(root / DELAY_OFF, ms);
}
} // namespace led
} // namespace phosphor
