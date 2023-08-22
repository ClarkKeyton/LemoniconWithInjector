// dllmain.cpp : Defines the entry point for the DLL application.
#include <iostream>
#include <fstream>
#include "includes.h"
#include <MinHook/MinHook.h>
#include <wininet.h>
#include <thread>
#include <Discord-RPC/discord_register.h>
#include <Discord-RPC/discord_rpc.h>
#include "minhook_holoEARTH.hpp"
#include "imgui/imgui_internal.h"
#include "IL2CPPResolver/Main.hpp"
#include "holoearth_il2cppresol.hpp"
#include "imgui/imgui_freetype.h"
#pragma warning (disable : 6387) //Disabling This Warning 
using namespace std;
using namespace IL2CPP_HoloEarth;
#pragma comment (lib, "Wininet")
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HMODULE Module_Holoearth;
FILE* fp;
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
bool panickey = true;
static bool initRPC = true;
bool find_gameobj = true;
bool checkinternetconnection = InternetCheckConnectionA("https://google.com", FLAG_ICC_FORCE_CONNECTION, 0);
bool get_highlight = false;
constexpr auto LOG_FILENAME = "HOLOEARTH_LOG.log";
std::vector<Unity::CComponent*> DumpComponents() {
	std::vector<Unity::CComponent*> compenents_to_return;

	auto list = Unity::Object::FindObjectsOfType<Unity::CComponent>(UNITY_COMPONENT_CLASS);
	for (int i = 0; i < list->m_uMaxLength + 1; i++)
	{
		if (!list->operator[](i))
			continue;

		compenents_to_return.push_back(list->operator[](i));
	}
	return compenents_to_return;
}

