// newprocess.cpp: 实现文件
//

#include "pch.h"
#include "test.h"
#include "newprocess.h"
#include "afxdialogex.h"


// newprocess 对话框

IMPLEMENT_DYNAMIC(newprocess, CDialogEx)

newprocess::newprocess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

newprocess::~newprocess()
{
}

void newprocess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Input_Process, m_input);
	DDX_Control(pDX, IDC_STATIC_ID1, m_PID1);
	DDX_Control(pDX, IDC_STATIC_ID2, m_PID2);
}


BEGIN_MESSAGE_MAP(newprocess, CDialogEx)
	ON_BN_CLICKED(IDOK, &newprocess::OnBnClickedOk)
END_MESSAGE_MAP()


// newprocess 消息处理程序



CString dwprocessid;
CString dwThreadId;
//创建新进程
void CreateNewProcess(CString processname)
{
	LPWSTR szCommandLine = processname.GetBuffer();
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW; //制定wShowWindow成员
	si.wShowWindow = TRUE; //为真，显示进程的主窗口
	BOOL bRet = ::CreateProcess(
		NULL,//不在此指定可执行文件的文件名
		szCommandLine, //命令行参数
		NULL,//默认进程的安全性
		NULL,//默认线程的安全性
		FALSE,//指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE,//为新进程创建一个新的控制台窗口
		NULL,//使用本进程的环境变量
		NULL,//使用本进程的驱动器和目录
		&si,
		&pi);
	if (bRet)
	{
		//既然我们不使用两个句柄，最好是立刻将他们关闭
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
		
		dwprocessid.Format(_T("%d"), pi.dwProcessId);
		dwThreadId.Format(_T("%d"), pi.dwThreadId);
		/*printf("新的进程的进程ID号：%d\n", pi.dwProcessId);
		printf("新进程的主线程ID号：%d\n", pi.dwThreadId);*/
	}
}
void newprocess::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString processname;
	m_input.GetWindowText(processname);	
	CreateNewProcess(processname);
	m_PID1.SetWindowTextW(dwprocessid);
	m_PID2.SetWindowTextW(dwThreadId);
}
