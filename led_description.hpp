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

#pragma once

#include <string>

struct LedDescr
{
    // may be empty
    std::string devicename;
    // may be empty
    std::string color;
    // may be empty
    std::string function;
};

/** @brief parse LED name in sysfs
 *  Parse sysfs LED name and sets corresponding
 *  fields in LedDescr struct.
 *
 *  @param[in] name      - LED name in sysfs
 *  @param[out] ledDescr - LED description
 */
void getLedDescr(const std::string& name, LedDescr& ledDescr);
