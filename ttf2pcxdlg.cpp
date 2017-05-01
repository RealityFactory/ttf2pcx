// ttf2pcxdlg.cpp : implementation file
//

#include "stdafx.h"
#include "ttf2pcx.h"
#include "ttf2pcxdlg.h"
#include <direct.h>

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
// Cttf2pcxDlg dialog

Cttf2pcxDlg::Cttf2pcxDlg(CWnd* pParent /*=NULL*/)
: CDialog(Cttf2pcxDlg::IDD, pParent)
{
	chars = new CCharacter[65536];
	
	//{{AFX_DATA_INIT(Cttf2pcxDlg)
	m_font = _T("Times New Roman");
	m_size = _T("12");
	m_style = _T("Regular");
	m_fonth = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Cttf2pcxDlg::~Cttf2pcxDlg()
{
	delete [] chars;
}

void Cttf2pcxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cttf2pcxDlg)
	DDX_Control(pDX, IDC_FONTCOLOR, m_fontcolor);
	DDX_Control(pDX, IDC_FONTHEIGHT, m_fontheight);
	DDX_Control(pDX, IDC_PREVIEW, m_preview);
	DDX_Control(pDX, IDC_STYLELIST, m_stylelist);
	DDX_Control(pDX, IDC_SIZELIST, m_sizelist);
	DDX_Control(pDX, IDC_FONTLIST, m_fontlist);
	DDX_CBString(pDX, IDC_FONTLIST, m_font);
	DDX_CBString(pDX, IDC_SIZELIST, m_size);
	DDX_CBString(pDX, IDC_STYLELIST, m_style);
	DDX_Text(pDX, IDC_FONTHEIGHT, m_fonth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(Cttf2pcxDlg, CDialog)
//{{AFX_MSG_MAP(Cttf2pcxDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_EXPORT, OnExport)
ON_BN_CLICKED(IDC_QUIT, OnQuit)
ON_CBN_SELCHANGE(IDC_FONTLIST, OnChange)
ON_CBN_SELCHANGE(IDC_STYLELIST, OnChange)
ON_CBN_SELCHANGE(IDC_SIZELIST, OnChange)
ON_CBN_EDITCHANGE(IDC_FONTLIST, OnChange)
ON_CBN_EDITCHANGE(IDC_STYLELIST, OnChange)
ON_CBN_EDITCHANGE(IDC_SIZELIST, OnChange)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cttf2pcxDlg message handlers


int CALLBACK enumfontproc(const LOGFONT *lpelfe, const TEXTMETRICA *lpntme, unsigned long FontType,     long lParam)
{
	CComboBox *combo = (CComboBox *)lParam;
	
	if (combo->FindStringExact(-1, lpelfe->lfFaceName) == CB_ERR)
		combo->AddString(lpelfe->lfFaceName);
	
	return 1;
}


BOOL Cttf2pcxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}
	
	SetIcon(m_hIcon, TRUE);                 // Set big icon
	SetIcon(m_hIcon, FALSE);                // Set small icon
	
	LOGFONT lf;
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfFaceName[0] = 0;
	lf.lfPitchAndFamily = 0;
	
	CClientDC cdc(this);
	
	EnumFontFamiliesEx(cdc.m_hAttribDC, &lf, enumfontproc, (DWORD)&m_fontlist, 0);
	
	m_fontlist.SelectString(-1, "Times New Roman");

	m_fontcolor.SetColor(RGB(255,255,255));
	_chdir("..");
	_getcwd(m_currentdir, 512);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void Cttf2pcxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cttf2pcxDlg::OnPaint() 
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


HCURSOR Cttf2pcxDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void iputw(int w, FILE *f)
{
	int b1, b2;
	
	b1 = (w & 0xff00) >> 8;
	b2 = w & 0x00ff;
	
	putc(b2, f);
	putc(b1, f);
}

