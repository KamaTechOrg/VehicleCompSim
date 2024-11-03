#pragma once
#include <QGraphicsView>
#include <QFrame>

class SceneMinimap : public QFrame {
    Q_OBJECT
public:
    explicit SceneMinimap(QGraphicsView* mainView, QWidget* parent = nullptr);
    void updateMinimap();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    QGraphicsView* m_mainView;
    QRectF m_viewportRect;
    bool m_isDragging = false;
    QPointF m_lastDragPos;
    const int MINIMAP_PADDING = 5;
    const int MARGIN = 10;
    const int BORDER_RADIUS = 8;
    QRectF m_currentSceneRect;
    qreal m_currentScale = 1.0;
    QSize m_maxSize{200, 200};  
    
    QRectF calculateViewportRect() const;
    void updateMainViewCenter(const QPointF& pos);
    void setupMinimapStyle();
    void setupConnections();
    QRectF getAdjustedSceneRect() const;
    void updateScale();
    QSize calculateMinimapSize() const;
    void updatePosition();
    bool isSceneInScrollingMode() const;
};