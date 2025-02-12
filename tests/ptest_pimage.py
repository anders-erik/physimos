from ptest import Ptest


ptest = Ptest("pimage")

# Files that will trigger test rerun
ptest.add_file_to_watch("/tests/ptest_pimage.cc")
ptest.add_file_to_watch("/tests/ptest.cc")
ptest.add_file_to_watch("/tests/ptest.hh")
ptest.add_file_to_watch("/src/image/bmp.cc")
ptest.add_file_to_watch("/src/image/bmp.hh")


# include directories
ptest.add_include_dir("/tests")
ptest.add_include_dir("/src")
ptest.add_include_dir("/src/lib")

ptest.add_include_dir("/src/image")


# objet files to compile
ptest.add_source_file("/tests/ptest.cc")
ptest.add_source_file("/src/process_info.cpp")
ptest.add_source_file("/src/lib/result.cc")

ptest.add_source_file("/src/image/bmp.cc")


# source file with main function for ptest binary build
ptest.add_main_file("/tests/ptest_pimage.cc")


ptest.start_watch()
