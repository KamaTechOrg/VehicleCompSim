#pragma once

#include <QGraphicsItem>
#include <QJsonObject>
#include "client/observer.h"

class Observer;

enum class ItemType { Sensor, Connector, Edge };

class SerializableItem{
public:
	SerializableItem();

	virtual QJsonObject serialize() const;
	virtual void deserialize(const QJsonObject &itemData);

	QString getId() const;
	void setId(const QString &newId);
	int getVersion();
	void incrementVersion();

    ItemType itemType() const { return m_type; }

	// Observer pattern methods
	void setObserver(Observer* observer);
	void notifyItemAdded();
	void notifyItemModified();
	void notifyItemDeleted();

protected:
	QString id;
	int version;
	ItemType m_type;

private:
    Observer* observer;
};

