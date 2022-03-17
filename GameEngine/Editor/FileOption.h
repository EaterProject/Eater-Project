#pragma once


// FileOption 대화 상자
#include <string>
#include "EditorData.h"
struct MeshOption;
class EditorManager;
class FileOption : public CDialogEx
{
	DECLARE_DYNAMIC(FileOption)

public:
	FileOption(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~FileOption();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_OPTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	std::string ChoiceMeshName;
	std::string ChoiceMeshPath;
	EditorManager* mEditor;

	void OnBoneWorld(bool On);

	void Initialize(EditorManager* _Editor);
	void SetName(std::string Name,std::string Path);
	void ChangeDataFormat();
	CEdit FileName_Edit;
	CComboBox FileType_Combo;
	MeshOption MeshData;
public:
	CEdit PosX , PosY , PosZ;
	CEdit RotX, RotY, RotZ;
	CEdit SclX , SclY , SclZ;

	CEdit PosX_Bone, PosY_Bone, PosZ_Bone;
	CEdit RotX_Bone, RotY_Bone, RotZ_Bone;
	CEdit SclX_Bone, SclY_Bone, SclZ_Bone;
	
	CButton BoneTransform_Text;
	CButton DinamicButton;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChange_Button();
	afx_msg void OnNoChange_Button();
	afx_msg void OnScaleDown_Button();
	afx_msg void OnCbnSelchangeCombo1();
};
