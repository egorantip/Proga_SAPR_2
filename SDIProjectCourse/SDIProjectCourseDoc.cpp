
// SDIProjectCourseDoc.cpp: реализация класса CSDIProjectCourseDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SDIProjectCourse.h"
#endif

#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants.h"
#include "C:\Program Files\ASCON\KOMPAS-3D v23 Study\SDK\Include\ksConstants3D.h"

#include "SDIProjectCourseDoc.h"
#include "CMyTreeView.h"
#include <propkey.h>
#include "MainFrm.h"
#include "afxdialogex.h"
#include "CAssembleDlg.h"
#include "CSalnikNestDlg.h"
#include "cmath"

#define PI 4*atan(1)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Kompas6API5;
//using namespace KompasAPI5;

// CSDIProjectCourseDoc

IMPLEMENT_DYNCREATE(CSDIProjectCourseDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIProjectCourseDoc, CDocument)
	ON_COMMAND(IDD_CAssembleDlg, &CSDIProjectCourseDoc::OnIddCAssembleDlg)
	ON_COMMAND(IDD_CSalnikNestDlg, &CSDIProjectCourseDoc::OnIddCSalnikNestDlg)
END_MESSAGE_MAP()


// Создание или уничтожение CSDIProjectCourseDoc

CSDIProjectCourseDoc::CSDIProjectCourseDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора
	m_nSelectedItem = 0;
	m_AssemblyType = 1;
	m_pTreeView = nullptr;
	m_pView = nullptr;
	
	// Инициализация параметров по умолчанию
	m_Salnik_D = 21.9;
	m_Salnik_L = 61;
	m_ThreadD = 16;
	m_ThreadP = 1;
	m_Gayka_d = 10;
	m_Gayka_D = 21.9;
	m_Gayka_L = 14;
	m_Gayka_S = 19;
	m_Shayba_D = 14;
	m_Shayba_d = 8;
	m_Shayba_S = 1.4;

	// Картинка по умолчанию – общая сборка
	m_pictureToDraw = IDB_ASSEMBLE;
}

CSDIProjectCourseDoc::~CSDIProjectCourseDoc()
{
}

void CSDIProjectCourseDoc::SetPicture(int pictureId)
{
	switch (pictureId)
	{
	case 1:
		m_pictureToDraw = IDB_SALNIK;
		break;
	case 2:
		m_pictureToDraw = IDB_GAYKA;
		break;
	case 3:
		m_pictureToDraw = IDB_SHAYBA;
		break;
	default:
		m_pictureToDraw = IDB_ASSEMBLE;
		break;
	}

	// Обновляем представления, чтобы перерисовать картинку
	UpdateAllViews(nullptr);
}

BOOL CSDIProjectCourseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CWnd* MainWnd = AfxGetMainWnd();

	this->SetTitle(L"Компас 3Д");
	m_pTreeView->FillTree();

	return TRUE;
}




// Сериализация CSDIProjectCourseDoc

void CSDIProjectCourseDoc::Serialize(CArchive& ar)
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
void CSDIProjectCourseDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CSDIProjectCourseDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CSDIProjectCourseDoc::SetSearchContent(const CString& value)
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

// Диагностика CSDIProjectCourseDoc

#ifdef _DEBUG
void CSDIProjectCourseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIProjectCourseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CSDIProjectCourseDoc

void CSDIProjectCourseDoc::LaunchKompas()
{
	pKompasAppUnk = nullptr;
	if (!pKompasApp5)
	{
		// Получаем CLSID для Компас
		CLSID InvAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &InvAppClsid);
		if (FAILED(hRes)) {
			pKompasApp5 = nullptr;
			AfxMessageBox(L"Kompass dont run, fail(");
			return;
		}

		// Проверяем есть ли запущенный экземпляр Компас
		//если есть получаем IUnknown
		hRes = ::GetActiveObject(InvAppClsid, NULL, &pKompasAppUnk);
		if (FAILED(hRes)) {
			// Приходится запускать Компас самим так как работающего нет
			// Также получаем IUnknown для только что запущенного приложения Компас
			TRACE(L"Could not get hold of an active Inventor, will start a new session\n");
			hRes = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnk);
			if (FAILED(hRes)) {
				pKompasApp5 = nullptr;
				AfxMessageBox(L"Kompass dont run, fail(");
				return;
			}
		}

		// Получаем интерфейс приложения Компас
		hRes = pKompasAppUnk->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes)) {
			AfxMessageBox(L"Kompass dont run, fail(");
			return;
		}
	}

	// делаем Компас видимым
	pKompasApp5->Visible = true;
	launched = true;

	CreateAssembly();
}

bool CSDIProjectCourseDoc::CheckData()
{
	// Проверка данных документа
	// UpdateData() - это метод диалогового окна, не документа
	// Здесь можно добавить проверку корректности параметров
	if (m_AssemblyType < 1 || m_AssemblyType > 6)
		return false;

	return true;
}

