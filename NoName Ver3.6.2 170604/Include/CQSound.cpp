#include "../pch.h"
#include "CQSound.h"

/*
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif
*/
#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

/*
#ifdef _XBOX
char * strFileName = "Resources\\������-�ϴùٶ��.wav3";*/
//#else
//TCHAR * strFileName = TEXT("Resources\\������-�ϴùٶ��.wav");
//#endif

CQSound::CQSound() {
	pXAudio = nullptr;
	pMasterVoice = nullptr;
	pSourceVoice = nullptr;
	wfx = { 0 };
	buffer = { 0 };
}


CQSound::~CQSound() {

}

HRESULT CQSound::Init() {
	HRESULT hr;
	strFileName = TEXT("Resources\\Sound\\BGM\\������-�ϴùٶ��.wav");
	
	//pXAudio ��ü �����ϸ�, IXAudio2�� �������̽��� ���Ϲ���
	hr = XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
		return hr;

	//pMasterVoice�� �����ϸ�, IXAudio2MasteringVoiceŬ������ ����� ��ġ�� ĸ��ȭ.
	//pMasterVoice�� ����� �׷����� ����ϴ� ��� ������� ���� ������ -> ��� ���尡 pMasterVoice�� ����Ͽ� ����ȴٴ� ���ε�
	hr = pXAudio->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr))
		return hr;
	
	
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	//RIFF ûũ�� ����� ���Ͽ��� ã��, ���� Ÿ�� Ȯ��

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD dwFileType;
	ReadChunkData(hFile, &dwFileType, sizeof(DWORD), dwChunkPosition);
	if (dwFileType != fourccWAVE)
		return S_FALSE;

	//fmt ûũ�� ã�� ������ WAVEFORMATEXTENSIBLE wfx����ü�� ����
	WAVEFORMATEXTENSIBLE wfx;
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//data ûũ�� ã��, ���ۿ� ����
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//XAUDIO2_BUFFER buffer ä���
	//buffer�� ������ �����ؼ� ����� ������ ���
	buffer.AudioBytes = dwChunkSize;  
	buffer.pAudioData = pDataBuffer;  
	buffer.Flags = XAUDIO2_END_OF_STREAM; 
	buffer.LoopCount = LoopCount();

	//IXAudio2SourceVoice pSourceVoice�� ������ wfx����ü�� ������ ������ ����
	//pSourceVoice�� ����Ͽ� ����� ������ ���
	hr = pXAudio->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);
	if (FAILED(hr))
		return hr;

	//pSourceVoice�� ��� ������ buffer���� ����Ͽ� ��� �غ�
	hr = pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr))
		return hr;

	return true;
}

void CQSound::Shutdown() {
	ReleaseCOM(pXAudio);
	delete pDataBuffer;
	pDataBuffer = nullptr;
	delete strFileName;
	strFileName = nullptr;
	delete pMasterVoice;
	pMasterVoice = nullptr;
	delete pSourceVoice;
	pSourceVoice = nullptr;
}

//����� ������ �Ľ��ϱ� ���� �غ��ϴ� �Լ�
//RIFF ������ ����� ������ �Ľ��غ��ϸ� .wav������ RIFF���¿� �ش�
HRESULT CQSound::FindChunk(HANDLE hFile, DWORD dwFourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition) {
	HRESULT hr = S_OK;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == dwFourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (dwBytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

//ûũ�����͸� �д� �Լ�
HRESULT CQSound::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset) {
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());
	DWORD dwRead;
	if (0 == ReadFile(hFile, buffer, bufferSize, &dwRead, NULL))
		hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}

HRESULT CQSound::Play() {
	HRESULT hr=S_OK;

	//��� xaudio2 ������ �����͸� ������ ����Ͽ� ���
	//Start()�Լ��� ����� ���
	hr = pSourceVoice->Start(0);
	if (FAILED(hr))
		return hr;

	return hr;
}
