#include "projectmodel.h"
#include <QUuid>

ProjectModel::ProjectModel(QString name, QString id, bool isPublished, QObject* parent) 
: QObject(parent), m_id(QUuid::createUuid().toString()), m_name(name), m_isPuplished(isPublished) {
    if (!id.isEmpty()) m_id = id;
}

void ProjectModel::addModel(SerializableItem* model) {
    if (!model) return;

    QString modelId = model->getId();
    if (!m_models.contains(modelId)) {
        model->setParent(this);
        m_models.insert(modelId, model);
        emit modelAdded(model);
    }
}

void ProjectModel::removeModel(SerializableItem* model) {
    if (!model) return;

    QString modelId = model->getId();
    if (m_models.contains(modelId)) {
        SerializableItem* removedModel = m_models.take(modelId);
        emit modelRemoved(removedModel);
        removedModel->setParent(nullptr);
        delete removedModel;
    }
}

void ProjectModel::updateModel(SerializableItem* model) {
    if (!model) return;

    QString modelId = model->getId();
    if (m_models.contains(modelId)) {
        SerializableItem* oldModel = m_models.value(modelId);
        oldModel->deserialize(model->serialize());
        emit modelUpdated(model);
    }
}