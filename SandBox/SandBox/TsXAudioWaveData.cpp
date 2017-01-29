#include "TsXAudioWaveData.h"

//===========================================
// �R���X�g���N�^
//===========================================
TsXaudioWaveData::TsXaudioWaveData() 
    : m_handle(nullptr)
{
	ZeroMemory(&m_WAVEFmtEx, sizeof(m_WAVEFmtEx));
    memset( m_filename , 0 , sizeof( m_filename ) );
}

//===========================================
// �f�X�g���N�^
//===========================================
TsXaudioWaveData::~TsXaudioWaveData()
{
    if( m_waveData )
        delete m_waveData;

    this->Close();
}

//===========================================
// WAVE �t�@�C���N���[�Y
//===========================================
bool TsXaudioWaveData::Close()
{
    // ! �n���h�����Ȃ��i�ǂݍ��߂ĂȂ�)�Ƃ��͊J�����Ȃ�
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
// WAVE �t�@�C���I�[�v��
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
// WAVEFORMATEX �Ƀf�[�^��ǂݍ���
//===========================================
bool TsXaudioWaveData::ReadToWaveFmtEx(size_t szFmtChunk)
{
	// mmioRead �͐�������Ɠǂ݂��񂾃T�C�Y��Ԃ�
    LONG wReadSize = mmioRead( m_handle , reinterpret_cast<HPSTR>( &m_WAVEFmtEx ) , szFmtChunk );

    if( wReadSize != szFmtChunk )
        return false;

    return true;
}


//===========================================
// WAVEData �� WAVE �f�[�^��ǂݍ���
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
// WAVE �t�@�C�������[�h
//===========================================
bool TsXaudioWaveData::LoadFromFile( const char* filename )
{
    MMCKINFO			riffChunk;    // RIFF �`�����N
    MMCKINFO			fmtChunk;     // FMT �`�����N
    MMCKINFO			dataChunk;    // DATA �`�����N
   
    // Chunk Header �́@�`�F�b�N
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

	// �t�@�C���I�[�v��
    if( Open( filename ) == false )
		return false;

	// WAVE �w�b�_�̃`�F�b�N
    if( CheckChankHeader( &riffChunk , nullptr , 'W' , 'A' , 'V' , 'E' , MMIO_FINDRIFF ) == false )
		return false;

	// FMT �`�����N�̃`�F�b�N
    if( CheckChankHeader( &fmtChunk , &riffChunk , 'f' , 'm' , 't' , ' ' , MMIO_FINDCHUNK ) == false )
		return false;

	// WAVEFORMATEX �\���̂Ƀf�[�^��ǂݍ���
	if( ReadToWaveFmtEx(fmtChunk.cksize) == false)
		return false;

	// �ǂݍ��݈ʒu��擪�ɖ߂�
	mmioAscend(m_handle, &fmtChunk, SEEK_SET);

    // Data �`�����N�̃`�F�b�N
    if( CheckChankHeader( &dataChunk , &riffChunk , 'd' , 'a' , 't' , 'a' , MMIO_FINDCHUNK ) == false )
        return false;

	// WAVE �f�[�^��ǂݍ���
	if(ReadToWaveData(dataChunk.cksize) == FALSE)
		return false;

    Close();

	return true;

}


//===========================================
// �t�@�C�������擾
//===========================================
const char * TsXaudioWaveData::GetFileName()const
{
	return m_filename;
}


//===========================================
// WAVE �t�H�[�}�b�g���擾
//===========================================
const WAVEFORMATEX& TsXaudioWaveData::GetWaveFmtEx()const
{
	return m_WAVEFmtEx;
}

//===========================================
// WAVE �f�[�^���擾
//===========================================
const BYTE* TsXaudioWaveData::GetWaveData()const
{
    return m_waveData;
}

//===========================================
// WAVE �f�[�^�̃T�C�Y���擾
//===========================================
const size_t TsXaudioWaveData::GetWaveSize()const
{
	return m_waveDataSize;
}
