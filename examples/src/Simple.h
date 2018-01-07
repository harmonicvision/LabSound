// License: BSD 2 Clause
// Copyright (C) 2015+, The LabSound Authors. All rights reserved.

#include "ExampleBaseApp.h"

struct SimpleApp : public LabSoundExampleApp
{
    void PlayExample()
    {
        auto context = lab::MakeRealtimeAudioContext();
        
        auto ac = context.get();
        
        std::shared_ptr<OscillatorNode> oscillator;
        std::shared_ptr<SampledAudioNode> musicClipNode;
        std::shared_ptr<GainNode> gain;
        std::shared_ptr<AudioBus> musicClip = MakeBusFromFile("samples/stereo-music-clip.wav", false);

        auto lockedArea = [&](ContextGraphLock & g, ContextRenderLock & r)
        {
            oscillator = std::make_shared<OscillatorNode>(r, context->sampleRate());
            gain = std::make_shared<GainNode>(context->sampleRate());
            gain->gain()->setValue(0.0625f);

            musicClipNode = std::make_shared<SampledAudioNode>(musicClip);

            // osc -> gain -> destination
            ac->connect(gain, oscillator, 0, 0);
            ac->connect(context->destination(), gain, 0, 0);

            //oscillator->start(0);
            oscillator->frequency()->setValue(440.f);
            oscillator->setType(r, OscillatorType::SINE);

            musicClipNode->start(0.0f);
        };

        lab::AcquireLocksForContext("Tone and Sample App", context.get(), lockedArea);

        const int seconds = 4;
        for (int t = 0; t < seconds; ++t)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }

    }
};
