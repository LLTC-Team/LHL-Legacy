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

for file in os.listdir(from_path):
    if file.endswith(suffix):
        path = os.path.normpath(os.path.join(from_path, file))
        print("Found dynamic library file: %s" % path)
        shutil.copy2(path, to_path)
        libraryCount += 1

if libraryCount == 0:
    print("No native library were found")
elif libraryCount == 1:
    print("Found 1 native library")
else:
    print("Found %d native libraries" % libraryCount)
