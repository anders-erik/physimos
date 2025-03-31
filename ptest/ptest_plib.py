

from ptest import Ptest


ptest = Ptest("plib")


# Files that will trigger test rerun
ptest.add_file_to_watch("/ptest/ptest_plib.cc")
ptest.add_file_to_watch("/ptest/ptest.cc")
ptest.add_file_to_watch("/ptest/ptest.hh")
ptest.add_file_to_watch("/src/lib/fs.cc")
ptest.add_file_to_watch("/src/lib/fs.hh")
ptest.add_file_to_watch("/src/lib/string.cc")
ptest.add_file_to_watch("/src/lib/string.hh")
ptest.add_file_to_watch("/src/lib/result.cc")
ptest.add_file_to_watch("/src/lib/result.hh")
ptest.add_file_to_watch("/src/lib/toml.cc")
ptest.add_file_to_watch("/src/lib/toml.hh")


# include directories
ptest.add_include_dir("/ptest")
ptest.add_include_dir("/src")
ptest.add_include_dir("/src/lib")



# object files to compile
ptest.add_source_file("/ptest/ptest.cc")
ptest.add_source_file("/src/lib/result.cc")
ptest.add_source_file("/src/lib/string.cc")
ptest.add_source_file("/src/lib/fs.cc")
ptest.add_source_file("/src/lib/toml.cc")

ptest.add_source_file("/src/lib/process.cc")


# source file with main function for ptest binary build
ptest.add_main_file("/ptest/ptest_plib.cc")


ptest.start_watch()
