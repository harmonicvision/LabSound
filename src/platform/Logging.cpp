/*
 * Copyright (C) 2003, 2006, 2013 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "LabSoundConfig.h"
#include "Logging.h"

#include <string>

#if !LOG_DISABLED

namespace WebCore {

    // Macro that returns a compile time constant with the length of an array, but gives an error if passed a non-array.
    template<typename T, size_t Size> char(&ArrayLengthHelperFunction(T(&)[Size]))[Size];
    // GCC needs some help to deduce a 0 length array.
#if COMPILER(GCC)
    template<typename T> char(&ArrayLengthHelperFunction(T(&)[0]))[0];
#endif
#define WTF_ARRAY_LENGTH(array) sizeof(ArrayLengthHelperFunction(array))

#define WTF_ARRAY_LENGTH(array) sizeof(ArrayLengthHelperFunction(array))


    /// @LabSound added, since the WebKit string database is not part of LabSound.
    /// Change this to some other value, such as "all" to enable logging.
    std::string logLevelString() {
        return std::string();
    }

#define DEFINE_LOG_CHANNEL(name) \
WTFLogChannel JOIN_LOG_CHANNEL_WITH_PREFIX(LOG_CHANNEL_PREFIX, name) = { WTFLogChannelOff, #name };
    WEBCORE_LOG_CHANNELS(DEFINE_LOG_CHANNEL)

#define LOG_CHANNEL_ADDRESS(name)  &JOIN_LOG_CHANNEL_WITH_PREFIX(LOG_CHANNEL_PREFIX, name),
    WTFLogChannel* logChannels[] = {
        WEBCORE_LOG_CHANNELS(LOG_CHANNEL_ADDRESS)
    };

    size_t logChannelCount = WTF_ARRAY_LENGTH(logChannels);

    WTFLogChannel* logChannelByName(const std::string& name)
    {
        return WTFLogChannelByName(logChannels, logChannelCount, name.c_str());
    }

    void initializeLoggingChannelsIfNecessary()
    {
        static bool haveInitializedLoggingChannels = false;
        if (haveInitializedLoggingChannels)
            return;
        haveInitializedLoggingChannels = true;

        WTFInitializeLogChannelStatesFromString(logChannels, logChannelCount, logLevelString().c_str());
    }
    
}

#endif // !LOG_DISABLED