void iputl(long l, FILE *f)
{
   int b1, b2, b3, b4;

   b1 = (int)((l & 0xFF000000L) >> 24);
   b2 = (int)((l & 0x00FF0000L) >> 16);
   b3 = (int)((l & 0x0000FF00L) >> 8);
   b4 = (int)l & 0x00FF;

   putc(b4,f);
   putc(b3,f);
   putc(b2,f);
   putc(b1,f);
}

void Cttf2pcxDlg::save_pcx(unsigned char *b, int bw, int bh, FILE *f, int type)
{
	int c;
	int x, y;
	int runcount;
	char runchar;
	char ch;
	
	putc(10, f);                /* manufacturer */
	putc(5, f);                 /* version */
	putc(1, f);                 /* run length encoding  */
	putc(8, f);                 /* 8 bits per pixel */
	iputw(0, f);                /* xmin */
	iputw(0, f);                /* ymin */
	iputw(bw-1, f);             /* xmax */
	iputw(bh-1, f);             /* ymax */
	iputw(320, f);              /* HDpi */
	iputw(200, f);              /* VDpi */
	for (c=0; c<16; c++) {
		putc(c, f);
		putc(c, f);
		putc(c, f);
	}
	putc(0, f);                 /* reserved */
	putc(1, f);                 /* one color plane */
	iputw(bw, f);               /* number of bytes per scanline */
	iputw(1, f);                /* color pallete */
	iputw(bw, f);               /* hscreen size */
	iputw(bh, f);               /* vscreen size */
	for (c=0; c<54; c++)        /* filler */
		putc(0, f);
	
	for (y=0; y<bh; y++) {      /* for each scanline... */
		runcount = 0;
		runchar = 0;
		for (x=0; x<bw; x++) {   /* for each pixel... */
			ch = b[x+y*bw];
			if (runcount==0) {
				runcount = 1;
				runchar = ch;
			}
			else {
				if ((ch != runchar) || (runcount >= 0x3f)) {
					if ((runcount > 1) || ((runchar & 0xC0) == 0xC0))
						putc(0xC0 | runcount, f);
					putc(runchar,f);
					runcount = 1;
					runchar = ch;
				}
				else
					runcount++;
			}
		}
		if ((runcount > 1) || ((runchar & 0xC0) == 0xC0))
			putc(0xC0 | runcount, f);
		putc(runchar,f);
	}
	
	putc(12,f);                 /* 256 color pallete flag */

	COLORREF color = m_fontcolor.GetColor();
	BYTE red = GetRValue(color);
	BYTE green = GetGValue(color);
	BYTE blue = GetBValue(color);

	if(type == 0)
	{
		if(red==0 && green==0 &&blue==0)
		{
			putc(255, f); // White - 255
			putc(255, f);
			putc(255, f);
		}
		else
		{
			putc(0, f); // Black - 0
			putc(0, f);
			putc(0, f);
		}
	}
	else
	{
		putc(0, f); // Black - 0
		putc(0, f);
		putc(0, f);
	}


	if(type == 0)
	{
		putc(GetRValue(color), f); // Font color - 1
		putc(GetGValue(color), f);
		putc(GetBValue(color), f);
	}
	else
	{
		putc(255, f); // White - 255
		putc(255, f);
		putc(255, f);
	}

	for (c=2; c<255; c++)
	{
		putc(0, f);
		putc(255, f);
		putc(255, f);
	}
	
	if(type == 0)
	{
		if(red==0 && green==0 &&blue==0)
		{
			putc(0, f); 
			putc(0, f);
			putc(0, f);
		}
		else
		{
			putc(255, f); 
			putc(255, f);
			putc(255, f);
		}
	}
	else
	{
		putc(255, f); 
		putc(255, f);
		putc(255, f);
	}
}

//
// type = 0 font bitmap
//      = 1 alpha bitmap
//      = 2 dat maker

