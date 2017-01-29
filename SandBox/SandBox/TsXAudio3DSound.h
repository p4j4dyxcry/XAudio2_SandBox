#pragma once
#include "TsXAudio2.h"
#include "TsXAudioSound.h"

class TsXAudio3DSound : public TsXAudioSound
{
public:

    //! リスナーデータ
    struct AUDIO_LISTENER
    {
        float front_x , front_y , front_z;
        float up_x , up_y , up_z;
        float postion_x , postion_y , postion_z;
        float velocity_x , velocity_y , velocity_z;
    };

    //! エミッターデータ
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

    //! サウンドを再生します。
    //  TsXAudioSound::Play()を実行＋3Dサウンドを適用します。
    virtual bool Play()override;

    //! 聞き手の正面ベクトルを設定します。
    void SetListenerFront( float x , float y , float z );

    //! 聞き手の上方向ベクトルを設定します。
    //  この値は正面ベクトルに直交していないといけません。
    void SetListenerTop( float x , float y , float z );

    //! 聞き手のワールド位置を設定します。
    void SetListenerPostion( float x , float y , float z );

    //! 聞き手の加速度を設定します。
    //  加速度はドップラー効果に利用されますが必須項目ではありません。
    void SetListenervelocity( float x , float y , float z );

    //! 音源の正面ベクトルを設定します。
    void SetEmitterFront( float x , float y , float z );

    //! 音源の上方向ベクトルを設定します。
    //  この値は正面ベクトルに直交していないといけません。
    void SetEmitterTop( float x , float y , float z );

    //! 音源のワールド位置を設定します。
    void SetEmitterPostion( float x , float y , float z );

    //! 音源の加速度を設定します。
    //  加速度はドップラー効果に利用されますが必須項目ではありません。
    void SetEmittervelocity( float x , float y , float z );

    //! ドップラー効果の強さを指定します。
    void SetEmitterDopplerLevel( float value );

    //! 音源の効果領域を指定します。
    void SetEmitterRadius( float value );


    //! 設定した聞き手と音源の変更を適用します。
    //  この関数をコールした際にTsXAudioSound::SetPan()で変更している値は無視されます。
    bool Apply();
protected:
    bool m_using3DSound;
    AUDIO_EMITTER    m_emitter;
    AUDIO_LISTENER   m_listener;
};