#pragma once

namespace Wave
{
    float Sawtooth( float time, float frequency );
    float Sine( float time, float frequency );
    float Square( float time, float frequency );
    float Triangle( float time, float frequency );
}


namespace Noise
{
    void Seed( unsigned int );
    float Brown();
    float Violet();
    float Pink();
    float White();
}

