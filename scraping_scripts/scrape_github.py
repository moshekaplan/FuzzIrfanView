import io
import os
import re
import sys
import time
import urllib.parse
import zipfile

import requests
from requests.auth import HTTPBasicAuth

# Github username, access token from https://github.com/settings/tokens
USERNAME = 'username'
ACCESS_TOKEN = 'password'

if ACCESS_TOKEN == 'password':
    print ("Need to provide an access token!")
    sys.exit(1)

def find_files(project_url):
    # project_url should be in the form: https://github.com/opencv/opencv
    parsed_url = urllib.parse.urlparse(project_url)
    issue_query_url = f'https://api.github.com/repos/{parsed_url.path.strip("/")}/issues?state=all&per_page=100&page='
    
    found_urls = []
    for i in range(10000):
        url = issue_query_url + str(i)
        response = requests.get(url, auth=HTTPBasicAuth(USERNAME, ACCESS_TOKEN))
        # If we're out of API queries, sleep until we have more.
        if int(response.headers['X-RateLimit-Remaining']) < 10:
            sleep_time = int(response.headers['X-RateLimit-Reset']) - time.time()
            time.sleep(sleep_time)
        
        issues = response.json()

        if not issues:
            return found_urls

        for issue in issues:
            if not issue['body']:
                continue

            zip_pattern = r'\((https://github.com/' + parsed_url.path.strip("/") + r'/files/[0-9]+/[^/\)]+\.zip)\)'
            image_pattern = r'\((https://user-images.githubusercontent.com/[0-9]+/[-0-9a-zA-Z]+\.[a-zA-Z0-9]+)\)'
            patterns = [zip_pattern, image_pattern]
            for pattern in patterns:
                for file_link in re.findall(pattern, issue['body']):
                    print(file_link)
                    found_urls.append(file_link)
    return found_urls

def download_urls(urls, outdir):
    os.makedirs(outdir, exist_ok=True)
    for i, url in enumerate(urls):
        if i %100 == 0:
            print(f"Downloading file {i}")
        r = requests.get(url)
        # If it's a zip file, extract the individual contents
        with open(f'{outdir + os.sep}{i}', 'wb') as fh:
            fh.write(r.content)

        if url.endswith('.zip'):
            try:
                zip = zipfile.ZipFile(io.BytesIO(r.content))
                for j, zip_entry in enumerate(zip.namelist()):
                    with zip.open(zip_entry, 'r') as zip_entry_fh:
                        data = zip_entry_fh.read()
                        with open(f'{outdir + os.sep}{i}_{j}', 'wb') as fh:
                            fh.write(data)
            except zipfile.BadZipFile:
                pass
                
        if url.endswith('.gz'):
            breakpoint()
            # unsupported

def main():
    project_urls = ["https://github.com/opencv/opencv",
                    "https://github.com/FLIF-hub/FLIF"
                    ]
    for project_url in project_urls:
        outdir = "".join([c if c.isalpha() or c.isdigit() or c==' ' else "_" for c in project_url.rstrip()])
        print(f"Downloading files for {project_url}")
        found_urls = find_files(project_url)
        print(f"Found {len(found_urls)} URLs to download")
        download_urls(found_urls, outdir)

main()