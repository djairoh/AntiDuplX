/*
* AntiDuplX utility (http://github.com/ermig1979/AntiDuplX).
*
* Copyright (c) 2023-2023 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include "AntiDuplX/AdxCommon.h"

#include "Cpl/Args.h"
#include "Cpl/Log.h"

namespace Adx
{
    struct Options : public Cpl::ArgsParser
    {
        String mode;
        Strings imageDirectories;
        Strings imageExtensions;
        bool subDirectories;
        Cpl::Log::Level logLevel;
        String logFile;
        double threshold;

        Options(int argc, char* argv[])
            : ArgsParser(argc, argv, true)
        {
            mode = GetArg("-m", "test");
            imageDirectories = GetArgs("-id", Strings({ "." }));
            imageExtensions = GetArgs("-ie", Strings( { ".jpg", ".png"}));
            subDirectories = Cpl::ToVal<bool>(GetArg("-sd", "1"));
            logLevel = (Cpl::Log::Level)Cpl::ToVal<Int>(GetArg2("-ll", "--logLevel", "4", false));
            logFile = GetArg2("-lf", "--logFile", "", false);
            threshold = Cpl::ToVal<double>(GetArg2("-t", "--threshold", "0.05", false));
        }

        ~Options()
        {
        }
    };
}
