/**
 * Copyright © 2024 9elements
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

#include "led_description.hpp"

#include <gtest/gtest.h>

static LedDescr runtest(const std::string& name)
{
    LedDescr d;
    try
    {
        getLedDescr(name, d);
    }
    catch (...)
    {
        // Ignore the error, for simple LED which was not added in 3-part form.
        // The simple LED can appear with it's plain name
    }
    return d;
}

TEST(LEDDescr, Has4PartsInvalid)
{
    LedDescr d = runtest("devicename:color:function:part4");

    ASSERT_EQ("devicename", d.devicename);
    ASSERT_EQ("color", d.color);
    ASSERT_EQ("function", d.function);
}

TEST(LEDDescr, Has3Parts)
{
    LedDescr d = runtest("devicename:color:function");

    ASSERT_EQ("devicename", d.devicename);
    ASSERT_EQ("color", d.color);
    ASSERT_EQ("function", d.function);
}

TEST(LEDDescr, Has2PartsColorFunction)
{
    LedDescr d = runtest("red:fault");

    ASSERT_EQ("", d.devicename);
    ASSERT_EQ("red", d.color);
    ASSERT_EQ("fault", d.function);
}

TEST(LEDDescr, Has2PartsDevicenameFunction)
{
    LedDescr d = runtest("input9::capslock");

    ASSERT_EQ("input9", d.devicename);
    ASSERT_EQ("", d.color);
    ASSERT_EQ("capslock", d.function);
}

TEST(LEDDescr, Has1PartColor)
{
    LedDescr d = runtest("green:");

    ASSERT_EQ("", d.devicename);
    ASSERT_EQ("green", d.color);
    ASSERT_EQ("", d.function);
}

TEST(LEDDescr, Has1PartFunction)
{
    LedDescr d = runtest(":boot");

    ASSERT_EQ("", d.devicename);
    ASSERT_EQ("", d.color);
    ASSERT_EQ("boot", d.function);
}

TEST(LEDDescr, Has1PartLabel)
{
    LedDescr d = runtest("identify");

    ASSERT_EQ("identify", d.devicename);
    ASSERT_EQ("", d.color);
    ASSERT_EQ("", d.function);
}
