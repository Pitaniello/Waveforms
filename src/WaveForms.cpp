#include <cmath>
#include <cstdlib>

#include "WaveForms.h"


static const float PI    = 4.0f * std::atanf( 1.0f );
static const float TWOPI = 2.0f * 4.0f * std::atanf( 1.0f );


namespace Wave
{
    float Sawtooth( float time, float frequency )
    {
        return std::fmod( time * frequency, 1.0f ) * 2.0f - 1.0f;
    }


    float Sine( float time, float frequency )
    {
        return std::sinf( time * TWOPI * frequency );
    }


    float Square( float time, float frequency )
    {
        if ( 0.5f < ( 1.0f - std::fmod( time * frequency, 1.0f ) ) )
        {
            return 1.0f;
        }
        else
        {
            return -1.0f;
        }
    }


    float Triangle( float time, float frequency )
    {
        return 2.0f * ( (1.0f - std::fabs( std::fmod( time * frequency, 1.0f ) * 2.0f - 1.0f ) ) - 0.5f );
    }
}


namespace Noise
{
    void Seed( unsigned int seedVal )
    {
        srand( seedVal );
    }

    static float s_brownPrev;

    float Brown()
    {
        return 0.0f;
    }

    float Violet()
    {
        return 0.0f;
    }

    float Pink()
    {
        return 0.0f;
    }

    static const int HALF_RAND_MAX = RAND_MAX / 2;

    float White()
    {
        return static_cast< float >( rand() - HALF_RAND_MAX ) / HALF_RAND_MAX;
    }
}
