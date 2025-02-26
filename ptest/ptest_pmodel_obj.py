
from ptest import Ptest

ptest = Ptest("pmodel_obj")

# Files that will trigger test rerun
ptest.add_file_to_watch("/ptest/ptest_pmodel_obj.cc")
ptest.add_file_to_watch("/ptest/ptest.cc")
ptest.add_file_to_watch("/ptest/ptest.hh")
ptest.add_file_to_watch("/src/lib/process.hh")

ptest.add_file_to_watch("/src/model/obj.cc")
ptest.add_file_to_watch("/src/model/obj.hh")
ptest.add_file_to_watch("/src/model/obj_types.hh")
ptest.add_file_to_watch("/src/res/bmp_loader.cpp")
ptest.add_file_to_watch("/src/res/bmp_loader.hpp")
ptest.add_file_to_watch("/src/logger/logger.cc")
ptest.add_file_to_watch("/src/logger/logger.hh")

# include directories
ptest.add_include_dir("/ptest")
ptest.add_include_dir("/src")
ptest.add_include_dir("/src/lib")

ptest.add_include_dir("/src/model")


# object files to compile
ptest.add_source_file("/ptest/ptest.cc")
ptest.add_source_file("/src/lib/process.hh")
ptest.add_source_file("/src/res/bmp_loader.cpp")
ptest.add_source_file("/src/logger/logger.cc")
ptest.add_source_file("/src/lib/string.cc")

ptest.add_source_file("/src/model/obj.cc")


# source file with main function for ptest binary build
ptest.add_main_file("/ptest/ptest_pmodel_obj.cc")


ptest.start_watch()


