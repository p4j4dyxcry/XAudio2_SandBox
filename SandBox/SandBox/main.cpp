#include "Header.h"
#include "TsSound.h"
void main()
{
    TsXaudioWaveData waveData;
    waveData.LoadFromFile( "test.wav" );

    TsXAudio3DSound pSource;
    pSource.CreateSound( &waveData );
    pSource.SetLoopFlag( true );
    //pSource.SetPan( -1 );
    pSource.SetEmitterPostion( -15 , 0 , -10 );
    pSource.SetEmitterRadius( 1 );
    pSource.SetVolume( 2 );
    pSource.Play();

    pSource.UsingReverb( true );

    float x = -15;
    int signe = 1;
    while( 1 )
    {
        x += 0.05f * signe;

        if( abs( x ) > 15.0f )
            signe *= -1;

        Sleep( 16 );
        pSource.SetEmitterPostion( x , 0 , 10 );
        pSource.Apply();

    }
}