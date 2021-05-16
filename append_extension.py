import os
import shutil

"""
Simple script for appending an extension to all files in a directory
"""

new_extension = '.xpm'

srcdir = sys.argv[1]
for fname in os.listdir(srcdir):
    fpath = srcdir + os.sep + fname
    shutil.copy(fpath, fpath + new_extension)