#include "../HeadersInt/helper.h"
#include "../CustomServer/BaseTransport/base_server.h"
QString parseUser(const QString& v){
    QString ret ="";
    QString descr = v;
    int idx_name = descr.indexOf("name");
    if(idx_name<0) return ret;
    int idx_sep = descr.indexOf(":",idx_name);
    if(idx_sep<0) return ret;
    bool bFind = false;
    int k=0;
    for(int i=idx_sep;i<descr.length();i++){
        QChar ch = descr.at(i);
        if(ch=='"'){
            if(k==0){
                bFind = true;
                k++;
                continue;
            }
            if(k==1){
                break;
            }
        }
        if(bFind){
            ret+=ch;
        }

    }
    return ret;
}


//#ifdef W_OS_LINUX
//    void start() __attribute__ ((constructor));
//    void finish() __attribute__ ((destructor));

//    void start() { BaseServerHolder::Instance(); }
//    void finish() {  }
//#else
//    #include <windows.h>
//    BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved ){
//        BaseServerHolder::Instance();
//    }
//#endif
