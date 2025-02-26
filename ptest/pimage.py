import subprocess
import sys


import ptest_pil
import ptest_config

from ptest import Ptest


ptest = Ptest("pimage")



def set_pimage_build_config():

    toml_build_config = ptest_config.get_ptest_build_config("pimage/build.toml")

    # watch files
    watch_files = toml_build_config.get('watch_files')
    for watch_file in watch_files:
        ptest.add_file_to_watch(watch_file)

    # include_dirs
    include_dirs = toml_build_config.get('include_dirs')
    for include_dir in include_dirs:
        ptest.add_include_dir(include_dir)

     # source_files
    source_files = toml_build_config.get('source_files')
    for source_file in source_files:
         ptest.add_source_file(source_file)
    
    # Main Executable
    main = toml_build_config.get('main')
    ptest.add_main_file(main)
    

   

# set_pimage_ptest_config_toml()
# exit(0)

def set_pimage_ptest_config_manually():

    # Files that will trigger test rerun
    ptest.add_file_to_watch("/ptest/ptest_pimage.cc")
    ptest.add_file_to_watch("/ptest/ptest.cc")
    ptest.add_file_to_watch("/ptest/ptest.hh")
    ptest.add_file_to_watch("/src/lib/process.hh")
    ptest.add_file_to_watch("/src/lib/fs.cc")
    ptest.add_file_to_watch("/src/image/bmp.cc")
    ptest.add_file_to_watch("/src/image/bmp.hh")


    # include directories
    ptest.add_include_dir("/ptest")
    ptest.add_include_dir("/src")
    ptest.add_include_dir("/src/lib")

    ptest.add_include_dir("/src/image")


    # objet files to compile
    ptest.add_source_file("/ptest/ptest.cc")
    ptest.add_source_file("/src/lib/process.hh")
    ptest.add_source_file("/src/lib/result.cc")
    ptest.add_source_file("/src/lib/string.cc")
    ptest.add_source_file("/src/lib/fs.cc")

    ptest.add_source_file("/src/image/bmp.cc")


    # source file with main function for ptest binary build
    ptest.add_main_file("/ptest/ptest_pimage.cc")



# print("sys.argv.count =", len(sys.argv))
if(len(sys.argv) == 1): # no arguments = watch mode
    ptest.start_watch()

elif(sys.argv[1] == '-w'):
    ptest.start_watch()

elif(sys.argv[1] == "-r"):
    # set_pimage_ptest_config_manually()
    set_pimage_build_config()
    ptest.compile_ptest()
    ptest.run_ptest(["-h"])

elif(sys.argv[1] == '-g'):
    # set_pimage_ptest_config_manually()
    set_pimage_build_config()
    ptest.compile_ptest()
    ptest.run_ptest(["-g"]) # png generation and verification test

elif(sys.argv[1] == '-i'):
    ptest_pil.pillow_write_new_file_to_tmp(ptest.tmp_physimos_dir)

elif(sys.argv[1] == '-h'):
    print("")
    print(" Ptest:")
    print("")
    print(" -w = watch mode      [default]")
    print(" -r = single run mode")
    print(" -g = generate and perform physimos load & write tests")
    print(" -i = pillow image generation tests")
    print("")
else: 
    print("")
    print("Unknown Ptest CLI argument. Use '-h' for help. ")
    print("")
