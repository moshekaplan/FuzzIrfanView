"""\
Script for downloading attachments from GraphicsMagick bugs
"""

import requests

# Use a search query to find all downloadable files with a single API request

# Note: This script is sloppy and only downloads a single project URL 
SOURCEFORGE_URL = "https://sourceforge.net/rest/p/graphicsmagick/bugs/search?q=attached"
SOURCEFORGE_URL = "https://sourceforge.net/rest/p/giflib/bugs/search?q=attached"

def find_attachments(bug_url):
    url = SOURCEFORGE_URL
    
    attachment_urls = []
    query_result = requests.get(bug_url).json()
    for ticket_entry in query_result['tickets']:
        if 'attachments' not in ticket_entry:
            continue
        for attachment in ticket_entry['attachments']:
            attachment_urls.append(attachment['url'])

    return attachment_urls

bug_attachment_urls = find_attachments(SOURCEFORGE_URL)

for i, attachment_url in enumerate(bug_attachment_urls):
    attachment_data = requests.get(attachment_url)
    with open(f'project_attachment_number_id_{i}', 'wb') as fh:
        fh.write(attachment_data.content)
