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
//! �����f�[�^���X�g���[���ɗ������݂܂��B
//  ���̊֐��͊O������Ăяo���܂���B
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
//! ���[�v�̃t���O�ݒ肵�܂��B
//  true��ݒ肵���ꍇ�̓��[�v���L���ɂȂ�Afalse��ݒ肵���ꍇ�̓��[�v�͖����ɂȂ�܂��B
//=========================================================================
void TsXAudioSound::SetLoopFlag( bool loopFlag )
{
    m_isLoop = loopFlag;
    Submit();
}

//=========================================================================
//! �T�E���h���Đ����܂��B�ȑO�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ���܂��B
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
//! ���݂̃��[�v�ŃT�E���h�����[�v���I�����܂��B
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
//! �T�E���h���~�����܂��B�Đ��ʒu�͕ۑ�����܂���B
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
//! �T�E���h���ŏ�����Đ����܂��B
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
//! ���݂̍Đ��ʒu��ێ������܂܃T�E���h�̍Œ���ꎞ��~���܂��B
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
//! *.wav�t�@�C�����特���f�[�^��ǂݍ��݂܂��B
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
//! �w�b�h�Z�b�g��X�s�[�J�[����o�͂���鉹�̃o�����X��ύX���܂��B
//  [-1.0] ���̂� [ 0.0] ���� [+1.0] �E�̂�   
//  -0.75f�Ȃǂ�ݒ肵���ꍇ�͍�75%�E25%�ŏo�͂���܂��B
//  -1�܂���1�𒴂���l��ݒ肵���ꍇ�͎����I�Ɂ@-1~1�@�ɐ��K������܂��B
//  ��3D�T�E���h���g�p����ꍇ���̃��\�b�h�͖����ɂȂ�܂��B
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
//! ���ʂ��擾���܂��B
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
//! ���ʂ�ݒ肵�܂��B
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
//! ���ʂ�ݒ肵�܂��B
//=========================================================================
void  TsXAudioSound::SetMaxVolume( float value )
{
    m_maxVolume = value;
}

//=========================================================================
//! ���݂̉��ʂ�����Z�܂��͌��Z�����܂��B
//  ���̊֐��͐ݒ肳��Ă���ő剹�ʈȏ�͑傫���Ȃ�܂���B
//  �܂��A���Z����ꍇ�����ʂ�0�������܂���B
//=========================================================================
bool TsXAudioSound::AdjustVolume( float value )
{
    return SetVolume( GetVolume() + value );
}

//=========================================================================
//! �s�b�`��ݒ肵�܂��B
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
//! �s�b�`���擾���܂��B
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
//! ���݂̃s�b�`������Z�܂��͌��Z�����܂��B
//  ���̊֐��͐ݒ肳��Ă���ő�s�b�`�𒴂��đ傫���Ȃ�܂���B
//  �܂��A���Z����ꍇ��0.0005�������܂���B
//=========================================================================
bool TsXAudioSound::AdjustPitch( float value )
{
    return SetPitch( GetPitch() + value );
}

//=========================================================================
//! �ő�s�b�`��ݒ肵�܂��B
//=========================================================================
void  TsXAudioSound::SetMaxPitch( float value )
{
    m_maxPitch = max( min( value , 2.0f ) , XAUDIO2_MIN_FREQ_RATIO );
}

//=========================================================================
//! Reverb(�����G�t�F�N�g)�̗L���t���O��ݒ肵�܂��B
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