// CMyListView.cpp: файл реализации
//


#include "pch.h"
#include "MySQL.h"
#include "CMyListView.h"
#include "MySQLDoc.h"
#include "MainFrm.h"

// CMyListView

IMPLEMENT_DYNCREATE(CMyListView, CListView)

CMyListView::CMyListView()
{
	m_currentTableName.Empty();
	m_pTree = nullptr;
	m_pEdit = nullptr;
	m_editingItem = -1;
	m_editingSubItem = -1;
}

CMyListView::~CMyListView()
{
	if (m_pEdit != nullptr)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
		m_pEdit = nullptr;
	}
}

BEGIN_MESSAGE_MAP(CMyListView, CListView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMyListView::OnDblclk)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(1002, &CMyListView::OnEndEditCommand)
END_MESSAGE_MAP()

CMySQLDoc* CMyListView::GetDocument() const
{
	return DYNAMIC_DOWNCAST(CMySQLDoc, m_pDocument);
}

void CMyListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	
	// Получаем указатель на ListCtrl
	CListCtrl& listCtrl = GetListCtrl();
	
	// Устанавливаем стиль для отчетного вида
	listCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL);
	listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
}

void CMyListView::DisplayTable(const CString& tableName)
{
	// Сбрасываем кэш первичного ключа при смене таблицы
	if (m_currentTableName != tableName)
	{
		m_primaryKeyColumn.Empty();
	}
	
	m_currentTableName = tableName;
	ClearList();
	
	// Получаем документ
	CMySQLDoc* pDoc = GetDocument();
	if (pDoc == nullptr || pDoc->m_conn == nullptr)
		return;
	
	CListCtrl& listCtrl = GetListCtrl();
	
	// Получаем колонки таблицы
	CStringArray columns;
	if (!pDoc->GetTableColumns(tableName, columns))
		return;
	
	// Сохраняем имена колонок для редактирования
	m_columnNames.RemoveAll();
	m_columnNames.Copy(columns);
	
	// Добавляем колонки в ListView
	listCtrl.DeleteAllItems();
	
	// Удаляем старые колонки
	int colCount = listCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = colCount - 1; i >= 0; i--)
	{
		listCtrl.DeleteColumn(i);
	}
	
	// Добавляем новые колонки
	for (int i = 0; i < columns.GetSize(); i++)
	{
		listCtrl.InsertColumn(i, columns[i], LVCFMT_LEFT, 150);
	}
	
	// Получаем данные таблицы
	CStringA sql;
	CString sqlUnicode;
	sqlUnicode.Format(_T("SELECT * FROM `%s`"), tableName);
	sql = pDoc->ConvertToUTF8(sqlUnicode);
	
	if (mysql_query(pDoc->m_conn, sql))
		return;
	
	MYSQL_RES* res = mysql_store_result(pDoc->m_conn);
	if (res == nullptr)
		return;
	
	unsigned int num_fields = mysql_num_fields(res);
	MYSQL_ROW row;
	int rowIndex = 0;
	
	// Заполняем данные
	while ((row = mysql_fetch_row(res)))
	{
		int itemIndex = listCtrl.InsertItem(rowIndex, _T(""));
		
		for (unsigned int i = 0; i < num_fields && i < (unsigned int)columns.GetSize(); i++)
		{
			CString cellValue;
			if (row[i])
			{
				cellValue = pDoc->ConvertFromUTF8(row[i]);
			}
			else
			{
				cellValue = _T("NULL");
			}
			listCtrl.SetItemText(itemIndex, i, cellValue);
		}
		rowIndex++;
	}
	
	mysql_free_result(res);
	
	// Автоматически подгоняем ширину колонок
	for (int i = 0; i < columns.GetSize(); i++)
	{
		listCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

void CMyListView::DisplayQueryResult(const CString& result)
{
	// Парсим результат запроса (формат строк с разделителями \r\n)
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.DeleteAllItems();
	
	// Удаляем старые колонки
	int colCount = listCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = colCount - 1; i >= 0; i--)
	{
		listCtrl.DeleteColumn(i);
	}
	
	CStringArray lines;
	CString temp = result;
	int pos = 0;
	CString line;
	
	// Разбиваем на строки
	while ((pos = temp.Find(_T("\r\n"))) != -1)
	{
		line = temp.Left(pos);
		if (!line.IsEmpty())
		{
			lines.Add(line);
		}
		temp = temp.Mid(pos + 2);
	}
	if (!temp.IsEmpty())
	{
		lines.Add(temp);
	}
	
	if (lines.GetSize() < 2)
		return;
	
	// Первая строка - заголовки (разделитель \t)
	CStringArray headers;
	CString headerLine = lines[0];
	pos = 0;
	
	while ((pos = headerLine.Find(_T('\t'))) != -1)
	{
		headers.Add(headerLine.Left(pos));
		headerLine = headerLine.Mid(pos + 1);
	}
	if (!headerLine.IsEmpty())
	{
		headers.Add(headerLine);
	}
	
	// Добавляем колонки
	for (int i = 0; i < headers.GetSize(); i++)
	{
		listCtrl.InsertColumn(i, headers[i], LVCFMT_LEFT, 150);
	}
	
	// Вторая строка может быть разделителем, пропускаем
	int dataStart = 1;
	if (lines.GetSize() > dataStart && lines[dataStart].Find(_T("---")) != -1)
	{
		dataStart = 2;
	}
	
	// Обрабатываем данные
	for (int i = dataStart; i < lines.GetSize(); i++)
	{
		// Пропускаем строку с количеством записей
		if (lines[i].Find(_T("Всего найдено")) != -1)
			continue;
			
		CStringArray cells;
		CString dataLine = lines[i];
		pos = 0;
		
		// Разбиваем на ячейки
		while ((pos = dataLine.Find(_T('\t'))) != -1)
		{
			cells.Add(dataLine.Left(pos));
			dataLine = dataLine.Mid(pos + 1);
		}
		if (!dataLine.IsEmpty())
		{
			cells.Add(dataLine);
		}
		
		if (cells.GetSize() > 0)
		{
			int itemIndex = listCtrl.InsertItem(i - dataStart, cells[0]);
			for (int j = 1; j < cells.GetSize() && j < headers.GetSize(); j++)
			{
				listCtrl.SetItemText(itemIndex, j, cells[j]);
			}
		}
	}
	
	// Автоматически подгоняем ширину колонок
	for (int i = 0; i < headers.GetSize(); i++)
	{
		listCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
	}
}

