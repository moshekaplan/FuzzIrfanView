import csv
import io
import os
import os.path
import re
import requests
import urllib.parse

import bugzilla # package is python-bugzilla
import requests


# Find all bugs with attachments:
# https://bugzilla.gnome.org/buglist.cgi?f1=attachments.isprivate&limit=0&o1=equals&product=gimp&query_format=advanced&v1=0&ctype=csv&human=1
# Then download the attachments for each
# Download as CSV, since this otherwise times out if sent as XMLRPC

ignored_extensions = set(['.diff', '.msg', '.txt'])

def find_bugs_with_attachments(project_url, product):
    parsed_url = urllib.parse.urlparse(project_url)
    csv_url = f"https://{parsed_url.netloc}/buglist.cgi?f1=attachments.isprivate&limit=0&o1=equals&product={product}&query_format=advanced&v1=0&ctype=csv&human=1"
    csv_data = requests.get(csv_url).text
    f_csv = io.StringIO(csv_data)
    csv_dict = csv.DictReader(f_csv)
    bug_ids = [int(row['Bug ID']) for row in csv_dict]
    return bug_ids

def download_bug_attachments(project_url, bug_ids, outdir):
    parsed_url = urllib.parse.urlparse(project_url)
    bz = bugzilla.Bugzilla(url=parsed_url.netloc)
    os.makedirs(outdir, exist_ok=True)
    extensions_seen = set()
    for bug_id in bug_ids:
        bug = bz.getbug(bug_id)
        for attachment_id, attachment in enumerate(bug.get_attachments()):
            attachment_fname = attachment['file_name']
            attachment_ext = os.path.splitext(attachment_fname)[1].lower()
            if attachment_ext in ignored_extensions:
                continue
            extensions_seen.add(attachment_ext)
            continue
            with open(f'{outdir + os.sep}{bug_id}_{attachment_id}', 'wb')as fh:
                fh.write(attachment['data'].data)
    breakpoint()

def main():
    bugzilla_urls = [("https://bugzilla.gnome.org","gimp"),
                     ("https://bugzilla.gnome.org","eog"),
                     ("https://bugs.kde.org/","gwenview"),
                    ]
    for bugzilla_url, product in bugzilla_urls:
        bug_ids = find_bugs_with_attachments(bugzilla_url, product)
        outdir = "".join([c if c.isalpha() or c.isdigit() or c==' ' else "_" for c in (bugzilla_url.rstrip() + product)])
        download_bug_attachments(bugzilla_url, bug_ids, outdir)
main()