#pragma once

#include "TsXAudio2.h"

class TsXaudioWaveData
{

public:
    TsXaudioWaveData();
    ~TsXaudioWaveData();

    // *.wavの読み込み
    bool LoadFromFile( const char* FileName );

    // WAVE フォーマットを取得
    const WAVEFORMATEX&     GetWaveFmtEx()const;

    // ファイル名の取得
    const char*             GetFileName()const;

    // Wave データを取得
    const BYTE*             GetWaveData()const;

    // Wave データのサイズを取得
    const size_t            GetWaveSize()const;

private:
    HMMIO               m_handle;           // MMIO ハンドル

    WAVEFORMATEX		m_WAVEFmtEx;        // WAVE フォーマット構造体	
    BYTE*	            m_waveData;         // Sound Data	
    size_t              m_waveDataSize;
    char                m_filename[512];

private:
    bool Open(const char* filename);		// WAVE ファイルオープン
    bool Close();                           // WAVE ファイルクローズ

    bool ReadToWaveFmtEx( size_t szFmtChunk );
    bool ReadToWaveData(size_t szDataSize);
};