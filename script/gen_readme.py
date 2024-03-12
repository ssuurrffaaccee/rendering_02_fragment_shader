import sys
import pathlib
import pathlib as pl


CURRENT_DIR = pathlib.Path(__file__).parent
sys.path.append(str(CURRENT_DIR))

def get_by_postfix(path,post_fix):
    paths = []
    for p in pl.Path(path).rglob("*"):
        str_p =str(p)
        if str_p.split(".")[-1] == post_fix:
            paths.append(str_p)
    return paths
from recipes import recipes
def clear_name(name):
    name = " ".join(name.split('-'))
    name = list(name)
    for idx,char in enumerate(name):
        if char =='_':
            name[idx]=' '
    return "".join(name)
def do_one(name):
    print("## %s\n"%(clear_name(name)))
    out_path = "./"+name+"/out/"
    for path  in get_by_postfix(out_path,'jpg'):
        print("![demo](%s)\n"%("./"+path))
    print("\n")
print("# Reading && Implementation")
print("reference: [The Book of Shaders](https://thebookofshaders.com/)")
print("# Fragment shader")
for recipe in recipes:
    do_one(recipe)