void CSDIProjectCourseDoc::SetParams()
{
	switch (m_AssemblyType) {
		case 1: // СКСД16
		{
			m_Salnik_D = 21.9;
			m_Salnik_L = 61;
			m_ThreadD = 16;
			m_ThreadP = 1;
			m_Gayka_d = 10;
			m_Gayka_D = 21.9;
			m_Gayka_L = 14;
			m_Gayka_S = 19;
			m_Shayba_D = 16;
			m_Shayba_d = 8;
			m_Shayba_S = 1.4;
			break;
		}
		case 2: // СКСД27
		{
			m_Salnik_D = 36.9;
			m_Salnik_L = 86;
			m_ThreadD = 27;
			m_ThreadP = 1.5;
			m_Gayka_d = 20;
			m_Gayka_D = 36.9;
			m_Gayka_L = 24;
			m_Gayka_S = 32;
			m_Shayba_D = 27;
			m_Shayba_d = 16;
			m_Shayba_S = 2.0;
			break;
		}
		case 3: // СКСД42
		{
			m_Salnik_D = 53.0;
			m_Salnik_L = 115;
			m_ThreadD = 42;
			m_ThreadP = 2;
			m_Gayka_d = 32;
			m_Gayka_D = 53.0;
			m_Gayka_L = 38;
			m_Gayka_S = 46;
			m_Shayba_D = 42;
			m_Shayba_d = 28;
			m_Shayba_S = 2.0;
			break;
		}
		case 4: // СКСД60
		{
			m_Salnik_D = 75.0;
			m_Salnik_L = 131;
			m_ThreadD = 60;
			m_ThreadP = 3;
			m_Gayka_d = 45;
			m_Gayka_D = 75.0;
			m_Gayka_L = 55;
			m_Gayka_S = 65;
			m_Shayba_D = 60;
			m_Shayba_d = 40;
			m_Shayba_S = 3.0;
			break;
		}
		case 5: // СКСД76
		{
			m_Salnik_D = 92.4;
			m_Salnik_L = 142;
			m_ThreadD = 76;
			m_ThreadP = 3;
			m_Gayka_d = 60;
			m_Gayka_D = 92.4;
			m_Gayka_L = 71;
			m_Gayka_S = 80;
			m_Shayba_D = 76;
			m_Shayba_d = 56;
			m_Shayba_S = 3.0;
			break;
		}
		case 6: // СКСД90
		{
			m_Salnik_D = 103.9;
			m_Salnik_L = 147;
			m_ThreadD = 90;
			m_ThreadP = 3;
			m_Gayka_d = 80;
			m_Gayka_D = 103.9;
			m_Gayka_L = 84;
			m_Gayka_S = 90;
			m_Shayba_D = 90;
			m_Shayba_d = 64;
			m_Shayba_S = 3.0;
			break;
		}
		default:
		{
			m_Salnik_D = 21.9;
			m_Salnik_L = 61;
			m_ThreadD = 16;
			m_ThreadP = 1;
			m_Gayka_d = 10;
			m_Gayka_D = 21.9;
			m_Gayka_L = 14;
			m_Gayka_S = 19;
			m_Shayba_D = 16;
			m_Shayba_d = 8;
			m_Shayba_S = 1.4;
			break;
		}
	}
}

ksEntityPtr m_pSalnikThread;     // резьба сальника
ksEntityPtr m_pGaykaThread;      // внешняя резьба гайки
ksEntityPtr m_pGaykaHexBoss;     // выдавливание шестигранника гайки
ksEntityPtr m_pShaybaExtrude;    // выдавливание шайбы

