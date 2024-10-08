////
//// Created by OWNER on 17/09/2024.
////
//
//#ifndef VEHICLECOMPSIM_CUSTOMINFOWINDOW_H
//#define VEHICLECOMPSIM_CUSTOMINFOWINDOW_H
//
//#include <QWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QTimer>
//#include <QGraphicsItem>
//#include <QGraphicsSceneMouseEvent>
//
//#include <QWidget>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <QPushButton>
//#include <QLabel>
//#include <QGraphicsItem>
//#include <QGraphicsSceneMouseEvent>
//#include <QGraphicsProxyWidget>
//#include <QScrollArea>
//
//
//class CustomInfoWindow : public QWidget {
//Q_OBJECT
//
//public:
//    CustomInfoWindow(QWidget* parent = nullptr) : QWidget(parent) {
//        setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
//
//        QVBoxLayout* mainLayout = new QVBoxLayout(this);
//        mainLayout->setContentsMargins(1, 1, 1, 1);  // Reduced margins for a smaller border
//        mainLayout->setSpacing(0);
//
//        // Create close button layout at the top
//        QHBoxLayout* topLayout = new QHBoxLayout();
//        topLayout->setContentsMargins(5, 5, 5, 5);
//        QPushButton* closeButton = new QPushButton("X", this);
//        closeButton->setFixedSize(20, 20);
//        topLayout->addWidget(closeButton, 0, Qt::AlignLeft | Qt::AlignTop);
//        topLayout->addStretch();
//
//        // Add top layout to the main layout
//        mainLayout->addLayout(topLayout);
//
//        // Create a scroll area for the info content
//        QScrollArea* scrollArea = new QScrollArea(this);
//        scrollArea->setWidgetResizable(true);
//        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//
//        QWidget* scrollContent = new QWidget(scrollArea);
//        QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
//
//        infoLabel = new QLabel(scrollContent);
//        infoLabel->setWordWrap(true);
//        scrollLayout->addWidget(infoLabel);
//        scrollLayout->addStretch();
//
//        scrollArea->setWidget(scrollContent);
//
//        mainLayout->addWidget(scrollArea);
//
//        connect(closeButton, &QPushButton::clicked, this, &CustomInfoWindow::onCloseButtonClicked);
//
//        setStyleSheet(
//                "CustomInfoWindow { "
//                "   background-color: white; "
//                "   border: 1px solid gray; "
//                "}"
//                "QScrollArea { border: none; }"
//                "QPushButton { "
//                "   background-color: #e0e0e0; "  // Light gray background
//                "   color: #505050; "             // Dark gray text
//                "   border: 1px solid #c0c0c0; "  // Medium gray border
//                "   border-radius: 12px; "        // Rounded corners
//                "   font-weight: bold; "          // Bold text
//                "   font-size: 16px; "            // Larger font size
//                "}"
//                "QPushButton:hover { "
//                "   background-color: #d0d0d0; "  // Slightly darker on hover
//                "}"
//                "QPushButton:pressed { "
//                "   background-color: #c0c0c0; "  // Even darker when pressed
//                "}"
//        );
//        setFixedSize(500, 400);  // Adjust size as needed
//    }
//
//    void setInfo(const QString& info) {
//        infoLabel->setText(info);
//    }
//
//signals:
//    void closed();
//
//private slots:
//    void onCloseButtonClicked() {
//        emit closed();
//        hide();
//    }
//
//private:
//    QLabel* infoLabel;
//};
//
//#endif //VEHICLECOMPSIM_CUSTOMINFOWINDOW_H
