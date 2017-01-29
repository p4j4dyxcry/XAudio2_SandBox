#include "TsXAudioWaveData.h"

//===========================================
// コンストラクタ
//===========================================
TsXaudioWaveData::TsXaudioWaveData() 
    : m_handle(nullptr)
{
	ZeroMemory(&m_WAVEFmtEx, sizeof(m_WAVEFmtEx));
    memset( m_filename , 0 , sizeof( m_filename ) );
}

//===========================================
// デストラクタ
//===========================================
TsXaudioWaveData::~TsXaudioWaveData()
{
    if( m_waveData )
        delete m_waveData;

    this->Close();
}

//===========================================
// WAVE ファイルクローズ
//===========================================
bool TsXaudioWaveData::Close()
{
    // ! ハンドルがない（読み込めてない)ときは開放しない
    if( m_handle == nullptr )
        return true;

	if( mmioClose(m_handle, 0) == MMIOERR_CANNOTWRITE)
	{
		return false;
	}
    m_handle = nullptr;
	return true;
}

//===========================================
// WAVE ファイルオープン
//===========================================
bool TsXaudioWaveData::Open( const char * filename )
{
    MMIOINFO mmio_info;
    memset( &mmio_info , 0 , sizeof( MMIOINFO ) );

    if( !( m_handle = mmioOpen( ( char* )filename , &mmio_info , MMIO_READ ) ) )
    {
        return false;
    }
    strcpy_s( m_filename , filename );
    return true;
}

//===========================================
// WAVEFORMATEX にデータを読み込む
//===========================================
bool TsXaudioWaveData::ReadToWaveFmtEx(size_t szFmtChunk)
{
	// mmioRead は成功すると読みこんだサイズを返す
    LONG wReadSize = mmioRead( m_handle , reinterpret_cast<HPSTR>( &m_WAVEFmtEx ) , szFmtChunk );

    if( wReadSize != szFmtChunk )
        return false;

    return true;
}


//===========================================
// WAVEData に WAVE データを読み込む
//===========================================
bool TsXaudioWaveData::ReadToWaveData(size_t szDataSize)
{
    m_waveDataSize = szDataSize;
    m_waveData = new BYTE[m_waveDataSize];

    if( mmioRead( m_handle , reinterpret_cast<HPSTR>( m_waveData ) , m_waveDataSize ) != m_waveDataSize )
	{
		return false;
	}

	return true;
}

//===========================================
// WAVE ファイルをロード
//===========================================
bool TsXaudioWaveData::LoadFromFile( const char* filename )
{
    MMCKINFO			riffChunk;    // RIFF チャンク
    MMCKINFO			fmtChunk;     // FMT チャンク
    MMCKINFO			dataChunk;    // DATA チャンク
   
    // Chunk Header の　チェック
    auto CheckChankHeader = [&]( MMCKINFO* chunk1 , MMCKINFO* chunk2, char c0 , char c1 , char c2 , char c3 ,UINT flag)
    {
        FOURCC* rcc = nullptr;
        if( flag == MMIO_FINDRIFF )
            rcc = &chunk1->fccType;
        if( flag == MMIO_FINDCHUNK )
            rcc = &chunk1->ckid;
        ( *rcc ) = mmioFOURCC( c0,c1,c2,c3 );

        MMRESULT result = mmioDescend( m_handle , chunk1 , chunk2 , flag );
        if( result != MMSYSERR_NOERROR )
        {
            return false;
        }
        return true;
    };

	// ファイルオープン
    if( Open( filename ) == false )
		return false;

	// WAVE ヘッダのチェック
    if( CheckChankHeader( &riffChunk , nullptr , 'W' , 'A' , 'V' , 'E' , MMIO_FINDRIFF ) == false )
		return false;

	// FMT チャンクのチェック
    if( CheckChankHeader( &fmtChunk , &riffChunk , 'f' , 'm' , 't' , ' ' , MMIO_FINDCHUNK ) == false )
		return false;

	// WAVEFORMATEX 構造体にデータを読み込む
	if( ReadToWaveFmtEx(fmtChunk.cksize) == false)
		return false;

	// 読み込み位置を先頭に戻す
	mmioAscend(m_handle, &fmtChunk, SEEK_SET);

    // Data チャンクのチェック
    if( CheckChankHeader( &dataChunk , &riffChunk , 'd' , 'a' , 't' , 'a' , MMIO_FINDCHUNK ) == false )
        return false;

	// WAVE データを読み込む
	if(ReadToWaveData(dataChunk.cksize) == FALSE)
		return false;

    Close();

	return true;

}


//===========================================
// ファイル名を取得
//===========================================
const char * TsXaudioWaveData::GetFileName()const
{
	return m_filename;
}


//===========================================
// WAVE フォーマットを取得
//===========================================
const WAVEFORMATEX& TsXaudioWaveData::GetWaveFmtEx()const
{
	return m_WAVEFmtEx;
}

//===========================================
// WAVE データを取得
//===========================================
const BYTE* TsXaudioWaveData::GetWaveData()const
{
    return m_waveData;
}

//===========================================
// WAVE データのサイズを取得
//===========================================
const size_t TsXaudioWaveData::GetWaveSize()const
{
	return m_waveDataSize;
}
