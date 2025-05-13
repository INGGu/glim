
// ThreePointCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ThreePointCircle.h"
#include "ThreePointCircleDlg.h"
#include "afxdialogex.h"
#include <random>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CThreePointCircleDlg 대화 상자



CThreePointCircleDlg::CThreePointCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREEPOINTCIRCLE_DIALOG, pParent),
	m_radius(30), m_lineWidth(2), m_dragIndex(-1), m_dragging(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Gdiplus::GdiplusStartupInput gsi;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gsi, nullptr);
}

void CThreePointCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_EDIT_RADIUS, m_editRadius);
	DDX_Control(pDX, IDC_EDIT_LINEWIDTH, m_editLineWidth);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BTN_RANDOM, m_btnRandom);
	DDX_Control(pDX, IDC_STATIC_COORD1, m_coordDisplays[0]);
	DDX_Control(pDX, IDC_STATIC_COORD2, m_coordDisplays[1]);
	DDX_Control(pDX, IDC_STATIC_COORD3, m_coordDisplays[2]);
}

BEGIN_MESSAGE_MAP(CThreePointCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RESET, &CThreePointCircleDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CThreePointCircleDlg::OnBnClickedBtnRandom)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CThreePointCircleDlg::OnEnChangeEditRadius)
	ON_STN_CLICKED(IDC_STATIC_COORD1, &CThreePointCircleDlg::OnStnClickedStaticCoord1)
	ON_STN_CLICKED(IDC_STATIC_COORD2, &CThreePointCircleDlg::OnStnClickedStaticCoord2)
	ON_STN_CLICKED(IDC_STATIC_COORD3, &CThreePointCircleDlg::OnStnClickedStaticCoord3)
	ON_BN_CLICKED(IDCANCEL, &CThreePointCircleDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CThreePointCircleDlg 메시지 처리기

BOOL CThreePointCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	m_editRadius.SetWindowTextW(_T("5"));
	m_editLineWidth.SetWindowTextW(_T("2"));
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CThreePointCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else


	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

HCURSOR CThreePointCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CThreePointCircleDlg::OnPaint()
{
	CPaintDC dc(this);
	DrawAll(&dc);
}

void CThreePointCircleDlg::DrawAll(CDC* pDC)
{
	Gdiplus::Graphics g(pDC->GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	// 사용자 입력 값 가져오기
	CString strRadius, strWidth;
	m_editRadius.GetWindowTextW(strRadius);
	m_editLineWidth.GetWindowTextW(strWidth);
	m_radius = _ttoi(strRadius);
	m_lineWidth = _ttoi(strWidth);

	// 클릭한 점들을 검은색 채워진 원으로 그림(최대 3개의 점)
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	for (size_t i = 0; i < m_points.size() && i < 3; ++i)
	{
		g.FillEllipse(&blackBrush,
			m_points[i].x - m_radius, // 원의 왼쪽 상단 x좌표를 계산
			m_points[i].y - m_radius, // 원의 왼쪽 상단 y좌표를 계산
			m_radius * 2,			  // 사각형의 가로길이
			m_radius * 2);  		  // 사각형의 세로길이
	}

	// 클릭한 3개의 점을 지나는 외접원 그리기
	if (m_points.size() >= 3)
	{
		double cx, cy, radius;
		if (CalcCircumcircle(m_points[0], m_points[1], m_points[2], cx, cy, radius))
		{
			Gdiplus::Pen penCircle(Gdiplus::Color(255, 0, 0, 0), (float)m_lineWidth);
			penCircle.SetDashStyle(Gdiplus::DashStyleSolid); //실선

			g.DrawEllipse(&penCircle,
				(Gdiplus::REAL)(cx - radius), // 원의 왼쪽 상단 x좌표를 계산
				(Gdiplus::REAL)(cy - radius), // 원의 왼쪽 상단 y좌표를 계산
				(Gdiplus::REAL)(radius * 2),  // 사각형의 가로길이
				(Gdiplus::REAL)(radius * 2)); // 사각형의 세로길이
		}
	}

	UpdateCoordUI();
}


BOOL CThreePointCircleDlg::CalcCircumcircle(CPoint a, CPoint b, CPoint c, double& dx, double& dy, double& radius)
{
	double ax = a.x, ay = a.y;
	double bx = b.x, by = b.y;
	double cx = c.x, cy = c.y;

	// 세 점이 일직선인지 먼저 확인
	double cross = (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
	if (fabs(cross) < 1e-6)
		return FALSE; // 세 점이 일직선이면 외접원 없음

	// 외접원의 중심 계산
	double d = 2 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
	if (fabs(d) < 1e-6)
		return FALSE; // 수치적으로 불안정할 경우

	double ux = ((ax * ax + ay * ay) * (by - cy) +
		(bx * bx + by * by) * (cy - ay) +
		(cx * cx + cy * cy) * (ay - by)) / d;

	double uy = ((ax * ax + ay * ay) * (cx - bx) +
		(bx * bx + by * by) * (ax - cx) +
		(cx * cx + cy * cy) * (bx - ax)) / d;

	dx = ux;
	dy = uy;
	radius = sqrt((dx - ax) * (dx - ax) + (dy - ay) * (dy - ay));

	return TRUE;
}


void CThreePointCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 기존 점 중 하나를 클릭했는지 확인
	for (int i = 0; i < m_points.size(); ++i)
	{
		if (SelectCircle(m_points[i], point))
		{
			m_dragIndex = i;
			m_dragging = true;
			SetCapture(); // 마우스 캡처
			return;
		}
	}

	// 3개의 점까지만 추가
	if (m_points.size() < 3)
	{
		m_points.push_back(point);
		Invalidate();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CThreePointCircleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dragging && m_dragIndex >= 0)
	{
		m_points[m_dragIndex] = point;
		Invalidate(); 
	}
}

void CThreePointCircleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_dragging = false;
	m_dragIndex = -1;
	ReleaseCapture();
}


BOOL CThreePointCircleDlg::SelectCircle(CPoint center, CPoint pt)
{
	//클릭 위치가 점의 반지름 내에 있는지 확인
	int dx = pt.x - center.x;
	int dy = pt.y - center.y;
	return dx * dx + dy * dy <= m_radius * m_radius;
}

void CThreePointCircleDlg::UpdateCoordUI()
{
	for (int i = 0; i < 3; ++i)
	{
		CString coord;
		if (i < m_points.size())
		{
			coord.Format(_T("%d번 좌표: (%d, %d)"), i + 1, m_points[i].x, m_points[i].y);
		}
		else
		{
			coord.Format(_T("%d번 좌표: ( , )"), i + 1);
		}
		m_coordDisplays[i].SetWindowTextW(coord);
	}
}

void CThreePointCircleDlg::RandomMoveThreadFunc()
{
	std::random_device rd;
	std::mt19937 gen(rd()); // 난수 생성기 초기화

	CRect rect;
	GetClientRect(&rect);
	std::uniform_int_distribution<> distX(rect.left + 30, rect.right - 30);
	std::uniform_int_distribution<> distY(rect.top + 30, rect.bottom - 30);

	for (int i = 0; i < 10; ++i)
	{
		for (auto& pt : m_points)
			pt = CPoint(distX(gen), distY(gen));

		Invalidate();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void CThreePointCircleDlg::OnBnClickedBtnRandom()
{
	if (m_points.size() < 3) return;

	// std::thread 사용
	std::thread t(&CThreePointCircleDlg::RandomMoveThreadFunc, this);
	t.detach();  // 백그라운드 스레드로 실행
}

void CThreePointCircleDlg::OnBnClickedBtnReset()
{
	m_points.clear();
	m_dragIndex = -1;
	m_dragging = false;
	Invalidate();
	UpdateCoordUI();
}

void CThreePointCircleDlg::OnEnChangeEditRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CThreePointCircleDlg::OnStnClickedStaticCoord1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CThreePointCircleDlg::OnStnClickedStaticCoord2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CThreePointCircleDlg::OnStnClickedStaticCoord3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CThreePointCircleDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
