#ifndef __FM_LD_IMPL__
#define __FM_LD_IMPL__

#include <string>
#include <map>
#include <vector>
#include <list>

#define LOG_LVL_INFO    0
#define LOG_LVL_WARN    1
#define LOG_LVL_ERROR   2
#define LOG_LVL_DEGUG   3
#define LOG_LVL_VERBOSE 4

typedef void (*log_fn_t)(const char *tag, const char fmt, ...);

// 返回 mods_impl 指针
typedef void *(*creater_fn_t)(int *prio);
// 删除 mods_impl 指针
typedef void (*deleter_fn_t)(void *ptr);

typedef struct mod_impl_item{
    std::string name;   //名称
    void *ptr;          //mods_impl 指针
    int prio;           //优先级
    deleter_fn_t deleter;
}mod_impl_item_t;
typedef std::list<mod_impl_item_t> mods_impl_lst_t;

class mods_impl {
    public:
        virtual std::string name() = 0;
        virtual bool ver_check(uint8_t major, uint8_t minor, uint8_t patch) = 0;
        virtual bool set_logger(log_fn_t fn) = 0;
        virtual bool set_log_lvl(uint8_t lvl) = 0;
        virtual bool set_lst(mods_impl_lst_t *libs, mods_impl_lst_t *mods) = 0;
        virtual bool startup() = 0;
        virtual bool stop() = 0;
};

#endif