void CMyListView::ClearList()
{
	// Завершаем редактирование, если оно активно
	if (m_pEdit != nullptr && m_pEdit->GetSafeHwnd() != nullptr)
	{
		EndEditCell(FALSE);
	}
	
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.DeleteAllItems();
	
	// Удаляем колонки
	int colCount = listCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = colCount - 1; i >= 0; i--)
	{
		listCtrl.DeleteColumn(i);
	}
	
	// Сбрасываем кэш первичного ключа
	m_primaryKeyColumn.Empty();
}

// Диагностика CMyListView

#ifdef _DEBUG
void CMyListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


void CMyListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// Проверяем, что выбрана строка
	if (pNMIA->iItem < 0)
	{
		*pResult = 0;
		return;
	}
	
	// Завершаем предыдущее редактирование, если оно было
	if (m_pEdit != nullptr && m_pEdit->GetSafeHwnd() != nullptr)
	{
		EndEditCell(FALSE);
	}
	
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetItemState(pNMIA->iItem, LVIS_SELECTED, LVIS_SELECTED);
	
	// Определяем, какую колонку редактировать
	int subItem = pNMIA->iSubItem;
	if (subItem < 0)
		subItem = 0; // Если клик был не на колонке, редактируем первую
	
	// Начинаем редактирование ячейки
	StartEditCell(pNMIA->iItem, subItem);
	
	*pResult = 0;
}

void CMyListView::StartEditCell(int item, int subItem)
{
	if (m_currentTableName.IsEmpty() || m_columnNames.GetSize() == 0)
		return;
	
	if (item < 0 || subItem < 0 || subItem >= m_columnNames.GetSize())
		return;
	
	CMySQLDoc* pDoc = GetDocument();
	if (pDoc == nullptr || pDoc->m_conn == nullptr)
		return;
	
	// Получаем первичный ключ таблицы (кэшируем, если еще не получен)
	if (m_primaryKeyColumn.IsEmpty())
	{
		if (!pDoc->GetPrimaryKeyColumn(m_currentTableName, m_primaryKeyColumn))
		{
			AfxMessageBox(_T("Не удалось определить первичный ключ таблицы."));
			return;
		}
	}
	
	// Проверяем, не пытаемся ли редактировать первичный ключ
	if (m_columnNames[subItem].CompareNoCase(m_primaryKeyColumn) == 0)
	{
		AfxMessageBox(_T("Первичный ключ нельзя редактировать."));
		return;
	}
	
	// Получаем значение первичного ключа для этой строки
	int primaryKeyIndex = -1;
	for (int i = 0; i < m_columnNames.GetSize(); i++)
	{
		if (m_columnNames[i].CompareNoCase(m_primaryKeyColumn) == 0)
		{
			primaryKeyIndex = i;
			break;
		}
	}
	
	if (primaryKeyIndex < 0)
	{
		AfxMessageBox(_T("Не удалось найти колонку с первичным ключом."));
		return;
	}
	
	CListCtrl& listCtrl = GetListCtrl();
	m_primaryKeyValue = listCtrl.GetItemText(item, primaryKeyIndex);
	
	// Сохраняем текущие значения редактирования
	m_editingItem = item;
	m_editingSubItem = subItem;
	m_originalValue = listCtrl.GetItemText(item, subItem);
	
	// Получаем координаты ячейки
	CRect rect;
	listCtrl.GetSubItemRect(item, subItem, LVIR_BOUNDS, rect);
	
	// Увеличиваем размер для лучшего отображения
	rect.left += 1;
	rect.right -= 1;
	rect.top += 1;
	rect.bottom -= 1;
	
	// Создаем или переиспользуем CEdit
	if (m_pEdit == nullptr)
	{
		m_pEdit = new CEdit();
		m_pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
			rect, this, 1001);
		m_pEdit->SetFont(listCtrl.GetFont());
	}
	else
	{
		// Если CEdit уже существует, перемещаем и показываем его
		if (m_pEdit->GetSafeHwnd() == nullptr)
		{
			// Если окно было уничтожено, создаем заново
			m_pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
				rect, this, 1001);
			m_pEdit->SetFont(listCtrl.GetFont());
		}
		else
		{
			m_pEdit->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(),
				SWP_NOZORDER | SWP_SHOWWINDOW);
		}
	}
	
	m_pEdit->SetWindowText(m_originalValue);
	m_pEdit->SetSel(0, -1); // Выделяем весь текст
	m_pEdit->SetFocus();
}

