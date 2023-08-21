#include <iostream>
#include <ShlObj.h>

#include <Windows.h>
#include <filesystem>
#include <vector>

#include <BlackBone/Process/Process.h>
#include <BlackBone/Patterns/PatternSearch.h>
#include <BlackBone/Process/RPC/RemoteFunction.hpp>
#include <BlackBone/Syscalls/Syscall.h>

using namespace std;
using namespace blackbone;
using std::filesystem::current_path;
namespace fs = std::filesystem;

std::set<std::wstring> nativeMods, modList;

void DllLoadLibrary(Process& proc, const wstring& path) {
	auto mainThread = proc.threads().getMain();
	if (auto pLoadLibrary = MakeRemoteFunction<decltype(&LoadLibraryW)>(proc, L"Kernel32.dll", "LoadLibraryW"); pLoadLibrary && mainThread)
	{
		auto result = pLoadLibrary.Call({ path.c_str() }, mainThread);
		if (*result) 
			cout << "LoadLibrary result: " << *result << "\n";
		else 
		{
			cout << "Failed!!!" << endl;
			Sleep(3400);
			exit(554);
		}
	}
}

void DllManualMap(Process& proc, const wstring& path)
{

	nativeMods.clear();
	modList.clear();

	nativeMods.emplace(L"combase.dll");
	nativeMods.emplace(L"user32.dll");
	modList.emplace(L"windows.storage.dll");
	modList.emplace(L"shell32.dll");
	modList.emplace(L"shlwapi.dll");

	auto callback = [](CallbackType type, void* /*context*/, Process& /*process*/, const ModuleData& modInfo)
	{
		if (type == PreCallback)
		{
			if (nativeMods.count(modInfo.name))
				return LoadData(MT_Native, Ldr_None);
		}
		else
		{
			if (modList.count(modInfo.name))
				return LoadData(MT_Default, Ldr_ModList);
		}

		return LoadData(MT_Default, Ldr_None);
	};

	std::wcout << L"Mapping Lemonicon.dll into Holoearth.exe" << std::endl;
	auto image = proc.mmap().MapImage(path, NoFlags, callback);
	if (!image)
		std::wcout << L"Mapping failed with error 0x" << std::hex << image.status
		<< L". " << Utils::GetErrorDescription(image.status) << std::endl << std::endl;
	else
		std::wcout << L"Mapping successful!\n";
}
int main() 
{
	SetConsoleTitleA("HoloEarthInjector by Quaxdrone");
	const wstring curDir = current_path().wstring();
	const wstring injectPath = curDir + L"\\Lemonicon.dll";
	Process holoearthproc;
	holoearthproc.Attach(L"Holoearth.exe", PROCESS_ALL_ACCESS);
	if (!holoearthproc.pid()) 
	{
		std::cout << "Failed to get PID of This Current Process" << std::endl;
		Sleep(2000);
		exit(443);
	}
	else {
		cout << "Attached!!!" << endl;
		Sleep(4500);
	}
	int n = 2;
	system("cls");
	cout << "Please Choose Method of DLL Injection: \n" << endl;
	cout << "1 - DLL LoadLibrary Method" << endl;
	cout << "2 - ManualMap Method" << endl;
	std::cin >> n;
	switch (n) {
		case 1:
			DllLoadLibrary(holoearthproc, injectPath);
		case 2:
			DllManualMap(holoearthproc, injectPath);
	}
	return 0;
}