#pragma once

#include "items/serializableitem.h"

class SerializableItem;

class Observer {
public:
	virtual ~Observer() = default;
	virtual void onItemModified(const SerializableItem& item) = 0;
	virtual void onItemAdded(const SerializableItem& item) = 0;
	virtual void onItemDeleted(const SerializableItem& item) = 0;
};
