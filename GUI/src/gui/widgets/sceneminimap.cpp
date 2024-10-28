#include "sceneminimap.h"

#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QScrollBar>
#include <QTimer>
#include <QResizeEvent>

SceneMinimap::SceneMinimap(QGraphicsView* mainView, QWidget* parent)
    : QFrame(parent)
    , m_mainView(mainView)
{
    setFixedSize(calculateMinimapSize());

    // Install event filter on main view for tracking resizes
    if (m_mainView) {
        m_mainView->viewport()->installEventFilter(this);
        m_mainView->setMouseTracking(true);
    }
    
    setupMinimapStyle();
    setupConnections();
    raise();

    // Initial update
    QTimer::singleShot(0, this, &SceneMinimap::updateMinimap);
}

void SceneMinimap::setupConnections()
{
    if (!m_mainView || !m_mainView->scene()) return;

    // Connect to scene changes
    connect(m_mainView->scene(), &QGraphicsScene::sceneRectChanged, this, &SceneMinimap::updateMinimap);
    
    // Connect to view changes
    connect(m_mainView->horizontalScrollBar(), &QScrollBar::valueChanged, this, &SceneMinimap::updateMinimap);
    connect(m_mainView->verticalScrollBar(), &QScrollBar::valueChanged, this, &SceneMinimap::updateMinimap);
    connect(m_mainView->horizontalScrollBar(), &QScrollBar::rangeChanged, this, &SceneMinimap::updateMinimap);
    connect(m_mainView->verticalScrollBar(), &QScrollBar::rangeChanged, this, &SceneMinimap::updateMinimap);
    connect(m_mainView, &QGraphicsView::viewportTransform, this, &SceneMinimap::updateMinimap);
}

void SceneMinimap::setupMinimapStyle()
{
    setStyleSheet(QString(
        "SceneMinimap {"
        "   background-color: rgba(245, 245, 245, 0.95);"
        "   border: 2px solid rgba(0, 0, 0, 0.2);"
        "   border-radius: %1px;"
        "}"
    ).arg(BORDER_RADIUS));
}

QSize SceneMinimap::calculateMinimapSize() const
{
    if (!m_mainView || !m_mainView->scene()) return QSize(200, 150);

    QRectF sceneRect = getAdjustedSceneRect();
    qreal sceneAspectRatio = sceneRect.width() / sceneRect.height();
    
    int width, height;
    if (sceneAspectRatio > 1.0) {
        width = m_maxSize.width();
        height = qRound(width / sceneAspectRatio);
        if (height > m_maxSize.height()) {
            height = m_maxSize.height();
            width = qRound(height * sceneAspectRatio);
        }
    } else {
        height = m_maxSize.height();
        width = qRound(height * sceneAspectRatio);
        if (width > m_maxSize.width()) {
            width = m_maxSize.width();
            height = qRound(width / sceneAspectRatio);
        }
    }

    return QSize(width + 2 * MINIMAP_PADDING, height + 2 * MINIMAP_PADDING);
}
QRectF SceneMinimap::getAdjustedSceneRect() const
{
    if (!m_mainView || !m_mainView->scene()) return QRectF();

    // Get the scene rect and items bounding rect
    QRectF sceneRect = m_mainView->scene()->sceneRect();
    QRectF itemsBoundingRect = m_mainView->scene()->itemsBoundingRect();

    // Use the larger of the two rects
    QRectF resultRect = sceneRect.united(itemsBoundingRect);

    // Add some padding
    resultRect.adjust(-10, -10, 10, 10);

    return resultRect;
}

void SceneMinimap::showEvent(QShowEvent* event)
{
    QFrame::showEvent(event);
    updatePosition();
}

bool SceneMinimap::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_mainView->viewport()) {
        switch (event->type()) {
            case QEvent::Resize:
                updatePosition();
                break;
            case QEvent::MouseMove:
                raise();
                break;
            default:
                break;
        }
    }
    return false;
}

void SceneMinimap::updatePosition()
{
    if (!m_mainView) return;

    // Calculate position relative to the view's viewport
    QRect viewportRect = m_mainView->viewport()->rect();
    QPoint newPos(
        viewportRect.right() - width() - MARGIN,
        viewportRect.bottom() - height() - MARGIN
    );
    
    // Convert to parent coordinates
    newPos = m_mainView->viewport()->mapTo(parentWidget(), newPos);
    move(newPos);
}

void SceneMinimap::updateScale()
{
    if (!m_mainView || !m_mainView->scene()) return;

    QRectF adjustedSceneRect = getAdjustedSceneRect();
    QRectF minimapRect = rect().adjusted(MINIMAP_PADDING, MINIMAP_PADDING, 
                                       -MINIMAP_PADDING, -MINIMAP_PADDING);

    // Calculate scales for both dimensions
    qreal scaleX = minimapRect.width() / adjustedSceneRect.width();
    qreal scaleY = minimapRect.height() / adjustedSceneRect.height();

    // Use the smaller scale to maintain aspect ratio
    m_currentScale = qMin(scaleX, scaleY);
}

