#include "cjson_hxj.h"
struct JSON{
	json_e type;
	vector<JSON*> value_array; 	//JSON_ARR OR JSON_OBJ
	string key;			//如果是OBJ则存储key值
	bool value_bool;		//JSON_BOL
	double value_double;		//JSON_NUM
	string value_string;		//JSON_STR
};
/*
int main()
{
	//json 构建
	//构建"basic":{}
	JSON* js_tmp=json_new(JSON_NONE);
	JSON* js_basic=json_new_obj("basic",js_tmp);
	
	//在basic中添加一个json（key：bool数值）------->"basic":{"enable":true}
	js_tmp=json_new_bool(true);
	json_add_member(js_basic,"enable",js_tmp,true);
	
	//在basic中添加一个json(key：字符串数值)------->"basic":{"enable":true}
	js_tmp=json_new_str("200.200.3.61");
	json_add_member(js_basic,"ip",js_tmp,true);

	//在basic中添加一个json（key：double数值）------->"basic":{"enable":"200.200.3.61","port":389}
	js_tmp=json_new_num(389);
	json_add_member(js_basic,"port",js_tmp,true);

	//在basic中添加一个json（key：字符串数组）------->"basic":{"enable":"200.200.3.61","port":389,"dns":["200.200.0.1","200.0.0.254"]}
	js_tmp=json_new_str("200.200.0.1");
	JSON* js_tmp2=json_new_str("200.0.0.254");
	json_add_element(js_tmp,js_tmp2,true);
	json_add_member(js_basic,"dns",js_tmp,true);

	//打印结果，释放内存
	json_print(js_basic);
	json_free(js_basic);
	json_free(js_tmp2);
	json_free(js_tmp);
	return 0;

}
*/
JSON *json_new(json_e type)//分配内存本身为空
{
	JSON *js= (JSON*)malloc(sizeof(JSON));
	js->type = type;
	return js;
}

JSON *json_new_num(double val)//根据输入参数创建一个新的JSON>，其中json_e对应JSON_NUM
{
	JSON *js=json_new(JSON_NUM);
	js->value_double=val;
	return js;
}

JSON *json_new_bool(bool val)//根据输入参数创建一个新的JSON>，其中json_e对应JSON_BOL
{
	JSON *js=json_new(JSON_BOL);
	js->value_bool=val;
	return js;
}

JSON *json_new_str(string str)//根据输入参数创建一个新的JSON>，其中json_e对应JSON_STR
{
	JSON *js=json_new(JSON_STR);
	js->value_string=str;
	return js;
}

JSON *json_new_array(vector<JSON*> arr)//根据输入参数创建一个新的JSON，其中json_e对应JSON_ARR
{
	JSON *js=json_new(JSON_ARR);
	js->value_array=arr;
	return js;
}

JSON *json_new_obj(string key,JSON *obj)//根据输入参数创建一个新的JSON>，其中json_e对应JSON_OBJ
{
        JSON *js=json_new(JSON_OBJ);
	js->key=key;
        js->value_array.push_back(obj);
	return js;
}
json_e json_type(const JSON *json)//获得对应JSON的数据类型
{
	return json->type;
}
void json_free(JSON *json)//释放对应JSON的内存
{
	if(json_type(json)==JSON_ARR||json_type(json)==JSON_OBJ)
	{
		for(auto i:json->value_array)
		{
			json_free(i);
		}
	}
	free(json);
	json=NULL;
}

void json_print(const JSON *json)//打印JSON节点的结构
{
	switch(json_type(json))
	{
		case JSON_NONE:
			cout<<"NULL";
			break;
		case JSON_NUM:
			cout<<json->value_double;
			break;
		case JSON_BOL:
                        cout<<json->value_bool;
                        break;
		case JSON_STR:
			cout<<"\"";
                        cout<<json->value_string;
			cout<<"\"";
                        break;
		case JSON_OBJ://该json满足obj格式，输出关键字，递归判断数值
			cout<<"\""<<json->key<<"\""<<":";
			json_print(json->value_array[0]);
                        break;
		case JSON_ARR://判断是否有key值，有则先输出键值，再对数组进行遍历输出
			if(json->key.length())
			{
				cout<<"\""<<json->key<<"\":{";
				for(auto i:json->value_array)
				{
					json_print(i);
					if(i!=json->value_array.back())
                                                cout<<",";
                                }
				cout<<"}";
			}
			else
			{
				cout<<"[";
				for(auto i:json->value_array)
				{
					json_print(i);
					if(i!=json->value_array.back())
						cout<<",";
				}
				cout<<"]";
			}
			break;
	}
}
int json_save(const JSON *json, string fname);//将JSON信息保存到fname中

JSON *json_get_member(JSON *json, string key)//查找是否存在key值
{
	if(json_type(json)==JSON_OBJ)//判断json结构是否可能有key
	{
		if(json->key==key)//找到key返回，没找到则对下一层的json进行判断
			return json;
		else
			return json_get_member(json->value_array[0],key);
	}
	else if(json_type(json)==JSON_ARR)
	{
		if(json->key==key)
			return json;
		else
		{
			int count=0;
			for(auto i:json->value_array)
			{
				count++;
				JSON *j=json_get_member(i,key);
				if(j!=NULL)//断言，找到key>则返回，不遍历之后数据
					return j;
			}
		}
	}
	return NULL;
}

