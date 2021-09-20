#include "mods_impl.h"

class mod_parse: public mods_impl
{
    public:
        std::string name() {
            return "parse";
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
            printf("startup parse\n");
            return true;
        }

        bool stop() {
            printf("stop parse\n");
            return true;
        }
};

extern "C" void *creater(int *prio)
{
    if (prio == NULL) return NULL;
    *prio = 2;
    return new mod_parse();
}

extern "C" void deleter(void *ptr)
{
    delete (mod_parse *)ptr;
}