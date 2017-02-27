#ifdef ODT_HELPER

#include "../Headers/HelperODT.h"
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QDebug>
#include "../../quazip-0.7.2/quazip/JlCompress.h"

#define OPEN_TAG    "##!"
#define CLOSE_TAG   "!##"

HelperODT::HelperODT(){
    _pos = 0;
    _work_dir = new QTemporaryDir();
    if(!_work_dir->isValid()){
        qDebug()<<"Error while create temporary directory";
    }
}
HelperODT::~HelperODT(){
    delete _work_dir;
    _lskeyword.clear();
    foreach (TAG* iter, _keywordpos.values()) {
        delete iter;
    }
    _keywordpos.clear();
}
HelperODT::TAG* HelperODT::token(QTextStream& stream,bool &endfile){
    QString templ;
    QString tag;
    QString ch;
    int state = 1;

    TAG* ret = new TAG();

    while(ch=stream.read(1), !(stream.atEnd())){
        templ+=ch;
        if(templ.size()<3) {
            if(state==2){
                tag+=ch;
            }
            _pos++;
            continue;
        }
        if(state==1){
            if(templ.compare(OPEN_TAG)==0){
                ret->_start_pos = _pos-2;
                templ = "";
                state = 2;
            }else{
                templ = templ.remove(0,1);
            }
        }else if(state==2){
            tag+=ch;
            if(templ.compare(CLOSE_TAG)==0){
                ret->_end_pos = _pos;
                ret->_tag = tag.remove(tag.size()-3,3);
                state = 3;
                _pos++;
                break;
            }else{
                templ = templ.remove(0,1);
            }
        }

        _pos++;
    }
    endfile = stream.atEnd();
    if(state!=3){
        delete ret;
        ret = 0;
    }
    return ret;
}
void HelperODT::init()throw (CommonException::FileNotFoundException){
    QString file_name = _work_dir->path()+QDir::separator()+"content.xml";
    QFile content(file_name);
    if(!content.open(QIODevice::ReadOnly | QIODevice::Text)) throw CommonException::FileNotFoundException(file_name);
    QTextStream stream(&content);
    stream.setCodec("UTF-8");
    TAG* tag = 0;
    bool endfile;
    while(tag = token(stream,endfile), !endfile){
        if(tag!=0){
            _keywordpos[tag->_tag] = tag;
            _lskeyword<<tag;
        }
    }
    content.close();
}
void HelperODT::set(const QString& tag,const QString& text){
    if(!_keywordpos.contains(tag)){
        qDebug()<<"Tag '"<<tag<<"' not found";
        return;
    }
    _keywordpos[tag]->_replace = text;
}

HelperODT *HelperODT::open(const QString &file)throw(CommonException::FileNotFoundException,CommonException::FileFormatException,CommonException::ZipError){
    HelperODT* ret = new HelperODT();
    // проверка на существование файла
    QFile f(file);
    if(!f.exists()) throw CommonException::FileNotFoundException("File '"+file+"' not found");

    // распаковываем во временный каталог
    QStringList lsFile = JlCompress::extractDir(file, ret->_work_dir->path());
    if(lsFile.isEmpty()) throw CommonException::ZipError("Error while unzip odt file");

    // создаем карту ключей вставки
    ret->init();

    return ret;
}

bool HelperODT::write(const QString& file_name ){
    // открываем контент и делаем новый файл
    QString src_file = _work_dir->path()+QDir::separator()+"content.xml";
    QFile fsrc(src_file);
    if(!fsrc.open(QIODevice::ReadOnly | QIODevice::Text)) throw CommonException::FileNotFoundException(src_file);
    QTextStream ssrc(&fsrc);
    ssrc.setCodec("UTF-8");


    QString dest_file = _work_dir->path()+QDir::separator()+"~content.xml";

    QFile fdest(dest_file);
    if(!fdest.open(QIODevice::ReadWrite | QIODevice::Text)) throw CommonException::FileNotFoundException(dest_file);
    QTextStream sdest(&fdest);
    sdest.setCodec("UTF-8");

    qint64 pos = 0 ;
    foreach (TAG* iter, _lskeyword) {
        QString data = ssrc.read(iter->_start_pos - pos);
        ssrc.read(iter->_end_pos-iter->_start_pos+1);
        sdest<<data<<iter->_replace;
        pos = iter->_end_pos+1;

    }
    QString tail = ssrc.readAll();
    sdest<<tail;

    fsrc.remove();
    fsrc.close();
    fdest.rename(src_file);
    fdest.close();
    // сейвим
    return JlCompress::compressDir(file_name,_work_dir->path());
}

#endif // #ifdef ODT_HELPER
