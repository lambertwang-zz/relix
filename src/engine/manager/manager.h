#ifndef __MANAGER_H_
#define __MANAGER_H_

namespace rlx {
    class Manager {
    private:
        static Manager *instance;
    public:
        static Manager *getInstance() {
            return instance;
        }
    };
}

#endif
