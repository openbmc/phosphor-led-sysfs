/**
 * Copyright © 2016 IBM Corporation
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

#include <CLI/CLI.hpp>
#include <boost/algorithm/string.hpp>

#include <string>

/* LED sysfs name can be any of
 *
 * - devicename:color:function
 * - devicename::function
 * - color:function (e.g. "red:fault")
 * - label (e.g. "identify")
 * - :function (e.g. ":boot")
 * - color: (e.g. "green:")
 *
 * but no one prevents us from making all of this up and creating
 * a label with colons inside, e.g. "mylabel:mynoncolorstring:lala".
 *
 * Summary: It's bonkers (not my words, but describes it well)
 */
void getLedDescr(const std::string& name, LedDescr& ledDescr)
{
    std::vector<std::string> words;
    // FIXME: https://bugs.llvm.org/show_bug.cgi?id=41141
    // NOLINTBEGIN(clang-analyzer-cplusplus.NewDeleteLeaks)
    boost::split(words, name, boost::is_any_of(":"));
    // NOLINTEND(clang-analyzer-cplusplus.NewDeleteLeaks)

    if (name.empty())
    {
        return;
    }

    if (words.size() == 1)
    {
        ledDescr.devicename = words.at(0);
        return;
    }

    if (words.size() == 2)
    {
        ledDescr.color = words.at(0);
        ledDescr.function = words.at(1);
        return;
    }

    ledDescr.devicename = words.at(0);
    ledDescr.color = words.at(1);
    ledDescr.function = words.at(2);

    // if there is more than 3 parts we ignore the rest
}
