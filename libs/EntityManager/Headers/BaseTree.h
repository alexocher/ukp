#ifndef BASETREE_H
#define BASETREE_H

#include <QList>
#include <QMap>
#include <QDebug>
#include "commondef.h"

template <typename K, typename T>
class BaseTree;

//typedef QList<BaseTreeItem*>::iterator TreeItemIter;
/**
 * @brief The BaseTreeItem class
 * Базавый элемент дерева
 */
template <typename K, typename T>
class ENTITYMANAGERSHARED_EXPORT  BaseTreeItem{
    friend class BaseTree<K,T>;
private:

protected:
    K m_id;                              // идентификатор
    T m_type;                            // тип узла
    BaseTreeItem<K,T> *m_Parent;         // ссылка на родительский элемент, для корневого элемента 0
    QList<BaseTreeItem<K,T>*> m_lsChild; // список ссылок на детей

    BaseTreeItem(){m_Parent = 0;}

    BaseTreeItem(const BaseTreeItem<K,T> &obj){
        this->m_id = obj.m_id;
        this->m_Parent = 0;

        typename QList<BaseTreeItem<K,T>*>::const_iterator iter;
        for(iter = obj.m_lsChild.begin(); iter!= obj.m_lsChild.end(); ++iter){
            BaseTreeItem<K,T>* newItem = (*iter)->clone();
            newItem->setParent(this);
            this->m_lsChild<<newItem;
        }
    }
    int getSize() const{return m_lsChild.size();}
    bool isEmpty() const{return m_lsChild.isEmpty();}
    BaseTreeItem<K,T> *at(int index){return m_lsChild.at(index);}
    // номер в списке родителей
    int order(){
        if(m_Parent==0) return -1;
        return m_Parent->m_lsChild.indexOf(this);
    }
    void setNodeType(const T& type){m_type = type;}
public:
    // виртуальная функция копирования
    virtual BaseTreeItem<K,T>* clone() const{return new BaseTreeItem(*this);}
    virtual ~BaseTreeItem(){
//        typename QList<BaseTreeItem<K,T>*>::iterator iter;
//        for(iter = m_lsChild.begin(); iter!= m_lsChild.end(); ++iter){
//            if(*iter!=0){
//                delete (*iter);
//            }
//        }
//        qDebug()<<"("<<QString::number(getLevel())<<")";
        qDeleteAll(m_lsChild);
        m_lsChild.clear();
    }
    BaseTreeItem(const K& id,const T& type){
        m_id = id;
        m_Parent = 0;
        m_type = type;
    }

    /**
     * @brief getID - получить идентификатор
     * @return идентификатор
     */
    const K& getID() const{return m_id;}
    /**
     * @brief getType - тип узла
     * @return
     */
    const T& getType() const{return m_type;}
    const T& getNodeType() const{return getType();}
    /**
     * @brief getLevel - получить уровень узла
     * @return номар уровня
     */
    int getLevel() const{
        if(getParent()!=0){
            return getParent()->getLevel()+1;
        }
        return 1;
    }
    /**
     * @brief begin итераторы
     * @return
     */
    typename QList<BaseTreeItem<K,T>*>::iterator begin(){return m_lsChild.begin();}
    typename QList<BaseTreeItem<K,T>*>::iterator end(){return m_lsChild.end();}

    typename QList<BaseTreeItem<K,T>*>::const_iterator cbegin()const{return m_lsChild.cbegin();}
    typename QList<BaseTreeItem<K,T>*>::const_iterator cend()const{return m_lsChild.cend();}
    /**
     * @brief getParent получить родителя
     * @return
     */
    BaseTreeItem<K,T>* getParent() const{
        return m_Parent;
    }

protected:
    void setParent(BaseTreeItem<K,T>* item){
        m_Parent = item;
    }
    /**
     * @brief addChild добавить детей
     * @param item указатель на объект
     */
    void addChild(BaseTreeItem<K,T>* item){
        if(item!=0){
            item->setParent(this);
            m_lsChild<<item;
        }
    }
    /**
     * @brief addChild добавить детей
     * @param items список детей
     */
    void addChild(QList<BaseTreeItem<K,T>*> items){
        typename QList<BaseTreeItem<K,T>*>::iterator iter;
        for(iter = items.begin();iter!=items.end();++iter){
            addChild(*iter);
        }
    }
    /**
     * @brief remChild удалить дочерний узел (удаляется без очистки памяти)
     * @param item
     */
    void remChild(BaseTreeItem<K,T>* item){
        if(item!=0){
            m_lsChild.removeAll(item);
        }
    }
    /**
     * @brief remAllChild удалить всех детей
     */
    void remAllChild(){
        m_lsChild.clear();
    }

};
/**
 * @brief The BaseTree class
 * Класс дерева, хранит корневой элемент и предаставляет базовый функционал
 */

