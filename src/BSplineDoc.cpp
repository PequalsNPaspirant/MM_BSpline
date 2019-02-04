// BSplineDoc.cpp : implementation of the CBSplineDoc class
//

#include "stdafx.h"
#include "BSpline.h"

#include "BSplineDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "BSplineData.h"

/////////////////////////////////////////////////////////////////////////////
// CBSplineDoc

IMPLEMENT_DYNCREATE(CBSplineDoc, CDocument)

BEGIN_MESSAGE_MAP(CBSplineDoc, CDocument)
	//{{AFX_MSG_MAP(CBSplineDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBSplineDoc construction/destruction

CBSplineDoc::CBSplineDoc()
{
	// TODO: add one-time construction code here
	m_selectedPt = -1;
	m_selectedKnot = -1;

	pBSplineData = new BSplineData(3);
}

CBSplineDoc::~CBSplineDoc()
{
	delete pBSplineData;
	pBSplineData = NULL;
}

BOOL CBSplineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBSplineDoc serialization

void CBSplineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBSplineDoc diagnostics

#ifdef _DEBUG
void CBSplineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBSplineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBSplineDoc commands
