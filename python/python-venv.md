
# venv environment for pPython scripts 

## Installed packages
```
pip install notebook 
pip install numpy 
pip install matplotlib 
```

## attaching vs code to server

```
source ~/venvs/glthrowsim-venv/bin/activate
jupyter notebook
ll ~/.local/share/jupyter/runtime/ | grep .json
cat ~/.local/share/jupyter/runtime/jpserver-<serverid>.json | grep token
# Paste token as password in vs code
```

