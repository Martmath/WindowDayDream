
// WindowDayDreamDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "WindowDayDream.h"
#include "WindowDayDreamDlg.h"
#include "afxdialogex.h"


#include "DDConnector.h"
#include "MCommand.h"
#include "BaseÑycles.h"
//#define TO_SEARCH_DEVICE_UUID "{0000FE55-0000-1000-8000-00805F9B34FB}"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CWindowDayDreamDlg dialog



CWindowDayDreamDlg::CWindowDayDreamDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINDOWDAYDREAM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowDayDreamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CWindowDayDreamDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWindowDayDreamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWindowDayDreamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(STOP, &CWindowDayDreamDlg::OnBnClickedStop)
END_MESSAGE_MAP()


// CWindowDayDreamDlg message handlers

BOOL CWindowDayDreamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_List.AddString("Begin");
	//ShowWindow(SW_MAXIMIZE);

	//ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWindowDayDreamDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWindowDayDreamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWindowDayDreamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//checking a parallel loop to speed up the madgwick algorithm(this didn't help)
#include <thread>
Madgwick mM1 = Madgwick();
void mLoop(DataDevice* D, CWindowDayDreamDlg* dlg)
{
	while (true)
	{
		//DoEvents();	
		mM1.updateIMU(D->gir.x, D->gir.y, D->gir.z, D->accel.x, D->accel.y, D->accel.z);
		dlg->SetDlgItemText(IDC_EDIT16, std::to_string(mM1.getPitch()).c_str());
		dlg->SetDlgItemText(IDC_EDIT17, std::to_string(mM1.getRoll()).c_str());
		dlg->SetDlgItemText(IDC_EDIT18, std::to_string(mM1.getYaw()).c_str());
	}
}

DDConnector e;
void CWindowDayDreamDlg::OnBnClickedOk()
{
	//delegateTest();
	ICO.Init();
	ICO.Init();
	ICO.getDayDreamIco();
	auto jq = e.getDeviceList();
	TFirstDevice::EV::Ss::Start();
	e.init(jq[0], (DDConnector::PFunc)(TFirstDevice::SomethingHappened));
	
	//std::thread thread(mLoop, &(Device<0>::data), theApp.dlg);

	if (jq.size() > 1) 
	{
		TSecondDevice::EV::Ss::Start();
		e.init(jq[1], (DDConnector::PFunc)(TSecondDevice::SomethingHappened));
	}	
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CWindowDayDreamDlg::OnBnClickedCancel()
{
	e.stop();
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CWindowDayDreamDlg::OnBnClickedStop()
{
	e.stop();
	// TODO: Add your control notification handler code here
}
