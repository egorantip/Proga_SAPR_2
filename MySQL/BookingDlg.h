#pragma once

// CBookingDlg диалоговое окно

class CBookingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBookingDlg)

public:
	CBookingDlg(CWnd* pParent = nullptr);   // стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CString m_country;           // Страна назначения
	CString m_cityFrom;          // Город отправления
	CString m_cityTo;            // Город назначения
	CString m_seatType;          // Тип места в самолете
	COleDateTime m_departureDate; // Дата вылета
	COleDateTime m_returnDate;    // Дата возвращения
	int m_adults;                // Количество взрослых
	int m_children;              // Количество детей
	double m_totalPrice;         // Итоговая цена
	
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCountry();
	afx_msg void OnCbnSelchangeCityFrom();
	afx_msg void OnCbnSelchangeCityTo();
	afx_msg void OnCbnSelchangeSeatType();
	afx_msg void OnDtnDatetimechangeDepartureDate();
	afx_msg void OnDtnDatetimechangeReturnDate();
	afx_msg void OnEnChangeAdults();
	afx_msg void OnEnChangeChildren();
	afx_msg void OnBnClickedOk();
	
private:
	void UpdateCities();         // Обновление списка городов при изменении страны
	void CalculatePrice();      // Расчет итоговой цены
	void UpdatePriceDisplay();   // Обновление отображения цены
};

