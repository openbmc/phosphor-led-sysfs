#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <gtest/gtest.h>
#include <sys/param.h>

#include "sysfs.hpp"

namespace fs = std::experimental::filesystem;

constexpr auto MAX_BRIGHTNESS_VAL = 128;

class FakeSysfsLed : public phosphor::led::SysfsLed
{
  public:
    static FakeSysfsLed create()
    {
        const char* const tmplt = "/tmp/FakeSysfsLed.XXXXXX";
        char buffer[MAXPATHLEN] = {0};

        strncpy(buffer, tmplt, strlen(tmplt));
        char* dir = mkdtemp(buffer);
        if (!dir)
            throw std::system_error(errno, std::system_category());

        return FakeSysfsLed(fs::path(dir));
    }

    ~FakeSysfsLed()
    {
        fs::remove_all(root);
    }

  private:
    FakeSysfsLed(fs::path&& path) : SysfsLed(std::move(path))
    {
        std::string attrs[4] = {BRIGHTNESS, TRIGGER, DELAY_ON, DELAY_OFF};
        for (auto attr : attrs)
        {
            fs::path p = root / attr;
            std::ofstream f(p, std::ios::out);
            f.exceptions(f.failbit);
        }

        fs::path p = root / MAX_BRIGHTNESS;
        std::ofstream f(p, std::ios::out);
        f.exceptions(f.failbit);
        f << MAX_BRIGHTNESS_VAL;
    }
};

TEST(Sysfs, getBrightness)
{
    constexpr auto brightness = 127;
    FakeSysfsLed fsl = FakeSysfsLed::create();

    fsl.setBrightness(brightness);
    ASSERT_EQ(brightness, fsl.getBrightness());
}

TEST(Sysfs, getMaxBrightness)
{
    FakeSysfsLed fsl = FakeSysfsLed::create();

    ASSERT_EQ(MAX_BRIGHTNESS_VAL, fsl.getMaxBrightness());
}

TEST(Sysfs, getTrigger)
{
    constexpr auto trigger = "none";
    FakeSysfsLed fsl = FakeSysfsLed::create();

    fsl.setTrigger(trigger);
    ASSERT_EQ(trigger, fsl.getTrigger());
}

TEST(Sysfs, getDelayOn)
{
    constexpr auto delayOn = 250;
    FakeSysfsLed fsl = FakeSysfsLed::create();

    fsl.setDelayOn(delayOn);
    ASSERT_EQ(delayOn, fsl.getDelayOn());
}

TEST(Sysfs, getDelayOff)
{
    constexpr auto delayOff = 750;
    FakeSysfsLed fsl = FakeSysfsLed::create();

    fsl.setDelayOff(delayOff);
    ASSERT_EQ(delayOff, fsl.getDelayOff());
}
