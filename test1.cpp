#include "cjson_hxj.h"
//尝试上传到github上
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

