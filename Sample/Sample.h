#pragma once
#include"Hwindow.h"
#include"HCore.h"
#include"HDevice.h"

//봐야할 순서 : 1. window, std, sample , core, Device
//Sample이랑 묶어서 보기

//=============================================================
// 제일 중요한것 !!!!!!!!
// 이해 먼저 하고 순서 정리하고 이해 됬다면
// 코딩하면서 함수 내부 공부
// 그다음 완전이해 했다면 안보고 혼자 만들어 보고 설명한번 해보기
//=============================================================

//
//디자인 패턴: 어댑터 = 라이브러리에서 내가 필요한 함수가 없을때 상속받아 사용하는 방법
//ex)
// class Xwindow : public Hwindow
// {
//   public:
//     SetWindow(); 만들면 됨
// 메세지 프로시저가 또없다 그러면 위와 똑같이 상속받아서 만들어 사용!!
//

//===============================어댑터 디자인패턴============================
//class XDevice : public HDevice
//{
//public:
//	//1. 디바이스 생성, 스왑체인 생성, 컨텍스트 생성
//	HRESULT CreateDevice(HWND hWnd);
//	//2. 랜더타겟 뷰 생성
//	//3. 뷰포트 세팅
//	//매 프레임 마다 백버퍼 지우기
//	//뿌리기
//	//더블 버퍼링
//	bool Render();
//	//획득한 인터페이스 소멸
//	bool Release();
//};
//============================================================================
// 
// 
//Hwindow   m_window; 상속하기 전에 객체화 해서 한번해보기
// 
// 가상함수 virtual
// 객체화 되면 메모리가 다르다
// 동일성이 있어야 virtual이 사용된다, 동일성이 없으면 다른 메모리!!
class Sample : public HCore
{
public:
	Hwindow m_win;

public:
	virtual bool Init()   override;
	virtual bool Frame()  override;
	virtual bool Render() override;
	virtual bool Release()override;
};

