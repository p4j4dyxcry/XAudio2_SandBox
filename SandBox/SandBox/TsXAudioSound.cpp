#include "TsXAudioSound.h"
#include "TsXAudioMaster.h"

//=========================================================================
//! Constructor
//=========================================================================
TsXAudioSound::TsXAudioSound()
    :m_pSourceVoice( nullptr ) ,
    m_pOriginData( nullptr ) ,
    m_isLoop( false ),
    m_maxVolume(2.0f),
    m_maxPitch(2.0f)
{

}

//=========================================================================
//! Destructor
//=========================================================================
TsXAudioSound::~TsXAudioSound()
{
    if( m_pSourceVoice )
    {
        m_pSourceVoice->FlushSourceBuffers();

        XAUDIO2_VOICE_STATE xState;
        m_pSourceVoice->GetState( &xState );
        while( xState.BuffersQueued != 0 )
        {
            m_pSourceVoice->GetState( &xState );
            Sleep( 5 );
        }
        m_pSourceVoice->DestroyVoice();
    }
}

//=========================================================================
//! 音源データをストリームに流し込みます。
//  この関数は外部から呼び出せません。
//=========================================================================
bool TsXAudioSound::Submit()
{
    XAUDIO2_BUFFER buffer;
    memset( &buffer , 0 , sizeof( XAUDIO2_BUFFER ) );
    buffer.AudioBytes = m_pOriginData->GetWaveSize();
    buffer.pAudioData = m_pOriginData->GetWaveData();
    buffer.LoopCount = IsLoop() ? XAUDIO2_LOOP_INFINITE : 0;

    if( m_pSourceVoice )
        m_pSourceVoice->SubmitSourceBuffer( &buffer );

    return true;
}

bool TsXAudioSound::IsPlaying()const
{
    XAUDIO2_VOICE_STATE xState;
    m_pSourceVoice->GetState( &xState );

    return xState.BuffersQueued != 0;
}

//=========================================================================
//! ループのフラグ設定します。
//  trueを設定した場合はループが有効になり、falseを設定した場合はループは無効になります。
//=========================================================================
void TsXAudioSound::SetLoopFlag( bool loopFlag )
{
    m_isLoop = loopFlag;
    Submit();
}

//=========================================================================
//! サウンドを再生します。以前に一時停止されていた場合は記憶位置から再開します。
//=========================================================================
bool TsXAudioSound::Play()
{
    if( m_pSourceVoice )
    {
        XAUDIO2_VOICE_STATE xState;
        m_pSourceVoice->GetState( &xState );

        if( xState.BuffersQueued == 0 )
        {
            m_pSourceVoice->FlushSourceBuffers();
            Submit();
        }

        m_pSourceVoice->Start( 0 );
    }
    return true;
}

//=========================================================================
//! 現在のループでサウンドをループを終了します。
//=========================================================================
bool TsXAudioSound::ExitLoop()
{
    if( m_pSourceVoice )
    {
        m_pSourceVoice->ExitLoop();
    }
    else
    {
        return false;
    }
    return true;
}

//=========================================================================
//! サウンドを停止させます。再生位置は保存されません。
//=========================================================================
bool TsXAudioSound::Stop()
{
    if( m_pSourceVoice )
    {
        m_pSourceVoice->Stop( 0 );
        m_pSourceVoice->FlushSourceBuffers();
    }
    return true;
}

//=========================================================================
//! サウンドを最初から再生します。
//=========================================================================
bool TsXAudioSound::RePlay()
{
    if( m_pSourceVoice )
    {
        Stop();
        Play();
    }
    return true;
}

//=========================================================================
//! 現在の再生位置を保持したままサウンドの最低を一時停止します。
//=========================================================================
bool TsXAudioSound::Pause()
{
    if( m_pSourceVoice )
    {
        m_pSourceVoice->Stop( 0 );
    }
    return true;
}

//=========================================================================
//! *.wavファイルから音源データを読み込みます。
//=========================================================================
bool TsXAudioSound::CreateSound( TsXaudioWaveData * pWaveData )
{
    m_pOriginData = pWaveData;

    TsXAudio2Master& xAudioMaster = TsXAudio2Master::GetGlobalSystem();

    IXAudio2*   pHandle = xAudioMaster.GetInterface();

    HRESULT hr = pHandle->CreateSourceVoice( &m_pSourceVoice ,
                                             &m_pOriginData->GetWaveFmtEx() );

    if( FAILED( hr ) )
        return false;


    //! Create Reverb
    IUnknown * pReverbAPO;
    XAudio2CreateReverb( &pReverbAPO );

    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = false;
    desc.pEffect = pReverbAPO;
    desc.OutputChannels = 1;

    XAUDIO2_EFFECT_CHAIN chain;
    chain.EffectCount = 1;
    chain.pEffectDescriptors = &desc;
    m_pSourceVoice->SetEffectChain( &chain );
    Submit();


    return true;
}

