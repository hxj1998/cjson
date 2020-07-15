#include<iostream>
#include<string>
#include<vector>
using namespace std;
typedef enum  {
    JSON_NONE,
    JSON_BOL,           //BOOL类型
    JSON_NUM,           //数值类型
    JSON_STR,           //字符串类型
    JSON_ARR,           //数组类型
    JSON_OBJ,           //对象类型
} json_e;
typedef struct JSON JSON;

JSON *json_new(json_e type);			//新建一个类型为typeJSON对象
JSON *json_new_num(double val);			//根据输入参数创建一个新的JSON，其中json_e对应JSON_NUM
JSON *json_new_bool(bool val);			//根据输入参数创建一个新的JSON，其中json_e对应JSON_BOL
JSON *json_new_str(string str);			//根据输入参数创建一个新的JSON，其中json_e对应JSON_STR
JSON *json_new_array(vector<JSON*> arr);//根据输入参数创建一个新的JSON，其中json_e对应JSON_ARR
JSON *json_new_obj(string key,JSON *obj);	//根据输入参数创建一个新的JSON，其中json_e对应JSON_OBJ

json_e json_type(const JSON *json);		//获得对应JSON的数据类型
void json_free(JSON *json);				//释放对应JSON的内存
void json_print(const JSON *json);		//打印当前json结构
int json_save(const JSON *json, string fname);//将JSON信息保存到fname中

JSON *json_get_member(JSON *json, string key,bool parent);//查找是否存在key值
JSON *json_get_element(JSON *json, unsigned int idx);//获得JSON下的对应数组成员编号

JSON *json_add_member(JSON *json, string key, JSON *val,bool compatible);//在当前JSON添加一个键值对
JSON *json_add_element(JSON *json, JSON *val,bool compatible);//在当前JSON添加一个JSON

int json_arr_count(const JSON *json); //计算JSON中的数组元素个数

JSON* json_obj_set(JSON *json, string key, JSON* val);//修改当前JSON对象对应键值的数值

JSON* json_arr_set(JSON *json,unsigned int idx,JSON* val);//修改当前JSON对象数组对应的值

int json_obj_del(JSON *json,string key);//删除键值对应的对象
int json_arr_del(JSON *json,unsigned int idx);//删除数组指定下标的值
