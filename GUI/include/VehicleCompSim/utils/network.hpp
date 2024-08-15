#include <unordered_map>
#include <unordered_set>

template <typename TElement, typename TConnector>
class Network {
    std::unordered_map<TElement*, std::unordered_set<TConnector*>> elements;
    std::unordered_map<TConnector*, std::unordered_set<TElement*>> connectors;

public:
    void addElement(TElement* el);
    void addConnector(TConnector* cn);
    
    void connect(TElement* element, TConnector* connector);
    void disconnect(TElement* element, TConnector* connector);
    
    bool contains(TElement* el);
    bool contains(TConnector* cn);
    
    std::unordered_set<TConnector*>& getConnectors(TElement* el);
    std::unordered_set<TElement*>& getElements(TConnector* cn);
};

// Definitions

template <typename TElement, typename TConnector>
void Network<TElement, TConnector>::addElement(TElement* el) {
    elements[el]; 
}

template <typename TElement, typename TConnector>
void Network<TElement, TConnector>::addConnector(TConnector* cn) {
    connectors[cn]; 
}

template <typename TElement, typename TConnector>
void Network<TElement, TConnector>::connect(TElement* element, TConnector* connector) {
    if (!contains(element) || !contains(connector)) return;
    elements[element].insert(connector);
    connectors[connector].insert(element);
}

template <typename TElement, typename TConnector>
void Network<TElement, TConnector>::disconnect(TElement* element, TConnector* connector) {
    if (!contains(element) || !contains(connector)) return;
    elements[element].erase(connector);
    connectors[connector].erase(element);
}

template <typename TElement, typename TConnector>
bool Network<TElement, TConnector>::contains(TElement* el) {
    return elements.find(el) != elements.end();
}

template <typename TElement, typename TConnector>
bool Network<TElement, TConnector>::contains(TConnector* cn) {
    return connectors.find(cn) != connectors.end();
}

template <typename TElement, typename TConnector>
std::unordered_set<TConnector*>& Network<TElement, TConnector>::getConnectors(TElement* el) {
    return elements[el];
}

template <typename TElement, typename TConnector>
std::unordered_set<TElement*>& Network<TElement, TConnector>::getElements(TConnector* cn) {
    return connectors[cn];
}