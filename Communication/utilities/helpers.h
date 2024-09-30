#include <mutex>

class Locker {
    public:
    Locker (std::mutex &mut):m_mut{mut}{m_mut.lock();}
    ~Locker(){m_mut.unlock();}

    private:
    std::mutex & m_mut;
    
};