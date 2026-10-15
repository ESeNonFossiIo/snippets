import json, requests

url = 'http://localhost:8008'

headers = {
    'content-type'   : 'application/json',
    'Accept-Charset' : 'UTF-8'
}

resp = requests.get(url=url, headers=headers)
data = json.loads(resp.text)
print data
