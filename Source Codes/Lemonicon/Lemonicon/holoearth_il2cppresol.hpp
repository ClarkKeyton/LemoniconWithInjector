#pragma once
#include "IL2CPPResolver/Main.hpp"
#include <iostream>
#include <filesystem>
typedef void* voidptr_t;
using namespace std;
namespace IL2CPP_HoloEarth {
	void* DisableDLLCheck() 
	{
		Unity::CGameObject* game_obj = Unity::GameObject::Find("DllInjectCheck");
		if (game_obj) {
			std::cout << "DLL INJECT CHECK HAS COMPLETED!!!" << std::endl;
		}
		else 
		{
			std::cout << "DLL INJECT CHECK HAS FAILED!!! Error Code: " << GetLastError() << std::endl;
			Sleep(35000);
			exit(412);
		}
		Unity::CComponent* comp = game_obj->GetComponent("DllInjectCheck");
		void* term = comp->CallMethod<void*>("TerminateSpecificDllDetected", false);
		if (!term) {
			cout << "Failed!!! \nError Code: " << GetLastError() << endl;
			Sleep(5000);
			exit(443);
		}
		return term;
	}
	void AttachDLL() 
	{
		IL2CPP::Thread::Attach(IL2CPP::Domain::Get()); //Attach Domain IL2CPP Addresses :D
		void* m_DllAttach = IL2CPP::Thread::Attach(DisableDLLCheck);
		IL2CPP::Thread::Attach(DisableDLLCheck);
		if (m_DllAttach) {
			cout << "DLL Check has been Successfully Disabled" << endl;
			Sleep(4300);
			cout << "Trying Create Thread..." << endl;
			Sleep(210);
			IL2CPP::Thread::Create(m_DllAttach);
		}
		else {
			cout << "Failed to get Attach IL2CPP DLLInjectCheck" << endl;
		}
	}
	void GetFoV()
	{
		Unity::Camera::Initialize();
		Unity::CCamera* camera = Unity::Camera::GetMain();
		float fov = camera->GetFieldOfView();
		std::string name_camera = camera->GetName()->ToString();
		cout << "FoV: " << fov << endl;
		cout << "Name: " << name_camera << endl;
	}
	void SetFovTo100() {
		Unity::Camera::Initialize();
		Unity::CCamera* camera_init = Unity::Camera::GetMain();
		camera_init->SetFieldOfView(100.F);

	}
	void SetFoVTo120() {
		Unity::Camera::Initialize();
		Unity::CCamera* cam = Unity::Camera::GetMain();
		cam->SetFieldOfView(120.F);
	}
	void SetFoVTo180() 
	{
		Unity::Camera::Initialize();
		Unity::CCamera* unity_camera = Unity::Camera::GetMain();
		unity_camera->SetFieldOfView(180.F);
	}
	void RestoreCameraFoV() {
		Unity::Camera::Initialize();
		Unity::CCamera* camera = Unity::Camera::GetMain();
		camera->SetFieldOfView(40.F);
	}
	namespace Il2Cpp {
		void GetLocalPlayer() {
			Unity::CGameObject* game_obj = Unity::GameObject::Find("modelObject");
			if (!game_obj) {
				cout << "Failed to get GameObject!!!" << endl;
				Sleep(5000);
				exit(54);
			}
			else 
			{
				cout << "Successfully!!!" << endl;
				Sleep(15000);
			}
		}
		void InitUnityObj() {
			// Unity APIs
			Unity::Camera::Initialize();
			Unity::Component::Initialize();
			Unity::GameObject::Initialize();
			Unity::LayerMask::Initialize();
			Unity::Object::Initialize();
			Unity::RigidBody::Initialize();
			Unity::Transform::Initialize();

			// Caches
			IL2CPP::SystemTypeCache::Initializer::PreCache();
		}
		void GetGameObject_IL2CPP() 
		{
			/*
				Unity::CComponent* comp = IL2CPP::Helper::GetMonoBehaviour();
				Unity::CGameObject* game = comp->GetGameObject();
				std::string game_str = game->GetName()->ToString();
				return game_str;
			*/
			auto m_pObjects = Unity::Object::FindObjectsOfType<Unity::CComponent>(UNITY_GAMEOBJECT_GETACTIVE);
			for (uintptr_t u = 0U; m_pObjects->m_uMaxLength > u; ++u)
			{
				Unity::CComponent* m_pObject = m_pObjects->m_pValues[u];
				if (!m_pObject) continue; // Just in-case

				// Obtaining object name and then converting it to std::string
				std::string m_sObjectName = m_pObject->GetName()->ToString();
				cout << m_sObjectName << endl; //Enums and Writing All GameObjects :D
			}
				
		}
		bool CheckDLLInject() {
			Unity::CGameObject* game = Unity::GameObject::Find("DllInjectCheck");
			if (game) {
				return true;
			}
			else {
				return false;
			}
		}
		string DumpAllComps() {
			Unity::CComponent* comp = IL2CPP::Helper::GetMonoBehaviour();
			Unity::CGameObject* game = comp->GetGameObject();
			std::string game_str = game->GetName()->ToString();
			return game_str;
		}
		const char* DumpToString() {
			string m = DumpAllComps();
			const char* x = m.c_str();
			return x;
		}
		void DumpCompsToTxt() 
		{
			ofstream stream;
			std::filesystem::path x = std::filesystem::current_path();
			wstring path = x.wstring() + L"\\Dumped.txt";
			const wchar_t* path_current = path.c_str();
			while (true) {
				stream.open(path_current);
				for (uintptr_t x = 0; x > 155; x++)
				{
					stream << DumpToString()[x] << endl;
				}
				Sleep(8000); //Preventing Memory Leaked Errors :D
				stream.close();
			}
		}
		void DumpCompsToConsole() {
			while (true) {
				cout << "[Dumped]: GameObject Name --> " << DumpAllComps() << endl;
				Sleep(2000);
			}
		}
		bool CheckHighLightObject() {
			Unity::CGameObject* HighLight = Unity::GameObject::Find("HighLight");
			if (!HighLight) {
				cout << "Failed to get MainMenu Object!!!" << endl;
				return false;
			}
			else 
			{
				cout << "Successfully Getting Object!!!" << endl;
				return true;
			}
		}
	}
}