void SceneMinimap::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);
    
    if (!m_mainView || !m_mainView->scene()) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background with border
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    painter.setBrush(QColor(245, 245, 245));
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), BORDER_RADIUS, BORDER_RADIUS);

    QRectF minimapRect = rect().adjusted(MINIMAP_PADDING, MINIMAP_PADDING, 
                                       -MINIMAP_PADDING, -MINIMAP_PADDING);

    // Calculate the centered position for the scaled content
    qreal scaledWidth = m_currentSceneRect.width() * m_currentScale;
    qreal scaledHeight = m_currentSceneRect.height() * m_currentScale;
    qreal xOffset = minimapRect.x() + (minimapRect.width() - scaledWidth) / 2;
    qreal yOffset = minimapRect.y() + (minimapRect.height() - scaledHeight) / 2;

    // Create a scaled pixmap of the scene
    QPixmap scenePixmap(m_currentSceneRect.size().toSize());
    scenePixmap.fill(Qt::transparent);
    QPainter scenePainter(&scenePixmap);
    scenePainter.setRenderHint(QPainter::Antialiasing);
    
    // Render the scene to the pixmap with proper transformation
    m_mainView->scene()->render(&scenePainter, QRectF(), m_currentSceneRect);
    scenePainter.end();

    // Draw the scaled scene content
    painter.save();
    painter.translate(xOffset, yOffset);
    painter.drawPixmap(QRectF(0, 0, scaledWidth, scaledHeight), scenePixmap, scenePixmap.rect());
    painter.restore();

    // Draw viewport rectangle with improved visibility
    painter.setPen(QPen(QColor(0, 120, 215, 180), 2));
    painter.setBrush(QColor(0, 120, 215, 40));
    painter.drawRoundedRect(m_viewportRect, 4, 4);
}

QRectF SceneMinimap::calculateViewportRect() const
{
    if (!m_mainView || !m_mainView->scene()) return QRectF();

    QRectF minimapRect = rect().adjusted(MINIMAP_PADDING, MINIMAP_PADDING, 
                                       -MINIMAP_PADDING, -MINIMAP_PADDING);

    // Get the visible area in scene coordinates
    QRectF visibleSceneRect = m_mainView->mapToScene(m_mainView->viewport()->rect()).boundingRect();

    // Calculate scaled dimensions
    qreal scaledWidth = m_currentSceneRect.width() * m_currentScale;
    qreal scaledHeight = m_currentSceneRect.height() * m_currentScale;
    
    // Calculate offsets for centering
    qreal xOffset = minimapRect.x() + (minimapRect.width() - scaledWidth) / 2;
    qreal yOffset = minimapRect.y() + (minimapRect.height() - scaledHeight) / 2;

    // Map the viewport rectangle to minimap coordinates
    return QRectF(
        xOffset + (visibleSceneRect.x() - m_currentSceneRect.x()) * m_currentScale,
        yOffset + (visibleSceneRect.y() - m_currentSceneRect.y()) * m_currentScale,
        visibleSceneRect.width() * m_currentScale,
        visibleSceneRect.height() * m_currentScale
    );
}

void SceneMinimap::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastDragPos = event->pos();
        updateMainViewCenter(event->pos());
    }
}

void SceneMinimap::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        m_lastDragPos = event->pos();
        updateMainViewCenter(event->pos());
    }
}

void SceneMinimap::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
}

void SceneMinimap::updateMinimap()
{
    if (!m_mainView || !m_mainView->scene()) return;

    if (!isSceneInScrollingMode()) {
        hide();
        return;
    } else {
        show();
    }
    
    // Update size and position
    setFixedSize(calculateMinimapSize());
    updatePosition();
    
    m_currentSceneRect = getAdjustedSceneRect();
    updateScale();
    m_viewportRect = calculateViewportRect();
    
    // Force immediate update
    update();
}


void SceneMinimap::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
    updateMinimap();
}

void SceneMinimap::updateMainViewCenter(const QPointF& pos)
{
    if (!m_mainView || !m_mainView->scene()) return;

    QRectF minimapRect = rect().adjusted(MINIMAP_PADDING, MINIMAP_PADDING, 
                                       -MINIMAP_PADDING, -MINIMAP_PADDING);

    // Calculate scaled dimensions and offsets
    qreal scaledWidth = m_currentSceneRect.width() * m_currentScale;
    qreal scaledHeight = m_currentSceneRect.height() * m_currentScale;
    qreal xOffset = minimapRect.x() + (minimapRect.width() - scaledWidth) / 2;
    qreal yOffset = minimapRect.y() + (minimapRect.height() - scaledHeight) / 2;

    // Calculate the position in scene coordinates
    qreal sceneX = m_currentSceneRect.x() + (pos.x() - xOffset) / m_currentScale;
    qreal sceneY = m_currentSceneRect.y() + (pos.y() - yOffset) / m_currentScale;

    m_mainView->centerOn(QPointF(sceneX, sceneY));
    
    // Update the viewport rectangle immediately
    m_viewportRect = calculateViewportRect();
    update();
}

bool SceneMinimap::isSceneInScrollingMode() const {
    if (!m_mainView) return false;

    // bool horizontalScrolling = m_mainView->horizontalScrollBar()->isVisible() &&
    //                            (m_mainView->horizontalScrollBar()->value() > m_mainView->horizontalScrollBar()->minimum() ||
    //                             m_mainView->horizontalScrollBar()->value() < m_mainView->horizontalScrollBar()->maximum());

    // bool verticalScrolling = m_mainView->verticalScrollBar()->isVisible() &&
    //                          (m_mainView->verticalScrollBar()->value() > m_mainView->verticalScrollBar()->minimum() ||
    //                           m_mainView->verticalScrollBar()->value() < m_mainView->verticalScrollBar()->maximum());

    // return horizontalScrolling || verticalScrolling;
    return m_mainView->horizontalScrollBar()->isVisible() || m_mainView->verticalScrollBar()->isVisible();
}