// Построение сальника
bool CSDIProjectCourseDoc::CreateSalnik(ksPartPtr pPart, double D, double L, double threadP, double threadD)
{
	if (!pPart)
		return false;

	ksEntityPtr pPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);
	if (!pPlaneXOY)
		return false;

	// Корпус
	ksEntityPtr pSketchBody = pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchBodyDef = pSketchBody->GetDefinition();
	pSketchBodyDef->SetPlane(pPlaneXOY);
	if (!pSketchBody->Create())
		return false;

	ksDocument2DPtr pDocBody = pSketchBodyDef->BeginEdit();
	pDocBody->ksCircle(0, 0, D / 2.0, 1);
	pSketchBodyDef->EndEdit();

	ksEntityPtr pBoss = pPart->NewEntity(o3d_bossExtrusion);
	ksBossExtrusionDefinitionPtr pBossDef = pBoss->GetDefinition();
	pBossDef->SetSketch(pSketchBody);
	pBossDef->SetSideParam(TRUE, etBlind, L / 2, 0, FALSE);
	if (!pBoss->Create())
		return false;

	// Отверстие
	ksEntityPtr pSketchHole = pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchHoleDef = pSketchHole->GetDefinition();
	pSketchHoleDef->SetPlane(pPlaneXOY);
	if (!pSketchHole->Create())
		return false;

	ksDocument2DPtr pDocHole = pSketchHoleDef->BeginEdit();
	pDocHole->ksCircle(0, 0, threadD / 2.0, 1);
	pSketchHoleDef->EndEdit();

	ksEntityPtr pCut = pPart->NewEntity(o3d_cutExtrusion);
	ksCutExtrusionDefinitionPtr pCutDef = pCut->GetDefinition();
	pCutDef->SetSketch(pSketchHole);
	pCutDef->SetSideParam(TRUE, etThroughAll, 0, 0, FALSE);
	pCutDef->SetSideParam(FALSE, etThroughAll, 0, 0, FALSE);
	if (!pCut->Create())
		return false;

	// Поиск цилиндрической грани отверстия
	ksEntityPtr pCylindricalFace = nullptr;
	ksEntityCollectionPtr faces = pPart->EntityCollection(o3d_face);

	for (int i = 0; i < faces->GetCount(); i++)
	{
		ksEntityPtr face = faces->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (!def)
			continue;

		// Грань должна принадлежать операции выреза
		if (def->GetOwnerEntity() != pCut)
			continue;

		IDispatchPtr pSurfaceDisp = def->GetSurface();
		if (!pSurfaceDisp)
			continue;

		ksSurfacePtr surface;
		if (FAILED(pSurfaceDisp->QueryInterface(__uuidof(ksSurface), (void**)&surface)))
			continue;

		if (surface && surface->IsCylinder())
		{
			pCylindricalFace = face;
			break;
		}
	}

	if (!pCylindricalFace)
	{
		AfxMessageBox(L"Цилиндрическая грань отверстия не найдена");
		return false;
	}

	// Создание внутренней резьбы
	ksEntityPtr pThread = pPart->NewEntity(o3d_thread);
	if (!pThread)
		return false;

	ksThreadDefinitionPtr pThreadDef = pThread->GetDefinition();
	if (!pThreadDef)
		return false;

	// Базовая цилиндрическая грань
	pThreadDef->SetBaseObject(pCylindricalFace);

	// Параметры резьбы
	pThreadDef->Putdr(threadD);          // номинальный диаметр
	pThreadDef->Putp(threadP);               // шаг (ЗАДАЙ НУЖНЫЙ)
	pThreadDef->Putlength(L / 2.0);      // длина резьбы
	pThreadDef->PutallLength(FALSE);     // не на всю длину
	pThreadDef->PutautoDefinDr(FALSE);   // диаметр вручную

	if (!pThread->Create())
	{
		AfxMessageBox(L"Не удалось создать резьбу");
		return false;
	}

	// Сохраняем операцию резьбы
	m_pSalnikThread = pThread;

	// Фаска
	double faska = L * 0.03;

	ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);
	if (!pChamfer)
		return false;

	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	if (!pChamferDef)
		return false;

	pChamferDef->SetChamferParam(true, faska, faska);

	ksEntityCollectionPtr fl = pChamferDef->array();
	fl->Clear();

	ksEntityCollectionPtr flEdges = pPart->EntityCollection(o3d_edge);

	int circleCountSalnik = 0;

	for (int i = 0; i < flEdges->GetCount(); i++)
	{
		ksEntityPtr ed = flEdges->GetByIndex(i);
		ksEdgeDefinitionPtr def = ed->GetDefinition();
		if (!def)
			continue;

		if (def->GetOwnerEntity() != pBoss)
			continue;

		if (!def->IsCircle())
			continue;

		circleCountSalnik++;

		if (circleCountSalnik == 1 || circleCountSalnik == 2)
		{
			fl->Add(ed);
		}
		else break;
	}

	if (!pChamfer->Create())
	{
		AfxMessageBox(L"Ошибка создания фаски");
		return false;
	}

	pChamfer->Create();
	fl->Clear();

	// Именование граней гнезда Сальника
	ksEntityCollectionPtr facesForNaming = pPart->EntityCollection(o3d_face);

	// 1. Внешняя цилиндрическая поверхность корпуса
	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (def && def->IsCylinder())
		{
			double h, r;
			def->GetCylinderParam(&h, &r);
			if (abs(r - D / 2.0) < 0.1)
			{
				face->Putname(L"OuterCylSalnik");
				face->Update();
			}
		}
	}

	// 2. Торцевые плоскости (по Z, так как выдавливание вдоль Z)
	double minZ = 1e9, maxZ = -1e9;
	ksEntityPtr frontPlane = nullptr, backPlane = nullptr;

	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (def && def->IsPlanar())
		{
			ksEdgeCollectionPtr edges = def->EdgeCollection();
			if (edges->GetCount() > 0)
			{
				ksEdgeDefinitionPtr edge = edges->GetByIndex(0);
				ksVertexDefinitionPtr v = edge->GetVertex(true);
				if (v)
				{
					double x, y, z;
					v->GetPoint(&x, &y, &z);
					if (z < minZ) { minZ = z; frontPlane = face; }
					if (z > maxZ) { maxZ = z; backPlane = face; }
				}
			}
		}
	}

	if (frontPlane)
	{
		frontPlane->Putname(L"FrontSalnikPlane");
		frontPlane->Update();
	}
	if (backPlane)
	{
		backPlane->Putname(L"BackSalnikPlane");
		backPlane->Update();
	}

	// 3. Внутренняя цилиндрическая поверхность с резьбой (уже найдена как pCylindricalFace)
	pCylindricalFace->Putname(L"ThreadedHoleSalnik");
	pCylindricalFace->Update();

	return true;
}


