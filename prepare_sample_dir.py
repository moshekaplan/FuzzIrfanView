import argparse
import hashlib
import os
import os.path
import shutil
import sys

MAXSIZE = 1*1024*1024

"""
Takes many directories and places all unique
files in a single directory, as is required
for afl-cmin and afl-fuzz
"""
def prepare_sample_dir(src_dir, dest_dir):
    os.makedirs(dest_dir, exist_ok=True)
    seen_md5s = set()
    i = 0
    for root, dirs, files in os.walk(src_dir):
        for fname in files:
            fpath = os.path.join(root, fname)
            if os.path.getsize(fpath) > MAXSIZE:
                continue
            md5 = calculate_md5(fpath)
            if md5 in seen_md5s:
                continue
            seen_md5s.add(md5)
            dest_fname = dest_dir + os.sep + ("id_%08d" % i)
            shutil.copyfile(fpath, dest_fname)
            i += 1
            if i and (i % 1000 == 0):
                print(f"Processing file {i}")

def calculate_md5(fname):
    with open(fname, 'rb') as fh:
        return hashlib.md5(fh.read()).hexdigest()

def main():
    parser = argparse.ArgumentParser(description='Recursively copy all unique files from a directory into a target.')
    parser.add_argument('srcdir', nargs=1, help='Source directory')
    parser.add_argument('destdir', nargs=1, help='Dest directory')
    args = parser.parse_args()
    prepare_sample_dir(args.srcdir[0], args.destdir[0])
    
if __name__ == "__main__":
    main()