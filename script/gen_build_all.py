import sys
import pathlib

CURRENT_DIR = pathlib.Path(__file__).parent
sys.path.append(str(CURRENT_DIR))

from recipes import recipes
def do_one(name):
    print("cd %s\n"%(name))
    print("echo enter %s\n"%(name))
    print("source ./build_and_run.sh\n") 
    print("cd ..\n")
    print("\n")
for recipe in recipes:
    do_one(recipe)