std::vector<std::string> DumpComponentsString() {
	std::vector<std::string> compenents_to_return;
	cout << "\nDumping components...\n" << endl;

	std::vector<Unity::CComponent*> components = DumpComponents();
	for (Unity::CComponent* component : components)
	{
		if (!component)
			continue;

		Unity::CGameObject* object = component->GetMemberValue<Unity::CGameObject*>("gameObject");
		std::string name = object->GetName()->ToString();
		compenents_to_return.push_back(name);
	}

	return compenents_to_return;
}
void Dump() 
{
	std::vector<string> xnv = DumpComponentsString();
	for (string m : xnv) {
		cout << "\n[DumpedComponents]: Name -->" << m << endl;
	}
}
std::vector<Unity::CGameObject*> DumpGameObjects() {
	std::vector<Unity::CGameObject*> gameobjects_to_return;

	auto list = Unity::Object::FindObjectsOfType<Unity::CGameObject>(UNITY_GAMEOBJECT_CLASS);
	
	for (int i = 0; i < list->m_uMaxLength + 1; i++)
	{
		if (!list->operator[](i))
			continue;

		gameobjects_to_return.push_back(list->operator[](i));
	}
	return gameobjects_to_return; 
}
std::vector<string> TransformGameObjects() {
	std::vector<std::string> gameobject_to_return;
	cout << "\nDumping GameObjects...\n" << endl;

	std::vector<Unity::CComponent*> components = DumpComponents();
	for (Unity::CComponent* component : components)
	{
		if (!component)
			continue;

		Unity::CGameObject* object = component->GetMemberValue<Unity::CGameObject*>("gameObject");
		std::string name = object->GetName()->ToString();
		gameobject_to_return.push_back(name);
	}

	return gameobject_to_return;
}
void DumpObject() {
	std::vector<string> zzz = TransformGameObjects();
	for (string mxz_fcv : zzz) {
		cout << "\n[DumpedGameObjects]: Name --> " << mxz_fcv << endl;
	}
}
void GetPosition() {
	Unity::CGameObject* object = Unity::GameObject::Find("GlobalMenuModeFSM(Clone)"); //Collider it's Tetrahedron Player xD
	if (object) {
		Unity::CComponent* component = object->GetComponent("Mercury.Player.Remote::TetrahedronRemotePlayer"); //It's Component for Player :D
		if (!component) {
			cout << "Component Not Founded" << endl;
		}
		else {
			cout << "Component was founded Successfully" << endl;
		}
		Unity::Vector4 vector = component->GetMemberValue<Unity::Vector4>("targetPosition");
		cout << vector.x << endl;
		cout << vector.y << endl;
		cout << vector.z << endl;
	}
	else {
		cout << "Failed to get GameObject CursorMode" << endl;
	}
}
void InitCache() {
	IL2CPP::SystemTypeCache::Initializer::PreCache();
}
Unity::il2cppClass* GetClassCoverHololobby()
{
	Unity::il2cppClass* x = nullptr;
	std::vector<Unity::il2cppClass*> xsd;
	IL2CPP::Class::FetchClasses(&xsd, "cover-hololobby", nullptr); //nullptr it's finding all of Namespaces and All Values :D
	for (Unity::il2cppClass* class_class : xsd) {
		Unity::il2cppType* type_x = IL2CPP::Class::GetType(class_class);
		Unity::il2cppClass* xxx = IL2CPP::Class::Find(class_class->m_pName = "DllInjectCheck"); //Check if DLL Inject is Founded :D
		x = xxx;
		if (!xxx) {
			cout << "DllInjectCheck.cs is not founded Inside IL2CPP GameAssembly.dll" << endl;
			Sleep(5000);
			exit(32212);
		}
	}
	return x; //Returning Current Name Cover Hololobby :D
}
void CallDisableDLL() 
{
	Unity::CGameObject* obj = Unity::GameObject::Find("[DOTween]");
	Unity::CComponent* component = obj->GetComponent("cover-hololobby::DllInjectCheck");
	component->CallMethod<void*>("TerminateSpecificDllDetected", false);
}
void CreateConsole()
{
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA("Lemonicon by Ariflax");
}
void ReadToFileConsole(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	FILE* f;
	fopen_s(&f, LOG_FILENAME, "a");
	if (f)
	{
		vfprintf(f, fmt, args);
		fclose(f);
	}
}
DWORD WINAPI CheckInet(const HMODULE hMod) 
{
	CreateConsole();
	if (!checkinternetconnection) 
	{
		printf("Internet Connection has been Failed!!!\n");
		return FALSE;
	}
	printf("Internet Connection has been Successfully Initializated!!!\n");
	return TRUE;
}

