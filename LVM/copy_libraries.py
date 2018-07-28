# -*- coding: UTF-8 -*-

import sys
import os
import shutil

from_path = sys.argv[1]
to_path = sys.argv[2]

system = sys.platform
if system == "win32":
    suffix = ".dll"
elif system == "linux":
    suffix = ".so"
elif system == "darwin":
    suffix = ".dylib"
else:
    print("Unsupported system platform")
    sys.exit(1)

for file in os.listdir(from_path):
    if file.endswith(suffix):
        path = os.path.normpath(os.path.join(from_path, file))
        print("Found dynamic library file: %s" % path)
        shutil.copy2(path, to_path)
