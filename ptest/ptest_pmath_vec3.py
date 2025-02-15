import os
import time
import subprocess
import pathlib


# Grab the repo root directory
current_file_dir = pathlib.Path(__file__).parent.resolve()
repo_root_dir_path = current_file_dir.parent
repo_root_dir = str(repo_root_dir_path)
# repo_root_dir = os.getcwd() # old method requires execution from repo root dir


# Files to watch
ptest_main_file = repo_root_dir + "/ptest/ptest_pmath_vec3.cc"
vec3_source_file = repo_root_dir + "/src/math/vec3.cc"
vec3_header_file = repo_root_dir + "/src/math/vec3.hh"

vec3_include_dir = repo_root_dir + "/src/math"
vec3_include_compile_flag = "-I" + vec3_include_dir

ptest_bin_file = repo_root_dir + "/ptest/ptest_pmath_vec3"



# check when file was last changed
def get_modification_time(filepath):
    return os.path.getmtime(filepath)

def console_clear():
     os.system('cls' if os.name == 'nt' else 'clear')

def compile_ptest():
        """
        Compiles an executable ptest binary.

        returns: subprocess exit code.
        """
        print("Compiling ptest : pmath::vec3")
        compile_process = subprocess.run(["g++", vec3_include_compile_flag, "-o", ptest_bin_file, ptest_main_file, vec3_source_file])
        return compile_process.returncode

def run_ptest():
        print("Running ptest : pmath::vec3")
        subprocess.run([ptest_bin_file])


if __name__ == "__main__":

    # initial file change times
    last_time_ptest_main = get_modification_time(ptest_main_file)
    last_time_vec3_source = get_modification_time(vec3_source_file)
    last_time_vec3_header = get_modification_time(vec3_header_file)

    console_clear()
    print("\nPhysimos Test Started : pmath::vec3")
    print("\nWatching: ")
    print(ptest_main_file)
    print(vec3_source_file)
    print(vec3_header_file)

    while True:

        # check when file was last changed
        current_time_ptest_main = get_modification_time(ptest_main_file)
        current_time_vec3_source = get_modification_time(vec3_source_file)
        current_time_vec3_header = get_modification_time(vec3_header_file)

        # Compare times to to last change time
        ptest_source_changed =  current_time_ptest_main  != last_time_ptest_main
        vec3_source_changed  =  current_time_vec3_source != last_time_vec3_source
        vec3_header_changed  =  current_time_vec3_header != last_time_vec3_header

        
        if ptest_source_changed:
            console_clear()
            print("Change detected in ptest file.")
        if vec3_source_changed:
            console_clear()
            print("Change detected in vec3 source file.")
        if vec3_header_changed:
            console_clear()
            print("Change detected in vec3 header file.")

        if ptest_source_changed or vec3_source_changed or vec3_header_changed:

            compile_exit_code = compile_ptest()
            if compile_exit_code == 0:
                console_clear()
                run_ptest()

            # New last modified time
            last_time_ptest_main = current_time_ptest_main
            last_time_vec3_source = current_time_vec3_source
            last_time_vec3_header = current_time_vec3_header

        time.sleep(1)  # Poll every second