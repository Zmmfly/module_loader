#include "utils.h"
#include <string>

#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/stat.h>

int dir_walk(const char *path, walk_fn_t fn, void *arg)
{
    std::string base_path, file_path;
    DIR *dir;
    struct dirent *ptr;
    if (path == NULL || fn == NULL) return -1;
    if (!is_dir(path)) return -2;
    base_path = path;

    dir = opendir(path);
    if (dir == NULL)return -3;

    while( (ptr = readdir(dir)) != NULL ) {
        if (ptr->d_type != DT_REG) continue;
        file_path = base_path + "/";
        file_path += ptr->d_name;
        fn(file_path.c_str(), arg);
    }

    closedir(dir);

    return 0;
}

bool is_dir(const char *path)
{
    struct stat st;
    int ret = stat(path, &st);
    if (ret != 0) return false;
    return S_ISDIR(st.st_mode) ? true : false;
}

bool is_file(const char *path)
{
    struct stat st;
    int ret = stat(path, &st);
    if (ret != 0) return false;
    return S_ISREG(st.st_mode) ? true : false;
}
