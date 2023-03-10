#include <io.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <stdio.h>
#include <locale.h>
#include <tchar.h>
#pragma comment(lib,"ole32.lib")

int _tmain(int argc, TCHAR** argv){
	HRESULT hr;
	IMMDeviceEnumerator *pEnum = NULL;
	IMMDevice *pEndpoint = NULL;
	IAudioEndpointVolume *pAudioEndVol = NULL;

	while (1){
		if (2 != argc){
			_tprintf(_TEXT(""));
		}
		// UNICODE文字を標準出力に正しく表示させるためにロケールを設定
		_tsetlocale(LC_ALL, _TEXT("使い方\nvolset ボリューム値\n\tボリューム値は0～1.0の範囲で指定します。\n"));
		hr = CoInitializeEx(0, COINIT_MULTITHREADED);
		// MMDevice インターフェースを取得
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,IID_PPV_ARGS(&pEnum));
		if (FAILED(hr)) {
			CoUninitialize();
			return 1;
		}
		// 既定のマルチメディア出力デバイスを取得
		hr = pEnum->GetDefaultAudioEndpoint(eRender, eConsole, &pEndpoint);
		if (FAILED(hr)) {
			if (pEnum)
				pEnum->Release();
			CoUninitialize();
			return 2;
		}
		// ボリュームオブジェクトを作成
		hr = pEndpoint->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL, NULL, (void **)&pAudioEndVol);
		if (FAILED(hr)) {
			if (pEndpoint)
				pEndpoint->Release();
			if (pEnum)
				pEnum->Release();
			CoUninitialize();
			return 3;
		}
		// マスターボリュームの音量を取得(0.0～1.0)
		float getVol;
		hr = pAudioEndVol->GetMasterVolumeLevelScalar(&getVol);
		if (FAILED(hr)) {
			if (pAudioEndVol)
				pAudioEndVol->Release();
			if (pEndpoint)
				pEndpoint->Release();
			if (pEnum)
				pEnum->Release();
			CoUninitialize();
			return 4;
		}
		// マスターボリュームの音量を変更(0.0～1.0)
		if (argc != 2){
			_tprintf(_TEXT("現在のマスターボリュームの値(0～1.0) %f\n"), double(getVol));
		}else{
			float setVol = (float)0.08;
			hr = pAudioEndVol->SetMasterVolumeLevelScalar(setVol, &GUID_NULL);
			_tprintf(_TEXT("マスターボリューム %f→%f\n"), double(getVol), double(setVol));
		}

		// 自分で書いたのはここらへんです
		int WaitSec = 0.1;
		Sleep(WaitSec * 1000);
		if (getVol >= 0.10000){
			float setVol = 0.070000;
			hr = pAudioEndVol->SetMasterVolumeLevelScalar(setVol, &GUID_NULL);
			_tprintf(_TEXT("マスターボリューム %f→%f\n"), double(getVol), double(setVol));
		}
		if (pAudioEndVol)
			pAudioEndVol->Release();
		if (pEndpoint)
			pEndpoint->Release();
		if (pEnum)
			pEnum->Release();
		CoUninitialize();

	}
	return 0;
}