void CMyListView::EndEditCell(BOOL save)
{
	if (m_pEdit == nullptr || m_pEdit->GetSafeHwnd() == nullptr)
		return;
	
	if (m_editingItem < 0 || m_editingSubItem < 0)
	{
		m_pEdit->ShowWindow(SW_HIDE);
		return;
	}
	
	CListCtrl& listCtrl = GetListCtrl();
	CString newValue;
	
	if (save)
	{
		m_pEdit->GetWindowText(newValue);
		
		// Проверяем, изменилось ли значение
		if (newValue != m_originalValue)
		{
			CMySQLDoc* pDoc = GetDocument();
			if (pDoc != nullptr && pDoc->m_conn != nullptr)
			{
				// Получаем все текущие значения строки
				CStringArray columnNames;
				CStringArray values;
				
				for (int i = 0; i < m_columnNames.GetSize(); i++)
				{
					columnNames.Add(m_columnNames[i]);
					if (i == m_editingSubItem)
						values.Add(newValue);
					else
						values.Add(listCtrl.GetItemText(m_editingItem, i));
				}
				
				// Обновляем запись в базе данных
				if (pDoc->UpdateTableRecord(m_currentTableName, columnNames, values,
					m_primaryKeyColumn, m_primaryKeyValue))
				{
					// Обновляем отображение в ListView
					listCtrl.SetItemText(m_editingItem, m_editingSubItem, newValue);
				}
				else
				{
					CString errorMsg;
					errorMsg.Format(_T("Ошибка обновления записи: %s"), CString(mysql_error(pDoc->m_conn)));
					AfxMessageBox(errorMsg);
					// Восстанавливаем старое значение
					listCtrl.SetItemText(m_editingItem, m_editingSubItem, m_originalValue);
				}
			}
		}
	}
	
	m_pEdit->ShowWindow(SW_HIDE);
	m_editingItem = -1;
	m_editingSubItem = -1;
	
	// Возвращаем фокус ListView
	SetFocus();
}

BOOL CMyListView::PreTranslateMessage(MSG* pMsg)
{
	// Обрабатываем сообщения от CEdit
	if (m_pEdit != nullptr && m_pEdit->GetSafeHwnd() != nullptr && 
		pMsg->hwnd == m_pEdit->GetSafeHwnd())
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)
			{
				EndEditCell(TRUE);
				return TRUE;
			}
			else if (pMsg->wParam == VK_ESCAPE)
			{
				EndEditCell(FALSE);
				return TRUE;
			}
		}
		else if (pMsg->message == WM_KILLFOCUS)
		{
			// Завершаем редактирование при потере фокуса
			// Используем PostMessage, чтобы не завершать редактирование во время обработки сообщения
			PostMessage(WM_COMMAND, MAKEWPARAM(1002, 0), 0);
		}
	}
	
	return CListView::PreTranslateMessage(pMsg);
}

void CMyListView::OnEndEditCommand()
{
	// Завершаем редактирование при потере фокуса
	if (m_pEdit != nullptr && m_pEdit->GetSafeHwnd() != nullptr)
	{
		// Проверяем, действительно ли фокус потерян
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd != m_pEdit)
		{
			EndEditCell(TRUE);
		}
	}
}

void CMyListView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Завершаем редактирование при клике вне ячейки
	if (m_pEdit != nullptr && m_pEdit->GetSafeHwnd() != nullptr)
	{
		CRect editRect;
		m_pEdit->GetWindowRect(&editRect);
		ScreenToClient(&editRect);
		
		if (!editRect.PtInRect(point))
		{
			EndEditCell(TRUE);
		}
	}
	
	CListView::OnLButtonDown(nFlags, point);
}

// Обработчики сообщений CMyListView
