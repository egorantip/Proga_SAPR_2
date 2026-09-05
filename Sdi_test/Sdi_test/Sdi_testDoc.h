
// Sdi_testDoc.h: интерфейс класса CSditestDoc 
//
#include "CMyTreeView.h"
#pragma once


class CMyTreeView;
class CSditestView;

class CSditestDoc : public CDocument
{
public:
	int GetData() { return M_iDataForStorageInOurClass; }
	afx_msg void UpdateTreeView();

private:
	int m_Version = 0;
	int M_iDataForStorageInOurClass;
	float M_fData;
protected: // создать только из сериализации
	CSditestDoc() noexcept;
	DECLARE_DYNCREATE(CSditestDoc)

// Атрибуты
public:
	CMyTreeView* m_pTreeView;
	CSditestView* m_pView;
// Операции
public:
	bool m_bRoot, m_bCoord, m_bHatch, m_bSinus;
	// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CSditestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnLinesCoord();
	afx_msg void OnLinesHatch();
	afx_msg void OnLinesSinus();
	afx_msg void OnUpdateLinesCoord(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinesHatch(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLinesSinus(CCmdUI* pCmdUI);
	afx_msg void OnLinesRoot();
	afx_msg void OnUpdateLinesRoot(CCmdUI* pCmdUI);
	afx_msg void OnLinesModalDlg();
};
