// ttf2pcxdlg.h : header file
//

#include "previewbutton.h"
#include "character.h"
#include "ColorButton.h"

typedef struct	CharData
{
  int width;
  int height;
  int x;
  int y;
} CharData;

/////////////////////////////////////////////////////////////////////////////
// Cttf2pcxDlg dialog

class Cttf2pcxDlg : public CDialog
{
// Construction
public:
	Cttf2pcxDlg(CWnd* pParent = NULL);	// standard constructor
	~Cttf2pcxDlg();

	CFont *GetFont(BOOL big = FALSE);
	void SaveChars(FILE *f, int type);

	CCharacter *chars;
	TCHAR m_currentdir[512];

// Dialog Data
	//{{AFX_DATA(Cttf2pcxDlg)
	enum { IDD = IDD_TTF2PCX_DIALOG };
	CColorButton	m_fontcolor;
	CEdit	m_fontheight;
	CPreviewButton	m_preview;
	CComboBox	m_stylelist;
	CComboBox	m_sizelist;
	CComboBox	m_fontlist;
	CString	m_font;
	CString	m_size;
	CString	m_style;
	CString	m_fonth;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cttf2pcxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	void save_pcx(unsigned char *b, int bw, int bh, FILE *f, int type);
	void save_bmp(unsigned char *b, int bw, int bh, FILE *f, int type);
	CharData dat[96];

	// Generated message map functions
	//{{AFX_MSG(Cttf2pcxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnExport();
	afx_msg void OnQuit();
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
