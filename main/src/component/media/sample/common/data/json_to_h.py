import json

cfg_list = []
array_cfg_list = []
def parse_cfg():
    with open("config.ini", 'r') as cfg_file:
        for line in cfg_file:
            if line.count(",") == 2:
                cfg_list.append(line)
            elif line.count(",") == 3:
                array_cfg_list.append(line)
            else:
                print(f"invalid param. {line}")

def write_head(write_file, define_str):
    write_file.write("/*\n")
    write_file.write("  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.\n")
    write_file.write(" */\n\n")
    
    write_file.write(f"#ifndef {define_str}\n")
    write_file.write(f"#define {define_str}\n\n")

    write_file.write("#include \"ot_eis_vi_awb.h\"\n")
    write_file.write("#include \"ot_eis_vi_sns.h\"\n\n")

    write_file.write("#ifdef __cplusplus\n")
    write_file.write("#if __cplusplus\n")
    write_file.write("extern \"C\" {\n")
    write_file.write("#endif\n")
    write_file.write("#endif\n\n")
    
def write_tail(write_file, define_str):
    write_file.write("\n\n")
    write_file.write("#ifdef __cplusplus\n")
    write_file.write("#if __cplusplus\n")
    write_file.write("}\n")
    write_file.write("#endif\n")
    write_file.write("#endif /* End of #ifdef __cplusplus */\n\n")
    write_file.write(f"#endif /* {define_str} */\n")
    
def list_to_string_with_braces(nested_list):
    list_str = json.dumps(nested_list)
    list_str = list_str.replace('[[', '{\n        {').replace(']]', '}\n    }').replace('], [', '},\n        {').replace('[', '{').replace(']', '}')
    if '0x' in list_str:
        list_str = list_str.replace('\"', '')
    return list_str
    
def traverse_dict(write_file, obj, level=0):
    write_file.write("    {\n")
    for key,value in obj.items():
        if isinstance(value, dict):
            traverse_dict(write_file, value, level+1)
        elif isinstance(value, list):
            index = 0
            while index<=level:
                write_file.write(f"    ")
                index += 1
            result_str = list_to_string_with_braces(value)
            write_file.write(f"    {result_str}, /* {key} */\n")
        else:
            index = 0
            while index<=level:
                write_file.write(f"    ")
                index += 1
            write_file.write(f"    {value}, /* {key} */\n")
    write_file.write("    },\n")

def parse_json_data(item):
    item_list = item.split(",")
    src_path = item_list[0].strip()
    dst_path = item_list[1].strip()
    prefix_path = item_list[2].strip()
    last_slash_index = dst_path.rfind('/')
    define_str = dst_path[last_slash_index+1:]
    define_str = define_str.upper()
    define_str = define_str.replace(".", "_")

    with open(src_path) as read_file:
        data = json.load(read_file);

    write_file = open(dst_path, 'w')
    write_head(write_file, define_str)

    # 第一层 结构体名
    for key1,value1 in data.items():
        if isinstance(value1, dict):
            # 第二层 结构体对象名
            for key2,value2 in value1.items():
                if key2.count("_") > 0:
                    var_list = key2.split("_", 1)
                    var_name = "g_" + prefix_path + "_" + var_list[1]
                else:
                    var_name = "g_" + prefix_path + key2
                write_file.write(f"static const {key1} {var_name} =\n")
                write_file.write("{\n")
                if isinstance(value2, dict):
                    # 第三层 结构体中的数据
                    for key3,value3 in value2.items():
                        if isinstance(value3, dict):
                            traverse_dict(write_file, value3)
                        elif isinstance(value3, list):
                            result_str = list_to_string_with_braces(value3)
                            write_file.write(f"    {result_str}, /* {key3} */\n")
                        else: # 遇到dict的value直接显示
                            write_file.write(f"    {value3}, /* {key3} */\n")
                elif isinstance(value2, list):
                    result_str = list_to_string_with_braces(value2)
                    write_file.write(f"    {result_str}, /* {key3} */\n")
                else: # 遇到dict的value直接显示
                    write_file.write(f"    {value2}, /* {key2} */\n")
                write_file.write("};\n\n")
        
    write_tail(write_file, define_str)
    write_file.close()

def parse_array_json_data(item):
    item_list = item.split(",")
    src_path = item_list[0].strip()
    dst_path = item_list[1].strip()
    prefix_path = item_list[2].strip()
    line_cnt = int(item_list[3].strip())
    
    last_slash_index = dst_path.rfind('/')
    define_str = dst_path[last_slash_index+1:]
    define_str = define_str.upper()
    define_str = define_str.replace(".", "_")

    with open(src_path) as read_file:
        data = json.load(read_file);

    write_file = open(dst_path, 'w')
    write_head(write_file, define_str)

    # 第一层 结构体名
    for key1,value1 in data.items():
        if isinstance(value1, dict):
            # 第二层 结构体对象名
            for key2,value2 in value1.items():
                if key2.count("_") > 0:
                    var_list = key2.split("_", 1)
                    var_name = "g_" + prefix_path + "_" + var_list[1]
                else:
                    var_name = "g_" + prefix_path + key2
                write_file.write(f"static {key1} {var_name} =\n")
                write_file.write("{\n")
                # 第三层 结构体中的数据
                for index in range(0, len(value2), 2):
                    pair = value2[index:index + 2]
                    v1 = pair[0] 
                    v2 = pair[1]
                    if v1 == "//":
                        file_length = write_file.seek(0,2)
                        write_file.seek(file_length-2, 0)
                        write_file.write(f"     {v1}{v2}\n")
                    else:
                        write_file.write("    {")
                        write_file.write(f"{v1},{v2}")
                        write_file.write("},\n")
                
                write_file.write("};\n\n")
        
    write_tail(write_file, define_str)
    write_file.close()

if __name__ == "__main__":
    parse_cfg()
    for item in cfg_list:
        parse_json_data(item)
    for item in array_cfg_list:
        parse_array_json_data(item)