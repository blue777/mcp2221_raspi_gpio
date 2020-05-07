
// mcp2221_raspi_gpioDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "mcp2221_raspi_gpio.h"
#include "mcp2221_raspi_gpioDlg.h"
#include "afxdialogex.h"

#include "MCP2221_RasPiGPIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cmcp2221raspigpioDlg ダイアログ



Cmcp2221raspigpioDlg::Cmcp2221raspigpioDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MCP2221_RASPI_GPIO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmcp2221raspigpioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cmcp2221raspigpioDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_GP0, &Cmcp2221raspigpioDlg::OnClickedCheckGp0)
	ON_BN_CLICKED(IDC_CHECK_GP1, &Cmcp2221raspigpioDlg::OnClickedCheckGp1)
	ON_BN_CLICKED(IDC_CHECK_GP2, &Cmcp2221raspigpioDlg::OnClickedCheckGp2)
	ON_BN_CLICKED(IDC_CHECK_GP3, &Cmcp2221raspigpioDlg::OnClickedCheckGp3)
	ON_BN_CLICKED(IDC_CHECK_SSD1306, &Cmcp2221raspigpioDlg::OnClickedCheckSsd1306)
	ON_BN_CLICKED(IDC_BUTTON_DISP_UPDATE, &Cmcp2221raspigpioDlg::OnBnClickedButtonDispUpdate)
END_MESSAGE_MAP()


// Cmcp2221raspigpioDlg メッセージ ハンドラー

BOOL Cmcp2221raspigpioDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	{
		MCP2221_RasPiGPIO::mcp2221	handle;
		MCP2221_RasPiGPIO			gpio(handle);

		if (!handle.IsValid())
		{
			AfxMessageBox(TEXT("MCP2221 が見つかりません"), MB_ICONERROR);
			PostQuitMessage(-1);
			return	FALSE;
		}

		gpio.InitializeGPIO_Flash();
		gpio.InitializeGPIO();

		CheckDlgButton(IDC_CHECK_GP0, gpio.GetOutputValue(0) ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_GP1, gpio.GetOutputValue(1) ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_GP2, gpio.GetOutputValue(2) ? BST_CHECKED : BST_UNCHECKED);
		CheckDlgButton(IDC_CHECK_GP3, gpio.GetOutputValue(3) ? BST_CHECKED : BST_UNCHECKED);
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void Cmcp2221raspigpioDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void Cmcp2221raspigpioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR Cmcp2221raspigpioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cmcp2221raspigpioDlg::OnClickedCheckGp0()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO			gpio(handle);

	if (handle.IsValid())
	{
		gpio.SetOutputValue(0, IsDlgButtonChecked(IDC_CHECK_GP0) == BST_UNCHECKED ? 0 : 1);
		CheckDlgButton(IDC_CHECK_GP0, gpio.GetOutputValue(0) ? BST_CHECKED : BST_UNCHECKED);
	}
}


void Cmcp2221raspigpioDlg::OnClickedCheckGp1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO			gpio(handle);

	if (handle.IsValid())
	{
		gpio.SetOutputValue(1, IsDlgButtonChecked(IDC_CHECK_GP1) == BST_UNCHECKED ? 0 : 1);
		CheckDlgButton(IDC_CHECK_GP1, gpio.GetOutputValue(1) ? BST_CHECKED : BST_UNCHECKED);
	}
}


void Cmcp2221raspigpioDlg::OnClickedCheckGp2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO			gpio(handle);

	if (handle.IsValid())
	{
		gpio.SetOutputValue(2, IsDlgButtonChecked(IDC_CHECK_GP2) == BST_UNCHECKED ? 0 : 1);
		CheckDlgButton(IDC_CHECK_GP2, gpio.GetOutputValue(2) ? BST_CHECKED : BST_UNCHECKED);
	}
}


void Cmcp2221raspigpioDlg::OnClickedCheckGp3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO			gpio(handle);

	if (handle.IsValid())
	{
		gpio.SetOutputValue(3, IsDlgButtonChecked(IDC_CHECK_GP3) == BST_UNCHECKED ? 0 : 1);
		CheckDlgButton(IDC_CHECK_GP3, gpio.GetOutputValue(3) ? BST_CHECKED : BST_UNCHECKED);
	}
}


void Cmcp2221raspigpioDlg::OnClickedCheckSsd1306()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO::SSD1306	ssd1306(handle);

	if (handle.IsValid())
	{
		if (IsDlgButtonChecked(IDC_CHECK_SSD1306) == BST_UNCHECKED)
		{
			ssd1306.DispOff();
		}
		else
		{
			ssd1306.DispOn();
		}
	}
}


void Cmcp2221raspigpioDlg::OnBnClickedButtonDispUpdate()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	MCP2221_RasPiGPIO::mcp2221	handle;
	MCP2221_RasPiGPIO::SSD1306	ssd1306(handle);

	if (handle.IsValid())
	{
		CString		str;
		CBitmap32	iBmp;
		RECT		tRect = { 0, 0, 128, 64 };

		iBmp.CreateBitmap(128, 64);
		iBmp.Clear(RGB(0, 0, 0));

		::SetTextColor(iBmp, RGB(255, 255, 255));

		iBmp.SetFont(16, NULL);
		GetDlgItemText(IDC_EDIT_DISP_TITLE, str);
		::DrawText(iBmp, str, str.GetLength(), &tRect, DT_LEFT | DT_TOP);


		tRect.top = 16+3;

		iBmp.SetFont(42, NULL);
		GetDlgItemText(IDC_EDIT_DISP_TEXT, str);
		::DrawText(iBmp, str, str.GetLength(), &tRect, DT_LEFT | DT_TOP);

		ssd1306.DrawImage(iBmp);

		HWND	hWnd = *GetDlgItem(IDC_STATIC_SSD1306);
		HDC	hDC = ::GetDC(hWnd);
		::BitBlt(hDC, 0, 0, iBmp.GetSize().cx, iBmp.GetSize().cy, iBmp, 0, 0, SRCCOPY);
		::ReleaseDC(hWnd,hDC);
	}
}


void Cmcp2221raspigpioDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

//	CDialog::OnOK();
}
