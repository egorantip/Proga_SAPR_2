
// MySQLDoc.cpp: реализация класса CMySQLDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MySQL.h"
#endif

#include "MySQLDoc.h"
#include "ConnectDbDlg.h"
#include "FilterByIdDlg.h"
#include "CMyListView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMySQLDoc

IMPLEMENT_DYNCREATE(CMySQLDoc, CDocument)

BEGIN_MESSAGE_MAP(CMySQLDoc, CDocument)
	ON_COMMAND(ID_CONNECT_DATABASE, &CMySQLDoc::OnConnectDatabase)
	ON_COMMAND(ID_FILTER_BY_ID, &CMySQLDoc::OnFilterById)
END_MESSAGE_MAP()


// Создание или уничтожение CMySQLDoc

CMySQLDoc::CMySQLDoc() noexcept
{
	// Инициализируем указатель на подключение
	m_conn = nullptr;
	
	// Устанавливаем значения по умолчанию
	m_dbHost = _T("127.0.0.1");
	m_dbPort = 3307;
	m_dbUser = _T("root");
	m_dbPassword = _T("");
	m_dbName = _T("mydb"); // Имя базы данных по умолчанию
	
	// Загружаем сохраненные параметры подключения
	LoadConnectionParams();
}

CMySQLDoc::~CMySQLDoc()
{
	// Закрываем подключение при уничтожении документа
	CloseConnection();
}

BOOL CMySQLDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Загружаем параметры подключения из реестра
	LoadConnectionParams();

	// Не подключаемся автоматически - пользователь должен нажать кнопку "Подключиться" в меню
	return TRUE;
}




// Сериализация CMySQLDoc

void CMySQLDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMySQLDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMySQLDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMySQLDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMySQLDoc

#ifdef _DEBUG
void CMySQLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMySQLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMySQLDoc

// Подключение к базе данных (использует сохраненные параметры)
BOOL CMySQLDoc::ConnectToDatabase()
{
	return ConnectToDatabase(m_dbHost, m_dbPort, m_dbUser, m_dbPassword, m_dbName);
}

// Подключение к базе данных
BOOL CMySQLDoc::ConnectToDatabase(const CString& host, int port, const CString& user, 
	const CString& password, const CString& database)
{
	// Закрываем существующее подключение, если есть
	CloseConnection();
	
	// Инициализируем подключение
	m_conn = mysql_init(nullptr);
	if (m_conn == nullptr)
	{
		AfxMessageBox(_T("Ошибка инициализации MySQL!"));
		return FALSE;
	}

	// Преобразуем параметры в ANSI для MySQL
	CStringA hostA = ConvertToUTF8(host);
	CStringA userA = ConvertToUTF8(user);
	CStringA passwordA = ConvertToUTF8(password);
	CStringA databaseA = ConvertToUTF8(database);

	// Подключаемся к базе данных
	if (!mysql_real_connect(m_conn, hostA, userA, passwordA, databaseA, port, NULL, 0))
	{
		CString errorMsg;
		errorMsg.Format(_T("Ошибка подключения к базе данных: %s"), CString(mysql_error(m_conn)));
		AfxMessageBox(errorMsg);
		mysql_close(m_conn);
		m_conn = nullptr;
		return FALSE;
	}

	// Устанавливаем кодировку UTF-8 (используем utf8mb4 для полной поддержки Unicode)
	if (mysql_set_character_set(m_conn, "utf8mb4"))
	{
		// Если utf8mb4 не поддерживается, пробуем utf8
		if (mysql_set_character_set(m_conn, "utf8"))
		{
			CString errorMsg;
			errorMsg.Format(_T("Ошибка установки кодировки: %s"), CString(mysql_error(m_conn)));
			AfxMessageBox(errorMsg);
		}
	}

	return TRUE;
}

