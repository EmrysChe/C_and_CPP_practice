/************************
 * 目录过深会导致出现错误 *
 ************************/ 

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/stat.h>

#include "cJSON.h"

#define CURRENT_DIR "./"

int getFileStat(const char *path,struct stat *file_stat)
{
    if(lstat(path,file_stat) < 0)
        return -1;
    return 0;
}

int getFileStatToJSON(const char *file_name,cJSON *s_json_array)
{
    struct stat file_stat;
    char buf[1024] = {0};

    strncat(buf,file_name,strlen(file_name));

    if(getFileStat(buf,&file_stat) < 0)
    {
        printf("%s %d:getFileStat failed\n",__FILE__,__LINE__);
        return -1;
    }
    cJSON *s_json_obj;
    cJSON_AddItemToArray(s_json_array,s_json_obj = cJSON_CreateObject());

    cJSON_AddItemToObject(s_json_obj,"file_name",cJSON_CreateString(basename(buf)));
    cJSON_AddItemToObject(s_json_obj,"last_access",cJSON_CreateNumber(file_stat.st_atime));
    cJSON_AddItemToObject(s_json_obj,"last_modification",cJSON_CreateNumber(file_stat.st_mtime));
    cJSON_AddItemToObject(s_json_obj,"last_status_change",cJSON_CreateNumber(file_stat.st_ctime));
    cJSON_AddItemToObject(s_json_obj,"size",cJSON_CreateNumber(file_stat.st_size));

    return 0;
}

int getFileInfoFromDir(const char *dir_name,cJSON *s_json_array)
{
    int value = 1;
    DIR *dir_fd;
    struct dirent *s_dir_info;
    if ((dir_fd = opendir(dir_name)) == NULL)
    {
        return -1;
    }
    while((s_dir_info = readdir(dir_fd)) != NULL)
    {
        if(strncmp(s_dir_info->d_name,".",strlen(s_dir_info->d_name)) == 0 || strncmp(s_dir_info->d_name,"..",strlen(s_dir_info->d_name)) == 0)
            continue;

        if(s_dir_info->d_type & DT_DIR)
        {
            char child_dir[1024] = {0};
            cJSON *s_new_dir_json_obj;
            cJSON_AddItemToArray(s_json_array,s_new_dir_json_obj = cJSON_CreateObject());
            cJSON *s_new_dir_json_array;
            cJSON_AddItemToObject(s_new_dir_json_obj,s_dir_info->d_name,s_new_dir_json_array = cJSON_CreateArray());

            if(strncmp(dir_name,CURRENT_DIR,strlen(dir_name)) == 0)
                snprintf(child_dir,sizeof(child_dir) - 1,"./%s",s_dir_info->d_name);
            else
                snprintf(child_dir,sizeof(child_dir) - 1,"%s/%s",dir_name,s_dir_info->d_name);
            // printf("%s\n",child_dir);

            if(getFileInfoFromDir(child_dir,s_new_dir_json_array) < 0)
            {
                printf("%s %d:getFileInfoFromDir failed\n",__FILE__,__LINE__);
            }
        }
        else
        {
            char file[1024] = {0};
            // printf("%s\n",s_dir_info->d_name);
            // cJSON_AddItemToObject(s_json_array,s_dir_info->d_name,cJSON_CreateNumber(value));
            if(strncmp(dir_name,CURRENT_DIR,strlen(dir_name)) == 0)
                snprintf(file,sizeof(file) - 1,"./%s",s_dir_info->d_name);
            else
                snprintf(file,sizeof(file) - 1,"%s/%s",dir_name,s_dir_info->d_name);

            getFileStatToJSON(file,s_json_array);

            // cJSON_AddItemToArray(s_json_array,cJSON_CreateString(s_dir_info->d_name));
        }
    }
    return 0;
}

int main(int argc,char* argv[])
{
    if(argc < 2)
    {
        printf("please input dir path\n");
        return 0;
    }

    cJSON *root;
    printf("%s %d\n",__FILE__,__LINE__);
    root = cJSON_CreateObject();
    printf("%s %d\n",__FILE__,__LINE__);
    cJSON *root_array;
    cJSON_AddItemToObject(root,argv[1],root_array = cJSON_CreateArray());
    printf("%s %d\n",__FILE__,__LINE__);

    if(getFileInfoFromDir(argv[1],root_array) < 0)
    {
        printf("%s %d:getFileInfoFromDir failed\n",__FILE__,__LINE__);
    }

    char *out = NULL;
    out = (char *)malloc(strlen(cJSON_PrintUnformatted(root)));

    out = cJSON_Print(root);
    cJSON_Delete(root);
    printf("%s\n",out);
    free(out);
    return 0;
}
