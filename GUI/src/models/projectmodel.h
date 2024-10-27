#pragma once

#include <QObject>
#include <QVector>
#include "serializableitem.h"

class ProjectModel : public QObject {
    Q_OBJECT

public:
    explicit ProjectModel(QString name, QString id = "", bool isPublished = false, QObject* parent = nullptr);

    void addModel(SerializableItem* model);
    void removeModel(SerializableItem* model);
    void updateModel(SerializableItem* model);

    QString id() const { return m_id; }
    QString name() const { return m_name; }
    bool isPublished() const { return m_isPuplished; }
    void setPublished(bool value) { m_isPuplished = value; }
    
    QList<SerializableItem*> models() const { return  m_models.values(); }

signals:
    void modelAdded(SerializableItem* model);
    void modelRemoved(SerializableItem* model);
    void modelUpdated(SerializableItem* model);

private:
	QString m_id;
    QString m_name;
    bool m_isPuplished;
    QHash<QString, SerializableItem*> m_models;
};
