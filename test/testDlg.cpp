
// testDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "test.h"
#include "testDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "newprocess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestDlg 对话框



CtestDlg::CtestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_list);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Btn_Refresh, &CtestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(Btn_CloseProcess, &CtestDlg::OnBnClickedCloseprocess)
	ON_BN_CLICKED(Btn_CreateProcess, &CtestDlg::OnBnClickedCreateprocess)
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

//对话框初始化
BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
#pragma region 设置列表格式
	//给列表添加列  左对齐
	m_list.InsertColumn(0, _T("编号"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, _T("进程名称"), LVCFMT_LEFT, 200);
	m_list.InsertColumn(2, _T("PID"), LVCFMT_LEFT, 200);
	//设置拓展风格，添加网格线
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
#pragma endregion

	//遍历进程
	BrowserProcess();
	

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//遍历进程
void CtestDlg::BrowserProcess()
{
	HANDLE hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//获取快照失败，返回
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//枚举进程
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bMore=Process32First(hProcessSnap, &pe32);
	//此变量用于记录编号
	int i = 0;
	//此变量用于最终呈现编号
	CString NO;
	//此变量用于呈现PID
	CString PID;
	while (bMore)
	{
		NO.Format(_T("%d"), i+1);
		//编号插入列表
		m_list.InsertItem(i, NO);
		//进程名插入列表
		m_list.SetItemText(i, 1, pe32.szExeFile);
		//PID插入列表
		PID.Format(_T("%d"), pe32.th32ProcessID);
		m_list.SetItemText(i, 2, PID);
		bMore=Process32Next(hProcessSnap, &pe32);
		i++;
	}
}



void CtestDlg::OnBnClickedButton1()
{
	m_list.DeleteAllItems();
	BrowserProcess();
}


void CtestDlg::OnBnClickedCloseprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	int nselect=m_list.GetSelectionMark();
	//没选，返回
	if (nselect < 0)
	{
		return;
	}
	CString strPID=m_list.GetItemText(nselect, 2);
	DWORD dwPID = _wtoi(strPID);

	//打开PID进程,搞到进程句柄
	HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (hProcess == NULL)
	{
		MessageBox(L"打开进程失败！");
		return;
	}

	//关闭进程
	TerminateProcess(hProcess,0);
	//刷新进程列表
	OnBnClickedButton1();
}


void CtestDlg::OnBnClickedCreateprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	newprocess cp;
	cp.DoModal();
	
	
}
