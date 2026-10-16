#ifndef PAINT_TOOLS_HPP
#define PAINT_TOOLS_HPP

class PaintTools {
public:
    static HPEN TakePen(HDC h_dc, COLORREF color, HPEN& old_pen) {
        HPEN h_pen = CreatePen(PS_SOLID, 1, color);
        old_pen = reinterpret_cast<HPEN>(SelectObject(h_dc, h_pen));
        return h_pen;
    }
    static void ResetPen(HDC h_dc, HPEN old_pen, HPEN h_pen) {
        SelectObject(h_dc, old_pen);
        DeleteObject(h_pen);
    }
    static HBRUSH TakeBrush(HDC h_dc, COLORREF color, HBRUSH& old_brush) {
        HBRUSH h_brush = CreateSolidBrush(color);
        old_brush = reinterpret_cast<HBRUSH>(SelectObject(h_dc, h_brush));
        return h_brush;
    }
    static void ResetBrush(HDC h_dc, HBRUSH old_brush, HBRUSH h_brush) {
        SelectObject(h_dc, old_brush);
        DeleteObject(h_brush);
    }
    static void PaintLine(HDC h_dc, i32 x1, i32 y1, i32 x2, i32 y2) {
        MoveToEx(h_dc, x1, y1, NULL);
        LineTo(h_dc, x2, y2);
    }
    static void PaintRectangle(HDC h_dc, i32 x1, i32 y1, i32 x2, i32 y2) {
        PaintLine(h_dc, x1, y1, x2, y1);
        PaintLine(h_dc, x2, y1, x2, y2);
        PaintLine(h_dc, x2, y2, x1, y2);
        PaintLine(h_dc, x1, y2, x1, y1);
    }
    static void PaintFilledRectangle(HDC h_dc, i32 x1, i32 y1, i32 x2, i32 y2) {
        Rectangle(h_dc, x1, y1, x2, y2);
    }
    static void PaintText(HDC h_dc, const std::string& text, i32 x, i32 y) {
        TextOutA(h_dc, x, y, text.c_str(), sc<int>(text.size()));
    }
    static void PaintTextFilledRectangle(HDC h_dc, const std::string& text, i32 x1, i32 y1, i32 x2, i32 y2) {
        RECT rect = { x1, y1, x2, y2 };
        DrawTextA(h_dc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    static void PaintDot(HDC h_dc, i32 x, i32 y, u32 size, COLORREF color) {
        RECT rect = { x, y, x + sc<LONG>(size), y + sc<LONG>(size) };
        HBRUSH h_brush = CreateSolidBrush(color);
        FillRect(h_dc, &rect, h_brush);
        DeleteObject(h_brush);
    }
    static void PaintDot(HDC h_dc, i32 x, i32 y, u32 size, HBRUSH h_brush) {
        RECT rect = { x, y, x + sc<LONG>(size), y + sc<LONG>(size) };
        FillRect(h_dc, &rect, h_brush);
    }
    static void PaintDot(HDC h_dc, i32 x, i32 y, u32 size) {
        PatBlt(h_dc, x, y, sc<int>(size), sc<int>(size), PATCOPY);
    }
    static void PaintCurve(HDC h_dc, const POINT* points, i32 num_points) {
        PolyBezier(h_dc, points, num_points);
    }
    static void PaintPolyLine(HDC h_dc, const POINT* points, i32 num_points) {
        Polyline(h_dc, points, num_points);
    }
    static void PaintCross(HDC h_dc, i32 x, i32 y, u32 size) {
        PaintLine(h_dc, x - sc<i32>(size / 2), y, x + sc<i32>(size / 2), y);
        PaintLine(h_dc, x, y - sc<i32>(size / 2), x, y + sc<i32>(size / 2));
    }

};

#endif // PAINT_TOOLS_HPP