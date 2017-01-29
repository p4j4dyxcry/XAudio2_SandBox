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

    //! このサウンドがループするかどうか取得します。
    //  trueの場合はこのサウンドはループし、falseの場合はこのサウンドはループしません。
    bool IsLoop()const{ return m_isLoop; }

    //! 再生中かどうかを取得します。
    bool IsPlaying()const;

    //! ループのフラグ設定します。
    //  trueを設定した場合はループが有効になり、falseを設定した場合はループは無効になります。
    void SetLoopFlag( bool loopFlag );

    //! サウンドを再生します。以前に一時停止されていた場合は記憶位置から再開します。
    virtual bool Play();

    //! サウンドを停止させます。再生位置は保存されません。
    bool Stop();

    //! サウンドを最初から再生します。
    bool RePlay();

    //! 現在の再生位置を保持したままサウンドの最低を一時停止します。
    bool Pause();

    //! 現在のループでサウンドをループを終了します。
    bool ExitLoop();

    //! ピッチを設定します。
    bool SetPitch( float value );

    //! ピッチを取得します。
    float GetPitch()const;

    //! 現在のピッチから加算または減算させます。
    //  この関数は設定されている最大ピッチを超えて大きくなりません。
    //  また、減算する場合も0.0005を下回りません。
    bool AdjustPitch( float value );

    //! 最大音量を設定します。
    void  SetMaxPitch( float value );

    //! *.wavファイルから音源データを読み込みます。
    bool CreateSound( TsXaudioWaveData* pWavData );

    //! 音量を取得します。
    float GetVolume()const;

    //! 音量を設定します。
    bool  SetVolume(float value);

    //! 最大音量を設定します。
    void  SetMaxVolume(float value);

    //! 現在の音量から加算または減算させます。
    //  この関数は設定されている最大音量を超えて大きくなりません。
    //  また、減算する場合も音量は0を下回りません。
    bool  AdjustVolume( float value );

    //! ヘッドセットやスピーカーから出力される音のバランスを変更します。
    //  [-1.0] 左のみ [ 0.0] 両方 [+1.0] 右のみ   
    //  -0.75fなどを設定した場合は左75%右25%で出力されます。
    //  -1または1を超える値を設定した場合は自動的に　-1~1　に正規化されます。
    //  ※3Dサウンドを使用する場合このメソッドは無効になります。
    bool SetPan( float value );

    //! Reverb(反響エフェクト)の有効フラグを設定します。
    bool UsingReverb( bool flag );

protected:

    //! 音源データをストリームに流し込みます。
    //  この関数は外部から呼び出せません。
    bool Submit();

protected:
    //! 変数
    TsXaudioWaveData*       m_pOriginData;  //  サウンド音源データ
    IXAudio2SourceVoice *   m_pSourceVoice; //  サウンドを再生するストリーム
    
    bool                    m_isLoop;       //  ループフラグ
    float                   m_maxVolume;    //  最大音量
    float                   m_maxPitch;     //  最大ピッチ
};