//=========================================================================
//! ヘッドセットやスピーカーから出力される音のバランスを変更します。
//  [-1.0] 左のみ [ 0.0] 両方 [+1.0] 右のみ   
//  -0.75fなどを設定した場合は左75%右25%で出力されます。
//  -1または1を超える値を設定した場合は自動的に　-1~1　に正規化されます。
//  ※3Dサウンドを使用する場合このメソッドは無効になります。
//=========================================================================
bool TsXAudioSound::SetPan( float value )
{
    if( m_pSourceVoice == nullptr )
        return false;

    if( value >  1.0f )value =  1.0f;
    if( value < -1.0f )value = -1.0f;

    XAUDIO2_VOICE_DETAILS details;
    if( m_pSourceVoice )
        m_pSourceVoice->GetVoiceDetails( &details );

    int sz = details.InputChannels * 2;

    float *matrix = new float[sz];
    float halfPan = value / 2.0f;
    for( int i = 0; i < sz; ++i )
    {
        if( i % 2 == 0 )
            matrix[i] = 0.5f - halfPan;
        else if( i % 2 == 1 )
            matrix[i] = 0.5f + halfPan;
    }
    if( m_pSourceVoice )
        m_pSourceVoice->SetOutputMatrix( nullptr , details.InputChannels , 2 , matrix );
    delete[] matrix;
    return true;
}

//=========================================================================
//! 音量を取得します。
//=========================================================================
float TsXAudioSound::GetVolume()const
{
    float voluem = 0;
    if( m_pSourceVoice )
    {
        m_pSourceVoice->GetVolume( &voluem );
    }

    return voluem;
}

//=========================================================================
//! 音量を設定します。
//=========================================================================
bool  TsXAudioSound::SetVolume( float value )
{
    if( m_pSourceVoice )
    {
        m_pSourceVoice->SetVolume( max( min( value , m_maxVolume ) , 0 ) );
        return true;
    }

    return false;
}

//=========================================================================
//! 音量を設定します。
//=========================================================================
void  TsXAudioSound::SetMaxVolume( float value )
{
    m_maxVolume = value;
}

//=========================================================================
//! 現在の音量から加算または減算させます。
//  この関数は設定されている最大音量以上は大きくなりません。
//  また、減算する場合も音量は0を下回りません。
//=========================================================================
bool TsXAudioSound::AdjustVolume( float value )
{
    return SetVolume( GetVolume() + value );
}

//=========================================================================
//! ピッチを設定します。
//=========================================================================
bool TsXAudioSound::SetPitch( float value )
{
    if( m_pSourceVoice )
    { 
        m_pSourceVoice->SetFrequencyRatio( max( min( value , m_maxPitch ) , XAUDIO2_MIN_FREQ_RATIO ) );
    }
    else
    {
        return false;
    }
    return true;
}

//=========================================================================
//! ピッチを取得します。
//=========================================================================
float TsXAudioSound::GetPitch()const
{
    float pitch = XAUDIO2_MIN_FREQ_RATIO;
    if( m_pSourceVoice )
    {
        m_pSourceVoice->GetFrequencyRatio( &pitch );
    }
    return pitch;
}

//=========================================================================
//! 現在のピッチから加算または減算させます。
//  この関数は設定されている最大ピッチを超えて大きくなりません。
//  また、減算する場合も0.0005を下回りません。
//=========================================================================
bool TsXAudioSound::AdjustPitch( float value )
{
    return SetPitch( GetPitch() + value );
}

//=========================================================================
//! 最大ピッチを設定します。
//=========================================================================
void  TsXAudioSound::SetMaxPitch( float value )
{
    m_maxPitch = max( min( value , 2.0f ) , XAUDIO2_MIN_FREQ_RATIO );
}

//=========================================================================
//! Reverb(反響エフェクト)の有効フラグを設定します。
//=========================================================================
bool TsXAudioSound::UsingReverb( bool flag )
{
    if( m_pSourceVoice )
    {
        if( flag )
            m_pSourceVoice->EnableEffect( 0 );
        else
            m_pSourceVoice->DisableEffect( 0 );
    }
    else
    {
        return false;
    }

    return true;
}