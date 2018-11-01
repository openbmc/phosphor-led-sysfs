#pragma once
#include <experimental/filesystem>

namespace phosphor
{
namespace led
{
class SysfsLed
{
  public:
    SysfsLed(std::experimental::filesystem::path&& root) : root(root)
    {
    }

    virtual ~SysfsLed()
    {
    }

    virtual unsigned long getBrightness();
    virtual void setBrightness(unsigned long value);
    virtual unsigned long getMaxBrightness();
    virtual std::string getTrigger();
    virtual void setTrigger(std::string trigger);
    virtual unsigned long getDelayOn();
    virtual void setDelayOn(unsigned long ms);
    virtual unsigned long getDelayOff();
    virtual void setDelayOff(unsigned long ms);

  protected:
    static constexpr char BRIGHTNESS[] = "brightness";
    static constexpr char MAX_BRIGHTNESS[] = "max_brightness";
    static constexpr char TRIGGER[] = "trigger";
    static constexpr char DELAY_ON[] = "delay_on";
    static constexpr char DELAY_OFF[] = "delay_off";

    std::experimental::filesystem::path root;
};
} // namespace led
} // namespace phosphor
