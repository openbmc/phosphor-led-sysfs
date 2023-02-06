#include "physical.hpp"

#include <sys/param.h>

#include <sdbusplus/bus.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

constexpr auto ledObj = "/foo/bar/led";

using Action = sdbusplus::xyz::openbmc_project::Led::server::Physical::Action;
namespace fs = std::filesystem;

fs::path createSandbox()
{
    /* If your tests need to touch the filesystem, always use mkdtemp() or
     * mkstemp() for creating directories and files. Tests can be run in
     * parallel with `make -j`, and if use the same path in multiple tests they
     * will stomp on eachother and likely fail.
     */
    static constexpr auto tmplt = "/tmp/MockLed.XXXXXX";
    std::array<char, MAXPATHLEN> buffer = {0};

    strncpy(buffer.data(), tmplt, sizeof(buffer) - 1);
    auto* dir = mkdtemp(buffer.data());
    if (dir == nullptr)
    {
        throw std::system_error(errno, std::system_category());
    }

    /* We want to limit behaviours to mocks, and if methods aren't mocked they
     * may fall back to their base class implementation. Stop read/write to
     * directory to prevent streams from creating files.
     */
    if (chmod(dir, S_IXUSR | S_IXGRP) == -1)
    {
        throw std::system_error(errno, std::system_category());
    }

    return {dir};
}

class MockLed : public phosphor::led::SysfsLed
{
  public:
    /* Use a no-args ctor here to avoid headaches with {Nice,Strict}Mock */
    MockLed() : SysfsLed(createSandbox())
    {}
    MockLed(MockLed& other) = delete;
    MockLed(MockLed&& other) = delete;
    MockLed& operator=(MockLed& other) = delete;
    MockLed&& operator=(MockLed&& other) = delete;

    ~MockLed() override
    {
        chmod(root.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
        fs::remove_all(root);
    }

    MOCK_METHOD0(getBrightness, unsigned long());
    MOCK_METHOD1(setBrightness, void(unsigned long value));
    MOCK_METHOD0(getMaxBrightness, unsigned long());
    MOCK_METHOD0(getTrigger, std::string());
    MOCK_METHOD1(setTrigger, void(const std::string& trigger));
    MOCK_METHOD0(getDelayOn, unsigned long());
    MOCK_METHOD1(setDelayOn, void(unsigned long ms));
    MOCK_METHOD0(getDelayOff, unsigned long());
    MOCK_METHOD1(setDelayOff, void(unsigned long ms));
};

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

TEST(Physical, ctor_none_trigger)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    /* NiceMock ignores calls to methods with no expectations defined */
    NiceMock<MockLed> led;
    ON_CALL(led, getTrigger()).WillByDefault(Return("none"));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, ctor_maxbrightness_and_brightness_read_127)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    /* NiceMock ignores calls to methods with no expectations defined */
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillRepeatedly(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(127));
    EXPECT_CALL(led, getMaxBrightness()).WillOnce(Return(127));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::On);
}

TEST(Physical, ctor_maxbrightness_and_brightness_read_0)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    /* NiceMock ignores calls to methods with no expectations defined */
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillRepeatedly(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(0));
    EXPECT_CALL(led, getMaxBrightness()).WillOnce(Return(0));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, ctor_only_maxbrightness_read_127)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    /* NiceMock ignores calls to methods with no expectations defined */
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillRepeatedly(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(0));
    EXPECT_CALL(led, getMaxBrightness()).WillOnce(Return(127));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, ctor_only_brightness_read_127)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    /* NiceMock ignores calls to methods with no expectations defined */
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillRepeatedly(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(127));
    EXPECT_CALL(led, getMaxBrightness()).WillOnce(Return(0));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, ctor_timer_trigger)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillOnce(Return("timer"));
    EXPECT_CALL(led, getDelayOn()).WillOnce(Return(500));
    EXPECT_CALL(led, getDelayOff()).WillOnce(Return(500));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, off)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    ON_CALL(led, getMaxBrightness()).WillByDefault(Return(127));
    EXPECT_CALL(led, getTrigger()).WillOnce(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(phosphor::led::DEASSERT));
    EXPECT_CALL(led, setBrightness(phosphor::led::DEASSERT)).Times(0);
    phosphor::led::Physical phy(bus, ledObj, led);
    phy.state(Action::Off);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, on)
{
    constexpr unsigned long asserted = 127;

    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    ON_CALL(led, getMaxBrightness()).WillByDefault(Return(asserted));
    EXPECT_CALL(led, getTrigger()).WillOnce(Return("none"));
    EXPECT_CALL(led, setTrigger("none"));
    EXPECT_CALL(led, setBrightness(asserted));
    phosphor::led::Physical phy(bus, ledObj, led);
    phy.state(Action::On);
    EXPECT_EQ(phy.state(), Action::On);
}

TEST(Physical, blink)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillOnce(Return("none"));
    EXPECT_CALL(led, setTrigger("timer"));
    EXPECT_CALL(led, setDelayOn(500));
    EXPECT_CALL(led, setDelayOff(500));
    phosphor::led::Physical phy(bus, ledObj, led);
    phy.state(Action::Blink);
    EXPECT_EQ(phy.state(), Action::Blink);
}

TEST(Physical, ctor_none_trigger_asserted_brightness)
{
    sdbusplus::bus_t bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    EXPECT_CALL(led, getTrigger()).WillRepeatedly(Return("none"));
    EXPECT_CALL(led, getBrightness()).WillRepeatedly(Return(127));
    phosphor::led::Physical phy(bus, ledObj, led);
    EXPECT_EQ(phy.state(), Action::Off);
}

TEST(Physical, on_to_off)
{
    InSequence s;
    constexpr unsigned long asserted = 127;

    auto bus = sdbusplus::bus::new_default();
    NiceMock<MockLed> led;
    ON_CALL(led, getMaxBrightness()).WillByDefault(Return(asserted));
    EXPECT_CALL(led, getTrigger()).Times(1).WillOnce(Return("none"));
    constexpr auto deasserted = phosphor::led::DEASSERT;
    EXPECT_CALL(led, getBrightness()).WillOnce(Return(deasserted));
    EXPECT_CALL(led, setBrightness(asserted));
    EXPECT_CALL(led, setBrightness(deasserted));
    phosphor::led::Physical phy(bus, ledObj, led);
    phy.state(Action::On);
    EXPECT_EQ(phy.state(), Action::On);
    phy.state(Action::Off);
    EXPECT_EQ(phy.state(), Action::Off);
}
