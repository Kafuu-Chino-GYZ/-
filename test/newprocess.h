#pragma once


// newprocess 对话框

class newprocess : public CDialogEx
{
	DECLARE_DYNAMIC(newprocess)

public:
	newprocess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~newprocess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_input;
	afx_msg void OnBnClickedOk();
//	CStatic m_text1;
//	CStatic m_text2;
	CStatic m_PID1;
	CStatic m_PID2;
};
