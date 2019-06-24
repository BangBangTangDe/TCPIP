// CSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSocket.h"
#include "CSocketDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketDlg dialog

CCSocketDlg::CCSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSocketDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSocketDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CCSocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketDlg message handlers


//��ʼ���Ի���
BOOL CCSocketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	// TODO: Add extra initialization here

	count=0;
	m_list.InsertColumn(0,"��Ϣ");
	m_list.SetColumnWidth(0,435);
	m_edit.SetLimitText(99);
	for (int i=0;i<50;i++)
		msgsock[i]=NULL;

	//�趨��ַ
	serv.sin_addr.s_addr=htonl(INADDR_ANY);//�󶨵���������������ж����������Ĭ������
	serv.sin_family=AF_INET;//��ַ���壬Ĭ��Ϊtcp/ip
	serv.sin_port=5000;//htons(5000);
	addlen=sizeof(serv);
	m_button.EnableWindow(FALSE);
	//����socket
	sock=socket(AF_INET,SOCK_STREAM,0);
	//��
	if (bind(sock,(sockaddr*)&serv,addlen))
	{
		m_edit.SetWindowText("�󶨴���");
	}else
	{
	//m_list.InsertItem(count++,inet_ntoa(serv.sin_addr));
	m_edit.SetWindowText("�����������ɹ�");
	//��ʼ����	
	listen(sock,5);
	//�����߳�
	AfxBeginThread(&thread,0);

	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCSocketDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCSocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCSocketDlg::OnOK() 
{

	//	CDialog::OnOK();
}


//�������߳�
UINT thread(LPVOID p)
{
	char buff[100];
	CSize size;
	size.cx=0;
	size.cy=30;
	int s=1,msgcount,loop=1,flag=0;
	CCSocketDlg *dlg=(CCSocketDlg*)AfxGetApp()->GetMainWnd();
	//��ÿͻ�������
	msgcount=dlg->getcount();
	if (msgcount==-1)
		loop=0;
	if(loop)
	{
		s=1;
		dlg->msgsock[msgcount]=accept(dlg->sock,(sockaddr*)&(dlg->serv),&(dlg->addlen));
		if (dlg->msgsock[msgcount]==INVALID_SOCKET)
		{
			dlg->m_edit.SetWindowText("Error accept");
		}
		else
		{
			//�����߳�
			AfxBeginThread(thread,0);
			dlg->SetForegroundWindow();
			dlg->m_list.InsertItem(dlg->count++,"���ӳɹ�");
			dlg->m_list.InsertItem(dlg->count++,inet_ntoa(dlg->serv.sin_addr));
			dlg->m_list.Scroll(size);
			dlg->m_button.EnableWindow(TRUE);		
			while(s!=SOCKET_ERROR)
			{
				//ѭ����������
				s=recv(dlg->msgsock[msgcount],buff,100,0);
				dlg->SetForegroundWindow();
				if (s!=SOCKET_ERROR)
				{
					dlg->m_list.InsertItem(dlg->count++,buff);
					dlg->m_list.Scroll(size);
					dlg->sendtoall(dlg->msgsock[msgcount],buff);
				}
			}

			send(dlg->msgsock[msgcount],"Disconnected",100,0);
			dlg->m_list.InsertItem(dlg->count++,"Disconnected");
			dlg->m_list.Scroll(size);
			dlg->msgsock[msgcount]=NULL;
			for (int i=0;i<50;i++)
				if (dlg->msgsock[i]!=NULL)
					flag=1;
			if (flag!=1)
				dlg->m_button.EnableWindow(FALSE);
			closesocket(dlg->msgsock[msgcount]);
		
		}
	}
	//��ֹ�߳�
	AfxEndThread(0);
	return 0;
}


//��������
void CCSocketDlg::OnButton1() 
{
	char buff[100];
	m_edit.GetWindowText(buff,99);
	m_edit.SetWindowText("");
	m_list.InsertItem(count++,buff);
	CSize size;
	size.cx=0;
	size.cy=30;
	m_list.Scroll(size);
	//ѭ�������пͻ�������Ϣ
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]!=NULL)
		send(msgsock[i],buff,100,0);
	}
}


CCSocketDlg::~CCSocketDlg()
{
	for (int i=0;i<50;i++)
		if (msgsock[i]!=NULL)
			send(msgsock[i],"Disconnected",100,0);
}

//��û�û��ʹ�õ�socket�����
int CCSocketDlg::getcount()
{
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]==NULL)
			return i;
	}
	return -1;
	
}

//�����пͻ���������
void CCSocketDlg::sendtoall(SOCKET s,char *buff)
{
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]!=NULL && msgsock[i]!=s)
			//����
			send(msgsock[i],buff,100,0);
	}
}
