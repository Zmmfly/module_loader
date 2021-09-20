#include "mods_impl.h"

class mod_cache: public mods_impl
{
    public:
        std::string name() {
            return "cache";
        }

        bool ver_check(uint8_t major, uint8_t minor, uint8_t patch) {
            return true;
        }

        bool set_logger(log_fn_t fn) {
            return true;
        }

        bool set_log_lvl(uint8_t lvl) {
            return true;
        }

        bool set_lst(mods_impl_lst_t *libs, mods_impl_lst_t *mods) {
            return true;
        }

        bool startup() {
            printf("startup cache\n");
            return true;
        }

        bool stop() {
            printf("stop cache\n");
            return true;
        }
};

extern "C" void *creater(int *prio)
{
    if (prio == NULL) return NULL;
    *prio = 1;
    return new mod_cache();
}

extern "C" void deleter(void *ptr)
{
    delete (mod_cache *)ptr;
}
