#include "serializableitem.h"
#include <QUuid>
#include "client/observerconfig.h"

SerializableItem::SerializableItem(ItemType type) : m_type(type),
version(0), id(QUuid::createUuid().toString()), observer(ObserverConfig::getObserver())
{
}

QJsonObject SerializableItem::serialize() const {
    QJsonObject itemData;
    itemData["id"] = id;
    itemData["version"] = version;
    itemData["type"] = static_cast<int>(m_type);
    itemData["x"] = x();
    itemData["y"] = y();
    return itemData;
}

void SerializableItem::deserialize(const QJsonObject &itemData) {
    id = itemData["id"].toString();
    version = itemData["version"].toInt();
    m_type = static_cast<ItemType>(itemData["type"].toInt());
    setX(itemData["x"].toDouble());
    setY(itemData["y"].toDouble());
}

QString SerializableItem::getId() const {
    return id;
}

void SerializableItem::setId(const QString &newId) {
    id = newId;
}

int SerializableItem::getVersion()
{
    return version;
}

void SerializableItem::setVersion(int newVersion)
{
    version = newVersion;
}

qreal SerializableItem::x() const { return m_x; }
void SerializableItem::setX(qreal x) { m_x = x; }

qreal SerializableItem::y() const { return m_y; }
void SerializableItem::setY(qreal y) { m_y = y; }

void SerializableItem::setObserver(Observer *observer)
{
    this->observer = observer;
}

void SerializableItem::notifyItemAdded()
{
    if (observer)
    {
        observer->onItemAdded(*this);
    }
}

void SerializableItem::notifyItemModified()
{
    if (observer)
    {
        observer->onItemModified(*this);
    }
}

void SerializableItem::notifyItemDeleted()
{
    if (observer)
    {
        observer->onItemDeleted(*this);
    }
}
