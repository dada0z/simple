#include "mime.h"
#include <stddef.h>
#include <string.h>
typedef struct {
    const char* extension;
    const char* mime_type;
} mime_type_t;
mime_type_t mimeMap[] = {{".html", "text/html"},
                         {".xml", "text/xml"},
                         {".xhtml", "application/xhtml+xml"},
                         {".txt", "text/plain"},
                         {".rtf", "application/rtf"},
                         {".pdf", "application/pdf"},
                         {".png", "image/png"},
                         {".gif", "image/gif"},
                         {".jpg", "image/jpeg"},
                         {".jpeg", "image/jpeg"},
                         {".mpeg", "video/mpeg"},
                         {".avi", "video/x-msvideo"},
                         {".tar", "application/x-tar"},
                         {".css", "text/css"},
                         {".js", "text/javascript"},
                         {".csv", "text/csv"},
                         {".doc", "application/msword"},
                         {".json", "application/json"},
                         {".svg", "image/svg+xml"}};
const char* get_file_mime_type(const char* extension) {
    if (extension == NULL) {
        return "application/octet-stream";
    }
    int i;
    size_t len = sizeof(mimeMap) / sizeof(mimeMap[0]);
    for (i = 0; i < len; ++i) {
        if (strcmp(extension, mimeMap[i].extension) == 0)
            return mimeMap[i].mime_type;
    }
    return "application/octet-stream";
}