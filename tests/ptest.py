import os
import time
import subprocess
import pathlib
import re

class PtestFile:
    dummy=1


class Ptest:
    repo_root_dir=''
    test_name=''
    ptest_bin_dir=''
    ptest_bin_file=''

    # Util dirs
    tmp_physimos_dir=''
    testdata_dir=''
    testdata_out_dir=''

    files_to_watch = []

    include_dirs = []
    '''has to be set first'''
    source_files = []
    '''has to be set second'''

    ptest_main_path=''
    '''has to be set last'''
    executable_command_list=[]


    def __init__(self, test_name):

        # Grab the repo root directory and set
        current_file_dir = pathlib.Path(__file__).parent.resolve()
        repo_root_dir_path = current_file_dir.parent
        repo_root_dir = str(repo_root_dir_path)
        # Store root directory for later
        self.repo_root_dir = repo_root_dir

        # Make sure directories used for testing exists
        self.testdata_out_dir = self.repo_root_dir + "/tests/testdata.out/"
        self.set_data_output_dir_envvar(self.testdata_out_dir)
        # Test data dir
        self.testdata_dir = self.repo_root_dir + "/tests/testdata"
        subprocess.run(["mkdir", "-p", self.testdata_dir])
        # tmp root dir
        self.tmp_physimos_dir = self.repo_root_dir + "/tmp"
        subprocess.run(["mkdir", "-p", self.tmp_physimos_dir])

        # Make sure all subprocesses can using root dir
        os.environ['PHYSIMOS_ROOT_DIR'] = repo_root_dir
        os.environ['PHYSIMOS_TEST'] = '1'

        self.test_name = test_name
        self.ptest_bin_dir = self.repo_root_dir + "/tests/bin"
        self.ptest_bin_file = self.ptest_bin_dir + "/ptest_" + test_name
    

    def set_data_output_dir_envvar(self, dir_path_str):
        """
        Set environment variable "PHYSIMOS_TEST_DATA_OUT_DIR".
        """
        os.environ['PHYSIMOS_TEST_DATA_OUT_DIR'] = dir_path_str
        subprocess.run(["mkdir", "-p", dir_path_str])

    
    def add_include_dir(self, dirPath_str):
        """
        Add include directory as a string relative to repo_root_dir.
        """
        if self.source_files or self.ptest_main_path:
            raise RuntimeError("Include Directories can not be after the addition of source files or main binary.")
        
        self.include_dirs.append(self.repo_root_dir + dirPath_str)
    
    
    def add_file_to_watch(self, filePath_str):
        """
        Add file path as a string relative to repo_root_dir.
        Any detected changes in these files during test watching will trigger rebuild and rerun
        """
        absolute_path = self.repo_root_dir + filePath_str
        new_file_to_watch = {
            "file_path": absolute_path, 
            "time_last_mod": self.get_modification_time(absolute_path),
        }
        self.files_to_watch.append(new_file_to_watch)


    def get_modification_time(self, filepath):
        """
        Returns time when file was last changed.
        """
        return os.path.getmtime(filepath)
    

    def watched_file_has_been_modified(self):
        """
        Returns True if any of the watched files have been modified

        Returns: boolean
        """
        file_change_detected = False
        
        for file in self.files_to_watch:
                file_change_detected = file['time_last_mod'] != self.get_modification_time(file['file_path'])

                if file_change_detected:
                    return True
                
        return file_change_detected


    def console_clear(self):
        """
        Clears terminal window.
        """
        os.system('cls' if os.name == 'nt' else 'clear')
    

    def add_source_file(self, source_path_string):
        """
        Add file path as a string relative to repo_root_dir.
        Any added source file will be compiled into an object file and linked with the main ptest binary
        """
        if self.ptest_main_path:
            raise RuntimeError("Source files can not be after the addition of a main binary file.")
        
        source_path_absolute = self.repo_root_dir + source_path_string
        object_path_absolute = ''

        object_file_name = ''
        
        # Extract source names and replace the extensions
        source_extensions = ['cpp', 'cc', 'c' ]
        # Regex to extract source file name
        # TODO: Reaplce with os path utilities..
        source_file_name_regex_template = r'\/([\w\-. ]+\.<extension>)$'

        for extension in source_extensions:
            # create regex to extract sourcefilename
            source_file_name_regex = re.sub(r'<extension>', extension, source_file_name_regex_template)
            source_file_name_matches = re.findall(source_file_name_regex, source_path_absolute)

            # exit if source
            if (source_file_name_matches):
                extension_regex = extension + '$' 
                object_file_name = re.sub(extension_regex, "o", source_file_name_matches[0])
                break
        
        # if the file extension of the object path was never replaced then the source file is invalid.
        # Very important to exit at this point. I do not want to risk overwriting source file during build.
        if (not object_file_name):
            raise ValueError(' No matching file extension was found for source file.')

        object_path_absolute = self.ptest_bin_dir + '/' + object_file_name
        # print("object_path_absolute = " + object_path_absolute)
        # exit(0)
            
        # BUILD COMMAND
        source_command_list = ["g++", "-g", "-c", source_path_absolute, "-o", object_path_absolute]
        for include_dir in self.include_dirs:
            source_command_list.append("-I" + include_dir)

        # Add source to ptest
        new_file_to_build = {
            "source_file_path": source_path_absolute, 
            "object_file_path": object_path_absolute, 
            "time_last_mod": self.get_modification_time(source_path_absolute),
            "command_list": source_command_list
        }
        self.source_files.append(new_file_to_build)
    

    def add_main_file(self, path_path_string):
        """
        Add main file path as a string relative to repo_root_dir.
        Provides the main function of the ptest binary.
        Only one file can be set.
        """
        self.ptest_main_path = self.repo_root_dir + path_path_string

        self.executable_command_list = ["g++", "-g", "-o", self.ptest_bin_file, self.ptest_main_path]

        # Source objects
        for source in self.source_files:
            self.executable_command_list.append(source['object_file_path'])

        # Iinclude dirs
        for dir in self.include_dirs:
            self.executable_command_list.append("-I" + dir)
    

    def compile_ptest(self):
        """
        Compiles an executable ptest binary.

        returns: subprocess exit code.
        """
        print("Compiling ptest : " + self.test_name)

        # BUILD OBJECTS
        for source in self.source_files:
            build_obj_cc_build_process = subprocess.run(source["command_list"])

        # BUILD EXECUTABLE
        compile_process = subprocess.run(self.executable_command_list)

        return compile_process.returncode
    
    
    def run_ptest(self, args):
        """
        Runs the test binary.
        """
        print("        Running ptest : " + self.test_name)
        subcommand_list = [self.ptest_bin_file]
        for arg in args:
            subcommand_list.append(arg)
        print(subcommand_list)
        subprocess.run(subcommand_list)
         

    def start_watch(self):
        self.console_clear()
        print("\nPhysimos Test Started : " + self.test_name)
        print("\nWatching: ")
        for file in self.files_to_watch:
            print(file['file_path'])
        
        while True:
            time.sleep(1) # Poll every second

            # return value will act as test rerun trigger
            if self.watched_file_has_been_modified():

                # compile and run of compilation was successful
                compile_exit_code = self.compile_ptest()
                if compile_exit_code == 0:
                    self.console_clear()
                    self.run_ptest()

                # reset file change times
                for file in self.files_to_watch:
                    file['time_last_mod'] = self.get_modification_time(file['file_path'])