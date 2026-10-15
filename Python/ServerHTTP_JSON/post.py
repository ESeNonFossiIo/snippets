import requests
import json

url = 'http://localhost:8008'

post_request_data = {
    'data'      : json.dumps( {'data': 'new_text'} ),
    'headers'   : {
        'content-type'   : 'application/json',
        'Accept-Charset' : 'UTF-8'
    }
}

r = requests.post(url, **post_request_data)