// Загрузка параметров подключения из реестра
void CMySQLDoc::LoadConnectionParams()
{
	// Загружаем из реестра через приложение
	CWinApp* pApp = AfxGetApp();
	if (pApp)
	{
		m_dbHost = pApp->GetProfileString(_T("Database"), _T("Host"), _T("127.0.0.1"));
		m_dbPort = pApp->GetProfileInt(_T("Database"), _T("Port"), 3307);
		m_dbUser = pApp->GetProfileString(_T("Database"), _T("User"), _T("root"));
		m_dbPassword = pApp->GetProfileString(_T("Database"), _T("Password"), _T(""));
		m_dbName = pApp->GetProfileString(_T("Database"), _T("Name"), _T("mydb"));
	}
}

// Сохранение параметров подключения в реестр
void CMySQLDoc::SaveConnectionParams()
{
	// Сохраняем в реестр через приложение
	CWinApp* pApp = AfxGetApp();
	if (pApp)
	{
		pApp->WriteProfileString(_T("Database"), _T("Host"), m_dbHost);
		pApp->WriteProfileInt(_T("Database"), _T("Port"), m_dbPort);
		pApp->WriteProfileString(_T("Database"), _T("User"), m_dbUser);
		pApp->WriteProfileString(_T("Database"), _T("Password"), m_dbPassword);
		pApp->WriteProfileString(_T("Database"), _T("Name"), m_dbName);
	}
}

// Закрытие подключения
void CMySQLDoc::CloseConnection()
{
	if (m_conn != nullptr)
	{
		mysql_close(m_conn);
		m_conn = nullptr;
	}
}

// Преобразование Unicode строки в UTF-8 для MySQL
CStringA CMySQLDoc::ConvertToUTF8(const CString& str)
{
	if (str.IsEmpty())
		return CStringA();

	int size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	if (size <= 0)
		return CStringA();

	CStringA result;
	LPSTR buffer = result.GetBuffer(size);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, buffer, size, NULL, NULL);
	result.ReleaseBuffer();

	return result;
}

// Преобразование UTF-8 строки в Unicode для отображения
CString CMySQLDoc::ConvertFromUTF8(const char* utf8str)
{
	if (utf8str == nullptr || strlen(utf8str) == 0)
		return CString();

	int size = MultiByteToWideChar(CP_UTF8, 0, utf8str, -1, NULL, 0);
	if (size <= 0)
		return CString();

	CString result;
	LPWSTR buffer = result.GetBuffer(size);
	MultiByteToWideChar(CP_UTF8, 0, utf8str, -1, buffer, size);
	result.ReleaseBuffer();

	return result;
}

// Вспомогательный метод для выполнения SELECT запроса и формирования результата
BOOL CMySQLDoc::ExecuteSelectQuery(const CStringA& sql, CString& result)
{
	result.Empty();

	if (m_conn == nullptr)
	{
		result = _T("Ошибка: нет подключения к базе данных");
		return FALSE;
	}

	// Выполняем запрос
	if (mysql_query(m_conn, sql))
	{
		CString errorMsg;
		errorMsg.Format(_T("Ошибка выполнения запроса: %s"), CString(mysql_error(m_conn)));
		result = errorMsg;
		return FALSE;
	}

	// Получаем результат
	MYSQL_RES* res = mysql_store_result(m_conn);
	if (res == nullptr)
	{
		if (mysql_field_count(m_conn) == 0)
		{
			// Запрос не возвращает результат (например, INSERT, UPDATE, DELETE)
			result = _T("Запрос выполнен успешно");
			return TRUE;
		}
		else
		{
			CString errorMsg;
			errorMsg.Format(_T("Ошибка получения результата: %s"), CString(mysql_error(m_conn)));
			result = errorMsg;
			return FALSE;
		}
	}

	// Получаем количество полей
	unsigned int num_fields = mysql_num_fields(res);
	MYSQL_FIELD* fields = mysql_fetch_fields(res);

	// Формируем заголовок таблицы
	CString header;
	for (unsigned int i = 0; i < num_fields; i++)
	{
		if (i > 0) header += _T("\t");
		header += ConvertFromUTF8(fields[i].name);
	}
	result += header + _T("\r\n");

	// Добавляем разделитель
	CString separator;
	for (unsigned int i = 0; i < num_fields; i++)
	{
		if (i > 0) separator += _T("\t");
		separator += _T("---");
	}
	result += separator + _T("\r\n");

	// Обрабатываем строки результата
	MYSQL_ROW row;
	int rowCount = 0;
	while ((row = mysql_fetch_row(res)))
	{
		CString rowStr;
		for (unsigned int i = 0; i < num_fields; i++)
		{
			if (i > 0) rowStr += _T("\t");
			if (row[i])
			{
				rowStr += ConvertFromUTF8(row[i]);
			}
			else
			{
				rowStr += _T("NULL");
			}
		}
		result += rowStr + _T("\r\n");
		rowCount++;
	}

	// Добавляем информацию о количестве строк
	CString footer;
	footer.Format(_T("\r\nВсего найдено записей: %d"), rowCount);
	result += footer;

	// Освобождаем ресурсы
	mysql_free_result(res);

	return TRUE;
}

