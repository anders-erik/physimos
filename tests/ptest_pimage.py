import subprocess
import sys


from ptest import Ptest



ptest = Ptest("pimage")

# Test data output directory
testdata_output_dir = ptest.ptest_testdata_out_dir + "/pimage"
subprocess.run(["mkdir", "-p", testdata_output_dir])
ptest.set_data_output_dir_envvar(testdata_output_dir)


# Files that will trigger test rerun
ptest.add_file_to_watch("/tests/ptest_pimage.cc")
ptest.add_file_to_watch("/tests/ptest.cc")
ptest.add_file_to_watch("/tests/ptest.hh")
ptest.add_file_to_watch("/src/process_info.cpp")
ptest.add_file_to_watch("/src/lib/fs.cc")
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
ptest.add_source_file("/src/lib/string.cc")
ptest.add_source_file("/src/lib/fs.cc")

ptest.add_source_file("/src/image/bmp.cc")


# source file with main function for ptest binary build
ptest.add_main_file("/tests/ptest_pimage.cc")

print("sys.argv.count =", len(sys.argv))
if(len(sys.argv) == 1): # no arguments = watch mode
    ptest.start_watch()
elif(sys.argv[1] == '-w'):
    ptest.start_watch()
elif(sys.argv[1] == '-r'):
    ptest.compile_ptest()
    ptest.run_ptest()
elif(sys.argv[1] == '-h'):
    print("")
    print(" Ptest:")
    print("")
    print(" -w = watch mode      [default]")
    print(" -r = single run mode")
    print("")
else: 
    print("")
    print("Unknown Ptest CLI argument. Use '-h' for help. ")
    print("")