void Cttf2pcxDlg::SaveChars(FILE *f, int type)
{
	int w, h, bw, bh, c;
	unsigned char *b;
	
	w = 0;
	h = 0;
	
	for (c=0; c<96; c++) 
	{
		if (chars[c].GetW() > w)
			w = chars[c].GetW();
		if (chars[c].GetH() > h)
			h = chars[c].GetH();
	}

	if(type == 2)
	{
		for (c=0; c<96; c++) 
		{
			dat[c].width = chars[c].GetW();
			if(c==0)
				dat[c].width = dat[c].width/2;
			dat[c].height = chars[c].GetH();
		}
	}

	w = (w+16) & 0xFFF0;
	h = (h+16) & 0xFFF0;
	
	bw = w*16;
	bh = h*((96+15)/16);
	b = (unsigned char *)malloc(bw*bh);
	memset(b, 255, bw*bh);
	
	int scale = 1;
	
	for (c=0; c<96; c++) 
	{
		int x = w*(c&15);
		int y = h*(c/16);

		if(type == 2)
		{
			dat[c].x = x;
			dat[c].y = y;
		}
		
		for (int _y=0; _y<chars[c].GetH()/scale; _y++) 
		{
			for (int _x=0; _x<chars[c].GetW()/scale; _x++) {
				int col = chars[c].GetPixel(_x, _y) ? 1 : 0;
				b[(_y+y)*bw+_x+x] = col;
			}
		}
	}
	
	if(type !=2)
		save_bmp(b, bw, bh, f, type);
	free(b);
}


void Cttf2pcxDlg::OnExport() 
{
	if (UpdateData()) 
	{
		CFileDialog dlg(FALSE, "dat", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"Font Dat Files (*.dat)|*.dat|All Files (*.*)|*.*||");
		
		dlg.m_ofn.lpstrTitle = "Export Font";
		TCHAR m_dir[512];
		strcpy(m_dir, m_currentdir);
		strcat(m_dir, "\\media\\bitmaps\\fonts");
		dlg.m_ofn.lpstrInitialDir = m_dir;
		
		if (dlg.DoModal() == IDOK) 
		{
			CWaitCursor wc;
			
			CFont *font = GetFont();
			
			for (int c=32; c<=127; c++)
				chars[c-32].Get(font, c);
			
			delete font;
			
			CString PathName = dlg.GetPathName();
			CString FileName = PathName.Mid(PathName.ReverseFind('\\')+1);
			FileName = FileName.Left(FileName.ReverseFind('.')+1) + "bmp";
			CString AlphaName;
			AlphaName = "A_"+FileName;
			CString Path = PathName.Left(PathName.ReverseFind('\\')+1);
			AlphaName = Path+AlphaName;
			FileName = Path+FileName;

			FILE *f = fopen(FileName, "wb");
			if (!f) 
			{
				AfxMessageBox("Error opening " + FileName);
				return;
			}
			SaveChars(f, 0);
			fclose(f);

			f = fopen(AlphaName, "wb");
			if (!f) 
			{
				AfxMessageBox("Error opening " + AlphaName);
				return;
			}
			SaveChars(f, 1);
			fclose(f);

			f = fopen(PathName, "wb");
			if (!f) 
			{
				AfxMessageBox("Error opening " + PathName);
				return;
			} 
			SaveChars(f, 2);
			fwrite(&dat[0].height, sizeof(int), 1, f);
			for(c=0;c<96;c++)
			{
				fwrite(&dat[c].width, sizeof(int), 1, f);
				fwrite(&dat[c].x, sizeof(int), 1, f);
				fwrite(&dat[c].y, sizeof(int), 1, f);
			}
			fclose(f);
		}
	}
}


void Cttf2pcxDlg::OnQuit() 
{
	OnCancel();
}