JSON *json_get_element(JSON *json, unsigned int idx)//获得JSON下的对应数组成员编号
{
	if(json_type(json)==JSON_ARR)//判断结构是否吻合
	{
		if(idx>=json->value_array.size())//判断是否数组越界
		{
			cout<<"error:idx is larger than array's size! array size: "<<json->value_array.size()<<endl;
			return NULL;
		}
		else
		{
			return json->value_array[idx];
		}
	}
	cout<<endl<<"error:json type error";
	return NULL;
}

JSON *json_add_member(JSON *json, string key, JSON *val,bool compatible)//当前JSON添加一个键值对
{
	if(json_get_member(json,key))//保证键值唯一性
	{
		cout<<endl<<"error:this key has existed,please rename your key";
		return NULL;
	}
	if(json_type(json)==JSON_ARR)
	{
		JSON *t=json_new_obj(key,val);
		if(json_type(json->value_array[0])!=JSON_NONE)
			json->value_array.push_back(t);
		else
			json->value_array[0]=t;
		return json;
	}
	else
	{
		if(compatible)//是否将当前json转化为数组
		{
			if(json_type(json)==JSON_BOL)
			{
				JSON *tmp=json_new_bool(json->value_bool);
				json->value_array.push_back(tmp);
			}
			else if(json_type(json)==JSON_NUM)
			{
				JSON *tmp=json_new_num(json->value_double);
                                json->value_array.push_back(tmp);
                        	json->value_double=0;
			}
			else if(json_type(json)==JSON_STR)
			{
				JSON *tmp=json_new_str(json->value_string);
                                json->value_array.push_back(tmp);
			        json->value_string.clear();
				json->value_string.shrink_to_fit();
			}
			JSON *t=json_new_obj(key,val);
			json->value_array.push_back(t);
			json->type=JSON_ARR;
			return json;
		}
		else
		{
			cout<<endl<<"error:json type error,if you want to add member in this json,please use this function with json_add_member(json,key,val,true)";
			return NULL;
		}
	}
	return NULL;
}
JSON *json_add_element(JSON *json, JSON *val,bool compatible)//在当前JSON添加>一个JSON
{
	if(json_type(json)==JSON_ARR)
        {
		if(json_type(json->value_array[0])==JSON_NONE)
			json->value_array[0]=val;
		else
			json->value_array.push_back(val);
                return json;
        }
        else
        {
                if(compatible)
                {
                        if(json_type(json)==JSON_BOL)
                        {
                                JSON *tmp=json_new_bool(json->value_bool);
                                json->value_array.push_back(tmp);
                        }
                        else if(json_type(json)==JSON_NUM)
                        {
				JSON *tmp=json_new_num(json->value_double);
                                json->value_array.push_back(tmp);
				json->value_double=0;
                        }
                        else if(json_type(json)==JSON_STR)
                        {
                                JSON *tmp=json_new_str(json->value_string);
                                json->value_array.push_back(tmp);
                        	json->value_string.clear();
				json->value_string.shrink_to_fit();
			}
                        json->value_array.push_back(val);
                        json->type=JSON_ARR;
                        return json;
		 }
                else
                {
                        cout<<endl<<"error:json type error,if you want to add element in this json,please use this function with json_add_element(json,val,true)";
                        return NULL;
                }
        }
        return NULL;

}
int json_arr_count(const JSON *json) //计算JSON中的数组元素个数
{
	if(json_type(json)==JSON_ARR)
		return json->value_array.size();
	else
	{
		cout<<endl<<"error:json doesn't have array";
		return -1;
	}
}

JSON* json_obj_set(JSON *json, string key, JSON *val)//修改当前JSON对象对应键值的数值
{
	JSON *t=json_get_member(json,key);
	if(t)
	{
		t->type=JSON_OBJ;
		for(auto i:t->value_array)
			json_free(i);
		t->value_array.resize(1);
		t->value_array[0]=val;
		return json;
	}
	else
	{
		cout<<endl<<"error:key not found";
		return NULL;
	}
	return NULL;
}

int json_del(JSON *json,JSON *tar)//在指定json范围下删除目标json数据
{
	if(json_type(json)==JSON_OBJ)
	{
		if(json->value_array[0]==tar){
			json_free(tar);
			json->type=JSON_NONE;
			return 1;
		}
		else
		{
			return (json_del(json->value_array[0],tar));
		}
	}
	else if(json_type(json)==JSON_ARR)
	{
		int idx=0;
		for(auto j:json->value_array)
		{
			if(j==tar)
			{
				json_free(tar);
				json->value_array.erase(json->value_array.begin()+idx);
				json->value_array.shrink_to_fit();
				return 1;
			}
			else
			{
				if(json_del(json->value_array[idx],tar)==1)
					return 1;
				idx++;
			}
		}
	}
	return 0;
}
int json_obj_del(JSON *json,string key)//删除指定key的json
{
	JSON *t=json_get_member(json,key);
	if(t)
	{
		if(json_del(json,t)==1){
			cout<<endl<<"delete success!";
			return 1;
		}
		else
		{
			cout<<endl<<"delete fail";
			return 0;
		}
	}
	else
	{
		cout<<endl<<"error:key doesn't exist under this json";
		return 0;
	}
}

int json_arr_del(JSON *json,unsigned int idx)//删除指定下标的json
{
	if(json_type(json)==JSON_ARR)
	{
		if(idx<json_arr_count(json))
        	{
			json_free(json->value_array[idx]);
			json->value_array.erase(json->value_array.begin()+idx);
			cout<<endl<<"delete success";
			return 1;
        	}
		else
		{
			cout<<endl<<"error:idx too large,array_size:"<<json_arr_count(json)<<" idx:"<<idx;
			return 0;
		}
	}
	else
	{
		cout<<endl<<"error:json type incorrect";
		return 0;
	}
}
