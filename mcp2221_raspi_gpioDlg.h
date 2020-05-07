
// mcp2221_raspi_gpioDlg.h : ヘッダー ファイル
//

#pragma once


// Cmcp2221raspigpioDlg ダイアログ
class Cmcp2221raspigpioDlg : public CDialog
{
// コンストラクション
public:
	Cmcp2221raspigpioDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MCP2221_RASPI_GPIO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedCheckGp0();
	afx_msg void OnClickedCheckGp1();
	afx_msg void OnClickedCheckGp2();
	afx_msg void OnClickedCheckGp3();
	afx_msg void OnClickedCheckSsd1306();
	afx_msg void OnBnClickedButtonDispUpdate();
	virtual void OnOK();
};