// Построение гайки
bool CSDIProjectCourseDoc::CreateGayka(ksPartPtr pPart, double d, double L, double D, double threadP, double threadD)
{
	if (!pPart)
		return false;

	// Создаем эскиз для шестигранника
	ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);
	if (!pSketch)
		return false;

	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
	if (!pSketchDef)
		return false;

	ksEntityPtr pPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);
	if (!pPlaneXOY)
		return false;

	pSketchDef->SetPlane(pPlaneXOY);
	if (!pSketch->Create())
		return false;

	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();
	if (!p2DDoc)
		return false;

	// Строим правильный шестигранник
	double R = D / 2.0;
	double angle60 = PI / 3.0;

	double hexPoints[7][2];
	for (int i = 0; i < 6; i++)
	{
		double angle = i * angle60;
		hexPoints[i][0] = R * cos(angle);
		hexPoints[i][1] = R * sin(angle);
	}
	hexPoints[6][0] = hexPoints[0][0];
	hexPoints[6][1] = hexPoints[0][1];

	// Рисуем шестигранник
	for (int i = 0; i < 6; i++)
	{
		p2DDoc->ksLineSeg(hexPoints[i][0], hexPoints[i][1], hexPoints[i + 1][0], hexPoints[i + 1][1], 1);
	}

	// Ось
	p2DDoc->ksLineSeg(-D, 0, D, 0, 3);

	pSketchDef->EndEdit();

	// Выдавлавание шестигранника
	ksEntityPtr pExtrude = pPart->NewEntity(o3d_bossExtrusion);
	if (!pExtrude)
		return false;

	ksBossExtrusionDefinitionPtr pExDef = pExtrude->GetDefinition();
	if (!pExDef)
		return false;

	pExDef->SetSketch(pSketch);
	pExDef->directionType = dtNormal;
	pExDef->SetSideParam(true, etBlind, L * 0.3, 0, false);
	if (!pExtrude->Create())
		return false;

	// Создаем средний цилиндр (от проточки)
	ksEntityPtr pSketchMidCylinder = pPart->NewEntity(o3d_sketch);
	if (!pSketchMidCylinder)
		return false;

	ksSketchDefinitionPtr pSketchMidCylinderDef = pSketchMidCylinder->GetDefinition();
	if (!pSketchMidCylinderDef)
		return false;

	pSketchMidCylinderDef->SetPlane(pPlaneXOY);
	if (!pSketchMidCylinder->Create())
		return false;

	ksDocument2DPtr pDocMid = pSketchMidCylinderDef->BeginEdit();
	if (!pDocMid)
		return false;

	pDocMid->ksCircle(0, 0, (d / 2.0) * 1.2, 1);

	pSketchMidCylinderDef->EndEdit();

	// Выдавливание
	ksEntityPtr pBossMid = pPart->NewEntity(o3d_bossExtrusion);
	if (!pBossMid)
		return false;

	ksBossExtrusionDefinitionPtr pBossMidDef = pBossMid->GetDefinition();
	if (!pBossMidDef)
		return false;

	pBossMidDef->SetSketch(pSketchMidCylinder);

	pBossMidDef->SetSideParam(TRUE, etBlind, L * 0.1 + L * 0.3, 0, FALSE);

	if (!pBossMid->Create())
		return false;

	ksEntityCollectionPtr faces = pPart->EntityCollection(o3d_face);
	if (!faces)
		return false;

	// Создание нижней части с отверстием
	ksEntityPtr pOffsetPlane = pPart->NewEntity(o3d_planeOffset);
	if (!pOffsetPlane)
		return false;

	ksPlaneOffsetDefinitionPtr pOffsetDef = pOffsetPlane->GetDefinition();
	if (!pOffsetDef)
		return false;

	pOffsetDef->SetPlane(pPlaneXOY);

	pOffsetDef->offset = L * 0.3 + L * 0.1;

	if (!pOffsetPlane->Create())
		return false;

	ksEntityPtr pSketchDown = pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchDownDef = pSketchDown->GetDefinition();

	pSketchDownDef->SetPlane(pOffsetPlane);
	pSketchDown->Create();

	ksDocument2DPtr pDocDown = pSketchDownDef->BeginEdit();
	pDocDown->ksCircle(0, 0, threadD / 2.0, 1);
	pSketchDownDef->EndEdit();

	// Выдавливание
	ksEntityPtr pBossDown = pPart->NewEntity(o3d_bossExtrusion);
	if (!pBossDown)
		return false;

	ksBossExtrusionDefinitionPtr pBossDownDef = pBossDown->GetDefinition();
	if (!pBossDownDef)
		return false;

	pBossDownDef->SetSketch(pSketchDown);

	pBossDownDef->directionType = dtNormal;

	pBossDownDef->SetSideParam(TRUE, etBlind, L * 0.6, 0, FALSE);

	if (!pBossDown->Create())
		return false;

	// Фаска
	double faska = L*0.05;

	ksEntityPtr pChamfer = pPart->NewEntity(o3d_chamfer);
	if (!pChamfer) return false;

	ksChamferDefinitionPtr pChamferDef = pChamfer->GetDefinition();
	if (!pChamferDef) return false;

	pChamferDef->SetChamferParam(true, faska, faska);
	ksEntityCollectionPtr fl = pChamferDef->array();
	fl->Clear();

	ksEntityCollectionPtr flEdges = pPart->EntityCollection(o3d_edge);

	ksEntityPtr targetEdge = nullptr;
	double MaxZ = -1e9;

	for (int i = 0; i < flEdges->GetCount(); i++)
	{
		ksEntityPtr ed = flEdges->GetByIndex(i);
		ksEdgeDefinitionPtr def = ed->GetDefinition();
		if (!def) continue;

		// Только рёбра резьбового цилиндра (pBossDown)
		if (def->GetOwnerEntity() != pBossDown)
			continue;

		if (!def->IsCircle())
			continue;

		// Получаем центр окружности
		IDispatchPtr pCurve3D = def->GetCurve3D();
		if (!pCurve3D) continue;

		ksCurve3DPtr curve3D;
		if (FAILED(pCurve3D->QueryInterface(__uuidof(ksCurve3D), (void**)&curve3D)) || !curve3D)
			continue;

		if (!curve3D->IsCircle()) continue;

		IDispatchPtr pParam = curve3D->GetCurveParam();
		if (!pParam) continue;

		ksCircle3dParamPtr circleParam;
		if (FAILED(pParam->QueryInterface(__uuidof(ksCircle3dParam), (void**)&circleParam)) || !circleParam)
			continue;

		IDispatchPtr pPlacement = circleParam->GetPlacement();
		if (!pPlacement) continue;

		ksPlacementPtr placement;
		if (FAILED(pPlacement->QueryInterface(__uuidof(ksPlacement), (void**)&placement)) || !placement)
			continue;

		double x, y, z;
		if (placement->GetOrigin(&x, &y, &z))
		{
			if (z > MaxZ)
			{
				MaxZ = z;
				targetEdge = ed;
			}
		}
	}

	if (targetEdge)
	{
		fl->Add(targetEdge);
		if (!pChamfer->Create())
		{
			AfxMessageBox(L"Ошибка создания фаски");
			return false;
		}
	}
	else
	{
		AfxMessageBox(L"Не найдено подходящее ребро для фаски");
		return false;
	}

	// Поиск внешней цилиндрической грани нижнего цилиндра
	ksEntityPtr pOuterCylFace = nullptr;
	ksEntityCollectionPtr facesAll = pPart->EntityCollection(o3d_face);

	for (int i = 0; i < facesAll->GetCount(); i++)
	{
		ksEntityPtr face = facesAll->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (!def)
			continue;

		// Только грани нижнего цилиндра
		if (def->GetOwnerEntity() != pBossDown)
			continue;

		IDispatchPtr pSurfaceDisp = def->GetSurface();
		if (!pSurfaceDisp)
			continue;

		ksSurfacePtr surface;
		if (FAILED(pSurfaceDisp->QueryInterface(__uuidof(ksSurface), (void**)&surface)))
			continue;

		if (surface && surface->IsCylinder())
		{
			pOuterCylFace = face;
			break;
		}
	}

	if (!pOuterCylFace)
	{
		AfxMessageBox(L"Внешняя цилиндрическая грань не найдена");
		return false;
	}

	// Создание внешней резьбы
	ksEntityPtr pThread = pPart->NewEntity(o3d_thread);
	if (!pThread)
		return false;

	ksThreadDefinitionPtr pThreadDef = pThread->GetDefinition();
	if (!pThreadDef)
		return false;

	// Базовая грань
	pThreadDef->SetBaseObject(pOuterCylFace);

	// Параметры резьбы
	pThreadDef->Putdr(threadD);          // номинальный диаметр
	pThreadDef->Putp(threadP);               // шаг (задай нужный)
	pThreadDef->Putlength(L * 0.6);      // длина резьбы
	pThreadDef->PutallLength(FALSE);
	pThreadDef->PutautoDefinDr(FALSE);

	if (!pThread->Create())
	{
		AfxMessageBox(L"Ошибка создания резьбы");
		return false;
	}

	m_pGaykaHexBoss = pExtrude; // или какое у вас имя переменной для шестигранника

	m_pGaykaThread = pThread; // внешняя резьба на нижнем цилиндре

	// Создание отверстия
	ksEntityPtr pOffsetPlaneHole = pPart->NewEntity(o3d_planeOffset);
	if (!pOffsetPlaneHole)
		return false;

	ksPlaneOffsetDefinitionPtr pOffsetDefHole = pOffsetPlaneHole->GetDefinition();
	if (!pOffsetDefHole)
		return false;

	pOffsetDefHole->SetPlane(pPlaneXOY);

	pOffsetDefHole->offset = L * 0.2;

	if (!pOffsetPlaneHole->Create())
		return false;

	ksEntityPtr pSketchHole = pPart->NewEntity(o3d_sketch);
	ksSketchDefinitionPtr pSketchHoleDef = pSketchHole->GetDefinition();

	pSketchHoleDef->SetPlane(pOffsetPlaneHole);
	pSketchHole->Create();

	ksDocument2DPtr pDocHole = pSketchHoleDef->BeginEdit();
	pDocHole->ksCircle(0, 0, d / 2.0, 1);
	pSketchHoleDef->EndEdit();

	// Вырез выдавливанием
	ksEntityPtr pCutHole = pPart->NewEntity(o3d_cutExtrusion);
	if (!pCutHole)
		return false;

	ksCutExtrusionDefinitionPtr pCutHoleDef = pCutHole->GetDefinition();
	if (!pCutHoleDef)
		return false;

	pCutHoleDef->SetSketch(pSketchHole);
	pCutHoleDef->directionType = dtReverse;
	pCutHoleDef->SetSideParam(FALSE, etBlind, L * 0.8, 0, FALSE);

	if (!pCutHole->Create())
		return false;

	// Именование граней Гайки
	ksEntityCollectionPtr facesForNaming = pPart->EntityCollection(o3d_face);

	// 1. Внешняя цилиндрическая поверхность с резьбой (уже найдена как pOuterCylFace)
	pOuterCylFace->Putname(L"ThreadedCylGayka");
	pOuterCylFace->Update();

	ksEntityPtr threadOuterCyl = nullptr;
	ksEntityPtr threadBasePlane = nullptr;  // у основания (ближе к шестиграннику)
	ksEntityPtr threadEndPlane = nullptr;   // у фаски (дальше)

	double baseZ = 1e9;   // минимальная Z — основание
	double endZ = -1e9;   // максимальная Z — торец

	// Цилиндрическая грань и плоские грани
	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (!def) continue;

		// Только грани цилиндра с резьбой
		if (def->GetOwnerEntity() != pBossDown)
			continue;

		if (def->IsCylinder())
		{
			// Внешняя цилиндрическая поверхность
			threadOuterCyl = face;
		}
		else if (def->IsPlanar())
		{
			// Определяем Z-координату грани
			ksEdgeCollectionPtr edges = def->EdgeCollection();
			if (edges->GetCount() > 0)
			{
				ksEdgeDefinitionPtr edge = edges->GetByIndex(0);
				ksVertexDefinitionPtr v = edge->GetVertex(true);
				if (v)
				{
					double x, y, z;
					v->GetPoint(&x, &y, &z);
					if (z < baseZ) { baseZ = z; threadBasePlane = face; }
					if (z > endZ) { endZ = z; threadEndPlane = face; }
				}
			}
		}
	}

	// Присваиваем имена
	if (threadOuterCyl)
	{
		threadOuterCyl->Putname(L"ThreadOuterCylGayka");
		threadOuterCyl->Update();
	}

	if (threadBasePlane)
	{
		threadBasePlane->Putname(L"ThreadBasePlaneGayka");
		threadBasePlane->Update();
	}

	if (threadEndPlane)
	{
		threadEndPlane->Putname(L"ThreadEndPlaneGayka");
		threadEndPlane->Update();
	}

	return true;
}

