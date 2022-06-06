#pragma once
// CustomDialog 대화 상자
/// EATER Editor에서 기본으로 상속 받는 Custom다이얼로그 클래스


#define FBX		 0
#define PNG		 1
#define EATER	 2
#define DDS		 3
#define SCENE	 4
#define PREFAB	 5
#define EMAT	 6
#define EMESH	 7
#define HDR	     8
#define PARTICLE 9


class CustomDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CustomDialog)

public:
	CustomDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CustomDialog::CustomDialog(UINT nIDTemplate, CWnd* pParent);
	virtual ~CustomDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
protected:
	///변환 함수들
	CString ChangeToCString(float Data);		//float		-> Cstring
	CString ChangeToCString(int Data);			//int		-> Cstring
	CString ChangeToCString(std::string Data);	//string	-> Cstring
	std::string ChangeToString(CString Data);	//Cstring	-> string
	std::string ChangeToString(int Data);		//int		-> string
	std::string ChangeToString(float Data);		//float		-> string
	float ChangeToFloat(std::string Data);		//string	-> float
	float ChangeToFloat(CString Data);			//Cstring	-> float
	int ChangeToInt(CString Data);				//Cstring	-> int
	int ChangeToInt(std::string Data);			//string	-> int
protected:
	///파일의 타입을 가져온다 (.png , .fbx)
	int GetFileNameType(std::string Name);
	int GetFileNameType(CString Name);
	std::string CutStringFileType(std::string& FileName);
	std::string CutStringFileType(CString& FileName);
protected:
	std::string ChangeFilePath(CString& FilePath);

protected:
	///기능
	bool DropRect(RECT& rect);
	float Lerp(float prev, float next, float time);
protected:
	CToolTipCtrl mTooltip;
	CFont mfont;

	DECLARE_MESSAGE_MAP()
};
