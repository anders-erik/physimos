import tomllib

import ptest_util

physimos_root_dir = ptest_util.get_physimos_root_dir()

config_file = physimos_root_dir + "/ptest/ptest.toml"

configDict={}


# Primary config file
with open(config_file, "rb") as file:
    configDict = tomllib.load(file)

def get_pimage_gen_paths_dict():
    """ Retrieve the KV pairs from the [pimage_gen_paths] property in ptest.toml """
    return configDict.get('pimage_gen')

def get_pimage_toml():
    """ Get all configs for pimage build as dict. """

    with open("pimage/pimage.toml", "rb") as f:
        data = tomllib.load(f)
        # print(data)
        return data
    
def get_ptest_build_config(spath):
    """ Get all configs for pimage build as dict. """

    with open(spath, "rb") as f:
        data = tomllib.load(f)
        # print(data)
        return data


#  
# EVERYTHING BELOW IS ME LEARNING TOML AND DICTS....
# 


def toml_and_dict_parsing():

    # STRING
    toml_str = """
    [python]
    python-version = "3.11.0"
    python-implementation = "CPython"

    [python2]
    python-version = "3.11.0"
    python-implementation = "CPython"
    """
    equivalentDict = {'python': {'python-version': '3.11.0', 'python-implementation': 'CPython'}, 'python2': {'python-version': '3.11.0', 'python-implementation': 'CPython'}}

    string_dict = tomllib.loads(toml_str)
    print(string_dict.get('python').get('python-version'))
    print(equivalentDict.get('python').get('python-version'))
    

    # FILE
    with open("ptest.toml", "rb") as f:
        data = tomllib.load(f)
        print(data.get("source_files"))

def loop_dict():
    """ Try to loop through the dict properties """
    dummyDict = {'first': {'first_1': 1, 'first_2': 2}, 'second': {'second_1': 1, 'second_2': 2}}
    print(dummyDict.get("first"))
    value_list = dummyDict.get("first").values()
    for item in value_list:
        print(item)
    # print(dummyDict.get("first").values())

def toml_array():
    """ Parsing toml array """

    toml_str = """
    [python-versions]
    array_key = ["3.11.0", "3.12.0"]
    """

    toml_list = tomllib.loads(toml_str)
    print(toml_list)



# pimage_toml_parse()
# toml_array()
# loop_dict()
# toml_and_dict_parsing()

