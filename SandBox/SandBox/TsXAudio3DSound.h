#pragma once
#include "TsXAudio2.h"
#include "TsXAudioSound.h"

class TsXAudio3DSound : public TsXAudioSound
{
public:

    //! ���X�i�[�f�[�^
    struct AUDIO_LISTENER
    {
        float front_x , front_y , front_z;
        float up_x , up_y , up_z;
        float postion_x , postion_y , postion_z;
        float velocity_x , velocity_y , velocity_z;
    };

    //! �G�~�b�^�[�f�[�^
    struct AUDIO_EMITTER : AUDIO_LISTENER
    {
        float InnerRadius;
        float InnerRadiusAngle;

        unsigned int ChannelCount;
        float ChannelRadius;
        
        float CurveDistanceScaler;
        float DopplerScaler;
    };

    TsXAudio3DSound();
    ~TsXAudio3DSound();

    //! �T�E���h���Đ����܂��B
    //  TsXAudioSound::Play()�����s�{3D�T�E���h��K�p���܂��B
    virtual bool Play()override;

    //! ������̐��ʃx�N�g����ݒ肵�܂��B
    void SetListenerFront( float x , float y , float z );

    //! ������̏�����x�N�g����ݒ肵�܂��B
    //  ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ��Ƃ����܂���B
    void SetListenerTop( float x , float y , float z );

    //! ������̃��[���h�ʒu��ݒ肵�܂��B
    void SetListenerPostion( float x , float y , float z );

    //! ������̉����x��ݒ肵�܂��B
    //  �����x�̓h�b�v���[���ʂɗ��p����܂����K�{���ڂł͂���܂���B
    void SetListenervelocity( float x , float y , float z );

    //! �����̐��ʃx�N�g����ݒ肵�܂��B
    void SetEmitterFront( float x , float y , float z );

    //! �����̏�����x�N�g����ݒ肵�܂��B
    //  ���̒l�͐��ʃx�N�g���ɒ������Ă��Ȃ��Ƃ����܂���B
    void SetEmitterTop( float x , float y , float z );

    //! �����̃��[���h�ʒu��ݒ肵�܂��B
    void SetEmitterPostion( float x , float y , float z );

    //! �����̉����x��ݒ肵�܂��B
    //  �����x�̓h�b�v���[���ʂɗ��p����܂����K�{���ڂł͂���܂���B
    void SetEmittervelocity( float x , float y , float z );

    //! �h�b�v���[���ʂ̋������w�肵�܂��B
    void SetEmitterDopplerLevel( float value );

    //! �����̌��ʗ̈���w�肵�܂��B
    void SetEmitterRadius( float value );


    //! �ݒ肵��������Ɖ����̕ύX��K�p���܂��B
    //  ���̊֐����R�[�������ۂ�TsXAudioSound::SetPan()�ŕύX���Ă���l�͖�������܂��B
    bool Apply();
protected:
    bool m_using3DSound;
    AUDIO_EMITTER    m_emitter;
    AUDIO_LISTENER   m_listener;
};