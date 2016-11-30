#pragma once
#include <tuple>
#include <systemd/sd-bus.h>
#include <sdbusplus/server.hpp>

namespace sdbusplus
{
namespace xyz
{
namespace openbmc_project
{
namespace Led
{
namespace server
{

class Physical
{
    public:
        /* Define all of the basic class operations:
         *     Not allowed:
         *         - Default constructor to avoid nullptrs.
         *         - Copy operations due to internal unique_ptr.
         *     Allowed:
         *         - Move operations.
         *         - Destructor.
         */
        Physical() = delete;
        Physical(const Physical&) = delete;
        Physical& operator=(const Physical&) = delete;
        Physical(Physical&&) = default;
        Physical& operator=(Physical&&) = default;
        virtual ~Physical() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         */
        Physical(bus::bus& bus, const char* path);

        enum class Action
        {
            Off,
            On,
            Blink,
        };
        enum class Palette
        {
            Unknown,
            Red,
            Green,
            Blue,
            Yellow,
        };



        /** Get value of State */
        virtual Action state() const;
        /** Set value of State */
        virtual Action state(Action value);
        /** Get value of DutyOn */
        virtual uint8_t dutyOn() const;
        /** Set value of DutyOn */
        virtual uint8_t dutyOn(uint8_t value);
        /** Get value of Color */
        virtual Palette color() const;
        /** Set value of Color */
        virtual Palette color(Palette value);

    /** @brief Convert a string to an appropriate enum value.
     *  @param[in] s - The string to convert in the form of
     *                 "xyz.openbmc_project.Led.Physical.<value name>"
     *  @return - The enum value.
     */
    static Action convertActionFromString(std::string& s);
    /** @brief Convert a string to an appropriate enum value.
     *  @param[in] s - The string to convert in the form of
     *                 "xyz.openbmc_project.Led.Physical.<value name>"
     *  @return - The enum value.
     */
    static Palette convertPaletteFromString(std::string& s);

    private:

        /** @brief sd-bus callback for get-property 'State' */
        static int _callback_get_State(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'State' */
        static int _callback_set_State(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'DutyOn' */
        static int _callback_get_DutyOn(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'DutyOn' */
        static int _callback_set_DutyOn(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'Color' */
        static int _callback_get_Color(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'Color' */
        static int _callback_set_Color(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);


        static constexpr auto _interface = "xyz.openbmc_project.Led.Physical";
        static const vtable::vtable_t _vtable[];
        sdbusplus::server::interface::interface
                _xyz_openbmc_project_Led_Physical_interface;

        Action _state = Action::Off;
        uint8_t _dutyOn = 50;
        Palette _color = Palette::Unknown;

};

/* Specialization of sdbusplus::server::bindings::details::convertForMessage
 * for enum-type Physical::Action.
 *
 * This converts from the enum to a constant c-string representing the enum.
 *
 * @param[in] e - Enum value to convert.
 * @return C-string representing the name for the enum value.
 */
std::string convertForMessage(Physical::Action e);
/* Specialization of sdbusplus::server::bindings::details::convertForMessage
 * for enum-type Physical::Palette.
 *
 * This converts from the enum to a constant c-string representing the enum.
 *
 * @param[in] e - Enum value to convert.
 * @return C-string representing the name for the enum value.
 */
std::string convertForMessage(Physical::Palette e);

} // namespace server
} // namespace Led
} // namespace openbmc_project
} // namespace xyz
} // namespace sdbusplus

