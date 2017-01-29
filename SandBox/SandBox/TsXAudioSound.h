#pragma once

#include "TsXAudio2.h"
#include "TsXAudioWaveData.h"

struct IXAudio2SourceVoice;

class TsXAudioSound
{
public:
    //! Constructor
    TsXAudioSound();

    //! Destructor
    virtual ~TsXAudioSound();

    //! ���̃T�E���h�����[�v���邩�ǂ����擾���܂��B
    //  true�̏ꍇ�͂��̃T�E���h�̓��[�v���Afalse�̏ꍇ�͂��̃T�E���h�̓��[�v���܂���B
    bool IsLoop()const{ return m_isLoop; }

    //! �Đ������ǂ������擾���܂��B
    bool IsPlaying()const;

    //! ���[�v�̃t���O�ݒ肵�܂��B
    //  true��ݒ肵���ꍇ�̓��[�v���L���ɂȂ�Afalse��ݒ肵���ꍇ�̓��[�v�͖����ɂȂ�܂��B
    void SetLoopFlag( bool loopFlag );

    //! �T�E���h���Đ����܂��B�ȑO�Ɉꎞ��~����Ă����ꍇ�͋L���ʒu����ĊJ���܂��B
    virtual bool Play();

    //! �T�E���h���~�����܂��B�Đ��ʒu�͕ۑ�����܂���B
    bool Stop();

    //! �T�E���h���ŏ�����Đ����܂��B
    bool RePlay();

    //! ���݂̍Đ��ʒu��ێ������܂܃T�E���h�̍Œ���ꎞ��~���܂��B
    bool Pause();

    //! ���݂̃��[�v�ŃT�E���h�����[�v���I�����܂��B
    bool ExitLoop();

    //! �s�b�`��ݒ肵�܂��B
    bool SetPitch( float value );

    //! �s�b�`���擾���܂��B
    float GetPitch()const;

    //! ���݂̃s�b�`������Z�܂��͌��Z�����܂��B
    //  ���̊֐��͐ݒ肳��Ă���ő�s�b�`�𒴂��đ傫���Ȃ�܂���B
    //  �܂��A���Z����ꍇ��0.0005�������܂���B
    bool AdjustPitch( float value );

    //! �ő剹�ʂ�ݒ肵�܂��B
    void  SetMaxPitch( float value );

    //! *.wav�t�@�C�����特���f�[�^��ǂݍ��݂܂��B
    bool CreateSound( TsXaudioWaveData* pWavData );

    //! ���ʂ��擾���܂��B
    float GetVolume()const;

    //! ���ʂ�ݒ肵�܂��B
    bool  SetVolume(float value);

    //! �ő剹�ʂ�ݒ肵�܂��B
    void  SetMaxVolume(float value);

    //! ���݂̉��ʂ�����Z�܂��͌��Z�����܂��B
    //  ���̊֐��͐ݒ肳��Ă���ő剹�ʂ𒴂��đ傫���Ȃ�܂���B
    //  �܂��A���Z����ꍇ�����ʂ�0�������܂���B
    bool  AdjustVolume( float value );

    //! �w�b�h�Z�b�g��X�s�[�J�[����o�͂���鉹�̃o�����X��ύX���܂��B
    //  [-1.0] ���̂� [ 0.0] ���� [+1.0] �E�̂�   
    //  -0.75f�Ȃǂ�ݒ肵���ꍇ�͍�75%�E25%�ŏo�͂���܂��B
    //  -1�܂���1�𒴂���l��ݒ肵���ꍇ�͎����I�Ɂ@-1~1�@�ɐ��K������܂��B
    //  ��3D�T�E���h���g�p����ꍇ���̃��\�b�h�͖����ɂȂ�܂��B
    bool SetPan( float value );

    //! Reverb(�����G�t�F�N�g)�̗L���t���O��ݒ肵�܂��B
    bool UsingReverb( bool flag );

protected:

    //! �����f�[�^���X�g���[���ɗ������݂܂��B
    //  ���̊֐��͊O������Ăяo���܂���B
    bool Submit();

protected:
    //! �ϐ�
    TsXaudioWaveData*       m_pOriginData;  //  �T�E���h�����f�[�^
    IXAudio2SourceVoice *   m_pSourceVoice; //  �T�E���h���Đ�����X�g���[��
    
    bool                    m_isLoop;       //  ���[�v�t���O
    float                   m_maxVolume;    //  �ő剹��
    float                   m_maxPitch;     //  �ő�s�b�`
};
