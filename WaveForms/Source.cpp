#include "..\src\WaveForms.h"

#include <fstream>

typedef float( *noiseMethod )();
typedef float( *waveMethod )( float, float );

void WriteNoiseToBuffer( noiseMethod func, short *buffer, long writeLength )
{
    for ( long idx = 0; idx < writeLength; ++idx )
    {
        buffer[idx] = static_cast< short >( func() * 32767 );
    }
}

void WriteWaveToBuffer( waveMethod func, short *buffer, long writeLength, double step, float frequency )
{
    long pos = 0;
    double time = 0.0;

    for ( long idx = 0; idx < writeLength; ++idx )
    {
        buffer[idx] = static_cast< short >( func( static_cast< float >( time ), frequency ) * 32767 );

        time += step;
    }
}

int main(void)
{
    const int HEADER_SIZE = 44;
    const unsigned int RATE = 44100;
    const short CHANNELS = 1;
    const short BIT_DEPTH = 16;
    const short LENGTH = 7; // seconds
    const float FREQUENCY = 440.0f;

    long segmentSize = static_cast< long >( RATE * CHANNELS );
    long bufferSize = static_cast< long >( RATE * CHANNELS * LENGTH );
    long dataSize = static_cast< long >( RATE * CHANNELS * LENGTH * BIT_DEPTH / 8 );
    long offset = 0;

    double step = 1.0 / RATE;
    double time = 0.0;

    char header[HEADER_SIZE] = { 0 };

    *( header + 0 ) = 'R';
    *( header + 1 ) = 'I';
    *( header + 2 ) = 'F';
    *( header + 3 ) = 'F';
    *reinterpret_cast< int32_t * >( header + 4 ) = dataSize + 36;
    *( header + 8 ) = 'W';
    *( header + 9 ) = 'A';
    *( header + 10 ) = 'V';
    *( header + 11 ) = 'E';

    *( header + 12 ) = 'f';
    *( header + 13 ) = 'm';
    *( header + 14 ) = 't';
    *( header + 15 ) = ' ';
    *reinterpret_cast< int32_t * >( header + 16 ) = 16;
    *reinterpret_cast< int16_t * >( header + 20 ) = 1;
    *reinterpret_cast< int16_t * >( header + 22 ) = 1;
    *reinterpret_cast< int32_t * >( header + 24 ) = RATE;
    *reinterpret_cast< int32_t * >( header + 28 ) = RATE * CHANNELS * BIT_DEPTH / 8;
    *reinterpret_cast< int16_t * >( header + 32 ) = CHANNELS * BIT_DEPTH / 2;
    *reinterpret_cast< int16_t * >( header + 34 ) = BIT_DEPTH;

    *( header + 36 ) = 'd';
    *( header + 37 ) = 'a';
    *( header + 38 ) = 't';
    *( header + 39 ) = 'a';
    *reinterpret_cast< int32_t * >( header + 40 ) = dataSize;


    short *buffer = nullptr;
    buffer = new short[bufferSize];

    WriteWaveToBuffer( Wave::Sawtooth,  buffer + offset, segmentSize, step, FREQUENCY );
    offset += segmentSize;
    WriteWaveToBuffer( Wave::Sine,      buffer + offset, segmentSize, step, FREQUENCY );
    offset += segmentSize;
    WriteWaveToBuffer( Wave::Square,    buffer + offset, segmentSize, step, FREQUENCY );
    offset += segmentSize;
    WriteWaveToBuffer( Wave::Triangle,  buffer + offset, segmentSize, step, FREQUENCY );
    offset += segmentSize;

    Noise::Seed( 0 );
    WriteNoiseToBuffer( Noise::Brown, buffer + offset, segmentSize );
    offset += segmentSize;
    WriteNoiseToBuffer( Noise::Pink,  buffer + offset, segmentSize );
    offset += segmentSize;
    WriteNoiseToBuffer( Noise::White, buffer + offset, segmentSize );
    offset += segmentSize;


    std::fstream fout;
    fout.open( "test.wav", std::fstream::binary | std::fstream::trunc | std::fstream::out );

    if ( fout.is_open() )
    {
        fout.write( header, HEADER_SIZE );
        fout.write( reinterpret_cast< char *>( buffer ), dataSize );
        fout.close();
    }

    delete[] buffer;

    return 0;
}
