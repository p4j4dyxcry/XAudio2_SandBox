#include "TsXAudio2.h"

#include "TsXAudioMaster.h"
bool TsXAudio2Master::s_isInstance = false;

//=========================================================================
//! XAudio Master本体の取得
//=========================================================================
TsXAudio2Master& TsXAudio2Master::GetGlobalSystem()
{
    static TsXAudio2Master xAudioMaster;
    return xAudioMaster;
}

//=========================================================================
//! Constructor
//=========================================================================
TsXAudio2Master::TsXAudio2Master()
    :m_pXAudioInterface(nullptr),
    m_pMasterVoice(nullptr)
{
    if( s_isInstance )
    {
        // todo error log
        return;
    }

    s_isInstance = true;
    Initialize();
}

//=========================================================================
//! Destructor
//=========================================================================
TsXAudio2Master::~TsXAudio2Master()
{
    s_isInstance = false;
    Release();
}

//=========================================================================
//! GetInterface
//=========================================================================
IXAudio2* TsXAudio2Master::GetInterface()const
{
    return m_pXAudioInterface;
}

//=========================================================================
//! GetMasterVoice
//=========================================================================
IXAudio2MasteringVoice* TsXAudio2Master::GetMasteringVoice()const
{
    return m_pMasterVoice;
}

//=========================================================================
//! Initialize
//=========================================================================
bool TsXAudio2Master::Initialize()
{
    // COM の初期化
    if( FAILED( CoInitializeEx( NULL , COINIT_MULTITHREADED ) ) &&
        FAILED( CoInitializeEx( NULL , COINIT_APARTMENTTHREADED ) ) )
    {
        //todo debug output string
        return false;
    }

    // XAudio2 インターフェースの作成
    if( FAILED( XAudio2Create( &m_pXAudioInterface , 0 ) ) )
        return false;

    // マスターボイスの作成
    if( FAILED( m_pXAudioInterface->CreateMasteringVoice( &m_pMasterVoice ) ) )
        return FALSE;


    X3DAudioInitialize( SPEAKER_STEREO,
                        X3DAUDIO_SPEED_OF_SOUND , m_x3DHandle );

    return true;
}

//=========================================================================
//! Release
//=========================================================================
void TsXAudio2Master::Release()
{
    if( m_pMasterVoice )
        m_pMasterVoice->DestroyVoice();
    if( m_pXAudioInterface )
        m_pXAudioInterface->Release();
    CoUninitialize();
}
//=========================================================================
//! 3Dサウンドを計算する
//=========================================================================
TsXAudio2Master::_3DSOUND_PARMETER TsXAudio2Master::Calculate3DSound(
    const X3DAUDIO_LISTENER *pListener ,
    const X3DAUDIO_EMITTER  *pEmitter )
{
    X3DAUDIO_DSP_SETTINGS dsp = { 0 };

    FLOAT32 matrix[12] = { 0 };
    dsp.SrcChannelCount = 1;
    dsp.DstChannelCount = 2;
    dsp.pMatrixCoefficients = matrix;

    X3DAudioCalculate( m_x3DHandle , 
                       pListener , 
                       pEmitter , 
                       X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER , 
                       &dsp );

    _3DSOUND_PARMETER result;
    result.doppler = dsp.DopplerFactor;
    result.dstChannnel = dsp.DstChannelCount;
    result.EmitterToListenerAngle = dsp.EmitterToListenerAngle;
    result.EmitterToListenerDistance = dsp.EmitterToListenerDistance;
    result.EmitterVelocityComponent = dsp.EmitterVelocityComponent;
    result.ListenerVelocityComponent = dsp.ListenerVelocityComponent;
    result.LPFDirectCoefficient = dsp.LPFDirectCoefficient;
    result.LPFReverbCoefficient = dsp.LPFReverbCoefficient;
    memcpy( result.matrix , dsp.pMatrixCoefficients , sizeof( matrix )  );
    result.ReverbLevel = dsp.ReverbLevel;
    result.srcChannel = dsp.SrcChannelCount;

    return result;
}