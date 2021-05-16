import os

"""
Simple script for replacing the first 'n' bytes of a file
with a new header.
"""

new_header = b"SIF"

srcdir = sys.argv[1]
for fname in os.listdir(srcdir):
    fpath = srcdir + os.sep + fname
    with open(fpath, 'rb+') as fh:
        fh.write(new_header)