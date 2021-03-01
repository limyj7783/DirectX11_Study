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
char * strFileName = "Resources\\정은지-하늘바라기.wav3";*/
//#else
//TCHAR * strFileName = TEXT("Resources\\정은지-하늘바라기.wav");
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
	strFileName = TEXT("Resources\\Sound\\BGM\\정은지-하늘바라기.wav");
	
	//pXAudio 객체 생성하며, IXAudio2의 인터페이스를 리턴받음
	hr = XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
		return hr;

	//pMasterVoice를 생성하며, IXAudio2MasteringVoice클래스는 오디오 장치를 캡슐화.
	//pMasterVoice는 오디오 그래프를 통과하는 모든 오디오의 최종 목적지 -> 모든 사운드가 pMasterVoice를 통과하여 재생된다는 뜻인듯
	hr = pXAudio->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr))
		return hr;
	
	
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

	//RIFF 청크를 오디오 파일에서 찾고, 파일 타입 확인

	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD dwFileType;
	ReadChunkData(hFile, &dwFileType, sizeof(DWORD), dwChunkPosition);
	if (dwFileType != fourccWAVE)
		return S_FALSE;

	//fmt 청크를 찾은 내용을 WAVEFORMATEXTENSIBLE wfx구조체에 복사
	WAVEFORMATEXTENSIBLE wfx;
	FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

	//data 청크를 찾고, 버퍼에 저장
	FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
	pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//XAUDIO2_BUFFER buffer 채우기
	//buffer의 내용을 참고해서 오디오 파일을 재생
	buffer.AudioBytes = dwChunkSize;  
	buffer.pAudioData = pDataBuffer;  
	buffer.Flags = XAUDIO2_END_OF_STREAM; 
	buffer.LoopCount = LoopCount();

	//IXAudio2SourceVoice pSourceVoice의 형식을 wfx구조체에 설정된 값으로 지정
	//pSourceVoice를 사용하여 오디오 파일을 재생
	hr = pXAudio->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx);
	if (FAILED(hr))
		return hr;

	//pSourceVoice의 재생 내용을 buffer값을 사용하여 재생 준비
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

//오디오 파일을 파싱하기 위해 준비하는 함수
//RIFF 형태의 오디오 파일을 파싱준비하며 .wav파일이 RIFF형태에 해당
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

//청크데이터를 읽는 함수
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

	//모든 xaudio2 음색은 마스터링 음색을 통과하여 출력
	//Start()함수로 오디오 재생
	hr = pSourceVoice->Start(0);
	if (FAILED(hr))
		return hr;

	return hr;
}
