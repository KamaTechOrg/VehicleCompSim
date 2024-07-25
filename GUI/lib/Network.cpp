#include "Network.hpp"

template<typename TElement, typename TConnector>
inline void Network<TElement, TConnector>::addElement(TElement* el)
{
	elements.insert(el);
}

template<typename TElement, typename TConnector>
void Network<TElement, TConnector>::addConnector(TConnector* cn)
{
	connectors.insert(cn);
}

template<typename TElement, typename TConnector>
void Network<TElement, TConnector>::connect(TElement* element, TConnector* connector)
{
	if (!contains(element) || !contains(connector)) return;
	elements[element].insert(connector);
	connectors[connector].insert(element);
}

template<typename TElement, typename TConnector>
void Network<TElement, TConnector>::disconnect(TElement* element, TConnector* connector)
{
	if (!contains(element) || !contains(connector)) return;
	elements[element].erase(connector);
	connectors[connector].erase(element);
}

template<typename TElement, typename TConnector>
bool Network<TElement, TConnector>::contains(TElement* el)
{
	return elements.find(el) != elements.end();
}

template<typename TElement, typename TConnector>
bool Network<TElement, TConnector>::contains(TConnector* cn)
{
	return connectors.find(cn) != connectors.end();
}

template<typename TElement, typename TConnector>
std::unordered_set<TConnector*>& Network<TElement, TConnector>::getConnectors(TElement* el)
{
	return elements[el];
}

template<typename TElement, typename TConnector>
std::unordered_set<TElement*>& Network<TElement, TConnector>::getElements(TConnector* cn)
{
	return connectors[cn];
}


