#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <list>

#include <sys/stat.h>
#include <dlfcn.h>
#include <unistd.h>
#include <signal.h>
#include "utils.h"
#include "mods_impl.h"

#define USE_LIBS
#define USE_MODS

#ifndef MAJOR
#define MAJOR   1
#endif

#ifndef MINOR
#define MINOR   0
#endif

#ifndef PATCH
#define PATCH   0
#endif

#ifdef  USE_LIBS
#ifndef LIBS_PATH
#define LIBS_PATH   "../libs"
#endif
#endif

#ifdef USE_MODS
#ifndef MODS_PATH
#define MODS_PATH   "../mods"
#endif
#endif

typedef std::vector<void *> ptrVec_t;
void dlopen_walk(const char *path, void *arg);
void dlsym_walk(ptrVec_t &vec, mods_impl_lst_t &lst);
void exit_handler(int signum);

#ifdef USE_LIBS
ptrVec_t libs_vec;
mods_impl_lst_t libs_impl_lst;
#endif

#ifdef USE_MODS
ptrVec_t mods_vec;
mods_impl_lst_t mods_impl_lst;
#endif

int main(int argc, char **argv)
{
    mods_impl_lst_t *libs_lst = NULL;
    mods_impl_lst_t *mods_lst = NULL;

    // load all dynamic
    #ifdef USE_LIBS
    dir_walk(LIBS_PATH, dlopen_walk, &libs_vec);
    dlsym_walk(libs_vec, libs_impl_lst);
    libs_lst = &libs_impl_lst;
    #endif

    #ifdef USE_MODS
    dir_walk(MODS_PATH, dlopen_walk, &mods_vec);
    dlsym_walk(mods_vec, mods_impl_lst);
    mods_lst = &mods_impl_lst;
    #endif

    //TODO sort list

    // Set list and startup
    #ifdef USE_LIBS
    for (auto it:libs_impl_lst) {
        ((mods_impl*)it.ptr)->set_lst(libs_lst, mods_lst);
        ((mods_impl*)it.ptr)->startup();
    }
    #endif

    #ifdef USE_MODS
    for (auto it:mods_impl_lst) {
        ((mods_impl*)it.ptr)->set_lst(libs_lst, mods_lst);
        ((mods_impl*)it.ptr)->startup();
    }
    #endif

    signal(SIGINT, exit_handler);
    signal(SIGTERM, exit_handler);

    while(1) sleep(1);

    return 0;
}

void exit_handler(int signum)
{
    #ifdef USE_MODS
    for (auto it:mods_impl_lst) {
        ((mods_impl*)it.ptr)->stop();
        it.deleter(it.ptr);
    }
    for(auto it = mods_vec.begin(); it != mods_vec.end(); it++) {
        dlclose(*it);
    }
    #endif

    #ifdef USE_LIBS
    for (auto it:libs_impl_lst) {
        ((mods_impl*)it.ptr)->stop();
        it.deleter(it.ptr);
    }
    for(auto it = libs_vec.begin(); it != libs_vec.end(); it++) {
        dlclose(*it);
    }
    #endif

    if (signum == SIGINT) printf("\n");
    printf("Exit with sig: %d\n", signum);
    exit(0);
}

void dlsym_walk(ptrVec_t &vec, mods_impl_lst_t &lst)
{
    creater_fn_t createfn = NULL;
    deleter_fn_t deletefn = NULL;
    mod_impl_item item;

    if (vec.size() == 0) {
        printf("No dynamic\n");
        return;
    }

    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        item.name = "";
        item.prio = 0;
        item.ptr  = NULL;
        // get creater and deleter function
        createfn = (creater_fn_t) dlsym(*it, "creater");
        deletefn = (deleter_fn_t) dlsym(*it, "deleter");
        if (createfn == NULL || deletefn == NULL) {
            // printf("Can't find creater or deleter\n");
            continue;
        }

        // create class and get priority(exec seq)
        item.ptr = (mods_impl *) createfn(&item.prio);
        if (item.ptr == NULL)return;

        // check version
        if (! ((mods_impl *)item.ptr)->ver_check(MAJOR, MINOR, PATCH)) {
            printf("version check fail\n");
            deletefn(item.ptr);
            continue;
        }

        // get name
        item.name = ((mods_impl *)item.ptr)->name();
        item.deleter = deletefn;
        lst.push_back(item);
    }
}

void dlopen_walk(const char *path, void *arg)
{
    ptrVec_t &vec = *((ptrVec_t *)arg);
    void *handle = NULL;
    bool rd_permission = false;

    rd_permission = access(path, R_OK) == 0 ? true : false;
    if (!rd_permission) return;

    handle = dlopen(path, RTLD_LAZY);
    if (handle) {
        vec.push_back(handle);
    }
}
