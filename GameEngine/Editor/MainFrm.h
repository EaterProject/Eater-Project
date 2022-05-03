
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once
class RightView;
class RenderView;
class OptionView;
class AssetView;
class Loading;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter[2];
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	static UINT ThreadFunction(LPVOID _mothod);
	bool m_ThreadStart;
	CWinThread* mThread;
protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar        m_wndToolBar;
	CStatusBar      m_wndStatusBar;

	//분활창
	RightView*		m_RightView;
	RenderView*		m_RenderView;
	OptionView*		m_OptionView;
	AssetView*		m_AssetView;
	
// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};


