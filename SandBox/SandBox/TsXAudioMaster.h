#pragma once

#include "TsXAudio2.h"

class TsXAudio2Master
{
public:

    struct _3DSOUND_PARMETER
    {
        int srcChannel;
        int dstChannnel;
        float matrix[12];
        float doppler;
        float EmitterToListenerAngle;
        float EmitterToListenerDistance;
        float EmitterVelocityComponent;
        float ListenerVelocityComponent;
        float ReverbLevel;
        float LPFReverbCoefficient;
        float LPFDirectCoefficient;
    };

    virtual ~TsXAudio2Master();

    IXAudio2* GetInterface()const;
    _3DSOUND_PARMETER Calculate3DSound( const X3DAUDIO_LISTENER *pListener ,
                                        const X3DAUDIO_EMITTER  *pEmitter );
    IXAudio2MasteringVoice* GetMasteringVoice()const;
    static TsXAudio2Master& GetGlobalSystem();
protected:

    TsXAudio2Master();
    TsXAudio2Master( const TsXAudio2Master & ) = delete;
    void operator = ( const TsXAudio2Master &) = delete;


protected:
    bool Initialize();
    void Release();

protected:
    X3DAUDIO_HANDLE         m_x3DHandle;
    IXAudio2 *              m_pXAudioInterface;                //XAudio2インターフェース
    IXAudio2MasteringVoice *m_pMasterVoice;                    //マスタボイス
    static bool s_isInstance;
};