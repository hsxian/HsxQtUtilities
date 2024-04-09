#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H

#include <QString>

#define copyOther(name)\
    name  = other.name

#define copyOtherFunc(name)\
    set##name(other.get##name())

#define PropertyBuilderByName(type,name,access_permission)\
    access_permission:\
    type m_##name;\
    public:\
    inline void set##name(type v){ m_##name=v;}\
    inline type get##name() const {return m_##name;}\

#define PropertyBuilderByNameAndInit(type,name,access_permission,init_value)\
    access_permission:\
    type m_##name = init_value;\
    public:\
    inline void set##name(type v){ m_##name=v;}\
    inline type get##name() const{return m_##name;}\

#define GetPropertyBuilderByName(type,name,access_permission)\
    access_permission:\
    type m_##name;\
    public:\
    inline type get##name()const{return m_##name;}\
    /*----------------------------------------
        *@brief      指针类型成员变量GetSet自动生成
        *@param[in]  type  类型
        *@param[in]  name  变量名
        *@param[in]  access_permission  权限
        *@return     void
        *@date       20210107
    ----------------------------------------*/
#define PointPropertyBuilderByName(type,name,access_permission)\
    access_permission:\
    type* m_##name =  0;\
    public:\
    inline void set##name(type* v){ m_##name=v;}\
    inline type* get##name()const{return m_##name;}\

enum tribool : int {False = 0, True = 1, Unknow = 2};

#endif // COMMONDEFINE_H
