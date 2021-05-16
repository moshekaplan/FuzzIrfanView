# Download launchpad bugs
import os
import sys
import subprocess

import requests
from launchpadlib.launchpad import Launchpad

cachedir = '.'
launchpad = Launchpad.login_anonymously('just testing', 'production', cachedir, version='devel')

ignored_extensions = set(['.diff', '.msg', '.txt'])

def download_bugs(project, outdir):
    os.makedirs(outdir, exist_ok=True)
    # list out all types to get all bugs
    bugs = project.searchTasks(status = ['New', 'Opinion', 'Invalid', 'Won\'t Fix', 'Expired', 'Confirmed', 'Triaged', 'In Progress', 'Fix Committed', 'Fix Released', 'Incomplete (with response)', 'Incomplete (without response)'])
    print(f"Downloading data from {len(bugs)} bugs")
    extensions_seen = set()
    for bug in bugs:
        # Beyond ugly approach
        bug_id=bug.bug_link.rsplit('/',1)[1]
        myj = requests.get(f"https://api.launchpad.net/1.0/bugs/{bug_id}/attachments").json()
        i = 0
        for attachment in myj['entries']:
            if attachment['type'] == "Patch":
                continue
            attachment_fname = attachment['title']
            attachment_ext = os.path.splitext(attachment_fname)[1].lower()
            if attachment_ext in ignored_extensions:
                continue
            # For examining extensions
            extensions_seen.add(attachment_ext)
            data_link_url = attachment['data_link']
            r = requests.get(data_link_url)
            with open(f'{outdir + os.sep}bug_id_{bug_id}_{i}', 'wb') as fh:
                fh.write(r.content)
            i += 1


def main():
    project_paths = [ ['ubuntu/+source/imagemagick', 'launchpad_imagemagick'],
                      ['ubuntu/+source/graphicsmagick', 'launchpad_graphicsmagick'],
                      ['inkscape', 'launchpad_inkscape'],
                      ['ubuntu/+source/evince', 'launchpad_evince'],
                      ['pinta', 'launchpad_pinta'],
                      ['ubuntu/+source/pinta', 'launchpad_pinta2']
                    ]
                      
    for project_name, outdir in project_paths:
        print(f"Scraping {project_name}" )
        project = launchpad.projects[project_name]
        download_bugs(project, outdir)


if __name__ == "__main__":
    main()