import requests,time, random, json, sys


urlNewSession = 'http://127.0.0.1:8000/service/register-machine-code/'
newID = ''

def register():

    js = {}

    req = requests.get(urlNewSession, json=js)
    if req.status_code == 200:
        data = json.loads(req.text)
        print(f'response : {data}')
        return data
    else:
        # Register fail!
        return None
register()
print(newID)