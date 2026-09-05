
// MySQLDoc.h: интерфейс класса CMySQLDoc 
//


#pragma once


// Предварительное объявление
class CConnectDbDlg;

class CMySQLDoc : public CDocument
{
protected: // создать только из сериализации
	CMySQLDoc() noexcept;
	virtual ~CMySQLDoc();
	DECLARE_DYNCREATE(CMySQLDoc)
	
	// Загрузка и сохранение параметров подключения
	void LoadConnectionParams();
	void SaveConnectionParams();

// Атрибуты
public:
	MYSQL* m_conn; // Указатель на подключение к MySQL
	
	// Параметры подключения к БД
	CString m_dbHost;
	int m_dbPort;
	CString m_dbUser;
	CString m_dbPassword;
	CString m_dbName;

// Операции
public:
	// Подключение к базе данных
	BOOL ConnectToDatabase();
	
	// Подключение к базе данных с параметрами
	BOOL ConnectToDatabase(const CString& host, int port, const CString& user, 
		const CString& password, const CString& database);
	
	// Закрытие подключения
	void CloseConnection();
	
	// Методы для получения структуры БД
	// Получить список всех таблиц
	BOOL GetTablesList(CStringArray& tables);
	
	// Получить список колонок таблицы
	BOOL GetTableColumns(const CString& tableName, CStringArray& columns);
	
	// Обновление записи в таблице
	BOOL UpdateTableRecord(const CString& tableName, const CStringArray& columnNames, 
		const CStringArray& values, const CString& primaryKeyColumn, const CString& primaryKeyValue);
	
	// Получить первичный ключ таблицы
	BOOL GetPrimaryKeyColumn(const CString& tableName, CString& primaryKeyColumn);
	
	// Фильтрация по ID
	BOOL FilterTableById(const CString& tableName, const CString& idValue, CString& result);
	
	// Преобразование Unicode строки в UTF-8 для MySQL (публичный для использования в представлениях)
	CStringA ConvertToUTF8(const CString& str);
	
	// Преобразование UTF-8 строки в Unicode для отображения (публичный для использования в представлениях)
	CString ConvertFromUTF8(const char* utf8str);

private:
	// Вспомогательный метод для выполнения SELECT запроса и формирования результата
	BOOL ExecuteSelectQuery(const CStringA& sql, CString& result);

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
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnConnectDatabase();
	afx_msg void OnFilterById();
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
