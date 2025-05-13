
// ThreePointCircleDlg.h: 헤더 파일
//

#pragma once
#include <vector>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

struct Circle {
    CPoint center; // 원의 중심
    int radius;    // 원의 반지름
};
// CThreePointCircleDlg 대화 상자
class CThreePointCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CThreePointCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREEPOINTCIRCLE_DIALOG };
#endif

protected:
    HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

	

private:
    std::vector<CPoint> m_points;
    int m_radius;
    int m_lineWidth;
    int m_dragIndex;
    bool m_dragging;
    ULONG_PTR m_gdiplusToken;

    CEdit m_editRadius;
    CEdit m_editLineWidth;
    CStatic m_coordDisplays[3];

    CButton m_btnReset;
    CButton m_btnRandom;

    void DrawAll(CDC* pDC);
    void UpdateCoordUI();
    BOOL SelectCircle(CPoint center, CPoint pt);
    //Circle CalcCircumcircle(CPoint p1, CPoint p2, CPoint p3);
    BOOL CalcCircumcircle(CPoint a, CPoint b, CPoint c, double& cx, double& cy, double& radius);
    
    void RandomMoveThreadFunc();
    //static UINT RandomMoveThreadProc(LPVOID pParam);

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnReset();
    afx_msg void OnBnClickedBtnRandom();
    afx_msg void OnEnChangeEditRadius();
    afx_msg void OnStnClickedStaticCoord1();
    afx_msg void OnStnClickedStaticCoord2();
    afx_msg void OnStnClickedStaticCoord3();
    afx_msg void OnBnClickedCancel();
};

