import sys
import subprocess

import ptest_util

physimos_root_dir   = ptest_util.get_physimos_root_dir()
test_dir            = physimos_root_dir + "/ptest"

def print_help():
    print("")
    print("    Welcome To PTest:")
    print("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^")
    print("")
    print("Available tests:")
    print("  pimage")
    print("  plib")
    print("")
    print("Shared Test Flags")
    print(" -h = test details ")
    print(" -w = Start test in watch mode. [default]")
    print(" -r = Run test once. ")
    print("")
    print("Other Commands:")
    print("  ui_curses")
    print("  config")
    print("  util")
    print("  pillow")
    print("")
    exit(0)


if(len(sys.argv) == 1): # no arguments = watch mode
    print_help()
elif(sys.argv[1] == '-h'):
    print_help()


if(sys.argv[1] == "pimage"):
    subprocess.run(["python3", test_dir+"/ptest_pimage.py", "-r"])