void IL2CPPInit() {
	if (IL2CPP::Initialize(true)) {
		cout << "IL2CPP IS INITIALIZATED SUCCESSFULLED" << endl;
	}
	else {
		cout << "failed to get il2cpp address" << endl;
		Sleep(4000);
		exit(445);
	}
}
void InitDiscord() 
{
	static int64_t TimeNow = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	DiscordEventHandlers hand = DiscordEventHandlers();
	memset(&hand, 0, sizeof(hand));
	Discord_Initialize("1111709186850570240", &hand, 1, "0");
	if (initRPC) 
	{
		DiscordRichPresence rich = DiscordRichPresence();
		memset(&rich, 0, sizeof(rich));
		rich.state = "Playing in HoloEarth with Cheat";
		rich.details = "Playing in HoloEarth with Cheat";
		rich.startTimestamp = TimeNow;
		rich.endTimestamp = NULL; //NULL it's UNLIMITED END TIMESTAMP!!!!.
		rich.largeImageKey = "holoearth_bigpictures";
		rich.largeImageText = "Playing in HoloEarth";
		rich.smallImageText = "Playing in HoloEarth";

		Discord_UpdatePresence(&rich);
		cout << "Discord Is Initializated" << endl;
	}
	else 
	{
		cout << "FAILED TO INIT DISCORD!!!" << endl;
		Sleep(3800);
		exit(3310);
	}
}
void InitImGui()
{
	ImGui::CreateContext();
	ImFontConfig config_p{};
	config_p.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Oblique | ImGuiFreeTypeBuilderFlags_Bold;
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 23.F, &config_p);
	io.WantCaptureMouse = true;
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImColor(155, 0, 0, 255);
	style.Colors[ImGuiCol_TitleBg] = ImColor(55, 43, 155, 255);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(55, 55, 55, 255);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(43, 122, 85, 255);
	style.Colors[ImGuiCol_Button] = ImColor(78, 78, 78, 255);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(55, 0, 0, 255);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(255, 33, 50, 255);
	style.Colors[ImGuiCol_CheckMark] = ImColor(55, 45, 88, 255);
	style.Colors[ImGuiCol_FrameBg] = ImColor(22, 22, 22, 255);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(22, 100, 27, 255);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(45, 45, 45, 255);
	style.Colors[ImGuiCol_Header] = ImColor(0, 0, 66, 255);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(66, 66, 0, 255);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(55, 55, 144, 255);
	style.Colors[ImGuiCol_Tab] = ImColor(55, 55, 55, 255);
	style.Colors[ImGuiCol_TabActive] = ImColor(75, 75, 75, 255);
	style.Colors[ImGuiCol_TabHovered] = ImColor(44, 44, 44, 255);
	style.Colors[ImGuiCol_Text] = ImColor(166, 166, 75, 255);
	style.Colors[ImGuiCol_TextDisabled] = ImColor(55, 55, 15, 255);
	style.Colors[ImGuiCol_MenuBarBg] = ImColor(75, 75, 75, 150);
	style.Colors[ImGuiCol_PopupBg] = ImColor(0, 115, 0, 255);
	style.Alpha = 1.0F;
	style.FrameBorderSize = 0.4F;
	style.WindowTitleAlign = ImVec2(0.0F, 0.8F);
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}
void Render()
{
	bool suc = false;
	bool x = false;
	bool z_d = false;
	bool init_il2cpp = false;
	bool fov_func_1 = false;
	bool fov_func_2 = false;
	bool fov_func_3 = false;
	bool fov_func_4 = false;
	bool find_gameobject = false;
	bool xbv = false;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
	ImGui::Begin("Lemonicon By Nijo Fukushi", 0, flags);
	ImGui::SetWindowSize(ImVec2(550.F, 550.F));
	ImGui::BulletText("It's my First Cheat For Game Holoearth");
	if (ImGui::Checkbox("Shutdown Public DLC", &suc)) 
	{
		kiero::shutdown();
		pDevice->Release();
		pContext->Release();
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
		fclose(stdout);
		Discord_ClearPresence();
		Discord_Shutdown();
		FreeConsole();
	}
	if (ImGui::Button("Shutdown Discord RPC", ImVec2(235, 155))) 
	{
		Discord_ClearPresence();
		Discord_Shutdown();
		cout << "Discord Is Shutdowning Successfully" << endl;
	}
	if (ImGui::Checkbox("Get Position Value", &xbv)) {
		GetPosition(); //Getting Position in Float :D
	}
	if (ImGui::BeginMenu("Lobby Menu")) 
	{
		ImGui::TextColored(ImColor(40, 205, 155, 255), "It's Lobby Menu, when you connected to a Lobby Server");
		if (ImGui::Button("Set Invisible Name", ImVec2(175, 155))) 
		{
			MinHook_InitNameInvisibleRemotePlayer();
		}
		if (ImGui::Checkbox("Disable DLL Check", &suc)) 
		{
			IL2CPP::Callback::OnUpdate::Add(IL2CPP_HoloEarth::DisableDLLCheck);
		}
		if (ImGui::Checkbox("Get FoV", &x)) 
		{
			while (true) {
				IL2CPP::Callback::OnLateUpdate::Add(IL2CPP_HoloEarth::GetFoV);
				Sleep(3000);
				break;
			}
		}
		if (ImGui::Checkbox("Get HighLight Object", &get_highlight)) {
			IL2CPP::Callback::OnLateUpdate::Add(reinterpret_cast<void*>(IL2CPP_HoloEarth::Il2Cpp::CheckHighLightObject));
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("IL2CPP Callback Menu")) 
	{
		if (ImGui::Checkbox("Disable IL2CPP Callback", &z_d)) 
		{
			IL2CPP::Callback::Uninitialize();
		}
		if (ImGui::Checkbox("Enable IL2CPP Callback", &init_il2cpp)) {
			IL2CPP::Callback::Initialize();
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("FoV Menu")) 
	{
		if (ImGui::Checkbox("Set FoV To 100", &fov_func_1)) {
			IL2CPP::Callback::OnLateUpdate::Add(IL2CPP_HoloEarth::SetFovTo100);
		}
		if (ImGui::Checkbox("Set FoV To 120", &fov_func_2)) {
			IL2CPP::Callback::OnLateUpdate::Add(IL2CPP_HoloEarth::SetFoVTo120);
		}
		if (ImGui::Checkbox("Set FoV To 180", &fov_func_3)) {
			IL2CPP::Callback::OnLateUpdate::Add(IL2CPP_HoloEarth::SetFoVTo180);
		}
		if (ImGui::Checkbox("Restore FoV", &fov_func_4)) {
			IL2CPP::Callback::OnLateUpdate::Add(IL2CPP_HoloEarth::RestoreCameraFoV);
		}
	}
	ImGui::End();

	ImGui::Render();

}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView); //C6387 is False Positive Warning D3D11 Device
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (!panickey) 
	{
		kiero::shutdown();
		pDevice->Release();
		pContext->Release();
		fclose(stdout);
		FreeConsole();
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
		oPresent(pSwapChain, SyncInterval, Flags);
		return 0;
	}
	if (!find_gameobj) 
	{
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(IL2CPP_HoloEarth::Il2Cpp::DumpCompsToConsole), 0, 0, 0);
	}
	return oPresent(pSwapChain, SyncInterval, Flags);
}
void MinHookInit() 
{
	MH_Initialize();
	cout << "MinHook is Initializated Successfully" << endl;
}
DWORD WINAPI MainThr(HMODULE hMod) 
{
	DWORD pid;
	HWND wind = FindWindow(0, "Holoearth");
	if (wind == INVALID_HANDLE_VALUE)
	{
		AllocConsole();
		FILE* file_cons;
		freopen_s(&file_cons, "CONOUT$", "w", stdout);
		SetConsoleTitleA("Lemonicon by ClarkKeyton(Nijo Fukushi)");
		cout << "FAILED TO GET WINDOW OF HOLOEARTH!!!" << endl;
		Sleep(500);
		FreeLibraryAndExitThread(hMod, 334);
	}
	pid = GetCurrentProcessId();
	CreateConsole();
	IL2CPPInit();
	MinHookInit();
	MinHook_InitCheckCheatTools();
	MinHook_MagicTerminateSpecificDLLDetected();
	InitDiscord();
	IL2CPP::Callback::Initialize();
	IL2CPP_HoloEarth::Il2Cpp::InitUnityObj();
	//IL2CPP_HoloEarth::Il2Cpp::GetGameObject_IL2CPP();
	CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(IL2CPP_HoloEarth::Il2Cpp::DumpCompsToConsole), 0, 0, 0);
	CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(IL2CPP_HoloEarth::Il2Cpp::DumpCompsToTxt), 0, 0, 0);
	//CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(IL2CPP_HoloEarth::Il2Cpp::GetGameObject_IL2CPP), 0, 0, 0);
	//CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Dump), 0, 0, 0);
	CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(DumpObject), 0, 0, 0);
	cout << "PID: " << pid << endl;
	cout << "PID in HEX: 0x" << std::hex << pid << endl;
	bool init_hook = false;
	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
	{
		kiero::bind(8, (void**)&oPresent, hkPresent);
		init_hook = true;
	}
	while (panickey) 
	{
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			panickey = false;
		}
		else if (GetAsyncKeyState(VK_F9) & 1) 
		{
			find_gameobj = false;
		}
	}
	return TRUE;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CheckInet, 0, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThr, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