// Получить список всех таблиц
BOOL CMySQLDoc::GetTablesList(CStringArray& tables)
{
	tables.RemoveAll();
	
	if (m_conn == nullptr)
		return FALSE;

	CStringA sql = "SHOW TABLES";
	if (mysql_query(m_conn, sql))
		return FALSE;

	MYSQL_RES* res = mysql_store_result(m_conn);
	if (res == nullptr)
		return FALSE;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		if (row[0])
		{
			tables.Add(ConvertFromUTF8(row[0]));
		}
	}

	mysql_free_result(res);
	return TRUE;
}

// Получить список колонок таблицы
BOOL CMySQLDoc::GetTableColumns(const CString& tableName, CStringArray& columns)
{
	columns.RemoveAll();
	
	if (m_conn == nullptr)
		return FALSE;

	CString sqlUnicode;
	sqlUnicode.Format(_T("SHOW COLUMNS FROM `%s`"), tableName);
	CStringA sql = ConvertToUTF8(sqlUnicode);
	
	if (mysql_query(m_conn, sql))
		return FALSE;

	MYSQL_RES* res = mysql_store_result(m_conn);
	if (res == nullptr)
		return FALSE;

	MYSQL_ROW row;
	while ((row = mysql_fetch_row(res)))
	{
		if (row[0])
		{
			columns.Add(ConvertFromUTF8(row[0]));
		}
	}

	mysql_free_result(res);
	return TRUE;
}

// Получить первичный ключ таблицы
BOOL CMySQLDoc::GetPrimaryKeyColumn(const CString& tableName, CString& primaryKeyColumn)
{
	primaryKeyColumn.Empty();
	
	if (m_conn == nullptr)
		return FALSE;

	CString sqlUnicode;
	sqlUnicode.Format(_T("SHOW KEYS FROM `%s` WHERE Key_name = 'PRIMARY'"), tableName);
	CStringA sql = ConvertToUTF8(sqlUnicode);
	
	if (mysql_query(m_conn, sql))
		return FALSE;

	MYSQL_RES* res = mysql_store_result(m_conn);
	if (res == nullptr)
		return FALSE;

	MYSQL_ROW row;
	if ((row = mysql_fetch_row(res)) && row[4]) // Column_name находится в 5-й колонке (индекс 4)
	{
		primaryKeyColumn = ConvertFromUTF8(row[4]);
		mysql_free_result(res);
		return TRUE;
	}

	mysql_free_result(res);
	return FALSE;
}

