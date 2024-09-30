#include "Poligon.h"

Poligon::Poligon(std::vector<cv::Point>& points) : m_poligon(points) {}
Poligon::Poligon(std::initializer_list<cv::Point> points) : m_poligon(points) {}

bool Poligon::doesRectIntersectPolygon(const cv::Rect& rect) const
{
    std::vector<cv::Point> rectPolygon;
    rectPolygon.push_back(cv::Point(rect.x, rect.y));  // ���� ������ ������
    rectPolygon.push_back(cv::Point(rect.x + rect.width, rect.y));  // ���� ������ �����
    rectPolygon.push_back(cv::Point(rect.x + rect.width, rect.y + rect.height));  // ���� ������ �����
    rectPolygon.push_back(cv::Point(rect.x, rect.y + rect.height));  // ���� ������ ������

    std::vector<cv::Point> intersection;  // ��� ����� �� ������ ������

    // ����� �� �� ����� ��� ���������� (����� ��������� ������)
    if (cv::intersectConvexConvex(rectPolygon, m_poligon, intersection, true) > 0) {
        return true;  // �� �����
    }
    return false;  // ��� �����
}
//======================================================================================
bool Poligon::doesLineIntersectPolygon(const std::pair<cv::Point, cv::Point>& line) const {
    if (m_poligon.size() < 3) {
        return false;
    }

    for (size_t i = 0; i < m_poligon.size(); ++i) {
        cv::Point p1 = m_poligon[i];
        cv::Point p2 = m_poligon[(i + 1) % m_poligon.size()];  // ����� ����, ���� ����� ����

        // ����� ����� ��� ��� ���� ��������
        if (doLinesIntersect(line.first, line.second, p1, p2)) {
            return true;
        }
    }

    return false;
}
//======================================================================================
// ������� ������ �� �� ����� ��� ��� �����
bool Poligon::doLinesIntersect(const cv::Point& p1, const cv::Point& p2, const cv::Point& q1, const cv::Point& q2) {
    auto orientation = [](const cv::Point& p, const cv::Point& q, const cv::Point& r) -> int {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;  // ������ ����������
        return (val > 0) ? 1 : 2;  // 1 -> �� ����� �����, 2 -> ��� ����� �����
        };

    auto onSegment = [](const cv::Point& p, const cv::Point& q, const cv::Point& r) -> bool {
        if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
            return true;
        return false;
        };

    // ������ �� �������� �� �������
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // ���� ����� �� �� �����
    if (o1 != o2 && o3 != o4) return true;

    // ����� ������ ����������
    if (o1 == 0 && onSegment(p1, q1, p2)) return true;
    if (o2 == 0 && onSegment(p1, q2, p2)) return true;
    if (o3 == 0 && onSegment(q1, p1, q2)) return true;
    if (o4 == 0 && onSegment(q1, p2, q2)) return true;

    return false;
}


