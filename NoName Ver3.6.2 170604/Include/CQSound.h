#pragma once

class CQSound
{
public:
	CQSound();
	~CQSound();

	HRESULT Init();
	void Shutdown();
	HRESULT FindChunk(HANDLE hFile, DWORD dwFourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
	HRESULT Play();
	int virtual LoopCount() = 0;

private:
	IXAudio2* pXAudio;
	IXAudio2MasteringVoice* pMasterVoice;
	IXAudio2SourceVoice* pSourceVoice;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;
	BYTE* pDataBuffer;
	TCHAR* strFileName;
};
