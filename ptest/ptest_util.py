
import pathlib

def get_physimos_root_dir():
    """Make sure that this file is in {physimos_root_dir}/ptest """
    current_file_dir = pathlib.Path(__file__).parent.resolve()
    repo_root_dir_path = current_file_dir.parent
    repo_root_dir = str(repo_root_dir_path)
    return repo_root_dir

def clear_tmp_dir():
    """Make sure that this file is in {physimos_root_dir}/ptest """
    physimos_dir = get_physimos_root_dir()
    tmp_dir = physimos_dir + "/tmp"
    print("[NOT YET LIVE]")
    print("Emptying \'" + tmp_dir + "\'")