CFont *Cttf2pcxDlg::GetFont(BOOL big)
{
	CString size;
	m_sizelist.GetWindowText(size);
	
	CString fontname;
	m_fontlist.GetWindowText(fontname);
	
	CString style;
	m_stylelist.GetWindowText(style);
	
	BOOL bold = (style.Find("Bold") >= 0);
	BOOL italic = (style.Find("Italic") >= 0);
	
	LOGFONT lf = {
		atoi(size) * (big ? 80 : 10),
			0,
			0,
			0,
			bold ? 700 : 400,
			italic,
			FALSE,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
	};
	
	strcpy(lf.lfFaceName, fontname.GetBuffer(0));
	
	CFont *font = new CFont;
	font->CreatePointFontIndirect(&lf, NULL);
	
	return font;
}


void Cttf2pcxDlg::OnChange() 
{
	m_preview.Invalidate();
}

void Cttf2pcxDlg::save_bmp(unsigned char *b, int bw, int bh, FILE *f, int type)
{
	int bfSize;
	int filler = 3 - ((bw-1) & 3);
	int c, i, j;
	
	bfSize = 54              // header 
	       +256*4           // palette 
		   +bw*bh;          // image data 
	
	/* file_header */
	iputw(0x4D42, f);              /* bfType ("BM") */
	iputl(bfSize, f);              /* bfSize */
	iputw(0, f);                   /* bfReserved1 */
	iputw(0, f);                   /* bfReserved2 */
	
	iputl(54+256*4, f);    /* bfOffBits */
	
	/* info_header */
	bfSize = bw * bh;
	
	iputl(40, f);                  /* biSize */
	iputl(bw, f);              /* biWidth */
	iputl(bh, f);              /* biHeight */
	iputw(1, f);                   /* biPlanes */
	iputw(8, f);                 /* biBitCount */
	iputl(0, f);                   /* biCompression */
	iputl(bfSize, f);              /* biSizeImage */
	iputl(0, f);                   /* biXPelsPerMeter */
	iputl(0, f);                   /* biYPelsPerMeter */
	
	iputl(256, f);              /* biClrUsed */
	iputl(256, f);              /* biClrImportant */
	
	/* palette */
	
	COLORREF color = m_fontcolor.GetColor();
	BYTE red = GetRValue(color);
	BYTE green = GetGValue(color);
	BYTE blue = GetBValue(color);
	
	if(type == 0)
	{
		if(red==0 && green==0 &&blue==0)
		{
			putc(255, f); // White - 255
			putc(255, f);
			putc(255, f);
			putc(0, f);
		}
		else
		{
			putc(0, f); // Black - 0
			putc(0, f);
			putc(0, f);
			putc(0, f);
		}
	}
	else
	{
		putc(0, f); // Black - 0
		putc(0, f);
		putc(0, f);
		putc(0, f);
	}
	
	
	if(type == 0)
	{
		putc(GetBValue(color), f); // Font color - 1
		putc(GetGValue(color), f);
		putc(GetRValue(color), f);
		putc(0, f);
	}
	else
	{
		putc(255, f); // White - 255
		putc(255, f);
		putc(255, f);
		putc(0, f);
	}
	
	for (c=2; c<255; c++)
	{
		putc(0, f);
		putc(255, f);
		putc(255, f);
		putc(0, f);
	}
	
	if(type == 0)
	{
		if(red==0 && green==0 &&blue==0)
		{
			putc(0, f); 
			putc(0, f);
			putc(0, f);
			putc(0, f);
		}
		else
		{
			putc(255, f); 
			putc(255, f);
			putc(255, f);
			putc(0, f);
		}
	}
	else
	{
		putc(255, f); 
		putc(255, f);
		putc(255, f);
		putc(0, f);
	}
	
	/* image data */
	for (i=bh-1; i>=0; i--) 
	{
		for (j=0; j<bw; j++) 
			putc(b[j+i*bw], f);

		for (j=0; j<filler; j++)
			putc(0, f);
	}
}

