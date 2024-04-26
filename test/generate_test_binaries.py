from os import chdir,system,mkdir,path
from shutil import rmtree
import platform
from multiprocessing import cpu_count

build_dir = 'build/'

def main():
    if path.exists(build_dir):    
        rmtree(build_dir,ignore_errors=True)
    mkdir(build_dir)
    
    os_platform = platform.system()
    print(f'Running in {os_platform} platform')
    
    cpus = cpu_count()
    
    if(os_platform == 'Linux'):
        cmake_command = 'cmake -S . -B build/'
        make_command = f'make -j{cpus}'
    elif(os_platform == 'Windows'):
        cmake_command = 'cmake -S . -B build/ -G "MinGW Makefiles"'
        make_command = f'mingw32-make -j{cpus}'
    else:
        print(f'Unsupported OS/platform {os_platform}')
        raise SystemExit(-1)
    system(cmake_command)
    chdir(build_dir)
    system(make_command)

if __name__ == '__main__':
    main()