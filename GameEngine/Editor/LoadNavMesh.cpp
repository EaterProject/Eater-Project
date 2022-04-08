// LoadNavMesh.cpp: 구현 파일
//

#include "pch.h"
#include "Editor.h"
#include "LoadNavMesh.h"
#include "afxdialogex.h"


// LoadNavMesh 대화 상자

IMPLEMENT_DYNAMIC(LoadNavMesh, CDialogEx)

LoadNavMesh::LoadNavMesh(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOAD_NAVMESH, pParent)
{

}

LoadNavMesh::~LoadNavMesh()
{
}

void LoadNavMesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LoadNavMesh, CDialogEx)
END_MESSAGE_MAP()


// LoadNavMesh 메시지 처리기
