import os
import time
import subprocess
import pathlib


# Grab the repo root directory
current_file_dir = pathlib.Path(__file__).parent.resolve()
repo_root_dir_path = current_file_dir.parent
repo_root_dir = str(repo_root_dir_path)

os.environ['PHYSIMOS_ROOT_DIR'] = repo_root_dir

# Ptest info
ptest_name = "pmodel::obj"
ptest_bin_dir = repo_root_dir + "/tests/bin"
ptest_bin_file = ptest_bin_dir + "/ptest_pmodel_obj"


include_dirs = [
    repo_root_dir + "/src",
    repo_root_dir + "/src/model",
]



# Files that will trigger test rerun
files_to_watch = []
# object_file_paths = []


# Files to be watched
ptest_main = {
     "file_path": repo_root_dir + "/tests/ptest_pmodel_obj.cc", 
     "time_last_mod": 0,
}
obj_cc = { 
     "file_path": repo_root_dir + "/src/model/obj.cc", 
     "time_last_mod": 0,
}
obj_hh = { 
     "file_path": repo_root_dir + "/src/model/obj.hh", 
     "time_last_mod": 0,
}
obj_types_hh = { 
     "file_path": repo_root_dir + "/src/model/obj_types.hh", 
     "time_last_mod": 0,
}
bmp_loader_cpp = { 
     "file_path": repo_root_dir + "/src/res/bmp_loader.cpp", 
     "time_last_mod": 0,
}
bmp_loader_hh = { 
     "file_path": repo_root_dir + "/src/res/bmp_loader.hpp", 
     "time_last_mod": 0,
}
logger_cc = { 
     "file_path": repo_root_dir + "/src/logger/logger.cc", 
     "time_last_mod": 0,
}
logger_hh = { 
     "file_path": repo_root_dir + "/src/logger/logger.hh", 
     "time_last_mod": 0,
}
files_to_watch.append(ptest_main)
files_to_watch.append(obj_cc)
files_to_watch.append(obj_hh)
files_to_watch.append(obj_types_hh)
files_to_watch.append(bmp_loader_cpp)
files_to_watch.append(bmp_loader_hh)
files_to_watch.append(logger_cc)
files_to_watch.append(logger_hh)

# object_file_paths.append(obj_cc["file_path"].replace(".cc", ".o"))
# object_file_paths.append(bmp_loader_cpp["file_path"].replace(".cpp", ".o"))




def get_modification_time(filepath):
    """
    Returns time when file was last changed.
    """
    return os.path.getmtime(filepath)


def watched_file_has_been_modified():
    """
    Returns True if any of the watched files have been modified

    Returns: boolean
    """
    file_change_detected = False
    
    for file in files_to_watch:
            file_change_detected = file['time_last_mod'] != get_modification_time(file['file_path'])

            if file_change_detected:
                return True
            
    return file_change_detected


def console_clear():
     """
     Clears terminal window.
     """
     os.system('cls' if os.name == 'nt' else 'clear')


def compile_ptest():
        """
        Compiles an executable ptest binary.

        returns: subprocess exit code.
        """
        print("Compiling ptest : pmath::vec3")


        # MAIN EXECUTABLE
        executable_command_list = ["g++", "-o", ptest_bin_file, ptest_main['file_path']]

        # DEFINE OBJECTS
        obj_o = ptest_bin_dir + "/obj.o" # obj_cc["file_path"].replace(".cc", ".o")
        obj_cc_command_list = ["g++", "-c", obj_cc["file_path"], "-o", obj_o]

        bmp_loader_o = ptest_bin_dir + "/bmp_loader.o" # bmp_loader_cpp["file_path"].replace(".cpp", ".o")
        bmp_loader_cpp_command_list = ["g++", "-c", bmp_loader_cpp["file_path"], "-o", bmp_loader_o]

        logger_o = ptest_bin_dir + "/logger.o" # bmp_loader_cpp["file_path"].replace(".cpp", ".o")
        logger_cc_command_list = ["g++", "-c", logger_cc["file_path"], "-o", logger_o]

        # add objects to executable
        executable_command_list.append(obj_o)
        executable_command_list.append(bmp_loader_o)
        executable_command_list.append(logger_o)

        # INCLUDE DIRS
        for dir in include_dirs:
            obj_cc_command_list.append("-I" + dir)
            bmp_loader_cpp_command_list.append("-I" + dir)
            logger_cc_command_list.append("-I" + dir)

            executable_command_list.append("-I" + dir)


        # BUILD OBJECTS
        build_obj_cc_build_process = subprocess.run(obj_cc_command_list)
        bmp_loader_cpp_build_process = subprocess.run(bmp_loader_cpp_command_list)
        logger_cc_build_process = subprocess.run(logger_cc_command_list)


        # BUILD EXECUTABLE
        compile_process = subprocess.run(executable_command_list)
        return compile_process.returncode


def run_ptest():
        """
        Runs the test binary.
        """
        print("Running ptest : " + ptest_name)
        subprocess.run([ptest_bin_file])



if __name__ == "__main__":

    # initial file change times
    for file in files_to_watch:
        file['time_last_mod'] = get_modification_time(file['file_path'])
    
    
    # Info when waiting for first file change
    console_clear()
    print("\nPhysimos Test Started : " + ptest_name)
    print("\nWatching: ")
    for file in files_to_watch:
        print(file['file_path'])


    # main loop
    while True:
        time.sleep(1) # Poll every second

        # return value will act as test rerun trigger
        if watched_file_has_been_modified():

            # compile and run of compilation was successful
            compile_exit_code = compile_ptest()
            if compile_exit_code == 0:
                console_clear()
                run_ptest()

            # reset file change times
            for file in files_to_watch:
                file['time_last_mod'] = get_modification_time(file['file_path'])

            file_change_detected = False

        