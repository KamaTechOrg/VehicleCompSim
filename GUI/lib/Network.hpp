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


