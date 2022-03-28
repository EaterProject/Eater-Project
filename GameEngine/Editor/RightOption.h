#pragma once
#include <string>
#include <map>



// RightOption 대화 상자
class CTAP_Transform;
class CTAP_Animation;
class CTAP_MeshFilter;
class CTAP_Light;
class CTAP_Particle;

class FileOption;
class Loading;
class EditorManager;
class CamAnimation;
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
	CTAP_Light*		 mLight;		//Light 를 보여줄탭
public:
	//Tag 관련
	CComboBox	Tag_Combo;
	CEdit		AddTag_Edit;
	std::map<int,std::string> TagList;
public:
	CString SaveSceneName;
	bool isDrag = false;
public:
	static GameObject*			ChoiceObject;
	FileOption*			mFileOption;		//자체포맷 변환 창
	EditorManager*		m_EditorManager;	//자체포맷 변환 관리 매니저
	Loading*			mLoading;			//로딩 바
	CamAnimation*		mCam;
public:
	static void MouseDown();
	void Create_Hirearchy_Item(GameObject* Obj, HTREEITEM TOP);
	void Delete_Hirearchy_Item(HTREEITEM TOP);
	void ChickTapDrag(CPoint point);
	void ChickHirearchyDarg(CPoint point);
	GameObject* FindGameObjectParent(HTREEITEM mItem);
	CEdit FilePathEdit;

	static RightOption* GetThis();
	static RightOption* thisPointer;
public:
	
	//버튼들
	afx_msg void OnChoice_Hirearchy_Item(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelteObject_Button();
	afx_msg void OnDeleteFile_Button();
	afx_msg void OnChange_DataFormat();
	afx_msg void OnClickTap(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnOpenOption();
	afx_msg void OnOpenCamAnimation();
	afx_msg void OnAddTag_Button();
	afx_msg void OnChoiceTag();
	afx_msg void OnDeleteTagButton();
};
