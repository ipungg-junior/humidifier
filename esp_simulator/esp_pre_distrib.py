import requests,time, random, json, sys

# variabel untuk menampung kode mesin (RAM)
myMachineCode = None

url = 'http://127.0.0.1:8000/service/register-machine-code/'

def get_my_code():
    response = requests.get(url)
    if (response.status_code==200):
        my_code = json.loads(response.text)
        return my_code['machine-code']



if __name__ == '__main__':
    
    if (myMachineCode is None):
        myMachineCode = get_my_code()
        
        # SAVE MACHINE CODE TO ROM
    else:
        # GOTO Register session
        pass