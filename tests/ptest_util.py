
import pathlib

def get_physimos_root_dir():
    """Make sure that this file is in {physimos_root_dir}/tests """
    current_file_dir = pathlib.Path(__file__).parent.resolve()
    repo_root_dir_path = current_file_dir.parent
    repo_root_dir = str(repo_root_dir_path)
    return repo_root_dir