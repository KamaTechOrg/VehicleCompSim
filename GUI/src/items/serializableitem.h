#pragma once

#include <QObject>
#include <QJsonObject>
#include "client/observer.h"

class Observer;

enum class ItemType { Sensor, Connector, Edge };

class SerializableItem : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString id READ getId WRITE setId)
	Q_PROPERTY(int version READ getVersion WRITE setVersion NOTIFY versionChanged)
	Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
  Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
public:
	SerializableItem(ItemType type = ItemType::Connector);

	virtual QJsonObject serialize() const;
	virtual void deserialize(const QJsonObject &itemData);

	QString getId() const;
	void setId(const QString &newId);

	int getVersion();
	void setVersion(int newVersion);

	qreal x() const;
  void setX(qreal x);

  qreal y() const;
  void setY(qreal y);

  ItemType itemType() const { return m_type; }

	// Observer pattern methods
	void setObserver(Observer* observer);
	void notifyItemAdded();
	void notifyItemModified();
	void notifyItemDeleted();

signals:
    void versionChanged();
    void xChanged();
    void yChanged();
		
protected:
	QString id;
	int version;
	ItemType m_type;
	qreal m_x;
	qreal m_y;

private:
    Observer* observer;
};