// Построение шайбы
bool CSDIProjectCourseDoc::CreateShayba(ksPartPtr pPart, double D, double d, double S)
{
	if (!pPart)
		return false;

	// Создаем эскиз для шайбы
	ksEntityPtr pSketch = pPart->NewEntity(o3d_sketch);
	if (!pSketch)
		return false;

	ksSketchDefinitionPtr pSketchDef = pSketch->GetDefinition();
	if (!pSketchDef)
		return false;

	ksEntityPtr pPlaneXOY = pPart->GetDefaultEntity(o3d_planeXOY);
	if (!pPlaneXOY)
		return false;

	pSketchDef->SetPlane(pPlaneXOY);
	if (!pSketch->Create())
		return false;

	ksDocument2DPtr p2DDoc = pSketchDef->BeginEdit();
	if (!p2DDoc)
		return false;

	// Рисуем внешний круг
	p2DDoc->ksCircle(0, 0, D / 2.0, 1);
	// Рисуем внутренний круг (отверстие)
	p2DDoc->ksCircle(0, 0, d / 2.0, 1);

	// Ось вращения
	p2DDoc->ksLineSeg(-10, 0, 10, 0, 3);

	pSketchDef->EndEdit();

	// Создаем операцию выдавливания
	ksEntityPtr pExtrude = pPart->NewEntity(o3d_bossExtrusion);
	if (!pExtrude)
		return false;

	ksBossExtrusionDefinitionPtr pExDef = pExtrude->GetDefinition();
	if (!pExDef)
		return false;

	pExDef->SetSketch(pSketch);
	pExDef->directionType = dtNormal;
	pExDef->SetSideParam(true, etBlind, S, 0, false);
	if (!pExtrude->Create())
		return false;

	// Именование граней шайбы
	ksEntityCollectionPtr facesForNaming = pPart->EntityCollection(o3d_face);

	// 1. Внешняя цилиндрическая поверхность
	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (def && def->IsCylinder())
		{
			double h, r;
			def->GetCylinderParam(&h, &r);
			if (abs(r - D / 2.0) < 0.1)
			{
				face->Putname(L"OuterCylShayba");
				face->Update();
			}
		}
	}

	// 2. Внутренняя цилиндрическая поверхность (отверстие)
	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (def && def->IsCylinder())
		{
			double h, r;
			def->GetCylinderParam(&h, &r);
			if (abs(r - d / 2.0) < 0.1)
			{
				face->Putname(L"InnerHoleShayba");
				face->Update();
			}
		}
	}

	// 3. Нижняя плоскость (по Z)
	ksEntityPtr bottomPlane = nullptr;
	double minZ = 1e9;
	for (int i = 0; i < facesForNaming->GetCount(); i++)
	{
		ksEntityPtr face = facesForNaming->GetByIndex(i);
		ksFaceDefinitionPtr def = face->GetDefinition();
		if (def && def->IsPlanar())
		{
			ksEdgeCollectionPtr edges = def->EdgeCollection();
			if (edges->GetCount() > 0)
			{
				ksEdgeDefinitionPtr edge = edges->GetByIndex(0);
				ksVertexDefinitionPtr v = edge->GetVertex(true);
				if (v)
				{
					double x, y, z;
					v->GetPoint(&x, &y, &z);
					if (z < minZ) { minZ = z; bottomPlane = face; }
				}
			}
		}
	}
	if (bottomPlane)
	{
		bottomPlane->Putname(L"BottomShaybaPlane");
		bottomPlane->Update();
	}

	return true;
}

