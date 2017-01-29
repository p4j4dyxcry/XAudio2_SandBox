#include "TsXAudio2.h"
#include "TsXAudio3DSound.h"
#include "TsXAudioMaster.h"

//=========================================================================
//! Constructor
//=========================================================================
TsXAudio3DSound::TsXAudio3DSound()
    :TsXAudioSound(),
    m_using3DSound(true)
{
    memset( &m_emitter , 0 , sizeof( m_emitter ) );
    memset( &m_listener , 0 , sizeof( m_listener ) );

    SetEmitterFront( 0 , 0 , 1 );
    SetEmitterTop( 0 , 1 , 0 );
    SetEmitterPostion( 0 , 0 , -1 );

    SetListenerFront( 0 , 0 , -1 );
    SetListenerTop( 0 , 1 , 0 );
    SetListenerPostion( 0 , 0 , 1 );

    m_emitter.ChannelCount = 1;
    m_emitter.CurveDistanceScaler = 1.0f;
    m_emitter.DopplerScaler = 1.0f;
    m_emitter.InnerRadius = 1;
}

bool TsXAudio3DSound::Play()
{
    Apply();
    return TsXAudioSound::Play();
}

//=========================================================================
//! Destructor
//=========================================================================
TsXAudio3DSound::~TsXAudio3DSound()
{

}
//=========================================================================
//! ������̐��ʃx�N�g����ݒ肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetListenerFront( float x , float y , float z )
{
    m_listener.front_x = x;
    m_listener.front_y = y;
    m_listener.front_z = z;
}

//=========================================================================
//! ������̏�����x�N�g����ݒ肵�܂��B
//  ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ��Ƃ����܂���B
//=========================================================================
void TsXAudio3DSound::SetListenerTop( float x , float y , float z )
{
    m_listener.up_x = x;
    m_listener.up_y = y;
    m_listener.up_z = z;
}

//=========================================================================
//! ������̃��[���h�ʒu��ݒ肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetListenerPostion( float x , float y , float z )
{
    m_listener.postion_x = x;
    m_listener.postion_y = y;
    m_listener.postion_z = z;
}

//=========================================================================
//! ������̉����x��ݒ肵�܂��B
//  �����x�̓h�b�v���[���ʂɗ��p����܂����K�{���ڂł͂���܂���B
//=========================================================================
void TsXAudio3DSound::SetListenervelocity( float x , float y , float z )
{
    m_listener.velocity_x = x;
    m_listener.velocity_y = y;
    m_listener.velocity_z = z;
}

//=========================================================================
//! �����̐��ʃx�N�g����ݒ肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetEmitterFront( float x , float y , float z )
{
    m_emitter.front_x = x;
    m_emitter.front_y = y;
    m_emitter.front_z = z;
}

//=========================================================================
//! �����̏�����x�N�g����ݒ肵�܂��B
//  ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ��Ƃ����܂���B
//=========================================================================
void TsXAudio3DSound::SetEmitterTop( float x , float y , float z )
{
    m_emitter.up_x = x;
    m_emitter.up_y = y;
    m_emitter.up_z = z;
}

//=========================================================================
//! �����̃��[���h�ʒu��ݒ肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetEmitterPostion( float x , float y , float z )
{
    m_emitter.postion_x = x;
    m_emitter.postion_y = y;
    m_emitter.postion_z = z;
}

//=========================================================================
//! �����̉����x��ݒ肵�܂��B
//  �����x�̓h�b�v���[���ʂɗ��p����܂����K�{���ڂł͂���܂���B
//=========================================================================
void TsXAudio3DSound::SetEmittervelocity( float x , float y , float z )
{
    m_emitter.velocity_x = x;
    m_emitter.velocity_y = y;
    m_emitter.velocity_z = z;
}

//=========================================================================
//! �h�b�v���[���ʂ̋������w�肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetEmitterDopplerLevel( float value )
{
    m_emitter.DopplerScaler = value;
}

//=========================================================================
//! �����̌��ʗ̈���w�肵�܂��B
//=========================================================================
void TsXAudio3DSound::SetEmitterRadius( float value )
{
    m_emitter.InnerRadius = value;
}

//=========================================================================
//! �ݒ肵��������Ɖ����̕ύX��K�p���܂��B
//  ���̊֐����R�[�������ۂ�TsXAudioSound::SetPan()�ŕύX���Ă���l�͖�������܂��B
//=========================================================================
bool TsXAudio3DSound::Apply()
{
    if( m_pSourceVoice == nullptr )
        return false;

    TsXAudio2Master& xAudioSystem = TsXAudio2Master::GetGlobalSystem();

    X3DAUDIO_LISTENER listener;
    memset( &listener , 0 , sizeof( listener ) );
    //! Convert listener 
    {
        listener.OrientFront.x = m_listener.front_x;
        listener.OrientFront.y = m_listener.front_y;
        listener.OrientFront.z = m_listener.front_z;

        listener.OrientTop.x = m_listener.up_x;
        listener.OrientTop.y = m_listener.up_y;
        listener.OrientTop.z = m_listener.up_z;

        listener.Position.x = m_listener.postion_x;
        listener.Position.y = m_listener.postion_y;
        listener.Position.z = m_listener.postion_z;

        listener.Velocity.x = m_listener.velocity_x;
        listener.Velocity.y = m_listener.velocity_y;
        listener.Velocity.z = m_listener.velocity_z;
    }

    X3DAUDIO_EMITTER  emitter;
    memset( &emitter , 0 , sizeof( emitter ) );
    //! Convert Emitter
    {
        emitter.OrientFront.x = m_emitter.front_x;
        emitter.OrientFront.y = m_emitter.front_y;
        emitter.OrientFront.z = m_emitter.front_z;

        emitter.OrientTop.x = m_emitter.up_x;
        emitter.OrientTop.y = m_emitter.up_y;
        emitter.OrientTop.z = m_emitter.up_z;

        emitter.Position.x = m_emitter.postion_x;
        emitter.Position.y = m_emitter.postion_y;
        emitter.Position.z = m_emitter.postion_z;

        emitter.Velocity.x = m_emitter.velocity_x;
        emitter.Velocity.y = m_emitter.velocity_y;
        emitter.Velocity.z = m_emitter.velocity_z;

        emitter.ChannelCount = m_emitter.ChannelCount;
        emitter.ChannelRadius = m_emitter.ChannelRadius;
        emitter.CurveDistanceScaler = m_emitter.CurveDistanceScaler;
        emitter.DopplerScaler = m_emitter.DopplerScaler;
        emitter.InnerRadius = m_emitter.InnerRadius;
        emitter.InnerRadiusAngle = m_emitter.InnerRadiusAngle;
    }

    TsXAudio2Master::_3DSOUND_PARMETER param = 
        xAudioSystem.Calculate3DSound( &listener , &emitter );

    if( m_pSourceVoice )
    {
        m_pSourceVoice->SetOutputMatrix( nullptr , param.srcChannel , param.dstChannnel , param.matrix );
        m_pSourceVoice->SetFrequencyRatio( param.doppler );
    }
    
    return true;
}