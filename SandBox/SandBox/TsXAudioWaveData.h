#pragma once

#include "TsXAudio2.h"

class TsXaudioWaveData
{

public:
    TsXaudioWaveData();
    ~TsXaudioWaveData();

    // *.wav�̓ǂݍ���
    bool LoadFromFile( const char* FileName );

    // WAVE �t�H�[�}�b�g���擾
    const WAVEFORMATEX&     GetWaveFmtEx()const;

    // �t�@�C�����̎擾
    const char*             GetFileName()const;

    // Wave �f�[�^���擾
    const BYTE*             GetWaveData()const;

    // Wave �f�[�^�̃T�C�Y���擾
    const size_t            GetWaveSize()const;

private:
    HMMIO               m_handle;           // MMIO �n���h��

    WAVEFORMATEX		m_WAVEFmtEx;        // WAVE �t�H�[�}�b�g�\����	
    BYTE*	            m_waveData;         // Sound Data	
    size_t              m_waveDataSize;
    char                m_filename[512];

private:
    bool Open(const char* filename);		// WAVE �t�@�C���I�[�v��
    bool Close();                           // WAVE �t�@�C���N���[�Y

    bool ReadToWaveFmtEx( size_t szFmtChunk );
    bool ReadToWaveData(size_t szDataSize);
};