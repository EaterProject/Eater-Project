#pragma once


// RightOption 대화 상자
class CTAP_Transform;
class CTAP_Animation;
class CTAP_MeshFilter;
class CTAP_Light;
class CTAP_Particle;

class FileOption;
class SceneSaveDialog;
class Loading;
class EditorManager;
#include <string>
class RightOption : public CDialogEx
{
	DECLARE_DYNAMIC(RightOption)

public:
	RightOption(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~RightOption();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RIGHT_OPTION };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//에셋창 변수들
	CTreeCtrl AssetsTree;				//에셋 폴더를 보여주는 트리
	CListCtrl AssetsFileList;			//에셋 파일을 보여주는 리스트
	CImageList ImgList_Folder;			//이미지 리스트
	CImageList ImgList_Big;				//이미지 리스트
	HTREEITEM AssetsTreeTopParent;		//에셋트리에 최상위 아이템
	std::string AssetsChoiceItemName;	//클릭한 아이템에 이름
	POINT DragItemPosOffset;			//드레그 중인 아이템의 오프셋
	CString DragItemName;				//드레그 중인 아이템의 이름
	CString ClickItemName;				//클릭한 아이템 이름
	CString ClickAssetsPath;
	CImageList* DragImg;				//드레그 중일때 보여줄 이미지 리스트
	int	LoadMaxCount;
	int DragItemIndex;
public:
	//하이어라키 창변수들
	CTreeCtrl HirearchyTree;
	CEdit HirearchyEdit;
	std::string ChoiceHirearchyName;
public:
	//컨퍼넌트 텝 변수들
	CTabCtrl Component_TapList;
	CTAP_Transform*  mTransform;	//Transform를 보여줄 탭
	CTAP_Animation*	 mAnimation;	//Animation를 보여줄 탭
	CTAP_MeshFilter* mMeshFilter;	//MeshFiltet를 보여줄 탭
	CTAP_Particle*	 mPrticle;		//MeshFiltet를 보여줄 탭
public:
	CString SaveSceneName;
public:
	FileOption*		mFileOption;		//자체포맷 변환 창
	SceneSaveDialog* mScene;			//씬 저장 창
	EditorManager*	m_EditorManager;	//자체포맷 변환 관리 매니저
	Loading*		mLoading;			//로딩 바
public:
	void Create_Hirearchy_Item(GameObject* Obj, HTREEITEM TOP);
	void Delete_Hirearchy_Item(HTREEITEM TOP);
	int FindChildFile(HTREEITEM hParentItem, CString str);
	void ChickTapDrag(CPoint point);
	void ChickHirearchyDarg(CPoint point);
	GameObject* FindGameObjectParent(HTREEITEM mItem);
	void AssetsInitialize();
public:
	afx_msg void OnAssetsTreeClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnChoice_Hirearchy_Item(NMHDR* pNMHDR, LRESULT* pResult);
	
	//버튼들
	afx_msg void OnDelteObject_Button();
	afx_msg void OnOpenAssetsFolder();
	afx_msg void OnDeleteFile_Button();
	afx_msg void OnChange_DataFormat();
	afx_msg void OnOpenExeFile_Button();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickTap(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSaveScene();
	afx_msg void OnCreateParticle();
	afx_msg void OnCreateTerrain();
	afx_msg LRESULT OnUserFun(WPARAM wParam, LPARAM lparam);
};
