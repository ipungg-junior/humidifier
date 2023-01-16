from itertools import count
import requests,time, random, json, sys

deviceID = sys.argv[1]
ngrokPort = ''
urlNewSession = f'https://67c0-103-23-224-244.jp.ngrok.io/service/register-session/'
urlPushData = f'https://67c0-103-23-224-244.jp.ngrok.io/service/publish/'
 

def publishData(sessionID, code):
    temp1 = random.randint(28, 36)
    temp2 = random.randint(0, 3)
    temp3 = random.randint(0, 1)
    myobj = {
    'deviceID':deviceID,
    'sessionID':sessionID,
    'suhu_chamber_a': str(temp1),
    'suhu_chamber_b': str(temp1+temp2),
    'suhu_output_pasien': str(temp1-temp3),
    'suhu_heater_plate':str(temp1+temp3),
    'arus_heater_plate':str(temp1+temp2),
    'arus_heater_wire':str(temp1+temp3+1),
    'code' : str(code),
    }
    response = requests.post(urlPushData, json=myobj)
    print(response.status_code)


def register(deviceID):
    requestSession = {
        'deviceID':deviceID
    }

    req = requests.post(urlNewSession, json=requestSession)
    if req.status_code == 200:
        data = json.loads(req.text)
        print(f'response : {data}')
        return data
    else:
        # Register fail!
        return None


time.sleep(1)

if __name__ == '__main__':
    response = register(deviceID)
    counter = 7
    if (response==None):
        print("Bad Request")
    else:    
        while True:
            if (counter==3):
                publishData(response['sessionID'], 1)
            else:
                publishData(response['sessionID'], 0)
            
            if (counter==0):
                counter=7
            else:
                counter -= 1
            time.sleep(1)