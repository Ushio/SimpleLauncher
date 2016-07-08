#include "ofApp.h"

#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>

inline std::string slash_to_backslash(std::string text) {
	for (int i = 0; i < text.size(); ++i) {
		if (text[i] == '/') {
			text[i] = '\\';
		}
	}
	return text;
}

inline void killProcessByName(const char *filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{
		// printf("%s\n", pEntry.szExeFile);
		if (strcmp(pEntry.szExeFile, filename) == 0)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

void ShutdownWindows(BOOL bReboot, BOOL bForce)
{
	DWORD ret;
	HANDLE hProcess;
	HANDLE hToken;
	LUID   Luid;
	TOKEN_PRIVILEGES tokenNew, tokenPre;
	UINT nFlag;

	hProcess = GetCurrentProcess();

	OpenProcessToken(hProcess,
		TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Luid);

	tokenNew.PrivilegeCount = 1;
	tokenNew.Privileges[0].Luid = Luid;
	tokenNew.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tokenNew,
		sizeof(tokenPre), &tokenPre, &ret);

	if (bReboot) {
		nFlag = EWX_REBOOT; // 再起動
	}
	else {
		nFlag = EWX_POWEROFF; // 電源ＯＦＦ
	}

	if (bForce) {
		nFlag |= EWX_FORCE; // プロセス強制終了
	}

	ExitWindowsEx(nFlag, 0); // シャットダウン処理

	return;
}

//--------------------------------------------------------------
void ofApp::setup(){
	// FreeConsole();
	_osc.setup(9500);
}

//--------------------------------------------------------------
void ofApp::update(){
	while (_osc.hasWaitingMessages()) {
		ofxOscMessage m;
		if (_osc.getNextMessage(m)) {
			auto address = m.getAddress();
			if (address == "/launch") {
				if (m.getNumArgs() == 1 && m.getArgType(0) == OFXOSC_TYPE_STRING) {
					auto arg = m.getArgAsString(0);
					auto app_path = slash_to_backslash(arg);
					std::cout << app_path << std::endl;
					int a = 0;
					ShellExecuteA(NULL, "open", app_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
				}
			}
			if (address == "/kill") {
				if (m.getNumArgs() == 1 && m.getArgType(0) == OFXOSC_TYPE_STRING) {
					auto app = m.getArgAsString(0);
					std::cout << app << std::endl;
					killProcessByName(app.c_str());
				}
			}
			if (address == "/reboot") {
				ShutdownWindows(true, true);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
