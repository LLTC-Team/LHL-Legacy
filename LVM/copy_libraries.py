# -*- coding: UTF-8 -*-

import sys
import os
import platform
import shutil

from_path = sys.argv[1]
to_path = sys.argv[2]
libraryCount = 0

system = platform.system()
print("System: %s" % system)
if system == "Windows":
    suffix = ".dll"
elif system == "Linux":
    suffix = ".so"
elif system == "Darwin":
    suffix = ".dylib"
else:
    print("Unsupported system platform")
    sys.exit(1)


def searchLibrary(directory):
    print("Searching %s" % directory)
    if not os.path.isdir(directory):
        print("%s is not a directory")
        return 0
    count = 0
    for file in os.listdir(directory):
        if file.endswith(suffix):
            path = os.path.normpath(os.path.join(directory, file))
            print("Found dynamic library file: %s" % path)
            shutil.copy2(path, to_path)
            count += 1
    return count


libraryCount += searchLibrary(from_path)
if system == "Windows:":
    libraryCount += searchLibrary(from_path + "/Debug")
    libraryCount += searchLibrary(from_path + "/Release")

if libraryCount == 0:
    print("No native library were found")
elif libraryCount == 1:
    print("Found 1 native library")
else:
    print("Found %d native libraries" % libraryCount)