// Обновление записи в таблице
BOOL CMySQLDoc::UpdateTableRecord(const CString& tableName, const CStringArray& columnNames, 
	const CStringArray& values, const CString& primaryKeyColumn, const CString& primaryKeyValue)
{
	if (m_conn == nullptr)
		return FALSE;

	if (columnNames.GetSize() != values.GetSize() || columnNames.GetSize() == 0)
		return FALSE;

	// Формируем SET часть UPDATE запроса
	CString setClause;
	for (int i = 0; i < columnNames.GetSize(); i++)
	{
		if (i > 0)
			setClause += _T(", ");
		
		// Экранируем специальные символы для SQL
		CString escapedValue = values[i];
		escapedValue.Replace(_T("'"), _T("''")); 
		
		setClause += _T("`");
		setClause += columnNames[i];
		setClause += _T("` = '");
		setClause += escapedValue;
		setClause += _T("'");
	}

	// Формируем WHERE часть
	CString escapedKeyValue = primaryKeyValue;
	escapedKeyValue.Replace(_T("'"), _T("''"));

	CString sqlUnicode;
	sqlUnicode.Format(_T("UPDATE `%s` SET %s WHERE `%s` = '%s'"), tableName, setClause, primaryKeyColumn, escapedKeyValue);

	CStringA sql = ConvertToUTF8(sqlUnicode);
	
	if (mysql_query(m_conn, sql))
	{
		return FALSE;
	}

	return TRUE;
}

void CMySQLDoc::OnConnectDatabase()
{
	CConnectDbDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// Подключение выполняется в диалоге
	}
}

// Фильтрация таблицы по ID
BOOL CMySQLDoc::FilterTableById(const CString& tableName, const CString& idValue, CString& result)
{
	result.Empty();
	
	if (m_conn == nullptr)
	{
		result = _T("Ошибка: нет подключения к базе данных");
		return FALSE;
	}
	
	// Получаем первичный ключ таблицы
	CString primaryKeyColumn;
	if (!GetPrimaryKeyColumn(tableName, primaryKeyColumn))
	{
		result = _T("Ошибка: не удалось определить первичный ключ таблицы");
		return FALSE;
	}
	
	// Экранируем значение ID для безопасности
	CStringA idValueA = ConvertToUTF8(idValue);
	char escapedId[1024];
	unsigned long escapedLen = mysql_real_escape_string(m_conn, escapedId, idValueA, (unsigned long)idValueA.GetLength());
	
	CStringA escapedIdValue;
	escapedIdValue.SetString(escapedId, (int)escapedLen);
	CString escapedIdUnicode = ConvertFromUTF8(escapedId);
	
	// Формируем SQL запрос для фильтрации по ID
	CString sqlUnicode;
	sqlUnicode.Format(_T("SELECT * FROM `%s` WHERE `%s` = '%s'"), tableName, primaryKeyColumn, escapedIdUnicode);
	
	CStringA sql = ConvertToUTF8(sqlUnicode);
	return ExecuteSelectQuery(sql, result);
}

// Обработчик команды фильтрации по ID
void CMySQLDoc::OnFilterById()
{
	if (m_conn == nullptr)
	{
		AfxMessageBox(_T("Нет подключения к базе данных!"));
		return;
	}
	
	// Создаем и показываем диалог фильтрации
	CFilterByIdDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_tableName.IsEmpty() || dlg.m_idValue.IsEmpty())
		{
			AfxMessageBox(_T("Заполните все поля!"));
			return;
		}
		
		// Выполняем фильтрацию
		CString result;
		if (FilterTableById(dlg.m_tableName, dlg.m_idValue, result))
		{
			// Получаем представление ListView и отображаем результат
			POSITION pos = GetFirstViewPosition();
			while (pos != NULL)
			{
				CView* pView = GetNextView(pos);
				CMyListView* pListView = DYNAMIC_DOWNCAST(CMyListView, pView);
				if (pListView != nullptr)
				{
					pListView->DisplayQueryResult(result);
					break;
				}
			}
		}
		else
		{
			AfxMessageBox(_T("Ошибка выполнения фильтрации!\r\n") + result);
		}
	}
}