template <typename K, typename T>
class ENTITYMANAGERSHARED_EXPORT BaseTree{
protected:
    BaseTreeItem<K,T> *m_RootItem; // корневой элемент
    QMap<K,BaseTreeItem<K,T>*> m_Map; // карта для быстрого поиска
protected:
    void setRootItem(BaseTreeItem<K,T> *m_RootItem){
        this->m_RootItem = m_RootItem;
        m_Map[m_RootItem->getID()]=m_RootItem;
    }
public:
    BaseTree(BaseTreeItem<K,T> *m_RootItem){
        if(m_RootItem!=0){
            this->m_RootItem = m_RootItem;
            m_Map[m_RootItem->getID()]=m_RootItem;
        }
    }
    virtual ~BaseTree(){
        if(m_RootItem!=0){
            delete m_RootItem;
            m_RootItem = 0;
        }
//        qDeleteAll(m_Map);
        m_Map.clear();
    }
    /**
     * @brief add добавить элемент
     * @param parent - родитель
     * @param item - новый элемент
     */
    virtual void add(BaseTreeItem<K,T> *parent,BaseTreeItem<K,T> *item){
        if(parent==0 || item==0) return;
        parent->addChild(item);
        item->setParent(parent);

        m_Map[item->getID()]=item;
    }
    /**
     * @brief rem удалить элемент
     * @param item - удаляемый элемент
     * @param free_mem - очистка памяти
     */
    virtual void rem(BaseTreeItem<K,T> *item,bool free_mem = true){
        if(item!=0){
            if(item->getParent()!=0){
                item->getParent()->remChild(item);
            }
            m_Map.remove(item->getID());
            // Удалять из памяти
            if(free_mem){
                delete item;
                item = 0;
            }
        }
    }
    /**
     * @brief rem удалить элемент
     * @param id - идентификатор
     */
    void rem(const K& id){
        BaseTreeItem<K,T> *item = find(id);
        if(item!=0){
            rem(item);
        }
    }
    /**
     * @brief getRoot получить корневой элемент
     * @return
     */
    virtual BaseTreeItem<K,T> *getRoot() const{
        return m_RootItem;
    }
    /**
     * @brief move перенести узел
     * @param parent - родитель
     * @param item - узел
     */
    virtual void move(BaseTreeItem<K,T> *parent,BaseTreeItem<K,T> *item){
        if(item==0)return;

        if(item->getParent()!=0){
            item->getParent()->remChild(item);
        }

        item->setParent(parent);

        if(parent==0){
            m_RootItem = item;
        }else{
            parent->addChild(item);
        }
    }
    /**
     * @brief find Поиск по идентификатору
     * @param id - идентификатор
     * @return указатель на объект
     */
    virtual BaseTreeItem<K,T>* find(const K& id) const{
        if(m_Map.contains(id)){
            return m_Map[id];
        }
        return 0;

    }
    /**
     * @brief testPrint вывод в строку дерева со структурой
     * @return строка с форматированием
     */
    QString testPrint() const{
        #ifdef QT_DEBUG
        QString ret = "";
        QStack<BaseTreeItem<K,T>*> stack;
        typename QList<BaseTreeItem<K,T>*>::iterator iter;
        stack.push(m_RootItem);
        while(!stack.isEmpty()){
            BaseTreeItem<K,T>* cur = stack.pop();
            for(int i=1;i<cur->getLevel();i++){
                ret+="----";
            }
            ret+=" "+cur->getID()+"\r\n";
            for(iter = cur->begin();iter!=cur->end();++iter){
                stack.push(*iter);
            }

        }

        return ret;
        #else
        return "";
        #endif
    }
    /*!
     * \brief Сериалтзация
     * \return массив байт
     */
    virtual QByteArray serialize() const = 0;
    /*!
     * \brief Десериализация
     * \param data - массив байт
     * \return код статуса
     */
    virtual int deserialize(const QByteArray& data) = 0;
};

#endif // BASETREE_H
