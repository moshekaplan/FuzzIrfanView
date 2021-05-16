import os
import re
import requests
import urllib.parse


def find_images(project_url):
    # project_url should be in the form: https://gitlab.com/inkscape/inkscape
    parsed_url = urllib.parse.urlparse(project_url)
    issue_query_url = f'https://{parsed_url.netloc}/api/v4/projects/{urllib.parse.quote(parsed_url.path.strip("/"), safe="")}/issues/?per_page=100&page='
    base_download_url = project_url.strip('/')
    
    found_urls = []
    for i in range(10000):
        url = issue_query_url + str(i)
        issues = requests.get(url).json()
        if not issues:
            return found_urls
        pattern = r"\(/uploads/[^ \/)]+/[^ \/)]+\)"
        for issue in issues:
            if not issue['description']:
                continue
            #print(issue['description'])
            for end_link in re.findall(pattern, issue['description']):
                full_url = base_download_url + end_link[1:-1]
                print(full_url)
                found_urls.append(full_url)
                
    return found_urls

def download_urls(urls, outdir):
    os.makedirs(outdir, exist_ok=True)
    for i, url in enumerate(urls):
        if i %100 == 0:
            print(f"Downloading file {i}")
        r = requests.get(url)
        with open(f'{outdir + os.sep}{i}', 'wb') as fh:
            fh.write(r.content)

def main():
    project_urls = ["https://gitlab.gnome.org/GNOME/librsvg",
                    "https://gitlab.gnome.org/GNOME/shotwell",
                    "https://gitlab.com/inkscape/inkscape/", 
                    "https://gitlab.com/inkscape/inbox/", 
                    "https://gitlab.gnome.org/GNOME/gimp",
                    ]
    for project_url in project_urls:
        outdir = "".join([c if c.isalpha() or c.isdigit() or c==' ' else "_" for c in project_url.rstrip()])
        print(f"Downloading files for {project_url}")
        found_urls = find_images(project_url)
        print(f"Found {len(found_urls)} URLs to download")
        download_urls(found_urls, outdir)

main()