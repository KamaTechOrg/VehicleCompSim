#include "serializableitem.h"
#include <QUuid>
#include "client/observerconfig.h"

SerializableItem::SerializableItem() :
version(0), id(QUuid::createUuid().toString()), observer(ObserverConfig::getObserver())
{
}

QJsonObject SerializableItem::serialize() const {
    QJsonObject itemData;
    itemData["id"] = id;
    itemData["version"] = version;
    return itemData;
}

void SerializableItem::deserialize(const QJsonObject &itemData) {
    id = itemData["id"].toString();
    version = itemData["version"].toInt();
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

void SerializableItem::incrementVersion() {
    version++;
}

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
