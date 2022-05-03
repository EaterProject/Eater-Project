#pragma once
#include "CustomDialog.h"

// AssetsDialog 대화 상자
class RightOption;
class FileOption;
class LoadTerrain;
class LoadNavMesh;
class AssetsDialog : public CustomDialog
{
	DECLARE_DYNAMIC(AssetsDialog)

public:
	AssetsDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~AssetsDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSETS };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl AssetsTree;
	CListCtrl AssetsFile;
	HTREEITEM AssetsTreeTopParent;

	CImageList ImgList_Big;
	CImageList ImgList_Folder;

	CString ClickAssetsPath;
	CString ClickItemName;
	CString DragItemName;

	int DragItemIndex;
	POINT DragItemPosOffset;
	CImageList* DragImg;
public:
	void Initialize(RightOption* mRight);
	int FindChildFile(HTREEITEM hParentItem, CString str);
	void CheckTexture(POINT point);
	RightOption*	mRightOption;
	FileOption*		mFileOption;
	LoadTerrain*	mLoadTerrain;
	LoadNavMesh*	mLoadNavMesh;
	CButton Terrain_Button;
	CButton Navigation_Button;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAssetsClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLvnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLoadBuffer();
	afx_msg void OnLoadNavMesh();
};
