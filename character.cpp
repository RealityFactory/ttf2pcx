// Character.cpp : implementation file
//

#include "stdafx.h"
#include "ttf2pcx.h"
#include "ttf2pcxdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCharacter::CCharacter()
{
	w = h = bw = 0;
	p = NULL;
}



CCharacter::~CCharacter()
{	
	Destroy();
}



void CCharacter::Destroy()
{
	w = h = bw = 0;

	if (p) {
		free(p);
		p = NULL;
	}
}



void CCharacter::Get(CFont *font, int c)
{
	Destroy();

	CDC cdc;
	cdc.CreateCompatibleDC(NULL);

	CFont *oldfont = cdc.SelectObject(font);

	char s[2] = { c, 0 };

	CSize size = cdc.GetTextExtent( s, 1 ) ;

	CBitmap *bmp = new CBitmap;
	bmp->CreateCompatibleBitmap(&cdc, size.cx*2, size.cy);

	CBitmap *oldbmp = cdc.SelectObject(bmp);

	cdc.FillSolidRect(0, 0, size.cx*2, size.cy, 0);
	cdc.SetTextColor(0x00FFFFFF);
	cdc.TextOut(0, 0, s, 1);

	cdc.SelectObject(oldbmp);
	cdc.SelectObject(oldfont);

	BITMAP b;
	bmp->GetBitmap(&b);

	w = b.bmWidth;
	h = b.bmHeight;
	bw = b.bmWidthBytes;

	p = (unsigned char *)malloc(bw*h);
	bmp->GetBitmapBits(bw*h, p);

	delete bmp;

	if (c > ' ') {
		while (w > 1) {
			for (int y=0; y<h; y++)
				if (GetPixel(w-2, y))
					goto done;

			w--;
		}
	}

	done:

	return;
}