void CSDIProjectCourseDoc::CreateAssembly()
{
	BeginWaitCursor();

	if (!CheckData())
	{
		EndWaitCursor();
		return;
	}

	CComPtr<IUnknown> pKompasAppUnkLocal = nullptr;
	if (!pKompasApp5)
	{
		CLSID KompasAppClsid;
		HRESULT hRes = CLSIDFromProgID(L"Kompas.Application.5", &KompasAppClsid);
		if (FAILED(hRes)) {
			AfxMessageBox(_T("Ошибка: Не удалось получить CLSID для Kompas-3D. Убедитесь, что Kompas-3D установлен."));
			EndWaitCursor();
			return;
		}

		hRes = ::GetActiveObject(KompasAppClsid, NULL, &pKompasAppUnkLocal);
		if (FAILED(hRes)) {
			TRACE(L"Could not get hold of an active Inventor, will start a new session\n");
			hRes = CoCreateInstance(KompasAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pKompasAppUnkLocal);
			if (FAILED(hRes)) {
				AfxMessageBox(_T("Ошибка: Не удалось запустить Kompas-3D. Убедитесь, что Kompas-3D установлен и доступен."));
				EndWaitCursor();
				return;
			}
		}

		hRes = pKompasAppUnkLocal->QueryInterface(__uuidof(KompasObject), (void**)&pKompasApp5);
		if (FAILED(hRes) || !pKompasApp5) {
			AfxMessageBox(_T("Ошибка: Не удалось получить интерфейс KompasObject."));
			EndWaitCursor();
			return;
		}
	}

	if (pKompasApp5)
	{
		pKompasApp5->Visible = true;
	}
	else
	{
		AfxMessageBox(_T("Ошибка: Объект Kompas не инициализирован."));
		EndWaitCursor();
		return;
	}

	// === 1. Создание и сохранение деталей ===

	// Сальник
	ksDocument3DPtr pDocLocal = pKompasApp5->Document3D();
	if (!pDocLocal || !pDocLocal->Create(false, true) || !pDocLocal->GetPart(pTop_Part))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать документ для сальника."));
		EndWaitCursor();
		return;
	}
	ksPartPtr pPartLocal = pDocLocal->GetPart(pTop_Part);
	SetParams();
	if (!CreateSalnik(pPartLocal, m_Salnik_D, m_Salnik_L, m_ThreadP, m_ThreadD))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать сальник."));
		EndWaitCursor();
		return;
	}
	if (!pDocLocal->SaveAs(L"C:\\ProgaAntipov\\Salnik.m3d"))
	{
		AfxMessageBox(_T("Ошибка: Не удалось сохранить Salnik.m3d."));
		EndWaitCursor();
		return;
	}

	// Гайка
	pDocLocal = pKompasApp5->Document3D();
	if (!pDocLocal || !pDocLocal->Create(false, true) || !pDocLocal->GetPart(pTop_Part))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать документ для гайки."));
		EndWaitCursor();
		return;
	}
	pPartLocal = pDocLocal->GetPart(pTop_Part);
	if (!CreateGayka(pPartLocal, m_Gayka_d, m_Gayka_L, m_Gayka_D, m_ThreadP, m_ThreadD))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать гайку."));
		EndWaitCursor();
		return;
	}
	if (!pDocLocal->SaveAs(L"C:\\ProgaAntipov\\Gayka.m3d"))
	{
		AfxMessageBox(_T("Ошибка: Не удалось сохранить Gayka.m3d."));
		EndWaitCursor();
		return;
	}

	// Шайба
	pDocLocal = pKompasApp5->Document3D();
	if (!pDocLocal || !pDocLocal->Create(false, true) || !pDocLocal->GetPart(pTop_Part))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать документ для шайбы."));
		EndWaitCursor();
		return;
	}
	pPartLocal = pDocLocal->GetPart(pTop_Part);
	if (!CreateShayba(pPartLocal, m_Shayba_D, m_Shayba_d, m_Shayba_S))
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать шайбу."));
		EndWaitCursor();
		return;
	}
	if (!pDocLocal->SaveAs(L"C:\\ProgaAntipov\\Shayba.m3d"))
	{
		AfxMessageBox(_T("Ошибка: Не удалось сохранить Shayba.m3d."));
		EndWaitCursor();
		return;
	}

	// === 2. Создание сборки ===
	ksDocument3DPtr pDoc = pKompasApp5->Document3D();
	if (!pDoc)
	{
		AfxMessageBox(_T("Ошибка: Не удалось получить 3D документ для сборки."));
		EndWaitCursor();
		return;
	}
	if (!pDoc->Create(false, false)) // сборка
	{
		AfxMessageBox(_T("Ошибка: Не удалось создать 3D документ для сборки."));
		EndWaitCursor();
		return;
	}

	ksPartPtr pPart = pDoc->GetPart(pTop_Part);
	if (!pPart)
	{
		AfxMessageBox(_T("Ошибка: Не удалось получить компонент сборки."));
		EndWaitCursor();
		return;
	}

	// === 3. Добавление компонентов ===
	if (!pDoc->SetPartFromFile(L"C:\\ProgaAntipov\\Salnik.m3d", pPart, true))
	{
		AfxMessageBox(_T("Ошибка загрузки Salnik.m3d"));
		EndWaitCursor();
		return;
	}
	if (!pDoc->SetPartFromFile(L"C:\\ProgaAntipov\\Gayka.m3d", pPart, true))
	{
		AfxMessageBox(_T("Ошибка загрузки Gayka.m3d"));
		EndWaitCursor();
		return;
	}
	if (!pDoc->SetPartFromFile(L"C:\\ProgaAntipov\\Gayka.m3d", pPart, true))
	{
		AfxMessageBox(_T("Ошибка загрузки второй Gayka.m3d"));
		EndWaitCursor();
		return;
	}
	if (!pDoc->SetPartFromFile(L"C:\\ProgaAntipov\\Shayba.m3d", pPart, true))
	{
		AfxMessageBox(_T("Ошибка загрузки Shayba.m3d"));
		EndWaitCursor();
		return;
	}
	if (!pDoc->SetPartFromFile(L"C:\\ProgaAntipov\\Shayba.m3d", pPart, true))
	{
		AfxMessageBox(_T("Ошибка загрузки второй Shayba.m3d"));
		EndWaitCursor();
		return;
	}

	// === 4. Получение компонентов ===
	ksPartPtr pSalnik = pDoc->GetPart(0);  // сальник
	ksPartPtr pGayka1 = pDoc->GetPart(1);  // первая гайка
	ksPartPtr pGayka2 = pDoc->GetPart(2);  // вторая гайка
	ksPartPtr pShayba1 = pDoc->GetPart(3); // первая шайба
	ksPartPtr pShayba2 = pDoc->GetPart(4); // вторая шайба

	// Переворачиваем вторую гайку
	ksPlacementPtr placeGayka2 = pGayka2->GetPlacement();
	if (placeGayka2)
	{
		double ox, oy, oz;
		if (placeGayka2->GetOrigin(&ox, &oy, &oz))
		{
			placeGayka2->SetOrigin(ox, oy, oz);
			placeGayka2->SetAxes(1, 0, 0, 0, -1, 0);
			pGayka2->SetPlacement(placeGayka2);
			pGayka2->UpdatePlacement();
		}
	}

	// === 5. Получение именованных граней ===
	auto GetNamedFace = [](ksPartPtr part, const wchar_t* faceName) -> ksEntityPtr {
		ksEntityCollectionPtr col = part->EntityCollection(o3d_face);
		if (!col) return nullptr;
		return col->GetByName(faceName, true, true);
		};

	// Сальник
	ksEntityPtr salnikThreadHole = GetNamedFace(pSalnik, L"ThreadedHoleSalnik");   // внутренняя резьба
	ksEntityPtr salnikFrontPlane = GetNamedFace(pSalnik, L"FrontSalnikPlane");     // передний торец
	ksEntityPtr salnikBackPlane = GetNamedFace(pSalnik, L"BackSalnikPlane");      // задний торец

	// Гайка 1
	ksEntityPtr gayka1OuterCyl = GetNamedFace(pGayka1, L"ThreadOuterCylGayka");    // внешняя резьба (цилиндр)
	ksEntityPtr gayka1Base = GetNamedFace(pGayka1, L"ThreadBasePlaneGayka");   // основание цилиндра (у сальника)
	ksEntityPtr gayka1End = GetNamedFace(pGayka1, L"ThreadEndPlaneGayka");    // торец цилиндра (у шайбы)

	// Гайка 2
	ksEntityPtr gayka2OuterCyl = GetNamedFace(pGayka2, L"ThreadOuterCylGayka");
	ksEntityPtr gayka2Base = GetNamedFace(pGayka2, L"ThreadBasePlaneGayka");
	ksEntityPtr gayka2End = GetNamedFace(pGayka2, L"ThreadEndPlaneGayka");

	// Шайба 1
	ksEntityPtr shayba1InnerHole = GetNamedFace(pShayba1, L"InnerHoleShayba");     // отверстие
	ksEntityPtr shayba1Bottom = GetNamedFace(pShayba1, L"BottomShaybaPlane");   // нижняя плоскость

	// Шайба 2
	ksEntityPtr shayba2InnerHole = GetNamedFace(pShayba2, L"InnerHoleShayba");
	ksEntityPtr shayba2Bottom = GetNamedFace(pShayba2, L"BottomShaybaPlane");

	// Проверка наличия ВСЕХ необходимых граней
	if (!salnikThreadHole || !salnikFrontPlane || !salnikBackPlane ||
		!gayka1OuterCyl || !gayka1Base || !gayka1End ||
		!gayka2OuterCyl || !gayka2Base || !gayka2End ||
		!shayba1InnerHole || !shayba1Bottom ||
		!shayba2InnerHole || !shayba2Bottom)
	{
		AfxMessageBox(_T("Ошибка: не все именованные грани найдены!\n"
			"Убедитесь, что в CreateSalnik, CreateGayka, CreateShayba заданы следующие имена:\n"
			"- Сальник: ThreadedHoleSalnik, FrontSalnikPlane, BackSalnikPlane\n"
			"- Гайка: ThreadOuterCylGayka, ThreadBasePlaneGayka, ThreadEndPlaneGayka\n"
			"- Шайба: InnerHoleShayba, BottomShaybaPlane"));
		EndWaitCursor();
		return;
	}

	// === 6. Сопряжения ===

	// Проверка
	if (!gayka1OuterCyl || !gayka1Base || !gayka1End ||
		!gayka2OuterCyl || !gayka2Base || !gayka2End)
	{
		AfxMessageBox(_T("Ошибка: не найдены грани цилиндра гайки!"));
		EndWaitCursor();
		return;
	}

	// Создание сопряжений
	pDoc->AddMateConstraint(mc_Concentric, salnikThreadHole, gayka2OuterCyl, 0, 1, 1);
	pDoc->AddMateConstraint(mc_Coincidence, gayka2Base, salnikFrontPlane, 1, -1, 1);
	pDoc->AddMateConstraint(mc_Concentric, gayka2OuterCyl, shayba1InnerHole, 0, 0, 1);
	pDoc->AddMateConstraint(mc_Coincidence, shayba1Bottom, gayka2End, 0, 0, 1);

	pDoc->AddMateConstraint(mc_Concentric, salnikThreadHole, gayka1OuterCyl, 0, 1, 1);
	pDoc->AddMateConstraint(mc_Coincidence, gayka1Base, salnikBackPlane, 1, -1, 1);
	pDoc->AddMateConstraint(mc_Concentric, gayka1OuterCyl, shayba2InnerHole, 0, 0, 1);
	pDoc->AddMateConstraint(mc_Coincidence, shayba2Bottom, gayka1End, 0, 0, 1);

	// === 7. Сохранение сборки ===
	std::string cwd = "C:\\ProgaAntipov";
	std::string name = cwd + "\\Sborka.a3d";
	CString resp = CA2T(name.c_str());
	if (!pDoc->SaveAs(resp.AllocSysString()))
	{
		AfxMessageBox(_T("Ошибка: Не удалось сохранить файл сборки."));
		EndWaitCursor();
		return;
	}

	EndWaitCursor();
	AfxMessageBox(_T("Сборка успешно создана!\nФайл: C:\\ProgaAntipov\\Sborka.a3d"));
}

void CSDIProjectCourseDoc::OnIddCAssembleDlg()
{
	CAssembleDlg dlg;
	dlg.m_pDoc = this;
	if (dlg.DoModal() != IDOK)
		return;
}

void CSDIProjectCourseDoc::OnIddCSalnikNestDlg()
{
	CSalnikNestDlg dlg;
	dlg.m_pDoc = this;
	if (dlg.DoModal() != IDOK)
		return;
}